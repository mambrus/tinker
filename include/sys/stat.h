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
@ingroup kernel_reimpl_ansi

@brief ANSI sys/stat.h file <b>IMPORTANT:</b> See the note in the detailed section!

@note Needed to wrap out incompatible definitions in newlib

For in-depth discussions about re-implementing ANSI functions, see \ref
kernel_reimpl_ansi.

@see kernel_reimpl_ansi

<HR>
*/

#if defined (__GNUC__)
#include <tk_ansi.h>
#include <tk_ansi_dirwrap.h>

#if defined (__rtems__)
#undef __rtems__
#define __was_rtems__
#endif

#include BUILDCHAIN(sys/stat.h)

#if defined (__was_rtems__)
#define __rtems__
#endif

/*!
http://www.opengroup.org/onlinepubs/009695399/functions/mknod.html
*/
int mknod(const char *filename, mode_t mode, dev_t dev);
/*!
http://www.opengroup.org/onlinepubs/009695399/functions/mkfifo.html
*/
int mkfifo(const char *path, mode_t mode);

#endif

#ifndef SYS_TYPES_H_TK
#define SYS_TYPES_H_TK

#endif				//SYS_TYPES_H_TK

