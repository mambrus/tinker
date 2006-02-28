#ifndef ASSERT_H_TK
#define ASSERT_H_TK

//#include "kernel/src/tk_ansi.h"
#include <../src/tk_ansi.h>

#include BUILDCHAIN(assert.h)

#if   !defined (assert) && ( \
         defined(__CYGWIN32__)  || defined(__CYGWIN__)  || \
         defined(__GNUC__)      || defined(__USE_GNU)   || \
         defined(_WIN32)        || defined(__BORLANDC__) || defined(__BCPLUSPLUS__) \
      )

   #error "DONT USE TINKER'S ASSERT FOR THIS TOOL_CHAIN"
#endif

/*
#ifdef NDEBUG
#  define assert(p)   ((void)0)
#else
#  
#  define assert(p) ((p) ? (void)0 : (void) _tk_assertfail( \
                    #p, __FILE__, __LINE__ ) )              
#endif
*/


#if   !defined (assert)
/*!
The assert macro. 

@note Always assert, ignore NDEBUG setting
*/
#define assert(p) ((p) ? (void)0 : (void) _tk_assertfail( \
                    #p, __FILE__, __LINE__ ) )
#endif

#endif ASSERT_H_TK
/*!
 * @defgroup CVSLOG_assert_h assert_h
 * @ingroup CVSLOG
 *  $Log: assert.h,v $
 *  Revision 1.5  2006-02-28 18:59:23  ambrmi09
 *  Minor adjustments and verified that all (intended) build environment
 *  compile and produce runnable code. I.e.:
 *  - uVisionKernelProject
 *  - BC5
 *  - Core and POSIX projects in MSVC
 *
 *  Revision 1.4  2006/02/28 13:18:19  ambrmi09
 *  Got MSVC target back-on track also.
 *
 *  Seems both MSVC and BCC have the same "error" regarding \ref CLK_TICK
 *  and/or \ref clock(). Could it be that it's TinKer's interpretation that
 *  is in fact wrong?
 *
 *  Revision 1.3  2006/02/22 13:05:45  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  
 */
