/*

This is a replacement file for Keils version of putchar.c

*/


//#include <tk.h>  //!< will create lots of redef warnings

/*! Stupid dave requires MAIN.H to be included before any of the other
dave files instead of including it in the .H files itself */
#include <../bsp/XC167_Keil/dave/MAIN.H>
#include <../bsp/XC167_Keil/dave/ASC0.H>

#define XON  0x11
#define XOFF 0x13
/*
//Due to bug in C166...
sfr  mASC0_TBUF            = 0xFEB0;       //Serial Channel 0 Transmitter Buffer Register (WO)
void ASC0_vSendData(uword uwData)
{
   mASC0_TBUF    = uwData;
} //  End of function ASC0_vSendData
*/


/*!
Set this device if you want this driver to work on another device.
This should be the only place you need to modify.

*/
#define dev()              ASC0

/*!
The interrupt control register us used to determine if tx has been sent.

@note Remeber to clear it when done (no ISR will do it for us).
*/
#define LAST_TX_SENT           dev()##_TBIC_IR

#define FIFO_ENABLED           dev()##_TXFCON_TXFEN
#define FIFO_TRANSPARENT_MODE  dev()##_TXFCON_TXTMEN
#define SEND_BYTE              dev()##_vSendData

long putchar_burntime(int x, int y, int z){
   long rc;
   rc  = 10000uL * x;
   rc += 100uL * y;
   rc += z;

   return z;
}

unsigned int putchar_busywait( unsigned int time_ms ){
   int k,x,y,z;
   long rc;
   int xwait;
   int ywait;

   xwait = time_ms / 100;
   xwait ? ywait = time_ms / xwait : ywait = time_ms;
   for (k=0;k<13;k++)
      for (x=0;x<(xwait+1);x++)
         for (y=0;y<(ywait+1);y++)
            for (z=0;z<100;z++)
               rc = putchar_burntime(x,y,z);

   return 0;
}


typedef unsigned int fsleep( unsigned int time );
typedef fsleep *fsleep_p;



//fsleep_p sleepf = tk_msleep; //unsafe with printf. Same probs as with preempt. use when semaphores are working
fsleep_p sleepf = putchar_busywait;

signed char putchar (signed char c)  {

   if (FIFO_ENABLED  && FIFO_TRANSPARENT_MODE) {
      if (!LAST_TX_SENT ){
         //TxFFO set to 8 bytes and baudrate set to 9,6kbits/s implies a wait of ~
         //1k byte/sek
         sleepf(10); //<! dont be stingy, a missed time frame will cost a extra loop
      }
      LAST_TX_SENT = 0;

      if ( c == '\n' || c == '\r' || c == 0 ){
         SEND_BYTE(0x0D);
		 if (!LAST_TX_SENT) sleepf(1);
		 LAST_TX_SENT = 0;
         SEND_BYTE(0x0A);
      }else{
         SEND_BYTE(c);
      }
   } else {
      if ( c == '\n' || c == '\r' || c == 0 ){
         SEND_BYTE(0x0D);
         if (!LAST_TX_SENT) sleepf(1);
         LAST_TX_SENT = 0;
         SEND_BYTE(0x0A);
         if (!LAST_TX_SENT) sleepf(1);
         LAST_TX_SENT = 0;
      }else{
         SEND_BYTE(c);
         if (!LAST_TX_SENT) sleepf(1);
         LAST_TX_SENT = 0;
      }
   }

   return (1);


}