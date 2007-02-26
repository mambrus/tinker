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

@brief ANSI sys/stat.h file <b>IMPORTANT:</b> See the note in the detailed section!

@note Needed to wrap out incompatible definitions in newlib

For in-depth discussions about re-implementing ANSI functions, see \ref
kernel_reimpl_ansi.


@see kernel_reimpl_ansi

<HR>
*/

#if defined (__GNUC__)
   #include <tk_ansi.h>
   #include <tk_ansi_dirwrap.h>

   #if defined (__rtems__)
   #undef __rtems__
   #define __was_rtems__
   #endif

   #include BUILDCHAIN(sys/stat.h)

   #if defined (__was_rtems__)
   #define __rtems__
   #endif

/*!
The mknod function makes a special file with name filename. The 
mode specifies the mode of the file, and may include the various 
special file bits, such as S_IFCHR (for a character special file) 
or S_IFBLK (for a block special file). See Testing File Type.

The dev argument specifies which device the special file refers to. 
Its exact interpretation depends on the kind of special file being 
created.

The return value is 0 on success and -1 on error. In addition to 
the usual file name errors (see File Name Errors), the following 
errno error conditions are defined for this function:

EPERM
    The calling process is not privileged. Only the superuser can 
create special files.
ENOSPC
    The directory or file system that would contain the new file is 
full and cannot be extended.
EROFS
    The directory containing the new file can't be modified because 
it's on a read-only file system.
EEXIST
    There is already a file named filename. If you want to replace 
this file, you must remove the old file explicitly first. 

http://www.opengroup.org/onlinepubs/009695399/functions/mknod.html
*/

int mknod(const char *filename, mode_t mode, dev_t dev);
//int mknod (const char *filename, int mode, int dev);
//int	_EXFUN(mknod,( const char *__path, mode_t __mode, dev_t __dev ));

#endif

#ifndef SYS_TYPES_H_TK
#define SYS_TYPES_H_TK

#endif //SYS_TYPES_H_TK


/*!
 * @defgroup CVSLOG_stat_h stat_h
 * @ingroup CVSLOG
 *  $Log: stat.h,v $
 *  Revision 1.1  2007-02-26 14:16:53  ambrmi09
 *  1) Drivers    - structure added
 *  2) Filesystem - in progress
 *
 */

