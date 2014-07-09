/***************************************************************************
 *   Copyright (C) 2014 by Michael Ambrus                                  *
 *   michael.ambrus@gmail.com                                              *
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
#ifndef STYPES_GNU_X86_64_H
#define STYPES_GNU_X86_64_H

#include <stddef.h>

#ifndef STACK_DEBUG
#define STACK_DEBUG 1
#endif

#ifndef STACK_SMASH
#define STACK_SMASH 0
#endif

#if JUMPER_BASED

typedef void * stackptr_t;

#include <setjmp.h>
typedef struct {
	stackptr_t bos;		/* The stacks content. This is the base (BOS -
						   Bottom Of Stack) and it's what malloc/free/mmap
						   will operate on. When used, it's a "blob" as in
						   binary blob with unknown content. There's one
						   unique aspect of it however: It's always used
						   from one end first.  Combined with memory
						   management, malloc is commonly implemented using
						   mmap for large chunks, when running Tinker in
						   hosted mode, this means that you can overcommit
						   the size without actually consuming more physical
						   memory than gets used (rounded upwards in storage
						   blocks usually 4 or 16k) */

	size_t stack_size;	/* The original size allocated to stack */
	stackptr_t sp;		/* TOS - Top Of Stack (when not running) / copy of
						   $sp */

	jmp_buf context;	/* Current context (when not running) stored in a
						   format that longjmp can use */

#if STACK_DEBUG
#include <stddef.h>

/* Convenience variables to somewhat help debugging. Note that inspecting
   neither can be used to detect stack-smashing or rouge pointer writes on
   stack variables. Only use is to *roughly* determine stack-usage in a
   error-free program */

	ptrdiff_t free;    /* Size of stack when going going of context */
	ptrdiff_t inuse;   /* Stack in use when going out of context*/
#endif //STACK_DEBUG

#if STACK_SMASH
	unsigned long stack_crc; /* CRC of a predefined area */
#endif //STACK_SMASH


} tk_stack_t;
#endif

#endif
