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
#include <pthread.h>
#include <errno.h>
#include <assert.h>
#include "implement_pthread.h"
#include "implement_tk.h"

int pthread_attr_init(pthread_attr_t * attr)
{
	assert("Not implemented yet" == 0);
	_PTHREAD_NO_WARN_VAR(attr);
	return 0;
}

int pthread_attr_destroy(pthread_attr_t * attr)
{
	assert("Not implemented yet" == 0);
	_PTHREAD_NO_WARN_VAR(attr);
	return 0;
}

int pthread_attr_getschedparam(const pthread_attr_t * attr,
			       struct sched_param *param)
{
	assert("Not implemented yet" == 0);
	_PTHREAD_NO_WARN_VAR(attr);
	_PTHREAD_NO_WARN_VAR(param);
	return 0;
}

int pthread_attr_setschedparam(pthread_attr_t * attr,
			       const struct sched_param *param)
{
	assert("Not implemented yet" == 0);
	_PTHREAD_NO_WARN_VAR(param);
	_PTHREAD_NO_WARN_VAR(attr);
	return 0;
}

int pthread_attr_getstacksize(const pthread_attr_t * attr, size_t * size)
{
	assert("Not implemented yet" == 0);
	_PTHREAD_NO_WARN_VAR(size);
	_PTHREAD_NO_WARN_VAR(attr);
	return 0;
}

int pthread_attr_setstacksize(pthread_attr_t * attr, size_t size)
{
	assert("Not implemented yet" == 0);
	_PTHREAD_NO_WARN_VAR(size);
	_PTHREAD_NO_WARN_VAR(attr);
	return 0;
}
