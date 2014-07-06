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

/*!
@file
@ingroup PTIMER

@brief XXX

This is XXX

For in-depth discussions XXX \ref
PTIMER

@see PTIMER

*/

#ifndef TK_PTIMER_H
#define TK_PTIMER_H

#if defined(__GNUC__)
#include <tinker/config.h>
#if (!defined(TK_COMP_PTIMER) || (TK_COMP_PTIMER==0) ) && !defined(ECODES_ONLY)
#error "tk_ptime.h" belongs to [ptimer] that your configuration of TinKer did not include. Please reconfigure and rebuild TinKer.
#endif
#endif

/*- include files **/
//#include <time.h>

#include <tk_itc.h>		//last in error chain

/*- public definitions **/

/*COMPONENT PTIMER*/

enum PTIMER_ERROR_CODES {

	ERR_UNDEF_PTIMER = ERR_ITC_SENTINEL,	//!< @brief No such or invalid ptimer
	ERR_TIME_NEG,		//!< @brief Trying to set event for time that's passed
	ERR_TIMER_DELETED,	//!< @brief The timer (i.e. the requested event) has been deleted
	ERR_NO_MORE_TIMERS,	//!< @brief You try to exeed the maximum number of pending timers

	ERR_PTIMER_SENTINEL
};

/*!
@name Protocol macros

Macros involved with the protocol between HW clock and service thread

*/
//@{

#define ET_TIMEOUT       0	/*!< Timeout event finished - timer all completed.
				 */
#define ET_TIMEOUT_P     1	/*!< Timeout event part - timer partly completed.
				   HWclock_ needs reloading */
#define ET_RELOAD_NEW    2	/*!< Current timer was disrupted by a new timer that
				   expires earlier than the pending one.
				   THWP_RTIME will contain the remaining bullets */
#define ET_CANCELLED     3	/*!< Current timer was cancelled.
				   THWP_RTIME will contain the remaining bullets */

//@}

/*! This index enumeration determines that each index in the array passed from
    the HW ISR to the driver means. */
typedef enum {
	THWP_EVENT_ID,		/*!< Reason why the service thread is awaken (see the ET_XXX
				   macros */
	THWP_TIMER_ID,		/*!< The event relates to this timer number (used for sanity
				   checks for cases where interrupts happen during charging
				   of the HWclock_) */
	THWP_RTIME,		/*!< Time that remains. Bullets remaing in HWclock_ - This field
				   is very important and will always contain a valid value
				   (remaining in shift register) no matter the event
				   reason. The value is used by the service thread to use
				   to update it's own internal record of time. The value
				   tells the service how much time was left until the next
				   <b>expected</b> event. 0 is a special case meaning that
				   the event has arrived as expected in time. */
	THWP_LATCY,		/*!< Extra field that can be used for compensating latency
				   (for targets where ISR can measure it) */
	THWP_PROT_SZ		/*!< Internal - Use to determine needed sizes for array using
				   these indexes can be determined by name */
} e_timerHW_protIdx;

/*- local definitions **/

/*- Error codes **/

//#define ERR_OK                0x00 /*!< No error */
//#define ERR_UNDEF_PTIMER      0x02 /*!< No such or invalid ptimer */
//#define ERR_TIME_NEG          0x03 /*!< Trying to set event for time that's passed */
//#define ERR_TIMER_DELETED     0x04 /*!< The timer (i.e. the requested event) has been deleted */
//#define ERR_NO_MORE_TIMERS    0x06 /*!< You try to exeed the maximum number of pending timers */

/*!
TinKers ptimer_t structure. This structure contains all
information the kernel needs to know about pre-emptable timers (ptimers).
*/
typedef struct ptimer_s {
	char name[4];		//!< Name of the timer
	unsigned int tid;	//!< Identity of this timer
	struct ptimer_s *prev;	//!< Previous in list
	unsigned int active;	//!< If this timer is active or not @note: <b>internal use only </b>
	unsigned int numBlocked;	//!< Number of blocked threads on this timer
	time_t expTime;		//!< Expire time
	unsigned int neededCnts;	//!< Number of iterations needed to complete
	unsigned int count;	//!< Completed this number of iterations
	struct ptimer_s *next;	//!< Next in list
} ptimer_t;

/*- default settings **/

/*- external functions **/

/*- external data **/

/*- internal functions **/

/*- public data **/

/*- private data **/

/*- public functions **/
#include <tinker/config.h>
#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
extern "C" {
#endif

/*!
@name Creation and destruction of this component

Use these functions only at boot, and shut-down.

@see COMPONENTS
*/
//@{
	unsigned long tk_ptime(void);
	unsigned long tk_ptime_destruct(void);
//@}

/*!
@name API for createing and destoy timers

These functions are involved with creating and destroying timers
*/
//@{
	unsigned long tk_ptimeevent_at(thid_t * tid, time_t * absTime);
	unsigned long tk_ptimeevent_in(thid_t * tid, time_t * relTime);
	unsigned long tk_ptimer_destroy(thid_t tid);
//@}

/*!
@name API using the service

These functions are involved with creating and destroying timers
*/
//@{
	unsigned long tk_ptimer_block(thid_t tid);
	unsigned long tk_ptimer_awake(thid_t tid);
	unsigned long tk_ptimer_sleep(thid_t tid, time_t * relTime);
//@}

/*- private functions **/

//------1---------2---------3---------4---------5---------6---------7---------8

#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
}
#endif
#endif
