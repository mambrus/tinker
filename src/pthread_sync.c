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
@ingroup PTHREAD_SYNC

@brief POSIX 1003.1c API - Thread synchronisation

The content of this source-file implement thread syncronisation. I.e. mutex, rw-lock e.t.a,

For in-depth discussions about this component, see \ref
PTHREAD_SYNC

@see PTHREAD_SCHED
@see PTHREAD

*/

#include <pthread.h>
#include <errno.h>
#include <assert.h>

unsigned long tk_pthread_sync( void ){
   return ERR_OK;    	
}
//------1---------2---------3---------4---------5---------6---------7---------8
unsigned long tk_pthread_sync_destruct( void ){
   return ERR_OK;
}
//------1---------2---------3---------4---------5---------6---------7---------8

 



/** @defgroup PTHREAD_SYNC PTHREAD_SYNC: POSIX 1003.1c API - Thread synchronisation
@ingroup PTHREAD
@brief POSIX 1003.1c API - Thread synchronisation

Syncronisation between threads, i.e. 

- Mutexes and attribute handling thereof
- Event handling
- Timout event handling - special case of the above

<p><b>Go gack to</b> \ref COMPONENTS</p>

*/
  
/*!
 *  @defgroup CVSLOG_pthread_sync_c pthread_sync_c
 *  @ingroup CVSLOG
 *  $Log: pthread_sync.c,v $
 *  Revision 1.5  2006-03-05 11:11:27  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.4  2006/02/22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.3  2006/02/20 19:17:14  ambrmi09
 *  - Made the errno variable thread specific (each thread has it's own)
 *  - Hid the details of using errno so that setting and reading it looks
 *    like using a normal variable
 *  - Extracted some stuff from tk.h that doesn't need to be public
 *  - Implemented perros and strerror including a storage with all the error
 *    strings (will go into NV ROM on a embedded system).
 *
 *  Revision 1.2  2006/02/20 15:22:01  ambrmi09
 *  Documentation stuff. No code changes.
 *
 *  Revision 1.1  2006/02/19 22:12:07  ambrmi09
 *  CO of missed files
 *
 *  
 *******************************************************************/
 













