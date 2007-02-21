/*+***************************************************************************
**
**
**
**
** Original Author: Mihael Ambrus	Creation Date: 070129
**
** Approved:	Department: MCC-VS
**
**                 Copyright (c) Maquet Critical Care, Sweden
**                        All rights reserved
**
***************************************************************************-*/
#ifndef CONSOLE_H
#define CONSOLE_H


/*Example 9600 8N1 
console_init(9600, 8,'N',1);
*/
void console_init(int bpr, int nr_bits, int par, int nr_stop);
void console_write(const char* buffer, int buff_len);
void console_read(char* buffer, int max_len);

#endif //CONSOLE_H

