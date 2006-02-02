/*******************************************************************
 *
 *  DESCRIPTION: tk_hwclock.h  HW clock definitions
 *
 *  AUTHOR: Michael Ambrus
 *
 *  HISTORY:    
 *
 *  Current $Revision: 1.1 $
 *
 *******************************************************************/
   

 
#ifndef TK_HWCLOCK_H
#define TK_HWCLOCK_H

/** include files **/
//#include <time.h>

/** public definitions **/

/*! 
A 32 bit value (at least) representing the time in HW ticks in the HW clock.

@note the lengt in physical time that each tick represents is different between 
different targets depending on HW timers resolution and frequency.
*/
#define HWtick_t unsigned long

/*!
Stats of the Timer "HWclock_" that the HW layer of this service is 
supporting/providing. Note that the upper part (i.e. this file) is 
generic and is supposed to be transparent over many different targets.
*/
typedef struct{
   unsigned int   freq_khz;   /*!< The frequency driving the HWclock expressed in Kilo Hertz. I.e. frequency <b>after</b> any prescaler or chain of prescalers. */
   unsigned char  res;        /*!< Number of bits this HWClock_ handles */
   HWtick_t       perPebbles; /*!< Variable holds the vaule supposed to reload the clock with in case ofperiodical operations (i.e. CLK1 operation) */
   HWtick_t       maxPebbles; /*!< Number of HW-ticks the HWclock_ can handle without fraction. Any 
                                   timeout event requireing longer than this 
                                   time needs to have it's time chopped up in 
                                   multiples of this amount of time.
                                   This value is interesting for CLK2 operations
                                   @note Make sure type holding this value is at least 32 bits long.
                                   */
}HWclock_stats_t;


/*!
Get the quality of the HWclock. Will assume this never changes after system 
startup. (lower level must assure this to be true).

HWclock_stats_t *HWclock_stats

@note HWclockNR must be either CLK1 or CLK2. CLK1 is the system timer HW and CLK2 is 
used for high-res timeout events (ptime)

@note This function must be provided by lower level BSP 
*/
#define tk_getHWclock_Quality(HWclockNR, HWclock_stats) tk_getHWclock_Quality_##HWclockNR(HWclock_stats)


/*!
Set new time in HWclock. Pending timer will be prevented from fireing and current 
time will be overwritten.

HWtick_t ticks - i.e. a non pointer variable

@note HWclockNR must be either CLK1 or CLK2. CLK1 is the system timer HW and CLK2 is 
used for high-res timeout events (ptime)

@note This function must be provided by lower level BSP and you <b>must make sure this is ATOMIC</b>
*/
#define  tk_setHWclock(HWclockNR, ticks) tk_setHWclock_##HWclockNR(ticks)

/*!
Gets the time value in the HWclock while running (i.e. count down will not be 
stopped). Pending timer will NOT be prevented from fireing while read is done. 
To make an atomic read, first use disarmHWclock.

HWtick_t *ticks - i.e. a <b>pointer return</b> variable

@note HWclockNR must be either CLK1 or CLK2. CLK1 is the system timer HW and CLK2 is 
used for high-res timeout events (ptime)

@note This function must be provided by lower level BSP and you <b>must make sure this is ATOMIC</b>
*/
#define tk_getHWclock(HWclockNR, ticks) tk_getHWclock_##HWclockNR(ticks)

/*!
Arms the HW clock so that it can fire (i.e. enables generating an interrupt).

@note HWclockNR must be either CLK1 or CLK2. CLK1 is the system timer HW and CLK2 is 
used for high-res timeout events (ptime)

@note This function must be provided by lower level BSP
*/
#define tk_armHWclock(HWclockNR) tk_armHWclock_##HWclockNR()


/*!
Disarms the HW clock so that it can NOT fire (i.e. disables generating an interrupt).

@note HWclockNR must be either CLK1 or CLK2. CLK1 is the system timer HW and CLK2 is 
used for high-res timeout events (ptime)

@note This function must be provided by lower level BSP
*/
#define tk_disarmHWclock(HWclockNR) tk_disarmHWclock_##HWclockNR()


/*------------------------ obsolete follows? -----------------------------------*/

/*!
Arms the HWclock_ to trigger at a certain "time" from now. Time is expressed in 
"ticks" and are normalized to have correspond to the actual time 
required (this is handled in this component based on the HWclock_ quality provided).

@note This function must be provided by lower level BSP
*/
//extern void armHWclock           (HWtick_t ticks);

/*!
Dissarms any pending fireing of the HWclock_. Function will return if it was 
succesfull or not. Unsucessfull will most likelly mean that the fireing 
allready happened.

@note This function must be provided by lower level BSP
*/
//extern int disarmHWclock_       ();


/** external data **/

/** internal functions **/

/** public data **/

/** private data **/

/** public functions **/

/** private functions **/

/** include lower layer **/

#if defined(WIN32) && defined(_MSVC_)
   #error "HW timers not implemented for this target yet"
#elif defined(WIN32) && defined(_BC50_)
   #error "HW timers not implemented for this target yet"
#elif defined( __C166__ )
   #include <dave/GPT1.H> 
   #include <dave/GPT2.H>
#elif defined(__GNUC__)
   #error "Implementation (only simulated \"scheduler in process\" possible) for a GNU system not done yet"
#else
   #error "Can\'t determine the target for the TINKER kernel"   
#endif




#endif

  
/*! 
 * @addtogroup CVSLOG CVSLOG
 *
 *  $Log: tk_hwclock.h,v $
 *  Revision 1.1  2006-02-02 15:40:26  ambrmi09
 *  HW clock definitions added
 *
 *  
 *******************************************************************/









