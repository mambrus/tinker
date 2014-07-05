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

#ifndef TK_HWTYPES_MSVCX86_H
#define TK_HWTYPES_MSVCX86_H
//------1---------2---------3---------4---------5---------6---------7---------8

//------1---------2---------3---------4---------5---------6---------7---------8

#include <stddef.h>

/*!
Architecture specific representation of a stack adress. In x86 this can be a
simple char* even in 16bit modes, since a pointer contains both segment and
offset.
*/
typedef struct {
	char *tstack;
	size_t stack_size;
} stack_t;

//------1---------2---------3---------4---------5---------6---------7---------8

#endif

/*!
 * @defgroup CVSLOGtk_hwtypes_borlandX86_h tk_hwtypes_borlandX86_h
 * @ingroup CVSLOG
 *
 *  $Log: stypes_mvcsX86.h,v $
 *  Revision 1.1  2006-11-03 11:33:30  ambrmi09
 *  Moved architecture specific files out from bsp structure and into kernel
 *  source 'arch' structure. This makes sense since I plan to refine the
 *  project into kernel specific maintenence on one hand, and BSP (and
 *  eventually other's) on the other.
 *
 *  BSP's as part of the *kernlel* project will only be in the form of crude
 *  template BSP (community will take care of the rest).
 *
 *  Revision 1.3  2006/04/08 10:15:50  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.2.2.1  2006/03/30 10:52:18  ambrmi09
 *  First version of new threadstarter. It seems very promising. A *lot* of
 *  awfull pain concerning different targets has the potential to go away.
 *
 *  Revision 1.2  2006/02/28 11:50:06  ambrmi09
 *  - Trimmed the time constants (ruffly). 4sek per 14hrs drift
 *  - Revived the Borland C (BC5) target. Projectfile also added (BC5.ide)
 *  - Started experimenting with a indlude filename macro, that has the
 *    the potential of solving my ANSI header/function dilemma (\ref
 *    BUILDCHAIN )
 *  - Some "fishyness" about BC5 handling of time. Either \ref clock or
 *    \ref CLK_TCK doesn't follow standard (the latter I know for a fact,
 *    since it's 1e3 instead of 1e6 - but thats not all). \ref tk_msleep is
 *    adjusted to try to see the error.
 *
 *  Revision 1.1  2006/02/28 11:24:48  ambrmi09
 *  Initial checkin of missing files for Borland C and MSVC
 *
 *
 *******************************************************************/
