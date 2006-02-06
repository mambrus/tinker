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
// @Date          2006-02-04 13:07:12
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


#include <../bsp/XC167_Keil/tk_hwtypes_keilC166.h> //Note: This is a shaky thingy. This header must not in turn include any Keil regs.h

/*!

@name TIMECONSTANTS

@section intro Introduction

To make CLK1 run as accurate and precise as possible you have to carfully
adjust a couple of constants. The contants themselves can be cathegoriced as follows:

- Pre-determined constants    Comes from some physical fact (like the clock 
                              frequency driving the CPU and CLK(s)). 
- Compensation constants      These you are supposed to meassure and adjust.
- Derived constants           Defines based on the above. Used for compile time
                              checking and avoidance of complex and 
                              timeconsuming run-time calculations.

@section constants Constants
- REGVAL      Theoretical value supposed to go into the clock register
- RELOADVAL   Compensated value after ISR and flck inaccurancy is compensated
- ISRTIME     Latency time from interrupt to action expressed in nano seconds - 
              offset value <b>this one you meassure and set</b>
- ISRPEB      Same as above, but expressed in the current pepple unit
- FCLK        Frequency driving the CPU
- PRES        Prescalor selected <b>this one you set</b>
- PERT        Interrupt period time, expressed in mS <b>this one you set</b>

@section formulas Formulas explained
1) <p>

 \f[
    |I_2|=\left| \int_{0}^T \psi(t) 
             \left\{ 
                u(a,t)-
                \int_{\gamma(t)}^a 
                \frac{d\theta}{k(\theta,t)}
                \int_{a}^\theta c(\xi)u_t(\xi,t)\,d\xi
             \right\} dt
          \right|
  \f]


\f[
   \frak{REGVALr + 1} {FCLK / PRES} = 
   \frak 1000 PERT 
\f]

\f[
   REGVAL = (PERT * FLCLK)/(1000 * PRES) -1
\f]
                                                         


*/

//@{
#define PERT            6                 //!<@ref constants
#define FCLK            40000000UL
#define PRES            8
//#define ISRTIME         3000           //3.000 uS latency
//#define ISRPEB          0x0F   //works for 13ms
#define ISRPEB          0x0D      //works for 6ms
//#define ISRPEB          0x09     //works for 1ms

#define REGVAL          ((FCLK*PERT)/(1000*PRES) -1)
#define RELOADVAL       (REGVAL - ISRPEB)
//@}


#define _REGVALUE       0x1387 //1mS
#define _REGVAL          0xFDE7   //13mS
#define _ISRPEB          0x0F     //Meassured offset correction
#define _RELOADVAL       (REGVAL - ISRPEB)



#if ( REGVAL > 0xFFFF )
#error "HW clock counter exceeds limits. You choosed either a \
        too long CLK1 ISR period time or a to high presision"
#endif

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
// @Date          2006-02-04
//
//****************************************************************************

// USER CODE BEGIN (Init,1)

// USER CODE END

void GPT1_vInit(void)
{
  // USER CODE BEGIN (Init,2)
  unsigned int tmp1,tmp2;

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
  GPT12E_T2      =  0xFDE7;      // load timer 2 register

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

     /* testing the bit fields */
     tmp1 = ((GPT1_ControlRegCore_t*)&GPT12E_T3CON)->BPS1;
     tmp2 = tmp1;
     ((GPT1_ControlRegCore_t*)&GPT12E_T3CON)->BPS1 = tmp2;
     
     tmp1 = ((GPT1_ControlRegAux_t*)&GPT12E_T2CON)->TaI;
     tmp2 = tmp1;
     ((GPT1_ControlRegAux_t*)&GPT12E_T2CON)->TaI = tmp2;
    /* testing the bit fields */


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
// @Date          2006-02-04
//
//****************************************************************************

// USER CODE BEGIN (Tmr2,1)
//unsigned long idata sys_mackey = 0;
//unsigned long idata sys_mickey = 0;
//#define TICK_OWNER
#include <../src/tk_tick.h>

// USER CODE END

void GPT1_viTmr2(void) interrupt T2INT
{
  // USER CODE BEGIN (Tmr2,2)

  GPT1_vLoadTmr(GPT1_TIMER_2,RELOADVAL);
  //_tk_tick_1mS();
  _tk_tick_advance_mS(PERT);

/*
  sys_mickey++;

  if (!sys_mickey){ //Will occure every 4294967,295 seconds (1193 hrs and aprox 3 minuts)
    sys_mackey++;
  }
*/
  // USER CODE END


  // USER CODE BEGIN (Tmr2,5)

  // USER CODE END

} //  End of function GPT1_viTmr2




// USER CODE BEGIN (GPT1_General,10)

void tk_getHWclock_Quality_CLK1(HWclock_stats_t *HWclock_stats){
   HWclock_stats->freq_khz     = (FCLK / 1000) / PRES; /*40MHz, could this be determined using PLLCON?*/
   HWclock_stats->res          = 16;
   HWclock_stats->perPebbles   = RELOADVAL;  /*Trim this to hold the desired timeout interval time*/
   HWclock_stats->maxPebbles   = 1234;       /*dummy value. Fix this*/

}

// USER CODE END
























/*

1)

   REGVAL + 1        1000                                
  ------------  =   ------   =>                          
   FCLK / PRES       PERT                                
                                                         
                                                         
                     PERT * FLCLK                        
   REGVAL       =  --------------  - 1                   
                      1000 * PRES                        
                                                         

                                                         
2)                                                       

                                               \         
  RELOADVAL     =  REGVAL - ISRPEB             |         
                                               |         
                                                >        
                   ISRTIME * FLCLK/PRES        |         
  ISRPEB        =  --------------------        |         
                       10000000000             /         
                                                         
  RELOADVAL = REGVAL - ISRTIME * FLCLK/PRES * 1000       
                                                         


\f[
   (REGVAL + 1) / (FCLK / PRES) = 1000/PERT 
\f]

\f[
   REGVAL = (PERT * FLCLK)/(1000 * PRES) -1
\f]


*/