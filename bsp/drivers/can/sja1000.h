/***************************************************************************
 *   Copyright (C) 2007 by Michael Ambrus                                  *
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

#ifndef SJA1000_H
#define SJA1000_H

#include <sys/types.h>
#include "sja1000_basic.h"
#include "sja1000_pelican.h"

/*!
@file

*/
//-----------------------------------------------------------------------------

typedef union {
	sja1000_basic_t basic;	//!< Registers in basic mode
	sja1000_pelican_t pelican;	//!< Registers in pelican mode
} sja1000_t;

int sja1000_init(__uint32_t baddr, int IRQn, int pmode, int xmode, int bps,
		 __uint32_t ac, __uint32_t am);
int sja1000_read(const char *buffer, int buff_len);
int sja1000_write(char *buffer, int max_len);
void sja1000_Handler(void);

#endif				//SJA1000_H
