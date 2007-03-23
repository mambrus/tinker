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
@ingroup SCHED

@brief Definitions that don't need to be public 

Definitions that don't need to be public for the \ref SCHED component.
Keeps all the internals out of th.h

For in-depth discussions about this component, see \ref
SCHED

*/

#ifndef _IMPLEMENT_ITC_H
#define _IMPLEMENT_ITC_H


#include <tk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <time.h>
#include "implement_tk.h"
#include "context.h"
#include <tk_itc.h>

#define STI_return( ec ) \
	{ TK_STI(); return(ec);}

#define tk_yield_CLI( ) \
	{ tk_yield(); TK_CLI();}


#if (TK_HOWTO_CLOCK == TK_FNK_STUBBED)
   clock_t clock_stubbed();
   #define clock clock_stubbed
#endif


void _itc_removeBlocked(
	itc_t *queue_p, 
	unsigned int idx);

unsigned int _itc_findNextEmpySlot( 
	void );

unsigned long _itc_uintDiff(
	unsigned long x1, 
	unsigned long x2, 
	unsigned long max);

int _itc_proveConcistency(unsigned int qid);
int _itc_no_duplicateBlock(unsigned int qid, unsigned int mark);


#endif /* _IMPLEMENT_itc_H */


/*!
 * @defgroup CVSLOG_implement_itc_h implement_itc_h
 * @ingroup CVSLOG
 *  $Log: implement_itc.h,v $
 *  Revision 1.1  2007-03-23 20:27:23  ambrmi09
 *  1) Reorganization of ITC into several smaller files
 *  2) Component pthread now supports 3,5,9 and 16 priorities
 *
 *
 */

