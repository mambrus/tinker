/***************************************************************************
 *   Copyright (C) 2013 by Michael Ambrus                                  *
 *   ambrmi09@gmail.com                                                    *
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

#ifndef libmlist_local_h
#define libmlist_local_h
#include <mlist.h>

#if INIT_FINI_CTORS_DTORS
#define __init __attribute__((constructor))
#define __fini __attribute__((destructor))
#else
#define __init
#define __fini
#endif

#define TBD_UNFINISHED "Code not finished!!! (TBD)"

int dstrct_mlist(const handle_t handle);

/* Administrative keeper of all lists */
struct mlistmod_struct {
	int isinit;           /* Is this module initialized? I.e. does mlist
							 below contain a valid pointer e.t.a.*/
	int nlists;           /* Current number of lists in  list */
	struct node *mlists;  /* List-head of lists. No need to sort to find.
							 Handle is hash-key*/
};

/* Indicate to others that someone is keeping this as global module data
 * (easier to find with GDB) */
extern struct mlistmod_struct mlistmod_data;
extern struct mlistmod_settings mlistmod_settings;

/* Data of this struct is the payload for the mlist variable in mlistmod_struct.
 * It's the administrative keeper of each list. */
struct listheader {
	struct node *p;	      /* Current (file) pointer */
	//off_t o;              /* Offset from start (in jumps jumps) */
	int nelem;            /* Current size of this list */
	int pl_sz;            /* pay-load size */
	struct listheader *owner; /* If this is a dup, will point to the
						     original*/
	int nr_links;         /* if duped, will be larger than 0, note dups can
							 be duped*/

	/* Caller provided function used to search & sort list. Can be NULL if
	 * search and sort is not supported */
	int (*cmpfunc)(LDATA *lval, LDATA *rval);
	struct node *phead;   /* List-start */
	struct node *ptail;   /* List-end */
};

#endif /* libmlist_local_h */

