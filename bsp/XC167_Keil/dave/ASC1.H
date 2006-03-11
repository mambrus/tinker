//****************************************************************************
// @Module        Asynchronous/Synchronous Serial Interface (ASC1)
// @Filename      ASC1.H
// @Project       Dave.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XC167CI-16F40
//
// @Compiler      Keil
//
// @Codegenerator 2.4
//
// @Description   This file contains all function prototypes and macros for 
//                the ASC1 module.
//
//----------------------------------------------------------------------------
// @Date          2006-03-11 15:46:52
//
//****************************************************************************

// USER CODE BEGIN (ASC1_Header,1)

// USER CODE END



#ifndef _ASC1_H_
#define _ASC1_H_

//****************************************************************************
// @Project Includes
//****************************************************************************

// USER CODE BEGIN (ASC1_Header,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (ASC1_Header,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (ASC1_Header,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (ASC1_Header,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (ASC1_Header,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (ASC1_Header,7)

// USER CODE END


//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************

void ASC1_vInit(void);
void ASC1_vSendData(uword uwData);
uword ASC1_uwGetData(void);
void ASC1_vReceiverOn(void);


// USER CODE BEGIN (ASC1_Header,8)

// USER CODE END


//****************************************************************************
// @Macro         ASC1_ubTxBufFree() 
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

#define ASC1_ubTxBufFree() ASC1_TBIC_IR


//****************************************************************************
// @Macro         ASC1_vReceiverOff() 
//
//----------------------------------------------------------------------------
// @Description   This macro disables the receive function of the ASC1. A 
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

#define ASC1_vReceiverOff() ASC1_CON_REN = 0


//****************************************************************************
// @Interrupt Vectors
//****************************************************************************

#define ASC1_RINT    0x49

// USER CODE BEGIN (ASC1_Header,9)

// USER CODE END


#endif  // ifndef _ASC1_H_
