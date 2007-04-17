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

#ifndef CONSOLE_H
#define CONSOLE_H

#define SMC_BD_RX_BUFFLEN	160	//!< RX length of the buffert in DPRAM BD
#define SMC_BD_TX_BUFFLEN	160	//!< TX length of the buffert in DPRAM BD



/*Example 9600 8N1 
console_init(9600, 8,'N',1);
*/
int console_init(int bpr, int nr_bits, int par, int nr_stop);
int console_write(const char* buffer, int buff_len);
int console_read(char* buffer, int max_len);

#endif //CONSOLE_H

