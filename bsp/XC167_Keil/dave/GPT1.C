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
// @Date          2006-03-11 15:46:52
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

#include <kernel/include/assert.h>
#include <../bsp/XC167_Keil/tk_hwtypes_keilC166.h> //Note: This is a shaky thingy. This header must not in turn include any Keil regs.h

/*! 
If this is defied, it means that the whole program will break if
timer interrupt is delayed for too long. Great for debugging scheduler
critical sections protection. This is done by turning ISR's on and off
since no other means are available (in that particular case). Still we
do want to know that ISR are not turned of for to long, or else our
systimer will start loosing ticks. Being a little to late is OK, since
readout's of the time is done by combining systimer with HW 
*/
#define MONITOR_TISR_LATENCY 
#undef  MONITOR_TISR_LATENCY 

/*!

@name TIMECONSTANTS

<h2>Introduction</h2>
To make CLK1 run as accurate and precise as possible you have to
carefully
adjust a couple of constants. The constants themselves can be
categorized as follows:

- Pre-determined constants -  Physical facts (like the clock 
                              frequency driving the CPU and CLK(s)).
                               
- Compensation constants   -  These you are supposed to measure and
                              adjust.
                              
- Derived constants        -  Defines based on the above. Used for compile time
                              checking and avoidance of complex and 
                              time consuming run-time calculations. The
                              really nice thing about expressimg
                              complex relations with these macros, is
                              that they are calculated at compile time and
                              replaces with a numerical constant value.
                              I.e. the CPU is not spending time doing
                              this in run-time, but most importantly,
                              the <n>time for using such a value</b> is also
                              constant, whereas calculating it first and
                              then using it can differ depending on the
                              values themselves.

<h2>The constants</h2>
- REGVAL \f$\bf R\f$           - Theoretical value supposed to go into the 
                                 clock register

- RELOADVAL \f$\bf R_L\f$      - Compensated value after ISR and flck 
                                 inaccuracy is compensated

- ISRTIME \f$\bf T_{isr}\f$    - Latency time from interrupt to action 
                                 expressed in nano seconds (i.e. offset
                                 value). The entity is not used, it's
                                 only mentioned for reference. The
                                 reason is that it's much better to use
                                 the unit "pebble time", since this is
                                 directly proportional to the CPU clock
                                 (CPU clock drives both HW clock and the
                                 instruction pointer). \f$T_{isr}\f$ for
                                 a XC167 running at 40MHz is however
                                 \f$T_i * T_{pebble} = 8.5001 *
                                 200nS \approx 1,7uS\f$ (based on
                                 measurements
                                 and formulas defined below).
                                                 
- ISRPEB \f$\bf T_i\f$         - Same as above, but expressed in the
                                 current pebble unit (scaled X 1000000).
                                 T_i is a constant and will depend
                                 <b>only</b> on the CPU frequency. So in
                                 case you don't change \f$\Phi_0\f$,
                                 this value should remain untouched,
                                 otherwise <em>this one you measure &
                                 set</em>.

- FCLK \f$\bf \Phi_0\f$        - Theoretical frequency driving the CPU

- X_CLK \f$\bf x\f$            - Scaling factor that states the how much off 
                                 the oscillator is. (scaled X 1000000).
                                 <em>This one you measure & set</em>

- CMPPEB  \f$\bf I_P\f$        - Derived compensation value expressed in pebble 
                                 units

- PRES \f$\bf \eta \f$         - Prescalor selected <em>this one you
                                 set</em>

- PERT \f$\bf \omega \f$       - Interrupt period time, expressed in mS
                                 <em>this one you set</em>


<h2>Formulas explained</h2>
The <em>theoretical</em> reload value based on frequency and prescalor:

\f[
   \frac{R + 1} {\phi / \eta} =
   \frac {1000}{\omega}
\f]

\f[
   R = \frac {\phi / \eta*1000}{\omega} -1
\f]



The <em>actual</em> reload (i.e. compensated) value:

\f[
   \left .
      \matrix{
         R_L = R - I_P \cr
         I_P = T_i + R_Lx \cr
      }
   \right \}
   \Rightarrow 
\f]


\f[
   R_L = R - \underbrace{( T_i + R_Lx)}
\f]

But since we have meassured values of \f$ I_P \f$ allready, we need only express an equation system of two meassurements:


\f[
   \left .
      \matrix{
         I_{P_1} = T_i + R_{L_1}x \cr
         I_{P_2} = T_i + R_{L_2}x \cr
      }
   \right \}
   \Rightarrow 
\f]

The two unknowns that we seek are \f$ T_i \f$ and \f$ x \f$, hence we can derive the following expressions:


(1)

\f[
   R_{L_1} = I_{P_1} - T_i
\f]

\f[
   x = \frac{ I_{P_1} - T_i }{  R_{L_1} }
\f]


(2)

\f[
   T_i = I_{P_2} - R_{L_2}x 
\f]


(2 in 1) \f$ \Rightarrow  \f$


\f[
   x = \frac{I_{P_1} - ( I_{P_2} - R_{L_2} ) }
            { R_{L_1} }

\f]

\f[

   x = \frac{I_{P_1} - I_{P_2}  }
            { R_{L_1} }
     +
       \frac{R_{L_2}x }
          { R_{L_1} }
\f]


\f[

   x - \frac{R_{L_2}x }
          { R_{L_1} } 
   =
   \frac{I_{P_1} - I_{P_2}  }
            { R_{L_1} }

\f]

\f[
   R_{L_1}x - R_{L_2}x = I_{P_1} - I_{P_2}
\f]

\f[
   x(R_{L_1} - R_{L_2}) = I_{P_1} - I_{P_2}
\f]

\f[
   x = \frac{ I_{P_1} - I_{P_2} }
         { R_{L_1} - R_{L_2} }    
\f]



With actual numbers \f$ \Rightarrow  \f$



\f[
   x =  \frac{ 15 - 9 }
        { 64999 - 4999 }

\f]

\f[
   \bf {x = 1*10^{-4}}
\f]


and..


\f[
   T_i = 9 - 4999*10^{-4}
\f]

\f[
   \bf {T_i = 8.5001}
\f]


<hr>


*/

