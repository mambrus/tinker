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

//------1---------2---------3---------4---------5---------6---------7---------8
#ifndef TK_TUNING_H
#define TK_TUNING_H

#define SAFEZONE        0x40
/*!
Defines where the userstack is within the mallocated area. The system
stack will be on highest addresand the user stack will be below that.

Read ratio as x:y or user_size/system_size
*/
#define USR_SYS_RATIO 2

//Note that system stack has to fit even after usr_stack has been subtracted
//With a rato of 2 this will give a stack size for sys_stack of 64

#define minimum_system_stack_size  0xC0
//#define TK_MINIMUM_STACK_SIZE ((minimum_system_stack_size + 1)*USR_SYS_RATIO) + SAFEZONE)

#define TK_MINIMUM_STACK_SIZE 0x0300	//!< TBD this @todo TBD this
#define TK_NORMAL_STACK_SIZE  0x0600

//SCHED
#define TK_MAX_THREADS          50
#define TK_MAX_PRIO_LEVELS      0x10
#define TK_MAX_THREADS_AT_PRIO  TK_MAX_THREADS
#define TK_THREAD_NAME_LEN      0x17

//ITC
#define TK_MAX_BLOCKED_ON_Q    	10
#define TK_MAX_NUM_Q       	75

#endif				//TK_TUNING_H

