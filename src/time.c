#include <time.h>

//! An internal scaling factor that needs to be matched against the timer 
//  resolution so that the POSIX requirements for CLOCKS_PER_SEC is fullfilled.
//  The current resolution is 1000 tics per second (1kHZ interrupts). The 
//  POSIX CLOCKS_PER_SEC is preset to 1*10e6. How many clock_ticks is there 
//  (or would there be) in a sys_tick.

#define CLKS_PER_TCK 1000 



//! This is the internal data of the timer system. Observe that accessing this 
//  variable is not thread/interrupt safe. This needs to be revised. 
//
//  @note Also very important - always include the memory type "language 
//  extention" when declaring external cariables like this.
extern unsigned long idata sys_tick;
extern unsigned long idata sys_mackey;



//return (clock_t)(-1);

clock_t clock(){
   return CLKS_PER_TCK * sys_tick;
}


