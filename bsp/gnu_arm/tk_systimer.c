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
 @brief systimer for GNU_ARM
 
 This is the generic systimer hendler. It's common for all ARM boards, and 
 possibly even for all embedded GNU targets.
 
 @note that initialization and setup is handled by board specific code.
  
 */
 
#define TICK_OWNER         //!< systimer data resides in this oject module
#include <../src/tk_tick.h>

#include "tk_systimer.h"

void systimer_Handler( void )
{
   TIMER0_IR   = BIT( IR_MR0 );  //Command: Reset interrupt (not the counter thoug, that one is allready reseted)
   _tk_tick_advance_mS(PERT); 
}

/*!
 * @defgroup CVSLOG_tk_systimer_c tk_systimer_c
 * @ingroup CVSLOG
 *  $Log: tk_systimer.c,v $
 *  Revision 1.2  2006-04-08 10:15:55  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.1.2.1  2006/04/08 10:06:25  ambrmi09
 *  In preparation to join with main trunk - added forgotten files
 *
 *
 */
 
 

 