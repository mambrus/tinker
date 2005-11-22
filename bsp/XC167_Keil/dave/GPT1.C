//****************************************************************************
// @Module        General Purpose Timer Unit (GPT1)
// @Filename      GPT1.C
// @Project       Dave.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XC167CI-16F40
//
// @Compiler      Keil
//
// @Codegenerator 2.4
//
// @Description   This file contains functions that use the GPT1 module.
//
//----------------------------------------------------------------------------
// @Date          2005-11-19 14:27:40
//
//****************************************************************************

// USER CODE BEGIN (GPT1_General,1)

// USER CODE END

/*
******************************************************************************
@Note

GPT1 T2 is supposed to drive the kernel with ticks with a certain rate. Currently this rate is choosen to be 1KHz (choice btw 100KHz to 100Hz are available - which is suitable depends on the required tick resolution and overall performance of the MPU).

******************************************************************************
*/


//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (GPT1_General,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (GPT1_General,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (GPT1_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (GPT1_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (GPT1_General,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (GPT1_General,7)

// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (GPT1_General,8)

// USER CODE END


//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (GPT1_General,9)

// USER CODE END


//****************************************************************************
// @Function      void GPT1_vInit(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the GPT1 function 
//                library. It is assumed that the SFRs used by this library 
//                are in its reset state. 
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    None
//
//----------------------------------------------------------------------------
// @Date          2005-11-19
//
//****************************************************************************

// USER CODE BEGIN (Init,1)

// USER CODE END

void GPT1_vInit(void)
{
  // USER CODE BEGIN (Init,2)

  // USER CODE END

  ///  -----------------------------------------------------------------------
  ///  Configuration of Timer Block Prescaler 1:
  ///  -----------------------------------------------------------------------
  ///  - prescaler for timer block 1 is 8


  ///  -----------------------------------------------------------------------
  ///  Configuration of the GPT1 Core Timer 3:
  ///  -----------------------------------------------------------------------
  ///  - timer 3 works in timer mode
  ///  - external up/down control is disabled
  ///  - prescaler factor is 8
  ///  - up/down control bit is reset
  ///  - alternate output function T3OUT (P3.3) is disabled
  ///  - timer 3 output toggle latch (T3OTL) is set to 0
  ///  - timer 3 run bit is reset

  GPT12E_T3CON   =  0x0000;      // load timer 3 control register
  GPT12E_T3      =  0x0000;      // load timer 3 register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the GPT1 Auxiliary Timer 2:
  ///  -----------------------------------------------------------------------
  ///  - timer 2 works in timer mode
  ///  - external up/down control is disabled
  ///  - prescaler factor is 8
  ///  - up/down control bit is set
  ///  - timer 2 run bit is reset

  GPT12E_T2CON   =  0x0080;      // load timer 2 control register
  GPT12E_T2      =  0x1387;      // load timer 2 register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the GPT1 Auxiliary Timer 4:
  ///  -----------------------------------------------------------------------
  ///  - timer 4 works in timer mode
  ///  - external up/down control is disabled
  ///  - prescaler factor is 8
  ///  - up/down control bit is reset
  ///  - timer 4 run bit is reset

  GPT12E_T4CON   =  0x0000;      // load timer 4 control register
  GPT12E_T4      =  0x0000;      // load timer 4 register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used GPT1 Port Pins:
  ///  -----------------------------------------------------------------------


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used GPT1 Interrupts:
  ///  -----------------------------------------------------------------------
  ///  timer 2 service request node configuration:
  ///  - timer 2 interrupt priority level (ILVL) = 15
  ///  - timer 2 interrupt group level (GLVL) = 1
  ///  - timer 2 group priority extension (GPX) = 0

  GPT12E_T2IC    =  0x007D;     

  ///  Use PEC channel 5 for GPT1 T2 INT:
  ///  - normal interrupt
  ///  - pointers are not modified
  ///  - transfer a word
  ///  - service End of PEC interrrupt by a EOP interrupt node is disabled

  PECC5          =  0x0000;      // load PECC5 control register



  // USER CODE BEGIN (GPT1_Function,3)

  // USER CODE END

} //  End of function GPT1_vInit


//****************************************************************************
// @Function      void GPT1_viTmr2(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the GPT1 timer 2. 
//                It is called up in the case of over or underflow of the 
//                timer 2 register.
//                If the incremental interface mode is selected and the 
//                interrupt for this mode is not disabled it is called up if 
//                count edge or count direction was detected.
//                
//                Please note that you have to add application specific code 
//                to this function.
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    None
//
//----------------------------------------------------------------------------
// @Date          2005-11-19
//
//****************************************************************************

// USER CODE BEGIN (Tmr2,1)
//unsigned long idata sys_mackey = 0;
//unsigned long idata sys_tick = 0;
#define TICK_OWNER
#include <../src/tk_tick.h>

// USER CODE END

void GPT1_viTmr2(void) interrupt T2INT
{
  // USER CODE BEGIN (Tmr2,2)

  GPT1_vLoadTmr(GPT1_TIMER_2,0x1387);
  __tk_tick_1mS();

/*
  sys_tick++;

  if (!sys_tick){ //Will occure every 4294967,295 seconds (1193 hrs and	aprox 3 minuts)
    sys_mackey++;
  }
*/
  // USER CODE END


  // USER CODE BEGIN (Tmr2,5)

  // USER CODE END

} //  End of function GPT1_viTmr2




// USER CODE BEGIN (GPT1_General,10)

// USER CODE END

