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

#define SMC_BD_RX_BUFFLEN	160
#define SMC_BD_TX_BUFFLEN	160

/*Example 9600 8N1 
console_init(9600, 8,'N',1);
*/
int console_init(int bpr, int nr_bits, int par, int nr_stop);
int console_stdio_init();
int console_write(const char* buffer, int buff_len);
int console_read(char* buffer, int max_len);


#endif //CONSOLE_H

