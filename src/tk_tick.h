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
#ifndef TK_TICK_H
#define TK_TICK_H

#include <time.h>

#if defined (__GNUC__)
#include <sys/time.h>
#endif

#define MICKEYS_PER_SEC 1000000uL

#if defined(__C166__)
#define MEMTYPE idata
#else
#define MEMTYPE
#endif

#define HW_CLOCKED

#define SPEEDUP 1ul

#if (SPEEDUP > 1)
#define ADV(PERT) ( \
      ( PERT * SPEEDUP ) \
   )
#elif (SPEEDUP == 1)
#define ADV(PERT) ( \
      ( PERT ) \
   )
#else
#error Faulty SPEEDUP factor (use this for debugging dispatcher only)
#endif
#if defined(TICK_OWNER)
unsigned long MEMTYPE sys_mackey = 0;
unsigned long MEMTYPE sys_mickey = 0;
#else
extern unsigned long MEMTYPE sys_mickey;
extern unsigned long MEMTYPE sys_mackey;
#endif
#define _tk_tick_advance_mS(advance) \
   if (sys_mickey+advance < sys_mickey){ \
      sys_mickey+=ADV(advance); \
      sys_mackey++; \
   }else{ \
      sys_mickey+=ADV(advance); \
   }

void getnanouptime(struct timespec *tp);

#endif
