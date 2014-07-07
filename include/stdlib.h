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
#include <tk_ansi.h>
#include <tk_ansi_dirwrap.h>
#include BUILDCHAIN_INCLUDE(stdlib.h)
#include <tinker/config.h>

#ifndef STDLIB_H_TK
#define STDLIB_H_TK

#define exit \
   tk_exit

#include <stddef.h>
#if (!defined(TK_USE_BUILTIN_SORT) || (TK_USE_BUILTIN_SORT==1))
#define TINKER_SEARCH_SORT
#endif

#include <tinker/config.h>
#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
extern "C" {
#endif
#if !defined (__GNUC__) || !defined(__USE_GNU)
	typedef int comparison_fn_t(const void *L, const void *R);
#endif
#if !defined (__C166__) && defined (TINKER_SEARCH_SORT)

#define qsort tk_qsort
#define bsearch tk_bsearch

#endif
	void *bsearch(const void *, const void *, size_t, size_t,
		      comparison_fn_t);
	void qsort(void *, size_t, size_t, comparison_fn_t);
	void _tk_quicksort(void *, int, int, int, comparison_fn_t);
	int _tk_bsearch(void *, void *, int, int, int, comparison_fn_t);

#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
}
#endif
#endif
