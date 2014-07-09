/***************************************************************************
 *   Copyright (C) 2011 by Michael Ambrus                                  *
 *   ambrmi09@gmail.com                                                    *
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
#include <tinker/config.h>

#if  TK_SYSTEM != __SYS_HOSTED__

/* Define the root-stack for bare-silicon systems. The array will be placed
 * in a section of it's own defined by the linker script and can hence be
 * moved around
 *
 * Note that if section isn't defined, array will be placed somewhere in
 * .bss. I.e. Alignment might become a problem for some architectures. I.e.
 * you should define that section or add alignment attribute fitting your
 * CPU.*/
unsigned char root_stack[TK_ROOT_STACK_SIZE]
    __attribute__ ((section("stack")));
#else

#ifdef TK_ROOT_STACK_SIZE

#error TK_ROOT_STACK_SIZE should not be defined on a hosted system.\
	Please check your configuration.

#endif
#endif
