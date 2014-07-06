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
#ifndef TK_H
#define TK_H
#define TINKER

#if defined(__GNUC__)
#include <tinker/config.h>
#include <sys/types.h>

#if defined(_POSIX_THREADS)
#error newlib/libc headers somehow managed set _POSIX_THREADS and probably a whole set of others incompatible pthread related things
#endif
#endif

#include <stddef.h>
#include <time.h>

enum SCHED_ERROR_CODES {
	TK_OK = 0,
	TK_ERROR,
	TK_SENTINEL
};

#define ERR_OK TK_OK

#define TK_NO_WARN_ARG(x) ((void)x)

#ifdef __doxygen__
#error This section should never be enabled for a normal build
#define TK_MAX_THREADS 50
#define TK_MAX_PRIO_LEVELS 0x10
#define TK_MAX_THREADS_AT_PRIO TK_MAX_THREADS
#define TK_THREAD_NAME_LEN 0x17

#endif

#define YES 1
#define NO 0
#ifndef TK_COMP_ITC
#define TK_COMP_ITC YES
#endif
#ifndef TK_COMP_PTIMER
#define TK_COMP_PTIMER NO
#endif
#ifndef TK_COMP_KMEM
#define TK_COMP_KMEM NO
#endif
#ifndef TK_COMP_PTHREAD
#define TK_COMP_PTHREAD NO
#endif
#ifndef TK_COMP_POSIX_RT
#define TK_COMP_POSIX_RT NO
#endif
#define TC_ISA_ERR 0x8000
#define TC_NOERR (0x0000 | TC_ISA_ERR)

#define TC_MAX_THREADS (0x0001 | TC_ISA_ERR)
#define TC_ERR_ASSERT (0x0002 | TC_ISA_ERR)
#define TC_MAX_PRIO_LEVELS (0x0004 | TC_ISA_ERR)
#define TC_ERR_4 (0x0008 | TC_ISA_ERR)

#define TC_ERR_STACK (0x0010 | TC_ISA_ERR)
#define TC_ERR_STKINT (0x0020 | TC_ISA_ERR)
#define TC_THREADS_AT_PRIO (0x0040 | TC_ISA_ERR)
#define TC_AMOK (0x0080 | TC_ISA_ERR)

#define TC_NAME_LEN (0x0100 | TC_ISA_ERR)
#define TC_NOMEM (0x0200 | TC_ISA_ERR)
#define TC_TCB_INVALID (0x0400 | TC_ISA_ERR)
#define TC_ERR_12 (0x0800 | TC_ISA_ERR)

#define TC_ERR_13 (0x1000 | TC_ISA_ERR)
#define TC_ERR_14 (0x2000 | TC_ISA_ERR)
#define TC_ERR_HW (0x4000 | TC_ISA_ERR)
#define TC_UNKNOWN (0x8000 | TC_ISA_ERR)

typedef enum { TK_FALSE, TK_TRUE } TK_BOOL;
typedef void *start_func_ft(void *);
typedef start_func_ft *start_func_f;

typedef void init_func_ft(void *);
typedef init_func_ft *init_func_f;

typedef void isr_ft(void);
typedef isr_ft *isr_handler;

extern init_func_f boot_hook;
#if defined(HAVE_UINTPTR_T)
#include <stdint.h>
typedef uintptr_t tin_t;
#else
typedef int tin_t;
#endif
typedef tin_t thid_t;
typedef tin_t titc_t;
typedef tin_t titi_t;
#include <tinker/config.h>
#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
extern "C" {
#endif

	int tk_delete_thread(thid_t PID);
	thid_t tk_create_thread(char *name,
				unsigned int prio,
				start_func_f start_func,
				void *inpar, size_t stack_size);
	int tk_join(thid_t, void **);
	int tk_detach(thid_t);
	void tk_create_kernel(void);
	void tk_delete_kernel(void);
	void tk_yield(void);
	void tk_yield_event(void);
	void tk_exit(int ec);
	void tk_msleep(unsigned int time_ms);
	thid_t tk_thread_id(void);
	int tk_change_prio(thid_t tid, int newPrio);
	int tk_isr_install(int level, isr_handler isr);
	void tk_isr_eoi(int level);
	void tk_preemplist(thid_t);

#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
	extern int tk_root(void);

#ifdef __cplusplus
}
#endif
#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
extern "C" {
#endif

	extern int tk_bsp_sysinit(void);

#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
}
#endif
#if defined(TINKER)
#define main(x,y) tk_root()
#endif
#endif
