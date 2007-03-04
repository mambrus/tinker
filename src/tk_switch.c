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
/*! 
@file 
@ingroup SCHED

@brief Tinker inner-most \e"guts"

The contents in this file concerns context switching. This is a kernels most
critical area where it's extremely vounerable. 

Two types of threats exist:

- Interrupts - Certain context switching techniques are sensitive for 
  interrupts (but not, not all).

- Preemptiveness - the functions in this file do not like to be preempted

@note This is a mandatory file as part of the \ref SCHED component

For in-depth discussions about this component, see \ref
SCHED

@see COMPONENTS
*/
