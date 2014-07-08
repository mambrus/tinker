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
#ifndef YES
#define YES 1
#endif

#ifndef NO
#define NO 0
#endif

#ifndef _SYS_FEATURES_H
#define _SYS_FEATURES_H
#endif

#if defined(_WIN32) && defined(_MSC_VER)
#define CHAINPATH_INCLUDE C:\Program Files\Microsoft Visual Studio\VC98\Include

#elif defined(__BORLANDC__) || defined(__BCPLUSPLUS__)
#define CHAINPATH_INCLUDE C:/BC5/INCLUDE

#elif defined( __C166__ )

#define CHAINPATH_INCLUDE C:\Keil\C166\INC

#elif defined(__GNUC__)
#include <tinker/config.h>

#ifndef CHAINPATH_INCLUDE
#error Undefined CHAINPATH_INCLUDE (fatal error)
#endif

#ifndef CHAINPATH_SYS_INCLUDE
#error Undefined CHAINPATH_SYS_INCLUDE (fatal error) 
#endif

#ifdef bfin
#warning Workaround enabled for GCC builtin macro bug: bfin
#undef bfin
#ifndef __bfin__
#define __bfin__
#endif
#endif
#ifdef BFIN
#warning Workaround enabled for GCC builtin macro bug: BFIN
#undef BFIN
#ifndef __BFIN__
#define __BFIN__
#endif
#endif
#ifdef i386

#undef i386
#ifndef __i386__
#define __i386__
#endif
#endif

#else
#error "Can\'t determine the target for the TINKER kernel"
#endif

#define DEFSTR(x) \
   #x

#define FNAME(path,file) \
   DEFSTR( path/file )

#define BUILDCHAIN_INCLUDE(file) \
   FNAME( CHAINPATH_INCLUDE, file )

#define BUILDCHAIN_SYS_INCLUDE(file) \
   FNAME( CHAINPATH_SYS_INCLUDE, file )

