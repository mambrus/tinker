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
#if defined (__GNUC__)
#include <tk_ansi.h>
#include <tk_ansi_dirwrap.h>

#if defined (__rtems__)
#undef __rtems__
#define __was_rtems__
#endif

#include BUILDCHAIN_SYS_INCLUDE(stat.h)

#if defined (__was_rtems__)
#define __rtems__
#endif

int mknod(const char *filename, mode_t mode, dev_t dev);

int mkfifo(const char *path, mode_t mode);

#endif

#ifndef SYS_TYPES_H_TK
#define SYS_TYPES_H_TK

#endif
