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

#ifndef SYSTIMER_H
#define SYSTIMER_H

#include <sys/types.h>

//Bits
#define PISCR_PS	8
#define PISCR_PIE	13
#define PISCR_PITF	14
#define PISCR_PTE	15

#define MICKEYS_PER_SEC		1000

typedef union{
	__uint16_t raw;

	struct {
		__uint16_t PIRQ:8;	// Periodic interrupt request level.
			// Conﬁgures internal interrupt levels
			// for periodic interrupts.
		__uint16_t PS:1; // Periodic interrupt status. Can be cleared
			//by writing a 1 to it (zero has no effect)
		__uint16_t ZERO:4; // Reserverd (should be cleared)
		__uint16_t PIE:1; // Periodic interrupt enable

		__uint16_t PITF:1; // PIT freeze enable

		__uint16_t PTE:1; // Periodic timer enable


	}f;
}piscr_t;
typedef union{
	__uint32_t raw;
	struct {
		__uint16_t COUNT:16;	// Value
		__uint16_t ZERO:16; 	// Reserved (should be cleared)
	}f;
}pitcnt_t;


extern __uint32_t __sys_mickey;
extern __uint32_t __sys_mackey;

void systimer_init();
//#define __interrupt  __attribute__((interrupt))
//void __interrupt__ systimer_Handler( void );
void systimer_Handler( void );

#endif //SYSTIMER_H

