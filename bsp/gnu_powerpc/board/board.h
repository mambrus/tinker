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

#ifndef BOARD_H
#define BOARD_H

#include <tinker/config.h>
#include <asm/bits.h>

#if defined(TK_DCPU)
	#if TK_DCPU==860
		#include <CPU/860/mmap_regs.h>

		#include <CPU/860/asm/cm.h>
		#include <CPU/860/asm/cm_scc.h>
		#include <CPU/860/asm/cm_smc.h>
		#include <CPU/860/asm/ictrl.h>
		#include <CPU/860/asm/msr.h>
		#include <CPU/860/asm/plprcr.h>
		#include <CPU/860/asm/sccr.h>

	#else
		#Sorry, TinKer doesn't know how to use your CPU
	#endif
#else
	#error Must have CPU for board to work
#endif


#endif // BOARD_H

