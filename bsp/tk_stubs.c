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
@brief stubed funtions

This file contains stub-functions . Use these funtions iniially in your port
until you got working equivalents.

When involved with porting, the \ref clock, \ref printf and \ref malloc
functions are most often not implemented or wrong.

The functions in this file are an esential part of the \ref SCHED internals,
you need to have atleas stub that will do something  meantingful, so that
the dispaching can work.

*/

#include <time.h>

#if (TK_HOWTO_CLOCK == TK_FNK_STUBBED)
long int stub_sub_mickey = 0;
long int stub_mickey = 0;
/*!
Simulates time by increasing a variable everytime function is called.
 */
clock_t clock_stubbed()
{
	stub_sub_mickey++;
	if (stub_sub_mickey >= 100) {
		stub_sub_mickey = 0;
		stub_mickey++;
	}
	return (clock_t) stub_mickey;
}
#endif

