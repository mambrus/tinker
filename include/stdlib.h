/***************************************************************************
 *   Copyright (C) 2006 by Michale Ambrus                                  *
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
with special pointertypes. Using functions that use those 
pointers will most likelly break in RUN-TIME*/
#include <../src/tk_ansi.h>
#include <../src/tk_ansi_dirwrap.h>
#include BUILDCHAIN(stdlib.h)


#ifndef STDLIB_H_TK
#define STDLIB_H_TK

#include <stddef.h>  //Needed for size_t

#define TINKER_SEARCH_SORT //!< Opional define, use to debug qsort & bsearch. Undefine to use tool-chains own versions

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
typedef int comparison_fn_t (  
   const void *L,  //!< <em>"Leftmost"</em> element to compare with
   const void *R   //!< <em>"Rightmost"</em> element to compare with
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
void *bsearch (const void *, const void *, size_t , size_t , comparison_fn_t );
void  qsort   (void *, size_t , size_t , comparison_fn_t );         
//@}

/*!
@name 

Internal functions. Do \b NOT use this if you want to write portable \b application code

*/
//@{
void _tk_quicksort ( void *, int, int, int, comparison_fn_t );
int  _tk_bsearch   ( void *, void *, int, int, int, comparison_fn_t );
//@}

#endif //STDLIB_H_TK
/*!
 * @defgroup CVSLOG_stdlib_h stdlib_h
 * @ingroup CVSLOG
 *  $Log: stdlib.h,v $
 *  Revision 1.2  2006-03-27 13:04:17  ambrmi09
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

