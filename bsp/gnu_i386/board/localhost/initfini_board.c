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

/*
Initialice board specific internals and perepherials
*/

#include <board/board.h>

#if ( TK_DCPU == __tk_i386__ )
void __init_board()
{
	//console_init(9600, 8,'N',1);
	console_init(115200, 8, 'N', 1);
}

#else
#error Sorry, TinKer can't handle the chosen CPU for this board
#endif

void __fini_board()
{
}

void __exeptions_enable_board()
{
}

void __exeptions_disable_board()
{
}
