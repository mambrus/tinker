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
	Q_HW_TIMER_EVENT,	/*!< This special Q is to be used by HW clock to notify
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
} sysq_name_t;

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
