/***************************************************************************
 *   Copyright (C) 2005,2006 by Michael Ambrus                             *
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
#include <tk_itc.h>
#include <tk_sysqueues.h>

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define SYSQ_LENGT 10
unsigned long tk_sys_queues[TK_NUMBER_OF_SYSQ];

unsigned long _tk_create_system_queues()
{
	int i;
	unsigned long rc = ERR_OK;
	char name[4];

	for (i = 0; (i < TK_NUMBER_OF_SYSQ) && (rc == ERR_OK); i++) {

		strncpy(name, "SQ", 4);
		if (i > 10) {
			name[2] = 48 + i / 10;
			name[3] = 48 + i % 10;
		}
		rc = q_create_ny(name, SYSQ_LENGT, FIFO, &tk_sys_queues[i]);
	}
	return rc;
}
