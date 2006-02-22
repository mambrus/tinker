/*******************************************************************
 *
 *  DESCRIPTION: tk_sysqueues.h System queues. To be used between
 *  drivers and HW ISR. 
 *
 *  AUTHOR: Michael Ambrus
 *
 *  HISTORY:    
 *
 *  Current $Revision: 1.5 $
 *
 *******************************************************************/
   

#ifndef tk_sysqueues_h
#define tk_sysqueues_h

/*- include files **/

/*- local definitions **/

/*- Error codes **/

/*  default settings */

/*- external functions **/

/*- external data **/

/*- internal functions **/


/*- private data **/

/*- public declarations **/
typedef enum {
   Q_HW_TIMER_EVENT,  /*!< This special Q is to be used by HW clock to notify 
                      the ptime component that some sort of event has happened 
                      (i.e. a timeout mostly). */
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

/*- public data **/
extern unsigned long tk_sys_queues[TK_NUMBER_OF_SYSQ];

/*- private functions **/
unsigned long _tk_create_system_queues();


#endif




  
/*!
 * @defgroup CVSLOG_tk_sysqueues_h tk_sysqueues_h
 * @ingroup CVSLOG
 *
 *  $Log: tk_sysqueues.h,v $
 *  Revision 1.5  2006-02-22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.4  2006/02/19 12:44:33  ambrmi09
 *  - Documented ITC
 *  - Started to build up the structure for the \ref PTHREAD component
 *
 *  Revision 1.3  2006/02/02 15:51:02  ambrmi09
 *  A lot of thought has been invested into the new PTIME component. Had to
 *  change things even in the systime parts (integrated in the SHEDUL
 *  component) to make it more generic. Think this will be really nice when
 *  it's ready, but has been a long road to get PTIME running (and I'm
 *  still not there).
 *
 *  Revision 1.2  2005/12/03 14:04:30  ambrmi09
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
