/***************************************************************************
 *   Copyright (C) 2014 by Michael Ambrus                                  *
 *   michael.ambrus@sonymobile.com                                         *
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
#include <tk.h>
#define LDATA struct tcb_t_*
#include <mlist/mlist.h>

/* Lists: RUN, SLEEP, DIE e.t.a. These lists are indexes for searching, i.e.
 * they make things go much faster. They don't contain any data however,
 * only pointers to data. The penalty in form of dynamic memory usage is
 * acceptable */

/* Note that the list below all carry a tcb. But several list may contain
 * the same tcb which is fully allowed. You can't rely on these index list
 * to contain all th logic themselves. A list is only a look-up and each
 * list exists for a specific look-up purpose */

handle_t tk_list_sleep;
handle_t tk_list_die;

/* Per core global data */
int nThreads_ended = 0;
int nThreads_ended_skip_cntr = 0;



