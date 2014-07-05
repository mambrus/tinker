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
#ifndef STRING_H_TK
#define STRING_H_TK

/*!
@file
@ingroup kernel_reimpl_ansi

@brief ANSI string.h file <b>IMPORTANT:</b> See the note in the detailed section!

@note Wrap out incompatible definitions/functions in C library

For in-depth discussions about re-implementing ANSI functions, see \ref
kernel_reimpl_ansi.


@see kernel_reimpl_ansi

<HR>
*/

/*Always part of ANSI -> always wrap*/
#include <tk_ansi.h>
#include <tk_ansi_dirwrap.h>

#include BUILDCHAIN(string.h)

#if defined (__GNUC__)
	#define strerror tk_strerror
#else
	char * strerror (int errnum);
#endif



#endif //STRING_H_TK


/*!
 * @defgroup CVSLOG_string_h string_h
 * @ingroup CVSLOG
 *  $Log: string.h,v $
 *  Revision 1.1  2007-03-04 19:07:25  ambrmi09
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
 *
 */

