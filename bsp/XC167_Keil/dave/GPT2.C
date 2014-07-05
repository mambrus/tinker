//****************************************************************************
// @Module        General Purpose Timer Unit (GPT2)
// @Filename      GPT2.C
// @Project       Dave.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XC167CI-16F40
//
// @Compiler      Keil
//
// @Codegenerator 2.4
//
// @Description   This file contains functions that use the GPT2 module.
//
//----------------------------------------------------------------------------
// @Date          2006-03-11 15:46:53
//
//****************************************************************************

// USER CODE BEGIN (GPT2_General,1)
#include <../bsp/XC167_Keil/tk_hwtypes_keilC166.h>	//Note: This is a shaky thingy. This header must not in turn include any Keil regs.h
// USER CODE END

//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (GPT2_General,2)

// USER CODE END

//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (GPT2_General,3)

// USER CODE END

//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (GPT2_General,4)

// USER CODE END

//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (GPT2_General,5)

// USER CODE END

//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (GPT2_General,6)

// USER CODE END

//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (GPT2_General,7)

// USER CODE END

//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (GPT2_General,8)

// USER CODE END

//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (GPT2_General,9)

// USER CODE END

//****************************************************************************
// @Function      void GPT2_vInit(void)
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the GPT2 function
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

void GPT2_vInit(void)
{
	// USER CODE BEGIN (Init,2)

	// USER CODE END

	///  -----------------------------------------------------------------------
	///  Configuration of Timer Block Prescaler 2:
	///  -----------------------------------------------------------------------
	///  - prescaler for timer block 2 is 8

	///  -----------------------------------------------------------------------
	///  Configuration of the GPT2 Core Timer 5:
	///  -----------------------------------------------------------------------
	///  - timer 5 works in timer mode
	///  - prescaler factor is 8
	///  - up/down control bit is reset
	///  - external up/down control is disabled
	///  - timer 5 run bit is reset
	///  - timer 5 remote control is disabled

	GPT12E_T5CON = 0x0000;	// load timer 5 control register
	GPT12E_T5 = 0x0000;	// load timer 5 register

	///  -----------------------------------------------------------------------
	///  Configuration of the GPT2 Core Timer 6:
	///  -----------------------------------------------------------------------
	///  - timer 6 works in timer mode
	///  - prescaler factor is 8
	///  - up/down control bit is set
	///  - external up/down control is disabled
	///  - alternate output function T6OUT (P3.1) is disabled
	///  - timer 6 output toggle latch (T6OTL) is set to 0
	///  - timer 6 run bit is reset
	///  - timer 6 is not cleared on a capture

	GPT12E_T6CON = 0x1880;	// load timer 6 control register
	GPT12E_T6 = 0x30D4;	// load timer 6 register

	///  -----------------------------------------------------------------------
	///  Configuration of the GPT2 CAPREL:
	///  -----------------------------------------------------------------------
	///  - capture T5 into CAPREL is disabled
	///  - capture trigger from pin CAPIN
	///  - capure is disabled
	///  - timer 5 is not cleared on a capture
	///  - timer 5 is just captured without any correction

	GPT12E_T5CON |= 0x0000;	// load timer 5 control register
	GPT12E_CAPREL = 0x30D4;	// load CAPREL register

	///  -----------------------------------------------------------------------
	///  Configuration of the used GPT2 Port Pins:
	///  -----------------------------------------------------------------------

	///  -----------------------------------------------------------------------
	///  Configuration of the used GPT2 Interrupts:
	///  -----------------------------------------------------------------------
	///  timer 6 service request node configuration:
	//// - timer 6 interrupt is enabled but NO INTERRUPT WILL BE GENERATED
	////   because priority level is 0

	GPT12E_T6IC = 0x0040;

	// USER CODE BEGIN (GPT2_Function,3)

	// USER CODE END

}				//  End of function GPT2_vInit

// USER CODE BEGIN (GPT2_General,10)

void tk_getHWclock_Quality_CLK2(HWclock_stats_t * HWclock_stats)
{
	HWclock_stats->freq_hz = 40000 / 8;	/*40MHz, could this be determined using PLLCON? */
	HWclock_stats->res = 16;
	HWclock_stats->perPebbles = 0;	/*CLK2 not supposed to be run periodically */
	HWclock_stats->maxPebbles = 1234;	/*dummy value. Fix this */
}

// USER CODE END
