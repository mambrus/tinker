#ifndef TK_HWTYPES_H
#define TK_HWTYPES_H    

#if defined(DOXYGEN_ONLY)

//------1---------2---------3---------4---------5---------6---------7---------8


//------1---------2---------3---------4---------5---------6---------7---------8

#endif  //DOXYGEN_ONLY
   
//------1---------2---------3---------4---------5---------6---------7---------8
#if defined(WIN32) && defined(_MSVC_)
#include <../bsp/X86_msvc/tk_hwtypes_msvcX86.h>

#elif defined(WIN32) && defined(_BC50_)
#include <../bsp/X86_Borland/tk_hwtypes_borlandX86.h>


#elif defined( __C166__ )
#include <../bsp/XC167_Keil/tk_hwtypes_keilC166.h>


#elif defined(__GNUC__)
#error "Implementation (only simulated \"scheduler in process\" possible) for a GNU system not done yet"

#else
#error "Can\'t determine the target for the TINKER kernel"   

#endif

//------1---------2---------3---------4---------5---------6---------7---------8

#endif  //TK_HWSYS_H







