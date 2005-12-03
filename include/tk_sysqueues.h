/*******************************************************************
 *
 *  DESCRIPTION: tk_sysqueues.h System queues
 *
 *  AUTHOR: Michael Ambrus
 *
 *  HISTORY:    
 *
 *  Current $Revision: 1.2 $
 *
 *******************************************************************/
   

#ifndef tk_sysqueues_h
#define tk_sysqueues_h

/** include files **/

/** local definitions **/

/** Error codes **/

/*  default settings */

/** external functions **/

/** external data **/

/** internal functions **/


/** private data **/

/** public declarations **/
typedef enum {
   Q_SERIAL_0_I,
   Q_SERIAL_0_O,
   Q_SERIAL_1_I,
   Q_SERIAL_1_O,
   Q_CAN_0_I,
   Q_CAN_0_O,
   Q_CAN_1_I,
   Q_CAN_1_O,
   TK_NUMBER_OF_SYSQ             
}sysq_name_t;

/** public data **/
extern unsigned long tk_sys_queues[TK_NUMBER_OF_SYSQ];

/** private functions **/
unsigned long _tk_create_system_queues();


#endif




  
/*!
 * @addtogroup CVSLOG CVSLOG
 *
 *  $Log: tk_sysqueues.h,v $
 *  Revision 1.2  2005-12-03 14:04:30  ambrmi09
 *  A crude documentation structure added. Sorce files modified a little, but
 *  only in comments (for Doxygens sake).
 *
 *  Revision 1.1  2005/12/01 13:05:25  ambrmi09
 *  This check-in if for preparing for peemtive mechanism (first try)
 *  Done since last check-in
 *
 *  - Got rid of a nasty include bug for the target dependant sys-files
 *  - Added yet anoter sys header file with type only, so that kernel doesn't
 *    have to include the whole lot (which will render in  another "bug" due
 *    to that Keil and Dave define the same thing. This might be solved
 *    differently later, but the separation doesnt hurt anyway.
 *  - Started a concept of system queues - which I hope will be the basis for
 *    the TinKer drivers concept (yet to be invented).
 *  - Made first crude attempts with preemtion.
 *
 *
 *******************************************************************/
