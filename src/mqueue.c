/***************************************************************************
 *   Copyright (C) 2006 by Michael Ambrus                                  *
 *   michael.ambrus@maquet.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <pthread.h>
#include <mqueue.h>
#include <semaphore.h>

#if defined(_WIN32) && defined(_MSC_VER)
#include <errno.h>
#include <search.h>
#define PATH_MAX 255
#elif defined(__CYGWIN32__) || defined(__CYGWIN__)
#include <sys/errno.h>
#else
#include <errno.h>
#ifndef PATH_MAX
#define PATH_MAX 24
#endif
#endif

#if HOSTED
#define QNAME_LEN PATH_MAX
#else
#define QNAME_LEN 24
#endif

#define _MQ_NO_WARN_VAR(x) ((void)x)

#define NUMBER_OF_QUEUES 100
#define NUMBER_OF_MESSAGES 100
#define NUMBER_OF_FILES 100
#define CREATE_ONLY (NUMBER_OF_FILES + 1)

#define NUMB_MESS(Q) ((Q->mBox.mIdxIn>=Q->mBox.mIdxOut)? \
   Q->mBox.mIdxIn - Q->mBox.mIdxOut: \
   Q->mq_attr.mq_maxmsg - Q->mBox.mIdxOut + Q->mBox.mIdxIn \
)

typedef struct {
	time_t time;
	unsigned int prio;
	unsigned int inOrder;
} OrderT;

typedef struct {
	char *buffer;
	int msgSz;
	OrderT order;
} MessT;

typedef struct {
	size_t mIdxIn;
	size_t mIdxOut;
	MessT *messArray;
	unsigned int lastInOrder;
} MBox;

typedef struct {
	int taken;
	char mq_name[QNAME_LEN];
	struct mq_attr mq_attr;
	MBox mBox;
	sem_t sem;

} QueueD;

typedef struct {
	int taken;
	pthread_t tId;
	mode_t oflags;
	int qId;

} FileD;

static QueueD queuePool[NUMBER_OF_QUEUES];
static int qPIdx = 0;

static FileD filePool[NUMBER_OF_FILES];
static int fPIdx = 0;

static sem_t poolAccessSem;
static pthread_once_t mq_once = PTHREAD_ONCE_INIT;

static void initialize(void);
static void sortByPrio(QueueD * Q);

static int compMess(const void *elem1, const void *elem2);
int mq_close(mqd_t mq)
{
	pthread_once(&mq_once, initialize);
	_MQ_NO_WARN_VAR(mq);
	assert(sem_wait(&poolAccessSem) == 0);

	assert(sem_post(&poolAccessSem) == 0);

	return 0;
}

int mq_getattr(mqd_t mq, struct mq_attr *attrbuf)
{
	pthread_once(&mq_once, initialize);
	_MQ_NO_WARN_VAR(mq);
	_MQ_NO_WARN_VAR(attrbuf);
	assert(sem_wait(&poolAccessSem) == 0);

	assert(sem_post(&poolAccessSem) == 0);

	return 0;
}

mqd_t mq_open(const char *mq_name, int oflags, mode_t mode,
	      struct mq_attr * mq_attr)
{
	int i, j;
	size_t k;
	int qId;
	int dId;

	pthread_once(&mq_once, initialize);
	_MQ_NO_WARN_VAR(mode);

	assert(sem_wait(&poolAccessSem) == 0);

	if (strlen(mq_name) > QNAME_LEN) {
		errno = ENAMETOOLONG;
		assert(sem_post(&poolAccessSem) == 0);
		return (-1);
	}

	if (mq_name[0] != '/') {
		errno = EINVAL;
		assert(sem_post(&poolAccessSem) == 0);
		return (-1);
	}

	if (oflags & O_CREAT) {
		int reuse = 0;

		for (i = qPIdx, j = 0;
		     j < NUMBER_OF_QUEUES && queuePool[i].taken != 0; j++) {
			i++;
			i %= NUMBER_OF_QUEUES;
		}

		if (j == NUMBER_OF_QUEUES) {
			errno = EMFILE;
			assert(sem_post(&poolAccessSem) == 0);
			return (-1);
		}
		qId = j;
		queuePool[i].taken = 1;

		for (i = 0; i < NUMBER_OF_QUEUES; i++) {
			if (queuePool[i].taken) {
				if (strncmp
				    (queuePool[i].mq_name, mq_name,
				     QNAME_LEN) == 0) {

					if (oflags & O_CREAT) {

						assert(sem_post(&poolAccessSem)
						       == 0);
						errno = EEXIST;
						return (-1);
					} else {

						qId = i;
						reuse = 1;
					}
				}
			}
		}

		if (!reuse) {
			queuePool[qId].taken = 1;
			strncpy(queuePool[qId].mq_name, mq_name, QNAME_LEN);

			if (mq_attr == NULL) {
				errno = EINVAL;
				return (-1);
			}

			queuePool[qId].mq_attr.mq_maxmsg = mq_attr->mq_maxmsg;
			queuePool[qId].mq_attr.mq_msgsize = mq_attr->mq_msgsize;

			queuePool[qId].mBox.messArray =
			    (MessT *) malloc((mq_attr->mq_maxmsg) *
					     sizeof(MessT));

			if (queuePool[qId].mBox.messArray == NULL) {

				assert(sem_post(&poolAccessSem) == 0);
				errno = ENFILE;
				return (-1);
			}

			for (k = 0; k < mq_attr->mq_maxmsg; k++) {
				queuePool[qId].mBox.messArray[k].buffer =
				    (char *)malloc(mq_attr->mq_msgsize *
						   sizeof(char));
				if (queuePool[qId].mBox.messArray[k].buffer ==
				    NULL) {

					for (j = k; j >= 0; j--) {
						free(queuePool[qId].mBox.
						     messArray[j].buffer);
					}
					free(queuePool[qId].mBox.messArray);

					assert(sem_post(&poolAccessSem) == 0);
					errno = ENFILE;
					return (-1);
				}

				queuePool[qId].mBox.messArray[k].msgSz = 0;
				queuePool[qId].mBox.messArray[k].order.inOrder =
				    0;
				queuePool[qId].mBox.messArray[k].order.prio = 0;
				queuePool[qId].mBox.messArray[k].order.time = 0;
				queuePool[qId].mBox.lastInOrder = 0;
			}

			queuePool[qId].mBox.mIdxIn = 0;
			queuePool[qId].mBox.mIdxOut = 0;

			if (!(oflags & O_NONBLOCK))
				assert(sem_init(&queuePool[qId].sem, 0, 0) ==
				       0);
		}
	}

	if (oflags & (O_RDONLY | O_WRONLY | O_RDWR)) {
		if (!(oflags & O_CREAT)) {
			for (i = 0; i < NUMBER_OF_QUEUES; i++) {
				if (queuePool[i].taken)
					if (strncmp
					    (queuePool[i].mq_name, mq_name,
					     QNAME_LEN) == 0)
						break;
			}
			if (i == NUMBER_OF_QUEUES) {
				assert(sem_post(&poolAccessSem) == 0);
				errno = ENOENT;
				return (-1);
			}
			qId = i;
		}

		for (i = fPIdx, j = 0;
		     j < NUMBER_OF_FILES && filePool[i].taken != 0; j++) {
			i++;
			i %= NUMBER_OF_FILES;
		}

		if (j == NUMBER_OF_FILES) {
			errno = EMFILE;
			assert(sem_post(&poolAccessSem) == 0);
			return (-1);
		}
		dId = j;
		filePool[i].taken = 1;
		filePool[i].qId = qId;
		filePool[i].oflags = oflags;
		filePool[i].tId = pthread_self();

	} else {
		dId = CREATE_ONLY;
	}

	assert(sem_post(&poolAccessSem) == 0);
	return (dId);
}

size_t mq_receive(mqd_t mq,
		  char *msg_buffer, size_t buflen, unsigned int *msgprio)
{
	QueueD *Q;
	size_t msgSize;

	pthread_once(&mq_once, initialize);
	assert(sem_wait(&poolAccessSem) == 0);

	if (!(((mq >= 0) && (mq < NUMBER_OF_FILES)) &&
	      (filePool[mq].taken == 1) && (filePool[mq].tId == pthread_self())
	    )) {
		assert(sem_post(&poolAccessSem) == 0);
		errno = EBADF;
		return (-1);
	}

	if (!(filePool[mq].oflags & (O_RDONLY | O_RDWR))) {
		assert(sem_post(&poolAccessSem) == 0);
		errno = EBADF;
		return (-1);
	}

	Q = &queuePool[filePool[mq].qId];

	if (Q->taken != 1) {
		assert(sem_post(&poolAccessSem) == 0);
		errno = EBADF;
		return (-1);
	}

	if (!(buflen >= Q->mq_attr.mq_msgsize)) {
		assert(sem_post(&poolAccessSem) == 0);
		errno = EMSGSIZE;
		return (-1);
	}

	if (NUMB_MESS(Q) <= 0) {
		if (filePool[mq].oflags & O_NONBLOCK) {
			assert(sem_post(&poolAccessSem) == 0);
			errno = EAGAIN;
			return (-1);
		}
	}

	assert(sem_post(&poolAccessSem) == 0);

	assert(sem_wait(&Q->sem) == 0);

	memcpy(msg_buffer,
	       Q->mBox.messArray[Q->mBox.mIdxOut].buffer,
	       Q->mBox.messArray[Q->mBox.mIdxOut].msgSz);
	if (msgprio)
		*msgprio = Q->mBox.messArray[Q->mBox.mIdxIn].order.prio;

	msgSize = Q->mBox.messArray[Q->mBox.mIdxOut].msgSz;

	Q->mBox.mIdxOut++;
	Q->mBox.mIdxOut %= Q->mq_attr.mq_maxmsg;

	return (msgSize);
}

int mq_setattr(mqd_t mqdes,
	       const struct mq_attr *new_attrs, struct mq_attr *old_attrs)
{
	pthread_once(&mq_once, initialize);
	_MQ_NO_WARN_VAR(mqdes);
	_MQ_NO_WARN_VAR(new_attrs);
	_MQ_NO_WARN_VAR(old_attrs);
	assert(sem_wait(&poolAccessSem) == 0);

	assert(sem_post(&poolAccessSem) == 0);

	return 0;
}

int mq_send(mqd_t mq, const char *msg, size_t msglen, unsigned int msgprio)
{
	QueueD *Q;
	time_t ttime;

	pthread_once(&mq_once, initialize);
	assert(sem_wait(&poolAccessSem) == 0);

	if (!(((mq >= 0) && (mq < NUMBER_OF_FILES)) &&
	      (filePool[mq].taken == 1) && (filePool[mq].tId == pthread_self())
	    )) {
		assert(sem_post(&poolAccessSem) == 0);
		errno = EBADF;
		return (-1);
	}

	if (!(filePool[mq].oflags & (O_WRONLY | O_RDWR))) {
		assert(sem_post(&poolAccessSem) == 0);
		errno = EBADF;
		return (-1);
	}

	Q = &queuePool[filePool[mq].qId];

	if (Q->taken != 1) {
		assert(sem_post(&poolAccessSem) == 0);
		errno = EBADF;
		return (-1);
	}

	if (!(Q->mq_attr.mq_msgsize >= msglen)) {
		assert(sem_post(&poolAccessSem) == 0);
		errno = EMSGSIZE;
		return (-1);
	}

	if (!(NUMB_MESS(Q) < Q->mq_attr.mq_maxmsg - 1)) {

		if (filePool[mq].oflags & O_NONBLOCK) {
			assert(sem_post(&poolAccessSem) == 0);
			errno = EAGAIN;
			return (-1);
		} else {
			assert(sem_post(&poolAccessSem) == 0);
			errno = EAGAIN;
			return (-1);
		}
	}

	memcpy(Q->mBox.messArray[Q->mBox.mIdxIn].buffer, msg, msglen);

	Q->mBox.messArray[Q->mBox.mIdxIn].msgSz = msglen;

	Q->mBox.messArray[Q->mBox.mIdxIn].order.prio = msgprio;
	Q->mBox.messArray[Q->mBox.mIdxIn].order.inOrder = Q->mBox.lastInOrder++;
	Q->mBox.messArray[Q->mBox.mIdxIn].order.time = time(&ttime);

	Q->mBox.mIdxIn++;
	Q->mBox.mIdxIn %= Q->mq_attr.mq_maxmsg;

	sortByPrio(Q);

	assert(sem_post(&Q->sem) == 0);

	assert(sem_post(&poolAccessSem) == 0);

	return (0);
}

static void initialize(void)
{
	int i;

	assert(sem_init(&poolAccessSem, 0, 1) == 0);

	assert(sem_wait(&poolAccessSem) == 0);
	for (i = 0; i < NUMBER_OF_QUEUES; i++) {
		queuePool[i].taken = 0;
		memset(queuePool[i].mq_name, 'Q', QNAME_LEN);
		queuePool[i].mq_name[0] = 0;
	}
	for (i = 0; i < NUMBER_OF_FILES; i++) {
		filePool[i].taken = 0;
	}
	assert(sem_post(&poolAccessSem) == 0);
}

static int compMess(const void *elem1, const void *elem2)
{
	MessT *e1 = (MessT *) elem1;
	MessT *e2 = (MessT *) elem2;

	if (e1->order.prio > e2->order.prio)
		return (-1);
	if (e1->order.prio < e2->order.prio)
		return (1);

	if (e1->order.time < e2->order.time)
		return (-1);
	if (e1->order.time > e2->order.time)
		return (1);

	if (e1->order.inOrder < e2->order.inOrder)
		return (-1);
	if (e1->order.inOrder > e2->order.inOrder)
		return (1);
	return (0);
}

static void sortByPrio(QueueD * Q)
{
	int szMess = NUMB_MESS(Q);
	int i, j;

	if (Q->mBox.mIdxOut <= Q->mBox.mIdxIn) {
		qsort(&Q->mBox.messArray[Q->mBox.mIdxOut],
		      szMess, sizeof(MessT), compMess);
	} else {
		MessT *tempT;

		tempT = (MessT *) malloc(szMess * sizeof(MessT));

		i = Q->mBox.mIdxOut;

		for (j = 0; j < szMess; j++) {
			tempT[j] = Q->mBox.messArray[i];
			i++;
			i %= Q->mq_attr.mq_maxmsg;
		}

		qsort(tempT, szMess, sizeof(MessT), compMess);

		i = Q->mBox.mIdxOut;

		for (j = 0; j < szMess; j++) {
			Q->mBox.messArray[i] = tempT[j];
			i++;
			i %= Q->mq_attr.mq_maxmsg;
		}

		free(tempT);
	}

}

int mq_unlink(const char *mq_name)
{
	_MQ_NO_WARN_VAR(mq_name);
	return 0;
}
