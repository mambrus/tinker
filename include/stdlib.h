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

/*!
@file
@ingroup kernel_reimpl_ansi

@brief ANSI stdlib.h file

care of the issue.

For in-depth discussions about re-implementing ANSI functions, see \ref
kernel_reimpl_ansi.

@see kernel_reimpl_ansi

<HR>
*/

/*Note: it's very important that the Keil C166 tool-chain
stdlib.h get properly included. It contains definitions
with special pointer-types. Using functions that use those
pointers will most likely break in RUN-TIME*/
#include <tk_ansi.h>
#include <tk_ansi_dirwrap.h>
#include BUILDCHAIN(stdlib.h)
#include <tinker/config.h>

#ifndef STDLIB_H_TK
#define STDLIB_H_TK

//Always use TinKer exit
#define exit     \
   tk_exit

#include <stddef.h>		//Needed for size_t
#if (!defined(TK_USE_BUILTIN_SORT) || (TK_USE_BUILTIN_SORT==1))
#define TINKER_SEARCH_SORT	//!< Optional define, use to debug qsort & bsearch. Undefine to use tool-chains own versions
#endif

#include <tinker/config.h>
#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
extern "C" {
#endif

/*!
A comparison funtion definition type that ANSI search & sort funtions use.
It's return value is supposed to have the same meaning as for strcmp

@return \f$ = \cases{ 0, & if L = R \cr <0 , & if L < R \cr >0, & if L > R } \f$

@return  0 if eq,
@return <0 if L<R
@return >0 if L>R

GNU reference: http://www.gnu.org/software/libc/manual/html_mono/libc.html#Comparison%20Functions

@note <b>This is a GNU extension type and is not part of the ANSI
standard</b>. Please do not rely on this type being available for other
targets \b NOT using TinKer. I.e. for transparent \b applications, you
should not use this type. Instead you should define your own. If you do,
you will \b only be able to port your application to GNU operating
systems and tools.

*/
#if !defined (__GNUC__) || !defined(__USE_GNU)
	typedef int comparison_fn_t(const void *L,	//!< <em>"Leftmost"</em> element to compare with
				    const void *R	//!< <em>"Rightmost"</em> element to compare with
	    );
#endif

/*!
@name

These defines makes sure we're using our own versions of types and/or
functions even if both (or either) implementation and application refer
to them using their standard ANSI name.

@see COMPONENTS
*/
//@{
#if !defined (__C166__)  && defined (TINKER_SEARCH_SORT)

#define qsort                 tk_qsort
#define bsearch               tk_bsearch

#endif
//@}

/*!
@name

Public functions that TinKer eiter needs or need wrapped versions of.
*/
//@{
	void *bsearch(const void *, const void *, size_t, size_t,
		      comparison_fn_t);
	void qsort(void *, size_t, size_t, comparison_fn_t);
//@}

/*!
@name

Internal functions. Do \b NOT use this if you want to write portable \b application code

*/
//@{
	void _tk_quicksort(void *, int, int, int, comparison_fn_t);
	int _tk_bsearch(void *, void *, int, int, int, comparison_fn_t);
//@}

#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
}
#endif
#endif				//STDLIB_H_TK
/*!
 * @defgroup CVSLOG_stdlib_h stdlib_h
 * @ingroup CVSLOG
 *  $Log: stdlib.h,v $
 *  Revision 1.7  2007-03-04 19:07:25  ambrmi09
 *  1) Error handling refined - will handle error from different
 *     cathegories:
 *     - errno (perror)
 *     - TK errors
 *     - TK traps codes
 *     - exit handling can differ beween user exit codes and kernel
 *       trap codes.
 *  2) Extracted fluffy & un-critical code from tk.c (the error and exit
 *     stuff)
 *  3) Preparing to partition even further into tk_switch.c (saving this
 *     until next ci though).
 *
 *  Revision 1.6  2006-12-12 10:57:05  ambrmi09
 *  This adresses the second part of #1609064
 *
 *  Revision 1.5  2006/11/30 10:58:16  ambrmi09
 *  bug #1605910 (solved)
 *
 *  Revision 1.4  2006/11/27 22:29:22  ambrmi09
 *  Minor djustments completeing the move of some header files to public and due
 *  to some name clashed with user space naming conventions.
 *
 *  Revision 1.3  2006/04/08 10:15:58  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.2.2.1  2006/04/03 20:07:23  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.2  2006/03/27 13:04:17  ambrmi09
 *  Minor build tuning
 *
 *  Revision 1.1  2006/03/27 13:40:15  ambrmi09
 *  As part of the preparation for the first release, code has been cleaned up a little
 *  and project has been checked that it will build on all it's intended targets.
 *
 *  Problems that remained had to do the ANSI wrapping.
 *
 *  Some modifications were neserary to make the BC5 build, but the result is cleaner
 *  and more consistent with the rest of the wrapping. As a consequence, stdlib.h was
 *  introduced.
 *
 */
