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

#include "led.h"
#include <board/board.h>

void led_on(led_id id, led_color color){
	if (id==LED_2)
		if (color==LED_RED)
			bitclear_16(PADAT,8);	
		else
			bitclear_16(PADAT,9);	
	else
		if (color==LED_RED)
			bitclear_16(PADAT,10);	
		else
			bitclear_16(PADAT,11);	
}

void led_off(led_id id, led_color color){
	if (id==LED_2)
		if (color==LED_RED)
			bitset_16(PADAT,8);	
		else
			bitset_16(PADAT,9);	
	else
		if (color==LED_RED)
			bitset_16(PADAT,10);	
		else
			bitset_16(PADAT,11);	
}

