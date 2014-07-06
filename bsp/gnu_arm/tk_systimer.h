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

    This is the generic systimer header file..

    PERT stands for PERIod Time and is the time between two timer tick
    interrupts.

    Choosing a value for PERT is not critical for embedded ARM. The HW is
    really marvelous and values from 1uS to 10mS are tested without disturbing
    the kernel much.

    It's recomended to use a value of PERT as high as possible up to
    1/CLOCKS_PER_SEC which is normaly 10000 on a UN*X tool-chain. Using higher
    values might affect the presition on the clock() function but not nesesarily
    even thats is not always true).

    Choosing lower values will not add presition (wich one might tink), since TinKer uses the HW to
    read fractions of time in HW on each readout. That way  we can get high
    accurancy without stressing the system with a high interrupt SYSTIMER frequency.

    For high accurancy timer events another unrelated mechanism is used, so
    that is not a reason for using low PERT either.

    Conclusion: On this target, use a value of PERT equal to 1/CLOCKS_PER_SEC.

  */

#ifndef TK_SYSTIMER_H
#define TK_SYSTIMER_H

//#define PERT 1ul //!< Period time in uS that we want service to run with (1MHz)
//#define PERT 10000ul /*! UN*X standard 100Hz (i.e. CLOCKS_PER_SEC=[100]) */

#include <time.h>
#include <../src/tk_tick.h>
#define PERT (MICKEYS_PER_SEC/CLOCKS_PER_SEC)	//!< I.e. 1/CLOCKS_PER_SEC expressed in mickey unit (1uS)

void systimer_init(void *);
void systimer_Handler(void) __attribute__ ((interrupt("IRQ")));

#endif				//TK_SYSTIMER_H

