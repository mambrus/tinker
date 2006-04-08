//****************************************************************************
// @Module        General Purpose Timer Unit (GPT2)
// @Filename      GPT2.H
// @Project       Dave.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XC167CI-16F40
//
// @Compiler      Keil
//
// @Codegenerator 2.4
//
// @Description   This file contains all function prototypes and macros for 
//                the GPT2 module.
//
//----------------------------------------------------------------------------
// @Date          2006-03-11 15:46:53
//
//****************************************************************************

// USER CODE BEGIN (GPT2_Header,1)
#include <../src/tk_hwclock.h>

/*
Note TinKer has choosen (atm) to work with only one pcysical timer as it's CLK2. 
The cosen timer is T6 (core timer, also called SYS_TIMER by DAvE) 
*/

void tk_getHWclock_Quality_CLK2(HWclock_stats_t *HWclock_stats);

#define tk_setHWclock_CLK2(ticks) ( GPT12E_T6 = ticks )
#define tk_getHWclock_CLK2(ticks) ( *ticks = GPT12E_T6 )
#define tk_armHWclock_CLK2()      ( ((IC_t*)(&GPT12E_T6IC))->IE = 1 )
#define tk_disarmHWclock_CLK2()   ( ((IC_t*)(&GPT12E_T6IC))->IE = 0 )

// USER CODE END



#ifndef _GPT2_H_
#define _GPT2_H_

//****************************************************************************
// @Project Includes
//****************************************************************************

// USER CODE BEGIN (GPT2_Header,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (GPT2_Header,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

//   This parameter identifies GPT2 timer 5
#define GPT2_TIMER_5 5

//   This parameter identifies GPT2 timer 6
#define SYS_TIMER 6

// USER CODE BEGIN (GPT2_Header,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (GPT2_Header,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (GPT2_Header,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (GPT2_Header,7)

// USER CODE END


//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************

void GPT2_vInit(void);

// USER CODE BEGIN (GPT2_Header,8)

// USER CODE END

//****************************************************************************
// @Macro         GPT2_vStartTmr(TimerNr) 
//
//----------------------------------------------------------------------------
// @Description   This macro starts the selected GPT2 timer. The timer 
//                continues to count from where it had stopped. The following 
//                definitions are available for TimerNr:
//                GPT2_TIMER_5
//                SYS_TIMER
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    TimerNr: 
//                The name of the timer to be started
//
//----------------------------------------------------------------------------
// @Date          2006-03-11
//
//****************************************************************************

#define GPT2_vStartTmr(TimerNr) GPT2_vStartTmr_##TimerNr()
#define GPT2_vStartTmr_GPT2_TIMER_5() GPT12E_T5CON_T5R = 1
#define GPT2_vStartTmr_SYS_TIMER() GPT12E_T6CON_T6R = 1


//****************************************************************************
// @Macro         GPT2_vStopTmr(TimerNr) 
//
//----------------------------------------------------------------------------
// @Description   This macro stops the selected GPT2 timer. The contents of 
//                the timer register remain unchanged. The remote control of 
//                the selected timer is cleared. The following definitions 
//                are available for TimerNr:
//                GPT2_TIMER_5
//                SYS_TIMER
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    TimerNr: 
//                The name of the timer to be stopped
//
//----------------------------------------------------------------------------
// @Date          2006-03-11
//
//****************************************************************************

#define GPT2_vStopTmr(TimerNr) GPT2_vStopTmr_##TimerNr()
#define GPT2_vStopTmr_GPT2_TIMER_5() GPT12E_T5CON_T5R = 0; GPT12E_T5CON_T5RC = 0
#define GPT2_vStopTmr_SYS_TIMER() GPT12E_T6CON_T6R = 0


//****************************************************************************
// @Interrupt Vectors
//****************************************************************************


// USER CODE BEGIN (GPT2_Header,9)

// USER CODE END


#endif  // ifndef _GPT2_H_
