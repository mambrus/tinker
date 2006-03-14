/*
@file
This is the common header file for \ref STALLOC

*/
#ifndef STALLOCK_H
#define STALLOCK_H


/*
#if defined (XMEM)  
  #define MTYP          xhuge
  #define init_mempool  xinit_mempool
  #define calloc        xcalloc
  #define malloc        xmalloc
  #define free          xfree
  #define realloc       xrealloc
  #define __mp__        __xmp__
  #define TLEN          unsigned long
#else
  #define TLEN          unsigned int
  #if (__MODEL__ == 5 || __MODEL__ == 6)
    #define MTYP  huge   // for HCOMPACT and HLARGE
  #else
    #define MTYP  far    // for other models
  #endif
#endif*/

#define stsize_t          unsigned int
#define ST_MTYP           huge
#define ST_NULL           ((void ST_MTYP*)0)

void *stalloc ( stsize_t size);
void stalloc_free (void ST_MTYP* memp);
void stalloc_init_mempool (void ST_MTYP *pool, stsize_t size);

#endif //STALLOCK_H

//------1---------2---------3---------4---------5---------6---------7---------8
/** @defgroup STALLOC STALLOC - Stack malloc
@ingroup PACKAGES
@brief Dedicated malloc for thread stacks

This is a modified malloc to provide stack threads with their dedicated memory.

The main reasons for this package are:


- Improved debugging. By not having to see other memory blocks in the
layout. It's easier to se in the mem-dump if things are operating as
they should. In normal case

- It becomes easier to debug ISR and scheduler related issues since
other blocks don't (like like the malloced messaged blocks for queues)
clobber the memory layout.
 
- Rouge pointer operations on heap becomes less probable to corrupt
stacks threads (with normal malloc any block can come in between any
stack block). Common +1/-1 errors might corrupt nearby stack and in turn
hang the whole system.
 
- In XC167 case, we're limited to a memory address range of 16K for
<b>all stacks</b>. However we have plenty more memory on the eval card
(256KB + 512KB !)
 
- In other processors as in XC167 there might be memory areas with
faster (or non cashable) memory. Those are perfect for stacks (the
plainer the better).
 
- It might be possible to implement some kind of stack starvation trap.
This would be very nice indeed since that is the single most common
error in multi-threaded applications (i.e. this has nothing to do with
the kernel itself, but is a service to aid system development).
 
At the moment only XC167 has this package implemented. Other targets are
likely to use simple macro's to map \e stalloc to \e malloc and
\e stalloc_free to \e free. 

<p><b>Go back to</b> \ref PACKAGES</p>

*/
