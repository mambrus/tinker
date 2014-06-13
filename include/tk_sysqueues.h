/***************************************************************************
 *   Copyright (C) 2006 by Michael Ambrus                                  *
 *   michael.ambrus@maquet.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
   

#ifndef tk_sysqueues_h
#define tk_sysqueues_h

#if defined(__GNUC__)
   #include <tinker/config.h>
   #if (!defined(TK_COMP_ITC) || (TK_COMP_ITC==0) ) && !defined(ECODES_ONLY)
   #error "tk_sysqueues.h" belongs to [itc] that your configuration of TinKer did not include. Please reconfigure and rebuild TinKer.
   #endif
#endif

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
#include <tinker/config.h>
#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
extern "C" {
#endif

unsigned long _tk_create_system_queues();


#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
}
#endif

#endif




  
/*!
 * @defgroup CVSLOG_tk_sysqueues_h tk_sysqueues_h
 * @ingroup CVSLOG
 *
 *  $Log: tk_sysqueues.h,v $
 *  Revision 1.9  2006-12-12 10:57:06  ambrmi09
 *  This adresses the second part of #1609064
 *
 *  Revision 1.8  2006/11/05 19:06:03  ambrmi09
 *  Buildsystem adjusted to permit configuration of components.
 *  Now when component is enabled it will also be included in the build
 *  (instead of just sanity-tested in the source files).
 *
 *  Also a feature for application sanity is assed. When a header-file is
 *  included in the application, a check against the component it belongs
 *  to will be performed. That way user don't need to rely on run-time
 *  checks and can get feedback much earlier.
 *
 *  Revision 1.7  2006/04/08 10:15:59  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.6.2.1  2006/04/03 20:07:24  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.6  2006/03/05 11:11:26  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.5  2006/02/22 13:05:46  ambrmi09
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
