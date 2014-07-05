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

#ifndef ICTRL_H
#define ICTRL_H

/*Stuff for the Instruction Support Control Register (ICTRL)
The instruction support control register (ICTRL), shown in Figure 44-18, is used to
conﬁgure instruction breakpoint operations.
*/

#include <sys/types.h>

/*
 0–2    CTA  Compare type of comparator A–D
             0xx Not active (reset value)
 3–5    CTB  100 Equal
             101 Less than
 6–8    CTC
             110 Greater than
9–11    CTD  111 Not equal
12–13   IW0  Instruction ﬁrst watchpoint programming.
             0x Not active (reset value)
             10 Match from comparator A
             11 Match from comparators (A & B)
14–15   IW1  Instruction second watchpoint programming.
             0x Not active (reset value)
             10 Match from comparator B
             11 Match from comparators (A | B)
16–17   IW2  Instruction third watchpoint programming.
             0x Not active (reset value)
             10 Match from comparator C
             11 Match from comparators (C & D)
18–19   IW3  Instruction fourth watchpoint programming.
             0x Not active (reset value)
             10 Match from comparator D
             11 Match from comparators (C | D)
 20   SIW0EN Software trap enable selection of instruction watchpoints 0–3.
             0 Trap disabled (reset value)
 21   SIW1EN 1 Trap enabled
 22   SIW2EN
 23   SIW3EN
 24   DIW0EN Development port trap enable selection of the instruction watchpoints 0–3 (read-only bit).
             0 Trap disabled (reset value)
 25   DIW1EN 1 Trap enabled
 26   DIW2EN
 27   DIW3EN

 28      IFM   Ignore ﬁrst match, only for instruction breakpoints.
               0 Do not ignore ﬁrst match, used for “go to x” (reset value).
               1 Ignore ﬁrst match (used for “continue”).
29–31 ISCT_SER Instruction fetch show cycle/core serialize control. Changing the instruction show cycle
               programming takes effect only from the second instruction after the mtspr[ICTRL].
               000 Core is fully serialized; show cycle is performed for all fetched instructions (reset value).
               001 Core is fully serialized; show cycle is performed for all changes in program ﬂow.
               010 Core is fully serialized; show cycle is performed for all indirect changes in program ﬂow.
               011 Core is fully serialized; no show cycles is performed for fetched instructions.
               100 Illegal.
               101 Core is not serialized (normal mode); show cycle is performed for all changes in the
                     program ﬂow. If the fetch of the target of a direct branch is aborted by the core (because of
                     an exception), the target is not always visible on the external pins. Program trace is not
                     affected by this phenomenon.
               110 Core is not serialized (normal mode; show cycle is performed for all indirect changes in
                     program ﬂow.
               111 Core is not serialized (normal mode); no show cycle is performed for fetched instructions.
               When ISCT_SER = 010 or 110, the STS functionality of OP2/MODCK1/STS must be enabled by
               writing 10 or 11 to SIUMCR[DBGC]. The address on the external bus should be sampled only
               when STS is asserted.

*/

typedef union {
	__uint32_t raw;
	struct {

		__uint32_t CTA:3;
		__uint32_t CTB:3;
		__uint32_t CTC:3;
		__uint32_t CTD:3;

		__uint32_t IW0:2;
		__uint32_t IW1:2;
		__uint32_t IW2:2;
		__uint32_t IW3:2;

		__uint32_t SIW0EN:1;
		__uint32_t SIW1EN:1;
		__uint32_t SIW2EN:1;
		__uint32_t SIW3EN:1;
		__uint32_t DIW0EN:1;
		__uint32_t DIW1EN:1;
		__uint32_t DIW2EN:1;
		__uint32_t DIW3EN:1;
		__uint32_t IFM:1;

		__uint32_t ISCT_SER:3;
	} f;
} ictrl_t;

#endif				//ICTRL_H
