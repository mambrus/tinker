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

<HR>
*/

#if defined (__GNUC__)
	#include <tk_ansi.h>
	#include <tk_ansi_dirwrap.h>

	#include BUILDCHAIN(sys/fcntl.h)

	/*The following flags are incompatible with tinker*/
	#undef O_RDONLY
	#undef O_WRONLY
	#undef O_RDWR
	
#else
	#define O_CREAT      0x10
	#define O_EXCL       0x20
	#define O_NONBLOCK   0x40
#endif

#define O_RDONLY     0x01
#define O_WRONLY     0x02
#define O_RDWR       0x04


#endif //SYS_TYPES_H_TK


/*!
 * @defgroup CVSLOG_fcntl_h fcntl_h
 * @ingroup CVSLOG
 *  $Log: fcntl.h,v $
 *  Revision 1.2  2007-03-04 19:07:25  ambrmi09
 *  1) Error handling refined - will handle error from different
 *     cathegories:
 *     - errno (perror)
 *     - TK errors
 *     - TK traps codes
 *     - exit handling can differ beween user exit codes and kernel
 *       trap codes.
 *  2) Extracted fluffy & un-critical code from tk.c (the error and exit
 *     stuff)
 *  3) Preparing to partition even further into tk_switch.c (saving this
 *     until next ci though).
 *
 *  Revision 1.1  2007-03-03 23:01:32  ambrmi09
 *  Added driver support for FIFO's
 *
 *
 */