//@{

#define FCLK            40000000ul
#define PRES            8
#define PERT            1ul

#define REGVAL          ((FCLK*PERT)/(1000*PRES) -1)

#define X_CLK           100

//Possibly not needed anymore, since self adjusting timer ISR introduced
/*
#define ISRPEB          8500100   //Original calibration
#define ISRPEB          8490100   //4 secs per 60s too fast               +66700 ppm
#define ISRPEB          6000000   //15 sec 279s too fast
#define ISRPEB          1000000   //5  sec on 14:14:43 (51 283s) to slow  -97.5 ppm

#define ISRPEB          1011971   
*/
#define ISRPEB          1510000

#define CMPPEB           ((ISRPEB + X_CLK * REGVAL)/1000000)

//#define ISRTIME         3000           //3.000 uS latency
//#define CMPPEB          0x0F   //!< Non derived value that worked best for 13ms
//#define CMPPEB          0x0D   //!< Non derived value that worked best for 6ms
//#define CMPPEB          0x09   //!< Non derived value that worked best for 1ms

#define RELOADVAL       (REGVAL - CMPPEB)
//@}


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
// @Date          2006-03-11
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
// @Date          2006-03-11
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
  signed int pebbRest;

  pebbRest = GPT1_uwReadTmr(GPT1_TIMER_2);
  pebbRest *= -1;

  #ifdef MONITOR_TISR_LATENCY
  assert(RELOADVAL > pebbRest );                    //<- Interrupt flag has been turned off for too long. Missed one whole event (at least).
  #endif

  //The following row CAN actually occure. But the test should work at 
  //least for several minuts. Renable to thest your config.
  //assert( 0 < pebbRest );                           //<- Timer not counting past the event, i.e. latency correction not possible. Wrong config, correct your timer setup plz...
  
  GPT1_vLoadTmr(GPT1_TIMER_2,RELOADVAL+GPT1_uwReadTmr(GPT1_TIMER_2));  //<- Correct with the actual value. Notice the sign (+), this is intentional.
  _tk_tick_advance_mS(PERT);

  // USER CODE END


  // USER CODE BEGIN (Tmr2,5)

  // USER CODE END

} //  End of function GPT1_viTmr2




// USER CODE BEGIN (GPT1_General,10)

#define CLKOFS ((FCLK * X_CLK)/1000000) //!< Helper constant used locally in tk_getHWclock_Quality_CLK1

void tk_getHWclock_Quality_CLK1(HWclock_stats_t *HWclock_stats){
   HWclock_stats->freq_hz      = ((FCLK - CLKOFS) / PRES); 
   HWclock_stats->res          = 16;
   HWclock_stats->perPebbles   = RELOADVAL;  /*The actual period time in pebbles time-unit*/
   HWclock_stats->maxPebbles   = REGVAL;     /*The theoretical period time in pebbles time-unit*/
}

// USER CODE END

