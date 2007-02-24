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
#include "console.h"

#include <board/board.h>
#include <string.h>

static char RX_BUFFER[RX_BUFFLEN];
static char TX_BUFFER[TX_BUFFLEN];

int console_init(int bpr, int nr_bits, int par, int nr_stop){
	return 0; // i.e. OK
}

/* Simple reads and writes - ingore fancy stuff like IRQ handling and thread support for this example */

int console_write(const char* buffer, int buff_len){
	return buff_len;
}

int console_read(char* buffer, int max_len){
	return 0;
}


