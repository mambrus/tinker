/*******************************************************************
 *
 *  DESCRIPTION: TinKerOS.
 *
 *  AUTHOR: Michael Ambrus
 *                              
 *  HISTORY:    
 *
 *  Current $Revision: 1.5 $
 *
 *******************************************************************/
  

#ifndef TK_TICK_H
#define TK_TICK_H

#if defined(__C166__)
   #define MEMTYPE idata
#else
   #define MEMTYPE
#endif

/* 18 014 398 509 481 983 nS*/
/* 18 014 398 S*/
/* 208,5 days*/

//! This is the internal data of the timer system. Observe that accessing this 
//  variable is not thread/interrupt safe. This needs to be revised. 
//
//  @note Also very important - always include the memory type "language 
//  extention" when declaring external cariables like this.

#if defined(TICK_OWNER)
   unsigned long MEMTYPE sys_mackey = 0;             //!< Overflow counter for sys_mickey
   unsigned long MEMTYPE sys_mickey = 0;             //!< A timer tick. Zapp this: "Trimmed to be 1/10 of a mS"
#else
   extern unsigned long MEMTYPE sys_mickey;
   extern unsigned long MEMTYPE sys_mackey;
#endif   


/*!
Update sytem tick.

Call this from your timer ISR.

Usable for systems where <b>100kHz</b> interrupt frequency is suitable

@see _tk_tick_100uS()
@see _tk_tick_1mS()
*/
#define _tk_tick_10uS()               \
  sys_mickey+=10;                        \
                                       \
  if (!sys_mickey){                      \
    sys_mackey++;                      \
  }



/*!
Update sytem tick.

Call this from your timer ISR.

Usable for systems where <b>1kHz</b> interrupt frequency is suitable. This will give the 
highest possible relulution for tinker since 1 sys_mickey is equal to 100uS.

@see _tk_tick_1mS() 
@see _tk_tick_10uS()
@see _tk_tick_1mS()
@see _tk_tick_PC()
*/
#define _tk_tick_100us()              \
  sys_mickey++;                          \
                                       \
  if (!sys_mickey){                      \
    sys_mackey++;                      \
  }


/*!
Update sytem tick.

Call this from your timer ISR.

Usable for systems where <b>1KHz</b> interrupt frequency is suitable

@see _tk_tick_10uS()
@see _tk_tick_100uS()
@see _tk_tick_1mS()
@see _tk_tick_PC()
*/
#define _tk_tick_1mS()                \
  sys_mickey++;                          \
                                       \
  if (!sys_mickey){                      \
    sys_mackey++;                      \
  }


/*!
Update sytem tick.

Call this from your timer ISR.

Usable for systems where <b>100Hz</b> interrupt frequency is suitable (slow system)

@see _tk_tick_10uS()
@see _tk_tick_100uS()
@see _tk_tick_1mS()
@see _tk_tick_PC()
*/
#define _tk_tick_10mS()               \
  sys_mickey+=10;                        \
                                       \
  if (!sys_mickey){                      \
    sys_mackey++;                      \
  }


/*!
Update sytem tick.

Call this from your timer ISR.

Usable for systems where <b>18.2Hz</b> interrupt frequency is suitable (PC system)

@see _tk_tick_10uS()
@see _tk_tick_100uS()
@see _tk_tick_1mS()
@see _tk_tick_10mS()
*/
#define _tk_tick_PC()                 \
  sys_mickey+=10;                        \
                                       \
  if (!sys_mickey){                      \
    sys_mackey++;                      \
  }



#endif

  
/*!
 * @addtogroup CVSLOG CVSLOG
 *
 *  $Log: tk_tick.h,v $
 *  Revision 1.5  2006-02-02 15:51:02  ambrmi09
 *  A lot of thought has been invested into the new PTIME component. Had to
 *  change things even in the systime parts (integrated in the SHEDUL
 *  component) to make it more generic. Think this will be really nice when
 *  it's ready, but has been a long road to get PTIME running (and I'm
 *  still not there).
 *
 *  Revision 1.4  2005/12/03 14:04:31  ambrmi09
 *  A crude documentation structure added. Sorce files modified a little, but
 *  only in comments (for Doxygens sake).
 *
 *
 *******************************************************************/

