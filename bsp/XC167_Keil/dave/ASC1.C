//****************************************************************************
// @Module        Asynchronous/Synchronous Serial Interface (ASC1)
// @Filename      ASC1.C
// @Project       Dave.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XC167CI-16F40
//
// @Compiler      Keil
//
// @Codegenerator 2.4
//
// @Description   This file contains functions that use the ASC1 module.
//
//----------------------------------------------------------------------------
// @Date          2006-03-11 15:46:52
//
//****************************************************************************

// USER CODE BEGIN (ASC1_General,1)

// USER CODE END



//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.H"

// USER CODE BEGIN (ASC1_General,2)

#include <tk.h>
#include <tk_ipc.h>
#include <tk_sysqueues.h>

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (ASC1_General,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (ASC1_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (ASC1_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (ASC1_General,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (ASC1_General,7)

// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (ASC1_General,8)

// USER CODE END


//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (ASC1_General,9)

// USER CODE END


//****************************************************************************
// @Function      void ASC1_vInit(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the ASC1 function 
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

void ASC1_vInit(void)
{
  // USER CODE BEGIN (Init,2)

  // USER CODE END

  ///  -----------------------------------------------------------------------
  ///  Configuration of the ASC1 Baudrate Generator:
  ///  -----------------------------------------------------------------------
  ///  - additionally reduce serial clock to 2
  ///  - required baud rate = 115,200 kbaud
  ///  - real baud rate     = 113,636 kbaud
  ///  - deviation          = -1,357 %

  ASC1_BG        =  0x000A;      // load ASC1 baud rate time reload register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the ASC1 Operation Mode:
  ///  -----------------------------------------------------------------------
  ///  - 8-bit data asychronous operation with one stop bit
  ///  - receiver is enabled

  ASC1_CON       =  0x0011;      // load ASC1 control register

  ///  -----------------------------------------------------------------------
  ///  FIFO Configuration:
  ///  -----------------------------------------------------------------------
  ///  - receive FIFO is disabled
  ///  - transmit FIFO is enabled
  ///  - transmit FIFO transparent mode is enabled

  ASC1_RXFCON    =  0x0102;      // load ASC1 receive FIFO control register
  ASC1_TXFCON    =  0x0807;      // load ASC1 transmit FIFO control register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used ASC1 Port Pins:
  ///  -----------------------------------------------------------------------
  ///  - P3.0 is used for ASC1 Transmit Data Output (TxDA1)
  ///  - P3.1 is used for ASC1 Receive data Input (RxDA1)

  ALTSEL0P3     |=  0x0001;      // select alternate output function
  P3   = (P3   & ~(uword)0x0001) | 0x0001;    //set data register
  DP3  = (DP3  & ~(uword)0x0001) | 0x0001;    //set direction register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used ASC1 Interrupts:
  ///  -----------------------------------------------------------------------
  ///  Rx service request node configuration:
  ///  - Rx interrupt priority level (ILVL) = 9
  ///  - Rx interrupt group level (GLVL) = 0
  ///  - Rx group priority extension (GPX) = 0

  ASC1_RIC       =  0x0064;     


  //   -----------------------------------------------------------------------
  //   Default Settings for Service Request Flags:
  //   -----------------------------------------------------------------------
  ASC1_TBIC_IR   =  1;           // indicates that the transmit buffer is 
                                 // empty

  // USER CODE BEGIN (ASC1_Function,3)

  // USER CODE END

  ASC1_CON      |=  0x8000;      // enable baud rate generator


} //  End of function ASC1_vInit


//****************************************************************************
// @Function      void ASC1_vSendData(uword uwData) 
//
//----------------------------------------------------------------------------
// @Description   This function writes a send data initialization word into 
//                the transmit buffer register.
//                
//                Note: 
//                In a multiprocessor system the master with this function 
//                has the possibility to send data to the selected slave. To 
//                achieve this, the 9th bit must set on zero.
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    uwData: 
//                Data to be send
//
//----------------------------------------------------------------------------
// @Date          2006-03-11
//
//****************************************************************************

// USER CODE BEGIN (SendData,1)

// USER CODE END

void ASC1_vSendData(uword uwData)
{
  ASC1_TBIC_IR = 0;        //  reset transmit buffer interrupt request 
  ASC1_TBUF    = uwData;   //  load transmit buffer register

} //  End of function ASC1_vSendData


//****************************************************************************
// @Function      uword ASC1_uwGetData(void) 
//
//----------------------------------------------------------------------------
// @Description   This function reads out the content of the receive buffer 
//                register which contains the received data.
//
//----------------------------------------------------------------------------
// @Returnvalue   data that has been received
//
//----------------------------------------------------------------------------
// @Parameters    None
//
//----------------------------------------------------------------------------
// @Date          2006-03-11
//
//****************************************************************************

// USER CODE BEGIN (GetData,1)

// USER CODE END

uword ASC1_uwGetData(void)
{
  return(ASC1_RBUF);     // return receive buffer register

} //  End of function ASC1_uwGetData


//****************************************************************************
// @Function      void ASC1_viRx(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the receive interrupt service routine for the ASC1. 
//                It is called if the data has been received. 
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

// USER CODE BEGIN (Rx,1)
unsigned long c1buff[4];
// USER CODE END

void ASC1_viRx(void) interrupt ASC1_RINT
{

  // USER CODE BEGIN (Rx,2)
      
   c1buff[0] = ASC1_uwGetData();
   q_send_ny(tk_sys_queues[Q_SERIAL_1_I],c1buff);         
   tk_yield_event(); 
   
  // USER CODE END

} //  End of function ASC1_viRx


//****************************************************************************
// @Function      void ASC1_vReceiverOn(void) 
//
//----------------------------------------------------------------------------
// @Description   This function releases the receive function of the ASC1. 
//                After initialization this function does not need to be 
//                recalled. 
//                Note: 
//                This function must be called everytime data is to be 
//                received in synchronous mode.
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

// USER CODE BEGIN (ReceiverOn,1)

// USER CODE END

void ASC1_vReceiverOn(void)
{
  ASC1_CON_REN = 1;               // enable the receiver

} //  End of function ASC1_vReceiverOn




// USER CODE BEGIN (ASC1_General,10)

// USER CODE END

