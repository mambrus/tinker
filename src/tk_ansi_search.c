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
#include <string.h>
#include <stdlib.h>
#define MAX_SORT_ELEMENT_SIZE 255
void _swap(void *a, int l, int r, int sz)
{
	char t[MAX_SORT_ELEMENT_SIZE];
	memcpy(t, &((char *)a)[l * sz], sz);
	memcpy(&((char *)a)[l * sz], &((char *)a)[r * sz], sz);
	memcpy(&((char *)a)[r * sz], t, sz);
}

unsigned int qsort_depth = 0;
unsigned int curr_depth = 0;
void _tk_quicksort(void *a, int l, int r, int sz, comparison_fn_t cmp)
{
	int i, j;
	void *v;

	curr_depth++;
	if (curr_depth > qsort_depth)
		qsort_depth = curr_depth;

	if (r > l) {

		v = (void **)(((char *)a) + (r * sz));
		i = l - 1;
		j = r;
		for (;;) {
			while (cmp((void **)(((char *)a) + (++i * sz)), v) <
			       0) ;
			while (cmp((void **)(((char *)a) + (--j * sz)), v) >
			       0) ;
			if (i >= j)
				break;
			_swap(a, i, j, sz);
		}
		_swap(a, i, r, sz);

		_tk_quicksort(a, l, i - 1, sz, cmp);
		_tk_quicksort(a, i + 1, r, sz, cmp);
	}
	curr_depth--;
}

int _tk_bsearch(void *v, void *a, int l, int r, int sz, comparison_fn_t cmp)
{
	int x;
	void *m;
	int rc;

	while (r >= l) {
		x = (r + l) / 2;

		m = (void **)(((char *)a) + (x * sz));

		rc = cmp(v, m);
		if (rc == 0)
			return x;

		if (rc < 0)
			r = x - 1;
		else
			l = x + 1;
	}
	if (rc < 0) {

		return (-1 * x - 1);
	} else {

		return (-1 * x - 2);
	}
}

#ifdef TINKER_SEARCH_SORT
void qsort(void *array, size_t count, size_t size, comparison_fn_t compare)
{
	qsort_depth = 0;
	_tk_quicksort(array, 0, count - 1, size, compare);
}

void *bsearch(const void *key,
	      const void *array,
	      size_t count, size_t size, comparison_fn_t compare)
{
	int i;

	i = _tk_bsearch((void *)key, (void *)array, 0, count - 1, size,
			compare);
	if (i >= 0)
		return (void **)(((char *)array) + (i * size));
	else
		return NULL;
}
#endif
