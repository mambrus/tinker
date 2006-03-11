//****************************************************************************
// @Module        Parallel Ports
// @Filename      IO.C
// @Project       Dave.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XC167CI-16F40
//
// @Compiler      Keil
//
// @Codegenerator 2.4
//
// @Description   This file contains functions that use the IO module.
//
//----------------------------------------------------------------------------
// @Date          2006-03-11 15:46:51
//
//****************************************************************************

// USER CODE BEGIN (IO_General,1)

// USER CODE END



//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (IO_General,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (IO_General,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (IO_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (IO_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (IO_General,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (IO_General,7)

// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (IO_General,8)

// USER CODE END


//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (IO_General,9)

// USER CODE END


//****************************************************************************
// @Function      void IO_vInit(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the IO function 
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

// USER CODE BEGIN (IO_Function,1)

// USER CODE END

void IO_vInit(void)
{
  // USER CODE BEGIN (IO_Function,2)

  // USER CODE END

  ///  -----------------------------------------------------------------------
  ///  General Port Settings:
  ///  -----------------------------------------------------------------------
  PICON          =  0x0000;      // load input configuration register

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P0H:
  ///  -----------------------------------------------------------------------
  ///  P0H.0 is used as alternate output for the Port Pin (AD8)
  ///  P0H.1 is used as alternate output for the Port Pin (AD9)
  ///  P0H.2 is used as alternate output for the Port Pin (AD10)
  ///  P0H.3 is used as alternate output for the Port Pin (AD11)
  ///  P0H.4 is used as alternate output for the Port Pin (AD12)
  ///  P0H.5 is used as alternate output for the Port Pin (AD13)
  ///  P0H.6 is used as alternate output for the Port Pin (AD14)
  ///  P0H.7 is used as alternate output for the Port Pin (AD15)

  ///  P0H.0 - P0H.3 output driver characteristic: strong driver
  ///  P0H.4 - P0H.7 output driver characteristic: strong driver

  ///  P0H.0 - P0H.3 output edge characteristic: sharp edge mode
  ///  P0H.4 - P0H.7 output edge characteristic: sharp edge mode

  P0H            =  0x0000;      // load data register
  POCON0H        =  0x0000;      // load output control register
  DP0H           =  0x0000;      // load direction register

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P0L:
  ///  -----------------------------------------------------------------------
  ///  P0L.0 is used as alternate output for the Port Pin (AD0)
  ///  P0L.1 is used as alternate output for the Port Pin (AD1)
  ///  P0L.2 is used as alternate output for the Port Pin (AD2)
  ///  P0L.3 is used as alternate output for the Port Pin (AD3)
  ///  P0L.4 is used as alternate output for the Port Pin (AD4)
  ///  P0L.5 is used as alternate output for the Port Pin (AD5)
  ///  P0L.6 is used as alternate output for the Port Pin (AD6)
  ///  P0L.7 is used as alternate output for the Port Pin (AD7)

  ///  P0L.0 - P0L.3 output driver characteristic: strong driver
  ///  P0L.4 - P0L.7 output driver characteristic: strong driver

  ///  P0L.0 - P0L.3 output edge characteristic: sharp edge mode
  ///  P0L.4 - P0L.7 output edge characteristic: sharp edge mode

  P0L            =  0x0000;      // load data register
  POCON0L        =  0x0000;      // load output control register
  DP0L           =  0x0000;      // load direction register

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P1H:
  ///  -----------------------------------------------------------------------
  ///  - no pin of port P1H is used

  P1H            =  0x0000;      // load data register
  ALTSEL0P1H     =  0x0000;      // load alternate output function select 
                                 // register 0
  POCON1H        =  0x0000;      // load output control register
  DP1H           =  0x0000;      // load direction register

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P1L:
  ///  -----------------------------------------------------------------------
  ///  - no pin of port P1L is used

  P1L            =  0x0000;      // load data register
  ALTSEL0P1L     =  0x0000;      // load alternate output function select 
                                 // register 0
  POCON1L        =  0x0000;      // load output control register
  DP1L           =  0x0000;      // load direction register

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P2:
  ///  -----------------------------------------------------------------------
  ///  P2.8 is used as general purpose output
  ///  - push/pull output is selected
  ///  - the pin status is low level

  ///  P2.8 - P2.15 threshold type: TTL input

  ///  P2.8 - P2.11 output driver characteristic: strong driver
  ///  P2.12 - P2.15 output driver characteristic: strong driver

  ///  P2.8 - P2.11 output edge characteristic: sharp edge mode
  ///  P2.12 - P2.15 output edge characteristic: sharp edge mode

  ODP2           =  0x0000;      // load open-drain register
  P2             =  0x0000;      // load data register
  ALTSEL0P2      =  0x0000;      // load alternate output function select 
                                 // register 0
  POCON2         =  0x0000;      // load output control register
  DP2            =  0x0100;      // load direction register

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P3:
  ///  -----------------------------------------------------------------------
  ///  P3.0 is used as alternate output for the ASC1 Transmit Data Output 
  ///  (TxDA1)
  ///  - push/pull output is selected
  ///  P3.1 is used as alternate input for the ASC1 Receive data Input (RxDA1)
  ///  P3.10 is used as alternate output for the ASC Transmit Data Output 
  ///  (TxDA0)
  ///  - push/pull output is selected
  ///  P3.11 is used as alternate input for the ASC0 Receive data Input 
  ///  (RxDA0)
  ///  P3.12 is used as alternate output for the Byte High Enable Output 
  ///  (BHE_n)

  ///  P3.0 - P3.7 threshold type: TTL input
  ///  P3.8 - P3.15 threshold type: TTL input

  ///  P3.0 - P3.3 output driver characteristic: strong driver
  ///  P3.4 - P3.7 output driver characteristic: strong driver
  ///  P3.8 - P3.11 output driver characteristic: strong driver
  ///  P3.12 - P3.15 output driver characteristic: strong driver

  ///  P3.0 - P3.3 output edge characteristic: sharp edge mode
  ///  P3.4 - P3.7 output edge characteristic: sharp edge mode
  ///  P3.8 - P3.11 output edge characteristic: sharp edge mode
  ///  P3.12 - P3.15 output edge characteristic: sharp edge mode

  ODP3           =  0x0000;      // load open-drain register
  P3             =  0x0000;      // load data register
  ALTSEL0P3      =  0x0000;      // load alternate output function select 
                                 // register 0
  ALTSEL1P3      =  0x0000;      // load alternate output function select 
                                 // register 1
  POCON3         =  0x0000;      // load output control register
  DP3            =  0x0000;      // load direction register

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P4:
  ///  -----------------------------------------------------------------------
  ///  P4.0 is used as alternate input for the Port Pin (A16)
  ///  P4.1 is used as alternate input for the Port Pin (A17)
  ///  P4.2 is used as alternate input for the Port Pin (A18)
  ///  P4.3 is used as alternate input for the Port Pin (A19)

  ///  P4.0 - P4.7 threshold type: TTL input

  ///  P4.0 - P4.3 output driver characteristic: strong driver
  ///  P4.4 - P4.7 output driver characteristic: strong driver

  ///  P4.0 - P4.3 output edge characteristic: sharp edge mode
  ///  P4.4 - P4.7 output edge characteristic: sharp edge mode

  ODP4           =  0x0000;      // load open-drain register
  P4             =  0x0000;      // load data register
  ALTSEL0P4      =  0x0000;      // load alternate output function select 
                                 // register 0
  ALTSEL1P4      =  0x0000;      // load alternate output function select 
                                 // register 1
  POCON4         =  0x0000;      // load output control register
  DP4            =  0x0000;      // load direction register

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P5:
  ///  -----------------------------------------------------------------------
  ///  - no pin of port P5 is used

  P5             =  0x0000;      // load data register

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P6:
  ///  -----------------------------------------------------------------------
  ///  P6.0 is used as alternate input for the Chip select (CS0_n)
  ///  P6.1 is used as alternate input for the Chip select (CS1_n)
  ///  P6.2 is used as alternate input for the Chip select (CS2_n)
  ///  P6.3 is used as alternate input for the Chip select (CS3_n)
  ///  P6.4 is used as alternate input for the Chip select (CS4_n)
  ///  P6.5 is used as alternate input for the External hold request input 
  ///  (HOLD_n)
  ///  P6.6 is used as alternate output for the Hold acknowledge output 
  ///  (HLDA_n)
  ///  - push/pull output is selected
  ///  P6.7 is used as alternate output for the Bus request output (BREQ_n)
  ///  - push/pull output is selected

  ///  P6.0 - P6.7 threshold type: TTL input

  ///  P6.0 - P6.3 output driver characteristic: strong driver
  ///  P6.4 - P6.7 output driver characteristic: strong driver

  ///  P6.0 - P6.3 output edge characteristic: sharp edge mode
  ///  P6.4 - P6.7 output edge characteristic: sharp edge mode

  ODP6           =  0x0000;      // load open-drain register
  P6             =  0x0000;      // load data register
  ALTSEL0P6      =  0x0000;      // load alternate output function select 
                                 // register 0
  POCON6         =  0x0000;      // load output control register
  DP6            =  0x0000;      // load direction register

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P7:
  ///  -----------------------------------------------------------------------
  ///  - no pin of port P7 is used

  ODP7           =  0x0000;      // load open-drain register
  P7             =  0x0000;      // load data register
  ALTSEL0P7      =  0x0000;      // load alternate output function select 
                                 // register 0
  ALTSEL1P7      =  0x0000;      // load alternate output function select 
                                 // register 1
  POCON7         =  0x0000;      // load output control register
  DP7            =  0x0000;      // load direction register

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P9:
  ///  -----------------------------------------------------------------------
  ///  - no pin of port P9 is used

  ODP9           =  0x0000;      // load open-drain register
  P9             =  0x0000;      // load data register
  ALTSEL0P9      =  0x0000;      // load alternate output function select 
                                 // register 0
  ALTSEL1P9      =  0x0000;      // load alternate output function select 
                                 // register 1
  POCON9         =  0x0000;      // load output control register
  DP9            =  0x0000;      // load direction register

  ///  -----------------------------------------------------------------------
  ///  Configuration of Port P20:
  ///  -----------------------------------------------------------------------
  ///  P20.4 is used as alternate output for the address latch enable signal 
  ///  (ALE)

  ///  P20.0 - P20.7 threshold type: TTL input
  ///  P20.8 - P20.15 threshold type: TTL input

  ///  P20.0 - P20.3 output driver characteristic: strong driver
  ///  P20.4 - P20.7 output driver characteristic: strong driver
  ///  P20.12 - P20.15 output driver characteristic: strong driver

  ///  P20.0 - P20.3 output edge characteristic: sharp edge mode
  ///  P20.4 - P20.7 output edge characteristic: sharp edge mode
  ///  P20.12 - P20.15 output edge characteristic: sharp edge mode

  P20            =  0x0000;      // load data register
  POCON20        =  0x0000;      // load output control register
  DP20           =  0x0000;      // load direction register


  // USER CODE BEGIN (IO_Function,3)

  // USER CODE END

} //  End of function IO_vInit




// USER CODE BEGIN (IO_General,10)

// USER CODE END

