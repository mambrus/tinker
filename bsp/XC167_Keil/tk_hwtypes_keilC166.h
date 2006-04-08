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
  

#ifndef TK_HWTYPES_KEILC166_H
#define TK_HWTYPES_KEILC166_H      
//------1---------2---------3---------4---------5---------6---------7---------8

//------1---------2---------3---------4---------5---------6---------7---------8


#include <stddef.h>  //Needed for size_t

#pragma pack(1)     /* byte alignment */

//BIG NOTE!! The following #pragma will crash in dave file who use pointercasts for SFR data
//======================================================================
//Output will not work (in best case), in worst case C166 crashes
//Leave it here as a reference so that I remeber. Also, will tinket work?
//Structures might change, does it matter?
//#pragma bytealign   /* ptrs to byte-aligned objects */
 
/*!

@brief TBD

@note that the offset addres does not contain the 2 MSB bits that defines wich
DDP to use. It's your respponsibility to trunc these off before storing in the
off variable, and to add them back when you assign your user-stack pointer
(typically R0)

@note Orientation of bitfiels are compilor specific. Use commandline or pragma 
to ensure correct layout.

@note Keil C166 seem to lack directives for byte order within a struct.
This struct is empirically defined. Be observant about changes of
behaviour when changing compiler and/or version.

*/
typedef union{ 
   unsigned long linear; 
   union {
      struct {
         unsigned char padding8;     // These two togeather must be 
         unsigned int  padding6  :6; // ... exacylly 14 bits long.
         unsigned int _seg       :10;                     
      }seg24;
      struct {
         unsigned int _offs      :14; 
         unsigned int padding2   :2;
         unsigned char padding8;
      }offs24;  
   }u;
}userstackaddr_t;


/*!
@brief GTT1 core (T3) - Control register bit fields 

@ingroup XC167_SFR_bitfields
*/
typedef struct{ 
   unsigned int TcI       :3;  //<! Timer x Input Parameter Selection
   unsigned int TcM       :3;  //<! Timer x Mode Control 
   unsigned int TcR       :1;  //<! Timer x Run Bit         (1 = runs)
   unsigned int TcUD      :1;  //<! Timer x Up/Down Control (1 = counting down)
   unsigned int TcUDE     :1;  //<! Timer x External Up/Down Enable ( 1 = counting direction is externally controlled by line TaEUD)

   unsigned int TcOE      :1;  //<! Overflow/Underflow Output Enable
   unsigned int TcOTL     :1;  //<! Timer 3 Overflow Toggle Latch
   unsigned int BPS1      :2;  //<! Timer Block Prescaler 1 (Prescales T2,T3,T4)

   unsigned int TcEDGE    :1;  //<! Timer x Edge Detection
   unsigned int TcCHDIR   :1;  //<! Timer x Count Direction Change
   unsigned int TcRDIR    :1;  //<! Timer x Rotation Direction
}GPT1_ControlRegCore_t;

/*!
@brief GTT1 Aux (T2, T4) - Control register bit fields 

@ingroup XC167_SFR_bitfields
*/
typedef struct{ 
   unsigned int TaI       :3;  //<! Timer x Input Parameter Selection
   unsigned int TaM       :3;  //<! Timer x Mode Control (Basic Operating Mode)
   unsigned int TaR       :1;  //<! Timer x Run Bit          (1 = runs)
   unsigned int TaUD      :1;  //<! Timer x Up/Down Control  (1 = counting down)
   unsigned int TaUDE     :1;  //<! Timer x External Up/Down Enable ( 1 = counting direction is externally controlled by line TaEUD)

   unsigned int TaRC      :1;  //<! Timer x Remote Control
   unsigned int _paddingA :2;  //<! reserved for future use; reading returns 0;
   unsigned int TaIRDIS   :1;  //<! Timer x Interrupt Disable

   unsigned int TaEDGE    :1;  //<! Timer x Edge Detection
   unsigned int TaCHDIR   :1;  //<! Timer x Count Direction Change
   unsigned int TaRDIR    :1;  //<! Timer x Rotation Direction
}GPT1_ControlRegAux_t;

/*!
@brief GTT2 core (T6) - Control register bit fields

@ingroup XC167_SFR_bitfields
*/
typedef struct{ 
   unsigned int TcI       :3;  //<! Timer x Input Parameter Selection
   unsigned int TcM       :3;  //<! Timer x Mode Control 
   unsigned int TcR       :1;  //<! Timer x Run Bit           (1 = runs)
   unsigned int TcUD      :1;  //<! Timer x Up/Down Control   (1 = counting down)

   unsigned int _paddingC1:1;  //<! 

   unsigned int TcOE      :1;  //<! Overflow/Underflow Output Enable
   unsigned int TcOTL     :1;  //<! Timer 3 Overflow Toggle Latch
   unsigned int BPS2      :2;  //<! Timer Block Prescaler 2 (Prescales T5,T6)

   unsigned int _paddingC2:1;  //<! 
   unsigned int TcCLR     :1;  //<! 
   unsigned int TcSR      :1;  //<! 
}GPT2_ControlRegCore_t;

