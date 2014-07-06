/***************************************************************************
 *   Copyright (C) 2006, Michael Ambrus                                    *
 *   michael.ambrus@gmail.com                                              *
 *   Copyright (C) 2010,2014 by Michael Ambrus                             *
 *   michael.ambrus@gmail.com                                              *
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

#ifndef TK_HWTYPES_GNU_ARM_H
#define TK_HWTYPES_GNU_ARM_H
//------1---------2---------3---------4---------5---------6---------7---------8

//------1---------2---------3---------4---------5---------6---------7---------8

#include <stddef.h>

/*!
Architecture specific representation of a stack address. In x86 this can be a
simple char* even in 16bit modes, since a pointer contains both segment and
offset.
*/
typedef struct {
	char *tstack;
	size_t stack_size;
} stack_t;

//------1---------2---------3---------4---------5---------6---------7---------8

#endif

