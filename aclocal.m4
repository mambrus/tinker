AC_DEFUN([TINKER_AM_PREREQ],
[

AC_CHECK_PROG(HAS_AUTOMAKE, automake, yes, no) 

if test -f install-sh; then
   echo "install-sh exists... good!"
else
   m4_pattern_allow(AM_VERSION_LOCAL)
   m4_pattern_allow(THELINK)

   dnl remove any bad links
   rm -f install-sh
   rm -f config.sub
   rm -f config.guess
   
   AM_VERSION_LOCAL=$(automake --version | grep 'GNU' | sed -e 's/.* //' | sed -e 's/-.*$//')
   if test -f /usr/share/automake/install-sh; then
      ln -s /usr/share/automake/install-sh .
   else
      ln -s /usr/share/automake-$AM_VERSION_LOCAL/install-sh .
      if test ! -f install-sh; then
         AC_MSG_WARN([Can't link install-sh - trying 2 first digits instead])
         rm -f install-sh
         AM_VERSION_LOCAL=$(echo $AM_VERSION_LOCAL | cut -f1,2 -d".")
         ln -s /usr/share/automake-$AM_VERSION_LOCAL/install-sh .
      fi
   fi
   THELINK=$(ls -ald install-sh | cut -f2 -d">")
   echo "install-sh linked to $THELINK"
   AMDIR=$(echo $THELINK | sed -e 's/install-sh$//' | sed -e 's/.$//')
   echo "Automake data directory is $AMDIR"
   ln -s ${AMDIR}/config.sub .
   ln -s ${AMDIR}/config.guess .

   dnl The following seems needed to reninitialize the search-patch
   AC_CONFIG_AUX_DIR(.)
fi

])

AC_DEFUN([TINKER_CONFIGURE],
[

TINKER_PATH=$1
AC_SUBST(TINKER_PATH)

dnl if called from kernel directory create the AM prerequisits 
dnl All other should use AC_CONFIG_AUX_DIR
if test $1 == "."; then
   TINKER_AM_PREREQ
fi

dnl Find and set the C compiler
AC_PROG_CC
AC_PROG_CC(gcc)
AC_LANG_C

dnl Expand the canonicals
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AC_MSG_NOTICE([Build system CPU: $build_cpu])
AC_MSG_NOTICE([Build system vendor: $build_vendor])
AC_MSG_NOTICE([Build system OS: $build_os])
AC_MSG_NOTICE([Host system CPU: $host_cpu])
AC_MSG_NOTICE([Host system vendor: $host_vendor])
AC_MSG_NOTICE([Host system OS: $host_os])
AC_MSG_NOTICE([Target system CPU: $target_cpu])
AC_MSG_NOTICE([Target system vendor: $target_vendor])
AC_MSG_NOTICE([Target system OS: $target_os])

dnl Deduct the system call API based on TinKer's convention of asuming
dnl the vendor part will tell us.
if test $host_vendor == "unknown"; then
   SYSTEM="default"
else
   SYSTEM=$host_vendor
fi

AC_SUBST(SYSTEM)
AC_SUBST(host_alias)

dnl What is this?
AC_PROG_MAKE_SET 

AC_HEADER_STDC
AC_CHECK_FUNCS(clock)

dnl Check for broken compiler setup
AC_HAVE_LIBRARY(gcc_s)

AC_CHECK_PROG([FLASHUTIL], [lpc21isp], [lpc21isp], [no_flash_util])
AC_CHECK_PROG([SERIALTERM], [minicom], [xterm -e minicom &], [./tterm.sh])

dnl build system info
BS_MAIN=$(uname -s | sed -r 's/(.*)(-)(.*)/\1/' | sed -r 's/(.*)(_)(.*)/\1/')
BS_SUB=$(uname -s | sed -r 's/(.*)(-)(.*)/\1/' | sed -r 's/(.*)(_)(.*)/\3/')
BS_VER=$(uname -s | sed -r 's/(.*)(-)(.*)/\3/')

AC_SUBST(BS_MAIN)
AC_SUBST(BS_SUB)
AC_SUBST(BS_VER)

if test BS_MAIN == Cygwin; then
   SERIAL_PORT=COM1
else
   SERIAL_PORT=/dev/ttyS0
fi

AC_SUBST(SERIAL_PORT)

AC_CHECK_TOOL([LD],      [ld],      [:])
AC_CHECK_TOOL([AR],      [ar],      [:])
AC_CHECK_TOOL([OBJCOPY], [objcopy], [:])
AC_CHECK_TOOL([OBJDUMP], [objdump], [:])
AC_PATH_TOOL([GCC_PATH], [gcc],     [:])


CANONICAL_HOST=$host
AC_SUBST(CANONICAL_HOST)

TOOLDIR=$(echo $GCC_PATH | sed -e "s/\/bin\/$CC//")
AC_SUBST(TOOLDIR)

if test $cross_compiling == yes; then
   XCOMPILE=1
   ABI=$(echo $host | sed -e s/.*-//)
   ARCH=$(echo $host | sed -e s/-.*//)
else
   XCOMPILE=0

   #$build, $target, host are missing in this case. Need AC_CANNONICAL_HOST but that inplies extra scripts (?)
   #Prefered line below cant be used
   #ARCH=$(echo $build | sed -e s/-.*//)

   ARCH=$(uname -m | sed -e s/i.86/i386/ -e s/sun4u/sparc64/ -e s/arm.*/arm/ -e s/sa110/arm/)
   ABI=""
fi
AC_SUBST(XCOMPILE)
AC_SUBST(ARCH)
AC_SUBST(ABI)

GCC_VERSION=$($CC -v 2>&1 | grep version | sed -e 's/gcc version //' | cut -f1 -d " ")
AC_SUBST(GCC_VERSION)

dnl Do not accept default autoconf CFLAGS. Tinker will not run with them (not on ARM tdmi at leat)
CFLAGS=""
AC_SUBST(CFLAGS)

AC_ARG_VAR(BOARD, [Selects which board to build TinKer BSP for. Valid values are:\n
   BITFIRE])

if test $cross_compiling == yes; then
   if test -z $BOARD; then
      AC_MSG_WARN([<<< You are configuring for XCOMPILE but no BOARD is selected. 
                   Compilation will probably fail...])
   fi
else
   if test -n $BOARD; then
      AC_MSG_WARN([<<< You are configuring for NATIVE but BOARD is selected. 
                   This option does no purpose...])
   fi
fi

AC_PROG_INSTALL

#if test $cross_compiling == yes; then
#   case "$BOARD" in
#   BITFIRE | lpc21* )
#      AC_CONFIG_SUBDIRS([kernel/bsp kernel/bsp/gnu_arm kernel/bsp/gnu_arm/lpc21xx]) ;;
#   NOBOARD )
#       AC_CONFIG_SUBDIRS([kernel/NOBSP])
#   esac
#fi

ALL_C=$sources_c
AC_SUBST(ALL_C)
ALL_ASM=$sources_asm
AC_SUBST(ALL_ASM)

])