/***************************************************************************
 *   Copyright (C) 2011 by Michael Ambrus                                  *
 *   ambrmi09@gmail.com                                                    *
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
#include <tinker/config.h>

/*Root threads top of stack*/
extern char _stack_start; //linker provides
#define STACK_TOP ( &_stack_start + TK_ROOT_STACK_SIZE )
//#define STACK_TOP 0x20000800

/* Handler declarations */
void nmi_handler(void);
void hardfault_handler(void);
int start(void);				/* CPU startup (this is the convention) */
int board_main(void);			/* Directly to main of board */

//#define START_AT_BOARD_MAIN
#ifdef START_AT_BOARD_MAIN
#define CODE_START board_main
#else
/*_startup is in assembly and we need to force the adress odd so that the
 * compiler puts code in the startvector adress forcing CPSR to stay in thumb
 * mode
 * */
#define CODE_START (start+1)
#endif

/* Define the vector table (very elegant) */
unsigned int * myvectors[4]
   __attribute__ ((section("vectors")))= {
   	(unsigned int *)	STACK_TOP,			// stack pointer
   	(unsigned int *) 	CODE_START,			// code entry point
   	(unsigned int *)	nmi_handler,		// NMI handler (not really)
   	(unsigned int *)	hardfault_handler	// hard fault handler (let's hope not)
};

