/*!
@file
@ingroup kernel_reimpl_ansi

@brief ANSI functions reimplemented

Special file for getting tool-chain head files.

@note <b>This file is supposed to be included "several" times.</b> Hence 
these is no "#ifndef" protection.

For in-depth discussions about re-implementing ANSI functions, see \ref
kernel_reimpl_ansi

@see kernel_reimpl_ansi

*/


#ifndef YES
#define YES 1
#endif

#ifndef NO
#define NO 0
#endif


//------1---------2---------3---------4---------5---------6---------7---------8
#if defined(_WIN32) &&  defined(_MSC_VER)
   #define CHAINPATH C:\Program Files\Microsoft Visual Studio\VC98\Include
   #define USE_TINKER_TIME_F        NO
   #define USE_TINKER_CLOCK_F       NO

#elif defined(__BORLANDC__) || defined(__BCPLUSPLUS__)
   #define CHAINPATH C:/BC5/INCLUDE
   #define USE_TINKER_TIME_F        NO
   #define USE_TINKER_CLOCK_F       NO

#elif defined( __C166__ )
   #define CHAINPATH 
   #define USE_TINKER_TIME_F        YES
   #define USE_TINKER_CLOCK_F       YES

#elif defined(__GNUC__)
   #define CHAINPATH 
#else
   #error "Can\'t determine the target for the TINKER kernel"
#endif
//------1---------2---------3---------4---------5---------6---------7---------8

#define DEFSTR( x ) \
   #x

#define FNAME( path, file ) \
   DEFSTR( path/##file )
   
#define BUILDCHAIN( file ) \
   FNAME( CHAINPATH, ##file ) \

/*! 
 * @defgroup CVSLOG_tk_ansi_dirwrap_h tk_ansi_dirwrap_h
 * @ingroup CVSLOG
 *
 *  $Log: tk_ansi_dirwrap.h,v $
 *  Revision 1.1  2006-02-28 18:40:11  ambrmi09
 *  initial ci
 *
 *
 */
