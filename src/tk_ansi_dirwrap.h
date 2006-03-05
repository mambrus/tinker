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

@brief ANSI functions reimplemented

Special file for getting tool-chain head files.

@note <b>This file is supposed to be included "several" times.</b> Hence 
these is no "#ifndef" protection.

For in-depth discussions about re-implementing ANSI functions, see \ref
kernel_reimpl_ansi

@see kernel_reimpl_ansi

*/


#ifndef YES
#define YES 1
#endif

#ifndef NO
#define NO 0
#endif


//------1---------2---------3---------4---------5---------6---------7---------8
#if defined(_WIN32) &&  defined(_MSC_VER)
   #define CHAINPATH C:\Program Files\Microsoft Visual Studio\VC98\Include

#elif defined(__BORLANDC__) || defined(__BCPLUSPLUS__)
   #define CHAINPATH C:/BC5/INCLUDE

#elif defined( __C166__ )
   #define CHAINPATH C:\Program Files\Keil\C166\BIN

#elif defined(__GNUC__) /*|| defined(__CYGWIN32__)  || defined(__CYGWIN__)*/
   #ifndef CHAINPATH
      #define CHAINPATH /usr/include
   #endif

#else
   #error "Can\'t determine the target for the TINKER kernel"
#endif
//------1---------2---------3---------4---------5---------6---------7---------8

#define DEFSTR( x ) \
   #x

#define FNAME( path, file ) \
   DEFSTR( path/file )
   
#define BUILDCHAIN( file ) \
   FNAME( CHAINPATH, file ) \


/*! 
 * @defgroup CVSLOG_tk_ansi_dirwrap_h tk_ansi_dirwrap_h
 * @ingroup CVSLOG
 *
 *  $Log: tk_ansi_dirwrap.h,v $
 *  Revision 1.5  2006-03-05 11:11:27  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.4  2006/03/04 19:32:35  ambrmi09
 *  Modified sources to allow build GNU targets transparently.
 *
 *  Revision 1.3  2006/03/04 14:28:44  ambrmi09
 *  Finally got the \ref clock() representation right. Now timing is
 *  behaving equaly between the targets X86_Linux, Cygqing, MSVC, BC5 and
 *  XC167.
 *
 *  Revision 1.2  2006/03/02 14:05:50  ambrmi09
 *  Posting to GNU toolchain started
 *
 *  Revision 1.1  2006/02/28 18:40:11  ambrmi09
 *  initial ci
 *
 *
 */
