//****************************************************************************
// @Module        Project Settings
// @Filename      MAIN.C
// @Project       Dave.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XC167CI-16F40
//
// @Compiler      Keil
//
// @Codegenerator 2.4
//
// @Description   This file contains the project initialization function.
//
//----------------------------------------------------------------------------
// @Date          2006-03-11 15:46:50
//
//****************************************************************************

// USER CODE BEGIN (MAIN_General,1)

#include <kernel/src/tk_tick.h>

/*
unsigned char xhuge malloc_bigpool [0x10000];
*/

unsigned char far malloc_padding [0x6];
unsigned char far malloc_mempool [0x3FFA];

//Neither of the following works
//unsigned char xhuge malloc_mempool [0x10000];
//unsigned char xhuge malloc_mempool [0x4000];
//unsigned char xhuge malloc_mempool [0x1000];


// USER CODE END



/*
******************************************************************************
@Note

Notes of EBC Module:
External RAM def added by Michael Ambrus to fit PhyCoreXC167. 

PhyCore module has 128k external RAN starting at adress 0x200000

Bus arbitration enabled

******************************************************************************
*/


//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (MAIN_General,2)
//#include <XC167.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tk.h>            
#include <errno.h>
#include <time.h> 
#include <assert.h>


#pragma code

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (MAIN_General,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (MAIN_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (MAIN_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (MAIN_General,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (MAIN_General,7)

// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (MAIN_General,8)

// USER CODE END


//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (MAIN_General,9)

// USER CODE END


//****************************************************************************
// @Function      void MAIN_vInit(void) 
//
//----------------------------------------------------------------------------
// @Description   This function initializes the microcontroller.
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

void MAIN_vInit(void)
{
  // USER CODE BEGIN (Init,2)

  // USER CODE END

  ///  -----------------------------------------------------------------------
  ///  Configuration of the System Clock:
  ///  -----------------------------------------------------------------------
  ///  - VCO clock used, input clock is connected
  ///  - input frequency is 16,000 MHz
  ///  - VCO output frequency 100 .. 150 MHz
  //// - system clock is too high (40 MHz)

  MAIN_vUnlockProtecReg();       // unlock write security
  PLLCON         =  0x7814;      // load PLL control register


  //// -----------------------------------------------------------------------
  //// Begin of Important Settings for the Start-Up File
  //// -----------------------------------------------------------------------
  ///  All following settings must be set in the start-up file. You can use 
  ///  DAvE's project file (*.dpt) to include this register values into your 
  ///  compiler EDE.

    ///  ---------------------------------------------------------------------
    ///  Initialization of the CPUCON1 Register:
    ///  ---------------------------------------------------------------------
    ///  - space between two vectors is 2 words
    ///  - DISWDT executable until End of Init
    ///  - segmentation is enabled
    ///  - switch context is interruptible
    ///  - branch prediction is enabled
    ///  - zero cycle jump function is enabled

    //// this register must be set in the start-up file
    //// CPUCON1  =  0x0007

    ///  ---------------------------------------------------------------------
    ///  Initialization of the VECSEG Register:
    ///  ---------------------------------------------------------------------
    ///  - start from internal program memory

    //// this register must be set in the start-up file
    //// VECSEG  =  0x00C0

    ///  ---------------------------------------------------------------------
    ///  Initialization of the SYSCON0 Register:
    ///  ---------------------------------------------------------------------

    //// this register must be set in the start-up file
    //// SYSCON0  =  0x0000

    ///  ---------------------------------------------------------------------
    ///  Initialization of the SYSCON1 Register:
    ///  ---------------------------------------------------------------------
    ///  clock prescaler for system is fpll / 1

    //// this register must be set in the start-up file
    //// SYSCON1  =  0x0000

    ///  ---------------------------------------------------------------------
    ///  Initialization of the SYSCON3 Register:
    ///  ---------------------------------------------------------------------

    //// this register must be set in the start-up file
    //// SYSCON3  =  0x0000

    ///  ---------------------------------------------------------------------
    ///  Initialization of EBC
    ///  ---------------------------------------------------------------------

    //// this register must be set in the start-up file
    //// EBCMOD1  =  0x003F

    ///  EBC is enabled
    ///  pin #BHE is enabled
    ///  #WR and #BHE retain their normal function
    ///  pin ALE is enabled
    ///  as segment address are 4 pins enabled ( A[19] .. A[16] )
    ///  #CS4 .. #CS0 are enabled
    ///  pin #READY is disabled
    ///  bus arbitration is enabled, P6.7 .. P6.5 serve as #BREQ, #HLDA and 
    ///  #HOLD
    ///  bus arbiter acts in master mode

    //// this register must be set in the start-up file
    //// EBCMOD0  =  0x4154

  ///  - P6.5 is used for External hold request input (HOLD_n)
  ///  - P6.6 is used for Hold acknowledge output (HLDA_n)
  ///  - P6.7 is used for Bus request output (BREQ_n)

  DP6  = (DP6  & ~(uword)0x00C0) | 0x00C0;    //set direction register
    ///  ---------------------------------------------------------------------
    ///  ---------- chip select 0 is enabled ----------
    ///  ---------------------------------------------------------------------
    ///  - 16-bit Multiplexed Bus
    ///  access time is controlled by bitfield PHE0

    //// this register must be set in the start-up file
    //// FCONCS0  =  0x0031

    ///  Phase A: 3 clock cycles
    ///  Phase B: 1 clock cycles
    ///  Phase C: 0 clock cycles
    ///  Phase D: 0 clock cycles
    ///  Phase E: 10 clock cycles
    ///  Read Phase F: 0 clock cycles
    ///  Write Phase F: 3 clock cycles

    //// this register must be set in the start-up file
    //// TCONCS0  =  0x6243

    ///  ---------------------------------------------------------------------
    ///  ---------- chip select 1 is enabled ----------
    ///  ---------------------------------------------------------------------
    ///  - 16-bit Multiplexed Bus
    ///  access time is controlled by bitfield PHE1

    //// this register must be set in the start-up file
    //// FCONCS1  =  0x0031

    ///  Phase A: 0 clock cycles
    ///  Phase B: 1 clock cycles
    ///  Phase C: 1 clock cycles
    ///  Phase D: 1 clock cycles
    ///  Phase E: 2 clock cycles
    ///  Read Phase F: 1 clock cycles
    ///  Write Phase F: 1 clock cycles

    //// this register must be set in the start-up file
    //// TCONCS1  =  0x2868

    ///  address range size selection: 512 KByte
    ///  start address (A23..A0): 0x400000

    //// this register must be set in the start-up file
    //// ADDRSEL1  =  0x4007

    ///  ---------------------------------------------------------------------
    ///  ---------- chip select 2 is disabled ----------
    ///  ---------------------------------------------------------------------


    ///  ---------------------------------------------------------------------
    ///  ---------- chip select 3 is disabled ----------
    ///  ---------------------------------------------------------------------


    ///  ---------------------------------------------------------------------
    ///  ---------- chip select 4 is disabled ----------
    ///  ---------------------------------------------------------------------


  //// -----------------------------------------------------------------------
  //// End of Important Settings for the Start-Up File
  //// -----------------------------------------------------------------------


  //   -----------------------------------------------------------------------
  //   Initialization of the Peripherals:
  //   -----------------------------------------------------------------------

  //   initializes the Parallel Ports
  IO_vInit();

  //   initializes the Asynchronous/Synchronous Serial Interface (ASC0)
  ASC0_vInit();

  //   initializes the Asynchronous/Synchronous Serial Interface (ASC1)
  ASC1_vInit();

  //   initializes the General Purpose Timer Unit (GPT1)
  GPT1_vInit();

  //   initializes the General Purpose Timer Unit (GPT2)
  GPT2_vInit();

  // USER CODE BEGIN (Init,3)

  // USER CODE END

  //   globally enable interrupts
  PSW_IEN        =  1;          

} //  End of function MAIN_vInit


//****************************************************************************
// @Function      void MAIN_vUnlockProtecReg(void) 
//
//----------------------------------------------------------------------------
// @Description   This function makes it possible to write one protected 
//                register. After calling of this function and write on the 
//                protected register is the security level set to low 
//                protected mode.
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

// USER CODE BEGIN (UnlockProtecReg,1)

// USER CODE END

void MAIN_vUnlockProtecReg(void)
{
  ubyte ubPASSWORD;

  if((SCUSLS & 0x1800) == 0x0800)      // if low protected mode
  {

    ubPASSWORD = SCUSLS & 0x00FF;
    ubPASSWORD = ~ubPASSWORD;
    SCUSLC = 0x8E00 | ubPASSWORD;      // command 4

  }  // end if low protected mode

  if((SCUSLS & 0x1800) == 0x1800)      // if write protected mode
  {
    SCUSLC = 0xAAAA;                   // command 0
    SCUSLC = 0x5554;                   // command 1

    ubPASSWORD = SCUSLS & 0x00FF;
    ubPASSWORD = ~ubPASSWORD;

    SCUSLC = 0x9600 | ubPASSWORD;      // command 2
    SCUSLC = 0x0800;                   // command 3; new PASSWOR is 0x00

    ubPASSWORD = SCUSLS & 0x00FF;
    ubPASSWORD = ~ubPASSWORD;
    SCUSLC = 0x8E00 | ubPASSWORD;      // command 4

  }  // end if write protected mode

} //  End of function MAIN_vUnlockProtecReg


//****************************************************************************
// @Function      void main(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the main function.
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

// USER CODE BEGIN (Main,1)

void SetWdt() 
{
   WDI = 1;
   WDI = 0;
}
void SetWdt1()
{
   IO_vSetPin(WDI);
   IO_vResetPin(WDI);
}

// USER CODE END

void main(void)
{
  // USER CODE BEGIN (Main,2)  
  
   init_mempool (&malloc_mempool, sizeof(malloc_mempool));


      //Below is not needed anymore, better workaround made (including *.asm in start.a66 and using another symbol for PHE1 that was generated correctlly)
      //TCONCS1  =  0x2868;  //<-- This is in the wrong order, but should work since no acces is done yet
      //*((uword volatile *) 0xEE18) = 0x2868;

   // USER CODE END

  MAIN_vInit();

  // USER CODE BEGIN (Main,4)
      GPT1_vLoadTmr(GPT1_TIMER_2,4999);
      GPT1_vCountDown(GPT1_TIMER_2);
      GPT1_vStartTmr(GPT1_TIMER_2);

      //Wraparound testing that occures once every 71.6 minuts 
      
      /* Note that systimer (sys_mickey) wraps once every 49.7 days but
      kernel native time-keeping wraps 1000 times more often (71.6
      minutes). This is due to that current precision on sys_time is in
      mS, but kernel precision is in uS as a preparation to that the
      \ref clock will be replaced by a higher precision time function
      (\uptime or something similar). */
	   
      //sys_mickey = 0xFFFFE666;

      _tk_main();
      tk_exit(TK_OK);
      assert("You should never reach this line" == NULL);

   // USER CODE END

} //  End of function main



// USER CODE BEGIN (MAIN_General,10)

// USER CODE END