/*!
@brief GTT2 Aux (T5) - Control register bit fields 

@ingroup XC167_SFR_bitfields
*/
typedef struct{ 
   unsigned int TaI       :3;  //<! Timer x Input Parameter Selection
   unsigned int TaM       :3;  //<! Timer x Mode Control (Basic Operating Mode)
   unsigned int TaR       :1;  //<! Timer x Run Bit           (1 = runs)
   unsigned int TaUD      :1;  //<! Timer x Up/Down Control   (1 = counting down)

   unsigned int _paddingA1:1;  //<! 

   unsigned int TaRC      :1;  //<! Timer x Remote Control
   unsigned int TaCTc     :1;  //<! 
   unsigned int TaCC      :1;  //<! 

   unsigned int TaCI      :2;  //<! 
   unsigned int TaCLR     :1;  //<! 
   unsigned int TaSC      :1;  //<! 
}GPT2_ControlRegAux_t;

/*!
@brief Interrupt Control Register(s)

The layout of the interrupt control registers is applicable
to all xxIC registers.

All prioritiy level values means higher priority for higher value.

@ingroup XC167_SFR_bitfields
*/
typedef struct{ 
   unsigned int GLVL      :2;  //<! Group Priority Level 
   unsigned int ILVL      :4;  //<! Interrupt Priority Level
   unsigned int IE        :1;  //<! Interrupt Enable Control Bit (1= enabled)
   unsigned int IR        :1;  //<! (rhw) Interrupt Request Flag @note protected bit
   unsigned int GPX       :1;  //<! Group Priority Extension (extends GLVL)
   unsigned int _padding  :7;  //<! padding reads zero 
}IC_t;


#pragma pack()    /* reset to default alignment */



/*!

@brief TBD

@note Orientation of bitfiels are compilor specific. Use commandline or pragma 
to ensure correct layout.

@note Keil C166 seem to lack directives for byte order within a struct.
This struct is empirically defined. Be observant about changes of
behaviour when changing compiler and/or version.

*/

typedef union{ 
   unsigned long linear; 
   struct { 
      unsigned int _offs; 
      unsigned int _seg;         
   }segmented; 
   struct { 
      unsigned int _SP; 
      unsigned int _SPSEG;          
   }reg; 
}systemstackaddr_t;

/*! 
Architecture specific representation of a stack adress. In this obscure
MPU/Compiler combo, this need to be devided in two stacks for each
thread, that each is best represented in a different way. Both these
ways however also have a linear adrees for conveniant lookup in
physical memory.
*/

typedef struct { 
   systemstackaddr_t systemstack;      
   userstackaddr_t   userstack;   
   
   size_t            sys_stack_size;    //These two added togeather constitutes 
   size_t            usr_stack_size;    //the actual memory allocated
}stack_t;


//------1---------2---------3---------4---------5---------6---------7---------8
extern void _tk_reinit_stackaddr_xc167keil( stack_t *addr, size_t size );
#endif


  
/*!
 * @defgroup CVSLOG_tk_hwtypes_keilC166_h tk_hwtypes_keilC166_h
 * @ingroup CVSLOG
 *
 *  $Log: tk_hwtypes_keilC166.h,v $
 *  Revision 1.7  2006-04-08 10:15:51  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.6.2.2  2006/04/03 20:07:21  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.6.2.1  2006/03/30 10:52:18  ambrmi09
 *  First version of new threadstarter. It seems very promising. A *lot* of
 *  awfull pain concerning different targets has the potential to go away.
 *
 *  Revision 1.6  2006/03/05 11:11:23  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.5  2006/02/22 13:05:45  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.4  2006/02/02 15:51:02  ambrmi09
 *  A lot of thought has been invested into the new PTIME component. Had to
 *  change things even in the systime parts (integrated in the SHEDUL
 *  component) to make it more generic. Think this will be really nice when
 *  it's ready, but has been a long road to get PTIME running (and I'm
 *  still not there).
 *
 *  Revision 1.3  2005/12/03 14:04:30  ambrmi09
 *  A crude documentation structure added. Sorce files modified a little, but
 *  only in comments (for Doxygens sake).
 *
 *
 *******************************************************************/
