/* Time.h - part of XC167 BSP for TinKer */

#ifndef time_h
#define time_h

//! According to POSIX standard this must be 1 milion regardless of true resolution
#define CLOCKS_PER_SEC 1000000L

//! Obsolete macro for historical reasons
#define CLK_TCK CLOCKS_PER_SEC


//typedef unsigned long clock_t;
#define clock_t unsigned long
//#define clock_t long
//#define clock_t unsigned int

clock_t clock();

#endif /*time_h*/


