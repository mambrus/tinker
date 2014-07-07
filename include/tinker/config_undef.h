/* This file contains undefines of macros that can interfere with userspace
   They should have been redefined previously in config.h */
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME

/* For each define that can interfere with the BUILDCHAIN_INCLUDE:s macros undefine
   them but save their settings just in case in WAS_DEFINED_xxx */
#ifdef linux
#define WAS_DEFINED_linux linux
#undef linux
#endif
