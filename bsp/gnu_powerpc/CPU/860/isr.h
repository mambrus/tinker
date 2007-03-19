/***************************************************************************
 *   Copyright (C) 2007 by Michael Ambrus                                  *
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

#ifndef ISR_H
#define ISR_H
#include <tk.h>


/*
     IRQ0        0000_0000
Internal Level 0 0000_0100
     IRQ1        0000_1000
Internal Level 1 0000_1100
     IRQ2        0001_0000
Internal Level 2 0001_0100
     IRQ3        0001_1000
Internal Level 3 0001_1100
     IRQ4        0010_0000
Internal Level 4 0010_0100
     IRQ5        0010_1000
Internal Level 5 0010_1100
     IRQ6        0011_0000
Internal Level 6 0011_0100
     IRQ7        0011_1000
Internal Level 7 0011_1100
*/


#define     IRQ_0         0x00  /* 0000_0000 */
#define Intrnl_0          0x04  /* 0000_0100 */
#define      IRQ_1        0x08  /* 0000_1000 */
#define Intrnl_1          0x0C  /* 0000_1100 */
#define      IRQ_2        0x10  /* 0001_0000 */
#define Intrnl_2          0x14  /* 0001_0100 */
#define      IRQ_3        0x18  /* 0001_1000 */
#define Intrnl_3          0x1C  /* 0001_1100 */
#define      IRQ_4        0x20  /* 0010_0000 */
#define Intrnl_4          0x24  /* 0010_0100 */
#define      IRQ_5        0x28  /* 0010_1000 */
#define Intrnl_5          0x2C  /* 0010_1100 */
#define      IRQ_6        0x30  /* 0011_0000 */
#define Intrnl_6          0x34  /* 0011_0100 */
#define      IRQ_7        0x38  /* 0011_1000 */
#define Intrnl_7          0x3C  /* 0011_1100 */

/*
typedef void handler_function_t();
typedef handler_function_t *handler_function;
*/
extern isr_handler isr_table[16];

typedef enum {
	lvl_IRQ_0=0,
	lvl_Intrnl_0,
	lvl_IRQ_1,
	lvl_Intrnl_1,
	lvl_IRQ_2,
	lvl_Intrnl_2,
	lvl_IRQ_3,
	lvl_Intrnl_3,
	lvl_IRQ_4_,
	lvl_Intrnl_4,
	lvl_IRQ_5,
	lvl_Intrnl_5,
	lvl_IRQ_6,
	lvl_Intrnl_6,
	lvl_IRQ_7,
	lvl_Intrnl_7
}irq_level;


/*
   0101010101010101
   0    1    2    3    4    5    6    7    8    9    10  11    12  13    14  15
 IRQ0 LVL0 IRQ1 LVL1 IRQ2 LVL2 IRQ3 LVL3 IRQ4 LVL4 IRQ5 LVL5 IRQ6 LVL6 IRQ7 LVL7
*/

 //                 0     1     2     3     4     5     6     7     8     9    10    11    12    13    14    15
 typedef enum {_IRQ0=0, LVL0, IRQ1, LVL1, IRQ2, LVL2, IRQ3, LVL3, IRQ4, LVL4, IRQ5, LVL5, IRQ6, LVL6, IRQ7, LVL7} irq_bits;

void isr_external();

#endif //ISR_H

