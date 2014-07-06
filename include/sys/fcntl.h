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
#ifndef SYS_FCNTL_H_TK
#define SYS_FCNTL_H_TK

/*!
@file
@ingroup kernel_reimpl_ansi

@brief ANSI sys/fcntl.h file <b>IMPORTANT:</b> See the note in the detailed section!

@note Needed to wrap out incompatible definitions in newlib

For in-depth discussions about re-implementing ANSI functions, see \ref
kernel_reimpl_ansi.

@see kernel_reimpl_ansi
@note that for Newlib
#define	O_RDONLY	0		// +1 == FREAD
#define	O_WRONLY	1		// +1 == FWRITE
#define	O_RDWR		2		// +1 == FREAD|FWRITE
#define	O_APPEND	_FAPPEND
I.e. we can safely redefine the first three (mqueu needs to test for O_RDONLY so it can't be =0)

<HR>
*/

#if defined (__GNUC__)
#include <tk_ansi.h>
#include <tinker/config.h>
#include <tk_ansi_dirwrap.h>

#if defined(TK_MULTI_ARCH_TARGET)
#include BUILDCHAIN_MUARCH(sys/fcntl.h)
#else
#include BUILDCHAIN(sys/fcntl.h)
#endif

	/*The following flags are incompatible with tinker */
#undef O_RDONLY
#undef O_WRONLY
#undef O_RDWR

//The original values (we still need them)
#define O_RDONLY_NL	(FREAD-1)
#define O_WRONLY_NL	(FWRITE-1)
#define O_RDWR_NL	((FREAD|FWRITE) -1)

#else
#define O_CREAT      0x10
#define O_EXCL       0x20
#define O_NONBLOCK   0x40
#endif

#define O_RDONLY     0x01
#define O_WRONLY     0x02
#define O_RDWR       0x04

#endif				//SYS_TYPES_H_TK

