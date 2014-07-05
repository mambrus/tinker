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
 @brief Low level GNU_ARM based systimer for LPC2129

 Low level GNU_ARM based systimer for LPC2129

 */

#include "lpc2129_vic.h"
#include "lpc21xx.h"
#include "lpc21xx_bits.h"
#include "aeb01.h"
#include "../../tk_systimer.h"
#include <../src/tk_tick.h>


#define FCLK            VPB_CLOCK /*!< Which is hard coded to 60000000ul */
#define PRES            12        /*!< Make one pebble 200nS worth (i.e. fo=5MHz*/
#define X_CLK           -76422 /*-76509*/ /*-88889*/    /*!< Scaling factor that states how much off the
                                       oscillator is in PPM. (i.e. scaled X 1000000).
                                       <em>This one you measure & set</em>. */
#if (PERT < (100*PRES))
#   define REGVAL          ( ((FCLK/MICKEYS_PER_SEC)*PERT)/PRES -1)
#else
#   define REGVAL          ((FCLK/MICKEYS_PER_SEC)*(PERT/PRES) -1)
#endif

#if (REGVAL < 2000)
#   define RELOADVAL  ( ( (1000000uL + X_CLK) * REGVAL)/1000000uL )
#else
#   define RELOADVAL  ( ( (1000000uL + X_CLK)/1000uL * REGVAL)/1000uL )
#endif

void systimer_init(void *vc)
{
   TIMER0_TCR  &= ~BIT(TCR_ENABLE);
   TIMER0_TCR  |=  BIT(TCR_RESET);
   TIMER0_TCR  &= ~BIT(TCR_RESET);

   vic_install_isr((vic_control*)vc);
   vic_enable_int((vic_control*)vc);

   TIMER0_PR   =   PRES;

   TIMER0_IR   |=  BIT( IR_MR0 );  //Reset interrupt
   VICVectAddr = 0x0;

   TIMER0_MCR  =   BIT( MCR0_INT ) | BIT( MCR0_RES ); //Re-occuring interrupts on Match #0. All we have to do is reset the interrupt in the ISR
   TIMER0_MR0  =   RELOADVAL;


   TIMER0_TCR  |=  BIT(TCR_ENABLE);
}

/*!
 * @defgroup CVSLOG_lpc2129_systimer_c lpc2129_systimer_c
 * @ingroup CVSLOG
 *  $Log: lpc2129_systimer.c,v $
 *  Revision 1.3  2006-09-13 18:29:31  ambrmi09
 *  Commited needed in repocitory
 *
 *  Revision 1.2  2006/04/08 10:15:56  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.1.2.1  2006/04/08 10:06:25  ambrmi09
 *  In preparation to join with main trunk - added forgotten files
 *
 *
 */



