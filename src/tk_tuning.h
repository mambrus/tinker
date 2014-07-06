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
#ifndef TK_TUNING_GLOB
#define TK_TUNING_GLOB

#define TK_FNK_ORIGINAL 0
#define TK_FNK_STUBBED 1
#define TK_FNK_RENAMED 2
#define TK_FNK_VOIDED 4

#endif
#ifndef USED_TINKER_CONFIG_H
#define USED_TINKER_CONFIG_H
#if defined(__GNUC__)
#include <tinker/config.h>
#endif
#endif
#if defined(_WIN32) && defined(_MSC_VER)
#include <../bsp/X86_msvc/tk_tuning.h>

#elif defined(__BORLANDC__) || defined(__BCPLUSPLUS__)
#include <../bsp/X86_Borland/tk_tuning.h>

#elif defined( __C166__ )
#include <../bsp/XC167_Keil/tk_tuning.h>

#elif defined(__GNUC__)

#ifndef ARCH
#error For GNU targets, ARCH has to be defined
#endif

#define INCLNAME(farch) \
      <../bsp/gnu_ farch/tk_tuning.h>

#include INCLNAME( ARCH )

#undef INCLNAME

#else
#error "Can\'t determine the tool-chin you're using for the TINKER kernel"

#endif

#ifndef TINKER_TUNING_CATCHALL
#define TINKER_TUNING_CATCHALL

#ifndef TK_MINIMUM_STACK_SIZE
#define TK_MINIMUM_STACK_SIZE 0x200
#endif

#ifndef TK_NORMAL_STACK_SIZE
#define TK_NORMAL_STACK_SIZE 0x200
#endif

#ifndef TK_MAX_THREADS
#define TK_MAX_THREADS 50
#endif

#ifndef TK_MAX_PRIO_LEVELS
#define TK_MAX_PRIO_LEVELS 5
#endif

#ifndef TK_MAX_THREADS_AT_PRIO
#define TK_MAX_THREADS_AT_PRIO 10
#endif

#ifndef TK_THREAD_NAME_LEN
#define TK_THREAD_NAME_LEN 0x4
#endif

#ifndef TK_MAX_BLOCKED_ON_Q
#define TK_MAX_BLOCKED_ON_Q TK_MAX_THREADS
#endif

#ifndef TK_MAX_NUM_Q
#define TK_MAX_NUM_Q TK_MAX_THREADS
#endif

#ifndef TK_HOWTO_MALLOC
#define TK_HOWTO_MALLOC TK_FNK_STUBBED
#endif

#ifndef TK_HOWTO_CLOCK
#define TK_HOWTO_CLOCK TK_FNK_STUBBED
#endif

#ifndef TK_HOWTO_PRINTK
#define TK_HOWTO_PRINTK TK_FNK_VOIDED
#endif

#ifndef TK_HOWTO_ASSERT
#define TK_HOWTO_ASSERT TK_FNK_RENAMED
#endif

#endif

#ifndef TK_TUNING_H_ROOT_HEADER
#define TK_TUNING_H_ROOT_HEADER

#if defined(__GNUC__)
#if (TK_HOWTO_PRINTK == TK_FNK_VOIDED)
#define printk(x) ((void)0)
#elif (TK_HOWTO_PRINTK == TK_FNK_ORIGINAL)
#define printk(x) printf x
#elif (TK_HOWTO_PRINTK == TK_FNK_RENAMED)
#define eprintf(...) fprintf (stderr, __VA_ARGS__)
#define printk(x) eprintf x
#else
#error Can not handle requested option for printk
#endif

#if (!(TK_SYSTEM == __SYS_HIXS__))

#define tk_bsp_sysinit _tk_bsp_sysinit
#if TK_HOSTED
#if TK_COMP_PTHREAD == __tk_no
#define tk_root root
#endif
#endif
#define BOOT_BSP_STUB
#endif

#endif
#endif
