//****************************************************************************
// @Module        Asynchronous/Synchronous Serial Interface (ASC0)
// @Filename      ASC0.H
// @Project       Dave.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XC167CI-16F40
//
// @Compiler      Keil
//
// @Codegenerator 2.4
//
// @Description   This file contains all function prototypes and macros for
//                the ASC0 module.
//
//----------------------------------------------------------------------------
// @Date          2006-03-11 15:46:51
//
//****************************************************************************

// USER CODE BEGIN (ASC0_Header,1)

// USER CODE END



#ifndef _ASC0_H_
#define _ASC0_H_

//****************************************************************************
// @Project Includes
//****************************************************************************

// USER CODE BEGIN (ASC0_Header,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (ASC0_Header,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (ASC0_Header,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (ASC0_Header,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (ASC0_Header,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (ASC0_Header,7)

// USER CODE END


//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************

void ASC0_vInit(void);
void ASC0_vSendData(uword uwData);
uword ASC0_uwGetData(void);


// USER CODE BEGIN (ASC0_Header,8)

// USER CODE END


//****************************************************************************
// @Macro         ASC0_ubTxBufFree()
//
//----------------------------------------------------------------------------
// @Description   This macro can be used for checking the status of the
//                transmit buffer register. The transmit buffer may only be
//                written to if it is empty, this implies that the previous
//                content of the transmit buffer have been loaded into the
//                transmit shift register.
//
//----------------------------------------------------------------------------
// @Returnvalue   0 if buffer is not empty, else 1
//
//----------------------------------------------------------------------------
// @Parameters    None
//
//----------------------------------------------------------------------------
// @Date          2006-03-11
//
//****************************************************************************

#define ASC0_ubTxBufFree() ASC0_TBIC_IR


//****************************************************************************
// @Macro         ASC0_vReceiverOff()
//
//----------------------------------------------------------------------------
// @Description   This macro disables the receive function of the ASC0. A
//                data that is currently being received is received to
//                completion, including generation of the receive interrupt
//                request.
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

#define ASC0_vReceiverOff() ASC0_CON_REN = 0


//****************************************************************************
// @Interrupt Vectors
//****************************************************************************

#define ASC0_TINT    0x2A
#define ASC0_RINT    0x2B

// USER CODE BEGIN (ASC0_Header,9)

// USER CODE END


#endif  // ifndef _ASC0_H_
