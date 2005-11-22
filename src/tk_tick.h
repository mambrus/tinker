
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
   unsigned long MEMTYPE sys_mackey = 0;             //!< Overflow counter for sys_tick
   unsigned long MEMTYPE sys_tick = 0;               //!< A timer tick. Trimmed to be 1/10 of a mS
#else
   extern unsigned long MEMTYPE sys_tick;
   extern unsigned long MEMTYPE sys_mackey;
#endif   


/*!
Update sytem tick.

Call this from your timer ISR.

Usable for systems where <b>100kHz</b> interrupt frequency is suitable

@see __tk_tick_100uS()
@see __tk_tick_1mS()
*/
#define __tk_tick_10uS()               \
  sys_tick+=10;                        \
                                       \
  if (!sys_tick){                      \
    sys_mackey++;                      \
  }



/*!
Update sytem tick.

Call this from your timer ISR.

Usable for systems where <b>1kHz</b> interrupt frequency is suitable. This will give the 
highest possible relulution for tinker since 1 sys_tick is equal to 100uS.

@see __tk_tick_1mS() 
@see __tk_tick_10uS()
@see __tk_tick_1mS()
@see __tk_tick_PC()
*/
#define __tk_tick_100us()              \
  sys_tick++;                          \
                                       \
  if (!sys_tick){                      \
    sys_mackey++;                      \
  }


/*!
Update sytem tick.

Call this from your timer ISR.

Usable for systems where <b>1KHz</b> interrupt frequency is suitable

@see __tk_tick_10uS()
@see __tk_tick_100uS()
@see __tk_tick_1mS()
@see __tk_tick_PC()
*/
#define __tk_tick_1mS()                \
  sys_tick++;                          \
                                       \
  if (!sys_tick){                      \
    sys_mackey++;                      \
  }


/*!
Update sytem tick.

Call this from your timer ISR.

Usable for systems where <b>100Hz</b> interrupt frequency is suitable (slow system)

@see __tk_tick_10uS()
@see __tk_tick_100uS()
@see __tk_tick_1mS()
@see __tk_tick_PC()
*/
#define __tk_tick_10mS()               \
  sys_tick+=10;                        \
                                       \
  if (!sys_tick){                      \
    sys_mackey++;                      \
  }


/*!
Update sytem tick.

Call this from your timer ISR.

Usable for systems where <b>18.2Hz</b> interrupt frequency is suitable (PC system)

@see __tk_tick_10uS()
@see __tk_tick_100uS()
@see __tk_tick_1mS()
@see __tk_tick_10mS()
*/
#define __tk_tick_PC()                 \
  sys_tick+=10;                        \
                                       \
  if (!sys_tick){                      \
    sys_mackey++;                      \
  }



#endif


