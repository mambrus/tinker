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

/*!
@file
@ingroup POSIX_RT

@brief RT semaphores - POSIX 1003.1b API

*-* Doc in progress *-*

<p>
A very hastily done wrapper done in 1hr. Probably needs a lot of more work.
</p>


For in-depth discussions about this component, see \ref
POSIX_RT

@see POSIX_RT
@see PTHREAD
*/

#include <semaphore.h>
#include <errno.h>
#include <assert.h>
#include <tk_itc.h>


/*!
@brief Defines a semaphore. Equivalent to TinKer \ref sm_create

@see http://www.opengroup.org/onlinepubs/009695399/functions/sem_init.html
*/
int sem_init (
	sem_t          *sem,
	int            pshared,
	unsigned int   value
){
	unsigned int rc;

	if (pshared != 0)
		return EINVAL;

	rc = sm_create("PS", value, FIFO, sem);
	errno = rc;

	if (rc==ERR_OK)
		return 0;
	else
		return EINVAL;

	return 0;
}

/*!
@brief Undefines a semaphore. Equivalent to TinKer \ref sm_delete

@see http://www.opengroup.org/onlinepubs/009695399/functions/sem_destroy.html
*/
int sem_destroy (sem_t * sem){
	assert("sem_destroy - Not implemented" == NULL);

	return 0;
}

/*!
@brief TBD

@see http://www.opengroup.org/onlinepubs/009695399/functions/sem_trywait.html
*/
int sem_trywait (sem_t * sem){
	assert("sem_trywait - Not implemented" == NULL);

	return 0;
}

/*!
@brief Locks the semaphore. Equivalent to TinKer \ref sm_p

@see http://www.opengroup.org/onlinepubs/009695399/functions/sem_wait.html
*/
int sem_wait (sem_t * sem){
	unsigned int rc;
	extern int __tk_IntFlagCntr;


	if (__tk_IntFlagCntr)
		rc = sm_p_ny(*sem,WAIT,TK_FOREVER);
	else
		rc = sm_p(*sem,WAIT,TK_FOREVER);

	errno = rc;

	if (rc==ERR_OK)
		return 0;
	else
		return EINVAL;

	return 0;
}

/*!
@brief Unlocks the semaphore. Equivalent to TinKer \ref sm_v

@see http://www.opengroup.org/onlinepubs/009695399/functions/sem_post.html
*/
int sem_post (sem_t * sem){
	unsigned int rc;
	extern int __tk_IntFlagCntr;

	if (__tk_IntFlagCntr)
		rc = sm_v_ny(*sem);
	else
		rc = sm_v(*sem);

	errno = rc;

	if (rc==ERR_OK)
		return 0;
	else
		return EINVAL;

	return 0;
}

/*!
 * @defgroup CVSLOG_semaphore_c semaphore_c
 * @ingroup CVSLOG
 *  $Log: semaphore.c,v $
 *  Revision 1.14  2007-04-04 08:12:45  ambrmi09
 *  Improvement to lower latency in collaborative mode. Might need
 *  concideration because it alters the priority rule-set.
 *
 *  Revision 1.13  2007-03-23 20:27:23  ambrmi09
 *  1) Reorganization of ITC into several smaller files
 *  2) Component pthread now supports 3,5,9 and 16 priorities
 *
 *  Revision 1.12  2006-11-27 22:29:24  ambrmi09
 *  Minor djustments completeing the move of some header files to public and due
 *  to some name clashed with user space naming conventions.
 *
 *  Revision 1.11  2006/04/08 10:16:01  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.10.2.1  2006/04/03 20:07:28  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.10  2006/03/12 15:08:54  ambrmi09
 *  - Adjusted the source to accomodate the new file structure.
 *
 *  - All build environments uppdated and verified except BC5. For this one
 *  we stumbled across the header-file issue that I've been fearing. Seems
 *  we need to take care of that one after all.
 *
 *  @note The malloc bug still not solved.
 *
 *  Revision 1.9  2006/03/05 11:11:27  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.8  2006/03/02 14:05:49  ambrmi09
 *  Posting to GNU toolchain started
 *
 *  Revision 1.7  2006/02/23 15:33:33  ambrmi09
 *  Found a nasty "bug", that was not a read bug after all. At least not in the kernel as a feared. It turned out that I forgot some of the details about how timeouts were to be handled (especially in \ref ITC ). A timeout of value \b zero is equal of never to timeout (read more about it in define \ref TK_FOREVER). However two important lesson learned: Even simple add operations get "funny" when adding large numbers (see line 303 in tk_ipc.c - in the \ref lock_stage function). Anyway. TK_FOREVER should equal zero. (This issue makes me wonder sometimes how sane it really was to resurrect a project that has been dormant for nearly 10 years.) The CodeWright project ruler should be positioned on the actual line btw. This check-in will be accompanied  by a <tt>cvs tag</tt> for this reason, and for yet another nasty bug that seems to be a real dispatcher bug. The current source-set-up will show the bug within one mint (which is good since it makes it a little bit less of a search for the <I>"needle in the haystack</i>").
 *
 *  Revision 1.6  2006/02/22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *
 */
