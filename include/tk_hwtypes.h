/*******************************************************************
 *
 *  DESCRIPTION: TinKerOS.
 *
 *  AUTHOR: Michael Ambrus
 *                              
 *  HISTORY:
 *
 *  Current $Revision: 1.8 $
 *
 *******************************************************************/
  

#ifndef TK_HWTYPES_H
#define TK_HWTYPES_H

#if defined(DOXYGEN_ONLY)

//------1---------2---------3---------4---------5---------6---------7---------8


//------1---------2---------3---------4---------5---------6---------7---------8

#endif  //DOXYGEN_ONLY
   
//------1---------2---------3---------4---------5---------6---------7---------8

#if defined(_WIN32) &&  defined(_MSC_VER)
#include <kernel/bsp/X86_msvc/tk_hwtypes_mvcsX86.h>

#elif defined(__BORLANDC__) || defined(__BCPLUSPLUS__)
#include <../bsp/X86_Borland/tk_hwtypes_borlandX86.h>


#elif defined( __C166__ )
#include <../bsp/XC167_Keil/tk_hwtypes_keilC166.h>

#elif defined(__GNUC__)


   #ifndef ARCH
   #  error For GNU targets, ARCH has to be defined
   #endif

      
   #define str( x ) \
      #x

   #define INCLNAME( arch ) \
      <../bsp/gnu arch/tk_hwtypes-gnu arch.h>
   
   #define INCLABI( arch, abi )                      \
      <../bsp/gnu arch/tk_hwtypes-gnu arch abi.h>
   

   #ifdef ABI
   #   include INCLABI( ARCH, ABI )
   #else
   #   include INCLNAME( ARCH )
   #endif
   
   #undef str
   #undef INCLNAME
   #undef INCLABI

#else
#error "Can\'t determine the target for the TINKER kernel"

#endif

//------1---------2---------3---------4---------5---------6---------7---------8

#endif  //TK_HWSYS_H

  
/*!
 * @defgroup CVSLOG_tk_hwtypes_h tk_hwtypes_h
 * @ingroup CVSLOG
 *
 *  $Log: tk_hwtypes.h,v $
 *  Revision 1.8  2006-03-04 19:32:34  ambrmi09
 *  Modified sources to allow build GNU targets transparently.
 *
 *  Revision 1.7  2006/03/02 15:33:08  ambrmi09
 *  Experimenting with build under Linux
 *
 *  Revision 1.6  2006/03/02 14:05:48  ambrmi09
 *  Posting to GNU toolchain started
 *
 *  Revision 1.5  2006/02/28 13:18:19  ambrmi09
 *  Got MSVC target back-on track also.
 *
 *  Seems both MSVC and BCC have the same "error" regarding \ref CLK_TICK
 *  and/or \ref clock(). Could it be that it's TinKer's interpretation that
 *  is in fact wrong?
 *
 *  Revision 1.4  2006/02/28 11:50:07  ambrmi09
 *  - Trimmed the time constants (ruffly). 4sek per 14hrs drift
 *  - Revived the Borland C (BC5) target. Projectfile also added (BC5.ide)
 *  - Started experimenting with a indlude filename macro, that has the
 *    the potential of solving my ANSI header/function dilemma (\ref
 *    BUILDCHAIN )
 *  - Some "fishyness" about BC5 handling of time. Either \ref clock or
 *    \ref CLK_TCK doesn't follow standard (the latter I know for a fact,
 *    since it's 1e3 instead of 1e6 - but thats not all). \ref tk_msleep is
 *    adjusted to try to see the error.
 *
 *  Revision 1.3  2006/02/22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.2  2005/12/03 14:04:30  ambrmi09
 *  A crude documentation structure added. Sorce files modified a little, but
 *  only in comments (for Doxygens sake).
 *
 *
 *******************************************************************/






