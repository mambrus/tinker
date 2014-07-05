/***************************************************************************
 *   Copyright (C) 2007 by Michael Ambrus                                  *
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

/* libc/sys/linux/crt0.c - Run-time initialization */

/* FIXME: This should be rewritten in assembler and
          placed in a subdirectory specific to a platform.
          There should also be calls to run constructors. */

/* Written 2000 by Werner Almesberger */

#include <stdlib.h>
#include <time.h>
#include <string.h>

//extern char **environ;
char environ_buff[1024];
char **environ = environ_buff;

extern int main(int argc, char **argv, char **envp);

extern char _end;
extern char __bss_start;

void _start(int args)
{
	/*
	 * The argument block begins above the current stack frame, because we
	 * have no return address. The calculation assumes that sizeof(int) ==
	 * sizeof(void *). This is okay for i386 user space, but may be invalid in
	 * other cases.
	 */
	int *params = &args - 1;
	int argc = *params;
	char **argv = (char **)(params + 1);

	environ = argv + argc + 1;

	/* Note: do not clear the .bss section.  When running with shared
	 *       libraries, certain data items such __mb_cur_max or environ
	 *       may get placed in the .bss, even though they are initialized
	 *       to non-zero values.  Clearing the .bss will end up zeroing
	 *       out their initial values.  The .bss is already initialized
	 *       by this time by Linux.  */

	//tzset(); /* initialize timezone info */
	_exit(main(argc, argv, environ));

}
