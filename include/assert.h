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
#ifndef ASSERT_H_TK
#define ASSERT_H_TK

#include <tk_ansi.h>

#if defined (HAVE_CONFIG_H)
#include <tinker/config.h>
#endif

#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
extern "C" {
#endif

	void _tk_assertfail(char *assertstr, char *filestr, int line);

#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
}
#endif
#if !defined(__C166__)
#include <tk_ansi_dirwrap.h>
#include BUILDCHAIN(assert.h)
#endif
#if !defined (assert) && ( \
         defined(__CYGWIN32__) || defined(__CYGWIN__) || \
         defined(__GNUC__) || defined(__USE_GNU) || \
         defined(_WIN32) || defined(__BORLANDC__) || defined(__BCPLUSPLUS__) \
      )
#error "DONT USE TINKER'S ASSERT FOR THIS TOOL_CHAIN"
#endif
#define assure(p) ((p) ? (void)0 : (void) _tk_assertfail( \
                    #p, __FILE__, __LINE__ ) )

#if (TK_HOWTO_ASSERT == TK_FNK_RENAMED)
#undef assert
#endif
#if !defined (assert)
#define assert(p) assure(p)
#endif
#endif
