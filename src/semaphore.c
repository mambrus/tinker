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

#include <semaphore.h>
#include <errno.h>
#include <assert.h>
#include <tk_itc.h>

int sem_init(sem_t * sem, int pshared, unsigned int value)
{
	unsigned int rc;

	if (pshared != 0)
		return EINVAL;

	rc = sm_create("PS", value, FIFO, sem);
	errno = rc;

	if (rc == ERR_OK)
		return 0;
	else
		return EINVAL;

	return 0;
}

int sem_destroy(sem_t * sem)
{
	assert("sem_destroy - Not implemented" == NULL);

	return 0;
}

int sem_trywait(sem_t * sem)
{
	assert("sem_trywait - Not implemented" == NULL);

	return 0;
}

int sem_wait(sem_t * sem)
{
	unsigned int rc;
	extern int __tk_IntFlagCntr;

	if (__tk_IntFlagCntr)
		rc = sm_p_ny(*sem, WAIT, TK_FOREVER);
	else
		rc = sm_p(*sem, WAIT, TK_FOREVER);

	errno = rc;

	if (rc == ERR_OK)
		return 0;
	else
		return EINVAL;

	return 0;
}

int sem_post(sem_t * sem)
{
	unsigned int rc;
	extern int __tk_IntFlagCntr;

	if (__tk_IntFlagCntr)
		rc = sm_v_ny(*sem);
	else
		rc = sm_v(*sem);

	errno = rc;

	if (rc == ERR_OK)
		return 0;
	else
		return EINVAL;

	return 0;
}
