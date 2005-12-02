//****************************************************************************
// @Module        Asynchronous/Synchronous Serial Interface (ASC0)
// @Filename      ASC0.C
// @Project       Dave.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XC167CI-16F40
//
// @Compiler      Keil
//
// @Codegenerator 2.4
//
// @Description   This file contains functions that use the ASC0 module.
//
//----------------------------------------------------------------------------
// @Date          2005-12-01 14:09:11
//
//****************************************************************************

// USER CODE BEGIN (ASC0_General,1)

// USER CODE END



//****************************************************************************
// @Project Includes
//****************************************************************************


#include "MAIN.H"

// USER CODE BEGIN (ASC0_General,2)
#include <tk_sysqueues.h>
#include <tk.h>
#include <tk_ipc.h>
/*

#define TK_CLI() \
   PSW_IEN = 0;   

#define TK_STI() \
   PSW_IEN = 1;   
*/      
// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (ASC0_General,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (ASC0_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (ASC0_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (ASC0_General,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (ASC0_General,7)

// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (ASC0_General,8)

// USER CODE END


//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (ASC0_General,9)

// USER CODE END


//****************************************************************************
// @Function      void ASC0_vInit(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the ASC0 function 
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
// @Date          2005-12-01
//
//****************************************************************************

// USER CODE BEGIN (Init,1)

// USER CODE END

void ASC0_vInit(void)
{

  // USER CODE BEGIN (Init,2)

  // USER CODE END

  ///  -----------------------------------------------------------------------
  ///  Configuration of the ASC0 Baudrate Generator:
  ///  -----------------------------------------------------------------------
  ///  - additionally reduce serial clock to 2
  ///  - required baud rate = 115,200 kbaud
  ///  - real baud rate     = 113,636 kbaud
  ///  - deviation          = -1,357 %

  ASC0_BG        =  0x000A;      // load ASC0 baud rate time reload register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the ASC0 Operation Mode:
  ///  -----------------------------------------------------------------------
  ///  - 8-bit data asychronous operation with one stop bit
  ///  - receiver is enabled

  ASC0_CON       =  0x0011;      // load ASC0 control register

  ///  -----------------------------------------------------------------------
  ///  FIFO Configuration:
  ///  -----------------------------------------------------------------------
  ///  - receive FIFO is disabled
  ///  - transmit FIFO is enabled
  ///  - transmit FIFO transparent mode is enabled

  ASC0_RXFCON    =  0x0102;      // load ASC0 receive FIFO control register
  ASC0_TXFCON    =  0x0807;      // load ASC0 transmit FIFO control register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used ASC0 Port Pins:
  ///  -----------------------------------------------------------------------
  ///  - P3.10 is used for ASC Transmit Data Output (TxDA0)
  ///  - P3.11 is used for ASC0 Receive data Input (RxDA0)

  ALTSEL0P3     |=  0x0400;      // select alternate output function
  P3   = (P3   & ~(uword)0x0400) | 0x0400;    //set data register
  DP3  = (DP3  & ~(uword)0x0400) | 0x0400;    //set direction register

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used ASC0 Interrupts:
  ///  -----------------------------------------------------------------------
  ///  Tx service request node configuration:
  ///  - Tx interrupt priority level (ILVL) = 14
  ///  - Tx interrupt group level (GLVL) = 0
  ///  - Tx group priority extension (GPX) = 0

  ASC0_TIC       =  0x0078;     

  ///  Use PEC channel 0 for ASC0 Tx INT:
  ///  - decrement counter
  ///  - increment source pointer
  ///  - transfer a byte
  ///  - service End of PEC interrrupt by a EOP interrupt node is disabled

  PECC0          =  0x0501;      // load PECC0 control register

  //// - DO NOT FORGET TO SET THE SOURCE-POINTER FOR PECC CHANNEL 0 BECAUSE 
  ////   IT IS SELECTED FOR USER DEFINED

  DSTP0  =  _sof_(&ASC0_TBUF);  //set destination pointer

  ///  Rx service request node configuration:
  ///  - Rx interrupt priority level (ILVL) = 1
  ///  - Rx interrupt group level (GLVL) = 0
  ///  - Rx group priority extension (GPX) = 0

  ASC0_RIC       =  0x0044;     


  //   -----------------------------------------------------------------------
  //   Default Settings for Service Request Flags:
  //   -----------------------------------------------------------------------
  ASC0_TBIC_IR   =  1;           // indicates that the transmit buffer is 
                                 // empty

  // USER CODE BEGIN (ASC0_Function,3)

  // USER CODE END

  ASC0_CON      |=  0x8000;      // enable baud rate generator


} //  End of function ASC0_vInit


//****************************************************************************
// @Function      void ASC0_vSendData(uword uwData) 
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
// @Date          2005-12-01
//
//****************************************************************************

// USER CODE BEGIN (SendData,1)

// C166 used to crash around the line:
//  ASC0_TBUF    = uwData;   //  load transmit buffer register

//Try to remark it out if it happens again, then un-remark it back ... (!?)

//#pragma bytealign - here will crash even a clean DAVE file
// USER CODE END



void ASC0_vSendData(uword uwData)
{

   ASC0_TBUF    = uwData;


} //  End of function ASC0_vSendData



//****************************************************************************
// @Function      uword ASC0_uwGetData(void) 
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
// @Date          2005-12-01
//
//****************************************************************************

// USER CODE BEGIN (GetData,1)


// USER CODE END

uword ASC0_uwGetData(void)
{
  return(ASC0_RBUF);     // return receive buffer register

} //  End of function ASC0_uwGetData


//****************************************************************************
// @Function      void ASC0_viTx(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the transmit interrupt service routine for the 
//                ASC0. It is called when the sending of data is terminated. 
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
// @Date          2005-12-01
//
//****************************************************************************

// USER CODE BEGIN (Tx,1)

// USER CODE END

void ASC0_viTx(void) interrupt ASC0_TINT
{

  // USER CODE BEGIN (Tx,2)

  // USER CODE END

} //  End of function ASC0_viTx


//****************************************************************************
// @Function      void ASC0_viRx(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the receive interrupt service routine for the ASC0. 
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
// @Date          2005-12-01
//
//****************************************************************************

// USER CODE BEGIN (Rx,1)

sfr  SPSEG                = 0xFF0C;       //Bug in DaVE doesn't generate this

systemstackaddr_t stack_p;

typedef void yfunk(void);
typedef yfunk *yfunk_p;

yfunk_p        *f_p;

unsigned long mybuff[4];

tk_tcb_t *current_tcb;


// USER CODE END

void ASC0_viRx(void) interrupt ASC0_RINT
{

  // USER CODE BEGIN (Rx,2)
  //while (ASC0_RIC_IR){
     //TK_CLI();
     //ASC0_RIC_IR = 0;

     
	  mybuff[0] = ASC0_uwGetData();
     q_send_ny(tk_sys_queues[Q_SERIAL_0_I],mybuff); 
     tk_yield();

/*     
     stack_p.segmented._offs = SP;
     stack_p.segmented._seg = SPSEG;
     
     f_p = (yfunk_p *)(stack_p.linear+6);     
     
     current_tcb = _tk_current_tcb();


     //Swhich the return adress, but save it in TCB for later
     current_tcb->prmtRetAddr = *f_p;              
     *f_p = tk_yield;
*/
     

     //TK_CLI();
  //}

  // USER CODE END

} //  End of function ASC0_viRx




// USER CODE BEGIN (ASC0_General,10)

// USER CODE END

