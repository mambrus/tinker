dnl This function replaces the need of automake to get some files that autoconf
dnl needs. TinKer does not use automake because of it's version incompatibility
dnl problems. However, autoconf seems to have some dependency towards automake
dnl in the form of some mandatory scripts and files. This function takes care
dnl of that.
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

	dnl The following seems needed to re-initialize the search-patch
	AC_CONFIG_AUX_DIR(.)
fi

])

dnl Options affecting the build
dnl Note: Default values are set here.
dnl ----------------------------------------------------------------------------
AC_DEFUN([TINKER_OPTIONS_BUILD],
[
	AC_ARG_ENABLE(depmake,
		AS_HELP_STRING([--enable-depmake],[Build - Enable/disable makefile targets to depend on Makefile and Makefile-gnu. Optionally you can assign a value naming which specific files to depend on]),
		DEPMAKE=__tk_$enableval,
		DEPMAKE="Makefile-gnu Makefile"
	)
	AC_ARG_ENABLE(cplusplus,
		AS_HELP_STRING([--enable-cplusplus],[Build - Enable/disable using C++ for compilation and linking]),
		USECPLUSPLUS=__tk_$enableval,
		USECPLUSPLUS=__tk_no
	)
	dnl Note that the omission of '__tk_' below is intentional
	AC_ARG_ENABLE(makeopts,
		AS_HELP_STRING([--enable-makeopts],[Build - Pass specified make options to recursive makes (=arg). Options worth considering are -S, -w]),
		MAKEOPTS=$enableval,
		MAKEOPTS=""
	)

	dnl The following test will handle defaults but also permit specific argument values
	if test "$DEPMAKE" == __tk_yes; then
		DEPMAKE="Makefile-gnu Makefile"
	elif test "$DEPMAKE" == __tk_no; then
		DEPMAKE=""
	fi
	AC_SUBST(DEPMAKE)
	AC_SUBST(MAKEOPTS)

	AC_ARG_ENABLE(subdir-verbose,
		AS_HELP_STRING([--enable-subdir-verbose],[Build - If extra printouts should be made for each subdir build (=arg). Valid options are yes/no. Default is no]),
		SUB_VERBOSE=$enableval,
		SUB_VERBOSE="no"
	)
	AC_SUBST(SUB_VERBOSE)

	if test -z $CRT0_OBJECT; then
                AC_MSG_NOTICE([<<< ctr0.o is based on startup_gnu.ao (default)])
                CRT0_OBJECT=startup_gnu.ao
        else
                AC_MSG_NOTICE([<<< ctr0.o is based on $CRT0_OBJECT])
        fi
	AC_SUBST(CRT0_OBJECT)

	#echo "------------------------------------------------------> $extern_includes"
	#Annoying, the below does not work - ^ is the problem
	#ext_incl=$(echo $extern_includes  | sed -e 's/[^ ]\+/-I &/g')

	if test -n "$extern_includes"; then
		ext_incl=$(echo -n "-I "; echo $extern_includes  | sed -e 's/ \+/ -I &/g')
	fi

	#ext_incl=$(echo $extern_includes  | sed -e 's/[\/]\+/ XXX /g')
	#echo "------------------------------------------------------> $ext_incl"

	AC_SUBST(ext_incl)
])

dnl Options affecting the configuration
dnl Note: Default values are set here.
dnl ----------------------------------------------------------------------------
AC_DEFUN([TINKER_OPTIONS_CONFIG],
[
	AC_ARG_ENABLE(verbose-config,
		AS_HELP_STRING([--enable-verbose-config],[Configure - Shows extra information while running configure script]),
		CONF_VERBOSE=$enableval,
		CONF_VERBOSE="no"
	)

])

dnl Expands additional canonical information
dnl ----------------------------------------------------------------------------
AC_DEFUN([TINKER_CANONICAL_EXTRA],
[
	dnl Expand the canonicals
	AC_CANONICAL_BUILD
	AC_CANONICAL_HOST
	AC_CANONICAL_TARGET

   BUILD_CPU=$build_cpu
   BUILD_VENDOR=$build_vendor
   BUILD_OS=$build_os
   HOST_CPU=$host_cpu
   HOST_VENDOR=$host_vendor
   HOST_OS=$host_os
   TARGET_CPU=$target_cpu
   TARGET_VENDOR=$target_vendor
   TARGET_OS=$target_os

	AC_SUBST(BUILD_CPU)
	AC_DEFINE_UNQUOTED([CANON_BUILD_CPU],$BUILD_CPU)
	AC_SUBST(BUILD_VENDOR)
	AC_DEFINE_UNQUOTED([CANON_BUILD_VENDOR],$BUILD_VENDOR)
	AC_SUBST(BUILD_OS)
	AC_DEFINE_UNQUOTED([CANON_BUILD_OS],$BUILD_OS)

	AC_SUBST(HOST_CPU)
	AC_DEFINE_UNQUOTED([CANON_HOST_CPU],$HOST_CPU)
	AC_SUBST(HOST_VENDOR)
	AC_DEFINE_UNQUOTED([CANON_HOST_VENDOR],$HOST_VENDOR)
	AC_SUBST(HOST_OS)
	AC_DEFINE_UNQUOTED([CANON_HOST_OS],$HOST_OS)

	AC_SUBST(TARGET_CPU)
	AC_DEFINE_UNQUOTED([CANON_TARGET_CPU],$TARGET_CPU)
	AC_SUBST(TARGET_VENDOR)
	AC_DEFINE_UNQUOTED([CANON_TARGET_VENDOR],$TARGET_VENDOR)
	AC_SUBST(TARGET_OS)
	AC_DEFINE_UNQUOTED([CANON_TARGET_OS],$TARGET_OS)

])

dnl Prints configured components
dnl ----------------------------------------------------------------------------
AC_DEFUN([TINKER_PRINT_COMPONENTS],
[
   echo "                   COMPONENTS"
   echo "                   =========="
   echo "TK_COMP_ITC:                              $_TK_COMP_ITC"
   echo "TK_COMP_PTIMER:                           $_TK_COMP_PTIMER"
   echo "TK_COMP_KMEM:                             $_TK_COMP_KMEM"
   echo "TK_COMP_PTHREAD:                          $_TK_COMP_PTHREAD"
   echo "TK_COMP_POSIX_RT:                         $_TK_COMP_POSIX_RT"
   echo "TK_COMP_FILESYS:                          $_TK_COMP_FILESYS"
   echo
   echo "                 PACKAGES & misc"
   echo "                 ==============="
   echo "TK_USE_BUILTIN_SORT:                      $_TK_USE_BUILTIN_SORT"
])

dnl Prints configured component settings
dnl ----------------------------------------------------------------------------
AC_DEFUN([TINKER_PRINT_COMP_SETTINGS],
[
   echo "               COMPONENT SETTINGS"
   echo "               =================="
   echo "TK_MINIMUM_STACK_SIZE                     : $_TK_MINIMUM_STACK_SIZE"
   echo "TK_NORMAL_STACK_SIZE                      : $_TK_NORMAL_STACK_SIZE"
   echo "TK_ROOT_STACK_SIZE                        : $_TK_ROOT_STACK_SIZE"
   echo "TK_MAX_THREADS                            : $_TK_MAX_THREADS"
   echo "TK_MAX_PRIO_LEVELS                        : $_TK_MAX_PRIO_LEVELS"
   echo "TK_MAX_THREADS_AT_PRIO                    : $_TK_MAX_THREADS_AT_PRIO"
   echo "TK_THREAD_NAME_LEN                        : $_TK_THREAD_NAME_LEN"
   echo "TK_MAX_BLOCKED_ON_Q                       : $_TK_MAX_BLOCKED_ON_Q"
   echo "TK_MAX_NUM_Q                              : $_TK_MAX_NUM_Q"
   echo "TK_KMEM_NHEAPS                            : $_TK_KMEM_NHEAPS"

])

dnl Actuate COMPONENT selection
dnl ----------------------------------------------------------------------------
AC_DEFUN([TINKER_ACTUATE_COMPONENTS],
[
   AC_DEFINE_UNQUOTED([TK_COMP_ITC],            $_TK_COMP_ITC)
   AC_DEFINE_UNQUOTED([TK_COMP_PTIMER],         $_TK_COMP_PTIMER)
   AC_DEFINE_UNQUOTED([TK_COMP_KMEM],           $_TK_COMP_KMEM)
   AC_DEFINE_UNQUOTED([TK_COMP_PTHREAD],        $_TK_COMP_PTHREAD)
   AC_DEFINE_UNQUOTED([TK_COMP_POSIX_RT],       $_TK_COMP_POSIX_RT)
   AC_DEFINE_UNQUOTED([TK_COMP_FILESYS],        $_TK_COMP_FILESYS)
])

dnl Actuate COMPONENT tune-settings
dnl ----------------------------------------------------------------------------
AC_DEFUN([TINKER_ACTUATE_COMP_SETTINGS],
[
   AC_DEFINE_UNQUOTED([TK_MINIMUM_STACK_SIZE],  $_TK_MINIMUM_STACK_SIZE)
   AC_DEFINE_UNQUOTED([TK_NORMAL_STACK_SIZE],   $_TK_NORMAL_STACK_SIZE)
   if test ! -z $_TK_ROOT_STACK_SIZE; then
      AC_DEFINE_UNQUOTED([TK_ROOT_STACK_SIZE],     $_TK_ROOT_STACK_SIZE)
   fi
   AC_DEFINE_UNQUOTED([TK_MAX_THREADS],         $_TK_MAX_THREADS)
   AC_DEFINE_UNQUOTED([TK_MAX_PRIO_LEVELS],     $_TK_MAX_PRIO_LEVELS)
   AC_DEFINE_UNQUOTED([TK_MAX_THREADS_AT_PRIO], $_TK_MAX_THREADS_AT_PRIO)
   AC_DEFINE_UNQUOTED([TK_THREAD_NAME_LEN],     $_TK_THREAD_NAME_LEN)
   AC_DEFINE_UNQUOTED([TK_MAX_BLOCKED_ON_Q],    $_TK_MAX_BLOCKED_ON_Q)
   AC_DEFINE_UNQUOTED([TK_MAX_NUM_Q],           $_TK_MAX_NUM_Q)
   AC_DEFINE_UNQUOTED([TK_KMEM_NHEAPS],         $_TK_KMEM_NHEAPS)

])

dnl Prints component and component settings as defaults may be overloaded further
dnl down in the configure chain. Note that this info belonging to ./src
dnl *only* and reconfiguring in a leaf without (at least) re-building from the
dnl is an error. Having defaults in the leaf is strictly for convenience.
dnl ----------------------------------------------------------------------------
AC_DEFUN([TINKER_CONDITINAL_VERBOSE_PRINT],
[
   if test $CONF_VERBOSE == "yes"; then
      echo "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
      TINKER_PRINT_COMPONENTS
      echo "--------------------------------------------------------------"
      TINKER_PRINT_COMP_SETTINGS
      echo "VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV"
      echo "(Last printout you see is what counts. Verify with tinker/config.h)"
   fi
])

dnl prints input argument as hex-string. AWK is used for this as it is part
dnl of every standard system (may need to detect this as well to be safe)
AC_DEFUN([TINKER_TO_HEX],
[
	if [ "X$2" == "X" ]; then
		echo $1 | awk '{printf("0x%x",$[1])}'
	else
		echo $1 | awk '{printf("0x%0$2x",$[1])}'
	fi
])

dnl Mega everything. This function really needs to be broken up in bits...
AC_DEFUN([TINKER_CONFIGURE],
[
	TINKER_OPTIONS_CONFIG

	if test $CONF_VERBOSE == "yes"; then
		echo
		echo "================================================================================"
		echo "Configuring $PWD"
		echo "================================================================================"
	fi

	TINKER_PATH=$1
	AC_SUBST(TINKER_PATH)


	dnl if called from kernel directory create the AM prerequisites
	dnl All other should use AC_CONFIG_AUX_DIR
	if test $1 == "."; then
		TINKER_AM_PREREQ
	fi

	TINKER_OPTIONS_BUILD

	dnl Find and set the C compiler. The result of this will affect the $CC env. var in the make
	dnl files. Further more the TK_CPLUSPLUS macro in config.h will reflect the following:
	dnl * Undefined. Whomever intended to build TinKer did *not* intend TinKer to be build with g++
	dnl * Set but "FALSE". Whomever intended to build TinKer *did* intend TinKer to be build
	dnl    with g++, but the test failed and the build system falls back to gcc
	dnl * Set but "TRUE". Whomever intended to build TinKer *did* intend TinKer to be build
	dnl    with g++ and TinKer was built that way.
	dnl
	dnl Note: it's easy to mix up the two macros G++ and C++ because they look very similar
	dnl.They indicate the same logic, but their contents are very different.
	AC_PROG_CC
	if test $USECPLUSPLUS != __tk_no; then
		dnl Tries to use g++ as instructed. Should fall back on gcc if c++ is not available
		AC_PROG_CXX
		AC_PROG_CC(g++)
		AC_LANG_CPLUSPLUS
		dnl The TK_CPLUSPLUS macro in config.h will be set accordingly to the result
		AC_DEFINE_UNQUOTED([TK_CPLUSPLUS],__tk_$GXX)
		if test $GXX == yes; then
			CC=$CXX
			#AC_SUBST(CC)
		fi
	else
		dnl Use gcc
		AC_PROG_CC
		AC_PROG_CC(gcc)
		AC_LANG_C
	fi

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

   TINKER_CANONICAL_EXTRA

	dnl the vendor part will tell us.
	if test $host_vendor == "unknown"; then
		SYSTEM="default"
	else
		SYSTEM=$host_vendor
	fi
	dnl Note that system is not quoted in config.h in this file. This permits configure.in at lower level
	dnl to determine {if test $SYSTEM == 'default'} to set whatever default system they think is best
	dnl The "un-prefixed version is however quoted and is set to the canonical middle name
	AC_SUBST(SYSTEM)
	AC_DEFINE_UNQUOTED([__TK_SYSTEM_up__],$host_vendor)

	AC_SUBST(host_alias)
	AC_DEFINE_UNQUOTED([TK_ALIAS_HOST],$host_alias)

	dnl What is this?
	AC_PROG_MAKE_SET

	AC_HEADER_STDC

	AC_CHECK_FUNCS(memset)
	AC_CHECK_FUNCS(malloc)
	AC_CHECK_FUNCS(clock)
	AC_CHECK_FUNCS(time)
	AC_CHECK_FUNCS(times)
	AC_CHECK_FUNCS(gettimeofday)
	AC_CHECK_FUNCS(settimeofday)

   dnl Check for types
   AC_CHECK_TYPES(
      [ptrdiff_t, uintptr_t],
      [],
      [],
      [[#include <stdint.h>]]
   )

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
		THIS_DIR=$(cygpath -u $(PWD))
	else
		SERIAL_PORT=/dev/ttyS0
	THIS_DIR=$(pwd)
	fi

	dnl if called from kernel directory, save TK_TINKER_PATH  to later so sub configure use the same value
	dnl All other should use AC_CONFIG_AUX_DIR
	if test $1 == "."; then
		export SAVED_TK_DIR=$THIS_DIR
	fi
	AC_DEFINE_UNQUOTED([TK_TINKER_PATH],$SAVED_TK_DIR)

	AC_SUBST(SERIAL_PORT)
	AC_SUBST(THIS_DIR)

	AC_CHECK_TOOL([LD],      [ld],      [:])
	AC_CHECK_TOOL([AR],      [ar],      [:])
	AC_CHECK_TOOL([AS],      [as],      [:])
	AC_CHECK_TOOL([GDB],     [gdb],     [:])
	AC_CHECK_TOOL([OBJCOPY], [objcopy], [:])
	AC_CHECK_TOOL([OBJDUMP], [objdump], [:])
	AC_PATH_TOOL([GCC_PATH], [gcc],     [:])

	CANONICAL_HOST=$host
	AC_SUBST(CANONICAL_HOST)
	AC_DEFINE_UNQUOTED([TK_CANONICAL_HOST],$CANONICAL_HOST)

	if test $USECPLUSPLUS == __tk_no; then
		TOOLDIR=$(echo $GCC_PATH | sed -e "s/\/bin\/\+$CC//")
	else
		AC_PATH_TOOL([GXX_PATH], [g++],     [:])
		TOOLDIR=$(echo $GXX_PATH | sed -e "s/\/bin\/\+$CXX//")
	fi

	if test $cross_compiling == yes; then
		XCOMPILE=1
		ABI=$(echo $host | sed -e s/.*-//)
		if test "X${ARCH}" == "X"; then
			ARCH=$(echo $host | sed -e s/-.*//)
		fi
	else
		XCOMPILE=0

		#$build, $target, host are missing in this case. Need AC_CANNONICAL_HOST but that implies extra scripts (?)
		#Preferred line below cant be used
		#ARCH=$(echo $build | sed -e s/-.*//)

		ARCH=$(uname -m | sed -e s/i.86/i386/ -e s/sun4u/sparc64/ -e s/arm.*/arm/ -e s/sa110/arm/)
		ABI=""
	fi
	AC_SUBST(XCOMPILE)
	AC_SUBST(ARCH)
	AC_SUBST(ABI)
	AC_DEFINE_UNQUOTED([TK_XCOMPILE],$XCOMPILE)
	AC_DEFINE_UNQUOTED([TK_ARCH],__tk_${ARCH}__)
	AC_DEFINE_UNQUOTED([TK_ABI],__tk_${ABI}__)
	AC_DEFINE_UNQUOTED([__TK_ARCH_up__],$ARCH)
	AC_DEFINE_UNQUOTED([__TK_ABI_up__],$ABI)

	dnl Adapt for what could be ccache tricks where binary is installed
	dnl under local to gain PATH priority, but includes and libs are not.
	if test ! -f $TOOLDIR/$host_alias/include/stdio.h; then
		AC_MSG_WARN([<<< System header-files are not at expected location.
		Trying to work-around...])
		if test -f $TOOLDIR/$host_alias/../include/stdio.h; then
			pushd $TOOLDIR/$host_alias/..
			TOOLDIR=$(pwd)
			popd
		else
			AC_MSG_NOTICE([<<< Can't figure out location of system includes.
			Please rapport bug!])
		fi
	fi

	dnl Adapt for multi-arch
	if test ! -f $TOOLDIR/$host_alias/include/sys/types.h; then
		arch1=$target_cpu-$target_vendor-$target_os
		arch2=$target_cpu-$target_os
		AC_MSG_WARN([<<< Arch specific Header-files are not at expected location.
		This must be a multi-arch target, Adding arch specific sub-dir...])
		if test -f $TOOLDIR/$host_alias/include/$arch1/sys/types.h; then
			MULTI_ARCH_TARGET=$(echo $arch1 | sed -e 's/\/\//\//g')
			MULTI_ARCH_INCLUDES=$(
				echo $TOOLDIR/$host_alias/include/$arch1 | \
					sed -e 's/\/\//\//g'
			)
			AC_SUBST(MULTI_ARCH_TARGET)
			AC_DEFINE_UNQUOTED([TK_MULTI_ARCH_TARGET],$MULTI_ARCH_TARGET)
			AC_SUBST(MULTI_ARCH_TARGET)
			AC_DEFINE_UNQUOTED([TK_MULTI_ARCH_TARGET],$MULTI_ARCH_TARGET)
		elif test -f $TOOLDIR/$host_alias/include/$arch2/sys/types.h; then
			MULTI_ARCH_TARGET=$(echo $arch2 | sed -e 's/\/\//\//g')
			MULTI_ARCH_INCLUDES=$(
				echo $TOOLDIR/$host_alias/include/$arch2 | \
					sed -e 's/\/\//\//g'
			)
			AC_SUBST(MULTI_ARCH_TARGET)
			AC_DEFINE_UNQUOTED([TK_MULTI_ARCH_TARGET],$MULTI_ARCH_TARGET)
			AC_SUBST(MULTI_ARCH_INCLUDES)
			AC_DEFINE_UNQUOTED([TK_MULTI_ARCH_INCLUDES],$MULTI_ARCH_INCLUDES)
		else
			dnl echo "Tested: "
			dnl echo "$TOOLDIR/$host_alias/include/$arch1/sys/types.h"
			dnl echo "$TOOLDIR/$host_alias/include/$arch2/sys/types.h"
			AC_MSG_NOTICE([<<<Can't figure out location of arch includes and libraries
			echo "Please rapport bug!])
		fi
	fi

	AC_SUBST(TOOLDIR)
	AC_DEFINE_UNQUOTED([TK_TOOLDIR],$TOOLDIR)

	GCC_VERSION=$($CC -v 2>&1 | grep 'gcc version' | sed -e 's/gcc version //' | cut -f1 -d " ")
	AC_SUBST(GCC_VERSION)
	AC_DEFINE_UNQUOTED([TK_GCC_VERSION],$GCC_VERSION)

	dnl Some influential environment variables
	dnl =====================================
	dnl C compiler flags
 	AC_SUBST(CFLAGS)
	dnl linker flags, e.g. -L<lib dir> if you have libraries in a
	dnl nonstandard directory <lib dir>
 	AC_SUBST(LDFLAGS)
	dnl libraries to pass to the linker, e.g. -l<library>
 	AC_SUBST(LIBS)
	dnl (Objective) C/C++ preprocessor flags, e.g. -I<include dir> if
	dnl you have headers in a nonstandard directory <include dir>
 	AC_SUBST(CPPFLAGS)
	dnl C++ compiler flags
 	AC_SUBST(CXXFLAGS)

	AC_ARG_VAR(BOARD, [Selects which board to build TinKer BSP for. Valid values depend on each BSP and correspond to a sub-directory in that structure])
	AC_DEFINE_UNQUOTED([TK_BOARD],__tk_${BOARD}__)
	AC_DEFINE_UNQUOTED([__TK_BOARD_up__],$BOARD)

	AC_ARG_VAR(MCPU, [Sets the GCC CPU optimization switch (i.e. -mcpu=<MCPU>])

	if test -z $MCPU; then
		AC_MSG_NOTICE([<<< No -mcpu optimization])
		CPU_OPT=""
		DCPU=""
	else
		AC_MSG_NOTICE([<<< Using optimize CPU flag: -mcpu=$MCPU- ])
		CPU_OPT="-mcpu=$MCPU"
		DCPU="-DWITH_CPU=$MCPU"
	fi
	AC_SUBST(CPU_OPT)
	AC_SUBST(DCPU)
	AC_SUBST(MCPU)
	AC_DEFINE_UNQUOTED([TK_DCPU],__tk_${MCPU}__)
	AC_DEFINE_UNQUOTED([__TK_DCPU_up__],$MCPU)

   dnl *Components* config part. Use enable-feature/disable-feature without arguments
   dnl NOTE: Default values are set here, but *might get overridden in target specific sub-trees*
   dnl ----------------------------------------------------------------------------
   AC_ARG_ENABLE(itc,
      AS_HELP_STRING(
         [--enable-itc],
         [ITC - Enable/disable Inter thread communication component (native API)]),
      _TK_COMP_ITC=__tk_$enableval,
      _TK_COMP_ITC=__tk_yes
   )
   AC_ARG_ENABLE(ptimer,
      AS_HELP_STRING(
         [--enable-ptimer],
         [PTIMER - Enable/disable Preemptive timer component]),
      _TK_COMP_PTIMER=__tk_$enableval,
      _TK_COMP_PTIMER=__tk_no
   )
   AC_ARG_ENABLE(kmem,
      AS_HELP_STRING(
         [--enable-kmem],
         [KMEM - Enable/disable Kernel memory manager component]),
      _TK_COMP_KMEM=__tk_$enableval,
      _TK_COMP_KMEM=__tk_no
   )
   AC_ARG_ENABLE(pthread,
      AS_HELP_STRING(
         [--enable-pthread],
         [PTHRED - Enable/disable POSIX 1003.1c threads component]),
      _TK_COMP_PTHREAD=__tk_$enableval,
      _TK_COMP_PTHREAD=__tk_no
   )
   AC_ARG_ENABLE(posix_rt,
      AS_HELP_STRING(
         [--enable-posix_rt],
         [POSIX_RT - Enable/disable POSIX 1003.1b queues, semaphores component enabled]),
      _TK_COMP_POSIX_RT=__tk_$enableval,
      _TK_COMP_POSIX_RT=__tk_no
   )
   AC_ARG_ENABLE(filesys,
      AS_HELP_STRING(
         [--enable-filesys],
         [FIILESYS - Enable/disable embedded file-system abstraction component. (Not available for all possible targets. Target must be HIXS adapted.)]),
      _TK_COMP_FILESYS=__tk_$enableval,
      _TK_COMP_FILESYS=__tk_no
   )

   TINKER_ACTUATE_COMPONENTS

	dnl *Package* config part. (usage, tuning and misc) Use enable-feature/disable-feature (arguments optional)
	dnl NOTE: Default values are set here, but *might get overridden in target specific sub-trees*
	dnl ----------------------------------------------------------------------------
	AC_ARG_ENABLE(builtin_sorting,
		AS_HELP_STRING([--enable-builtin_sorting],[PACKAGE - Use built-in search/sort functions (overrides default for target)]),
		_TK_USE_BUILTIN_SORT=__tk_$enableval,
		_TK_USE_BUILTIN_SORT=__tk_yes
	)
	AC_ARG_ENABLE(emrgcy-console,
		AS_HELP_STRING([--enable-emrgcy-console=<val>],[Claim an emergency console is available by the BSP. Alternatively: name a function to use.]),
		AC_DEFINE_UNQUOTED([TK_USE_EMRGCY_CONSOLE],__tk_$enableval)
	)


	AC_DEFINE_UNQUOTED([TK_USE_BUILTIN_SORT],$_TK_USE_BUILTIN_SORT)

   dnl Configurable entities (TinKer tuning defines)
   dnl NOTE: Default values *NOT* set here, These are supposed to be set in
   dnl target specific sub-trees dnl   which goes in each configure.in in such
   dnl case.
   dnl ---------------------------------------------
   AC_ARG_ENABLE(min_stack,
      AS_HELP_STRING(
         [--enable-min_stack=<val>],
         [Maximum size of a stack for a thread ]),
      _TK_MINIMUM_STACK_SIZE=$enableval
   )
   AC_ARG_ENABLE(norm_stack,
      AS_HELP_STRING(
         [--enable-norm_stack=<val>],
         [Stack-size used when stack-size is omitted ]
      ),
      _TK_NORMAL_STACK_SIZE=$enableval
   )
   AC_ARG_ENABLE(root_stack,
      AS_HELP_STRING(
         [--enable-root_stack=<val>],
         [Stack-size for root-stack or process if hosted]
      ),
      _TK_ROOT_STACK_SIZE=$enableval
   )
   AC_ARG_ENABLE(max_threads,
      AS_HELP_STRING(
         [--enable-max_threads=<val>],
         [Maximum number of threads your system could have ]),
      _TK_MAX_THREADS=$enableval
   )
   AC_ARG_ENABLE(max_prio,
      AS_HELP_STRING(
         [--enable-max_prio=<val>],
         [Number of priorities for you system (recommended value: 3 or 16)]),
      _TK_MAX_PRIO_LEVELS=$enableval
   )
   AC_ARG_ENABLE(that_prio,
      AS_HELP_STRING(
         [--enable-that_prio=<val>],
         [Number of threads that can be scheduled at each priority ]),
      _TK_MAX_THREADS_AT_PRIO=$enableval
   )
   AC_ARG_ENABLE(thename_len,
      AS_HELP_STRING(
         [--enable-thename_len=<val>],
         [Lengts of the string in TCB that holds the name of the thread ]),
      _TK_THREAD_NAME_LEN=$enableval
   )
   AC_ARG_ENABLE(blocked_q,
      AS_HELP_STRING(
         [--enable-blocked_q=<val>],
         [Maximum number of threads blocked on any ITC primitive, i.e. not only Q ]),
      _TK_MAX_BLOCKED_ON_Q=$enableval
   )
   AC_ARG_ENABLE(max_q,
      AS_HELP_STRING(
         [--enable-max_q=<val>],
         [Maximum number of any ITC primitive, i.e not only Q ]),
      _TK_MAX_NUM_Q=$enableval
   )
   AC_ARG_ENABLE(max_heaps,
      AS_HELP_STRING(
         [--enable-max_heaps=<val>],
         [Maximum number of heaps for the KMEM component]),
      _TK_KMEM_NHEAPS=$enableval
   )

   TINKER_ACTUATE_COMP_SETTINGS

	dnl Other configurable features
	dnl ---------------------------------------------
	AC_ARG_ENABLE(dispatch,
		AS_HELP_STRING([--enable-dispatch=<val>],[Main type of dispatching. MIXED or EXCLUSIVE (default is MIXED)]),
		AC_DEFINE_UNQUOTED([TK_DISPATCH],__tk_$enableval),
		AC_DEFINE_UNQUOTED([TK_DISPATCH],MIXED)
	)

	dnl Advanced initial portining tuning.
	dnl These options provice certain funtion/macro mappings. Not all values are valid for each setting
	dnl possible values (NOTE: capital letters required!)
	dnl ORIGINAL 	- Use & link against original name somewhere
	dnl STUBBED 	- Use the stubbed version of the function
	dnl RENAMED	- Use a renamed version
	dnl VOIDED	- Void function call out
	dnl NOTE: Default values *NOT* set here, These are supposed to be set in target specific sub-trees
	dnl   which goes in each configure.in in such case.
	dnl ---------------------------------------------
	AC_ARG_ENABLE(how-to-malloc,
		AS_HELP_STRING([--enable-how-to-mallow=<val>],[Advanced primary porting - ORIGINAL/STUBBED/RENAMED/VOIDED]),
		AC_DEFINE_UNQUOTED([TK_HOWTO_MALLOC],TK_FNK_$enableval)
	)

	AC_ARG_ENABLE(how-to-clock,
		AS_HELP_STRING([--enable-how-to-clock=<val>],[Advanced primary porting - ORIGINAL/STUBBED/RENAMED/VOIDED]),
		AC_DEFINE_UNQUOTED([TK_HOWTO_CLOCK],TK_FNK_$enableval)
	)

	AC_ARG_ENABLE(how-to-printk,
		AS_HELP_STRING([--enable-how-to-printk=<val>],[Advanced primary porting - )]),
		AC_DEFINE_UNQUOTED([TK_HOWTO_PRINTK],TK_FNK_$enableval)
	)

	AC_ARG_ENABLE(how-to-assert,
		AS_HELP_STRING([--enable-how-to-assert=<val>],[Advanced primary porting - ]),
		AC_DEFINE_UNQUOTED([TK_HOWTO_ASSERT],TK_FNK_$enableval)
	)

	dnl Tuning section ended
	dnl --------------------------------------------

	if test $cross_compiling == yes; then
		if test -z $BOARD; then
			AC_MSG_WARN([<<< You are configuring for XCOMPILE but no BOARD is selected.
			Compilation will probably fail...])
		fi
	else
		if test "X${BOARD}" != "X"; then
			AC_MSG_WARN([<<< You are configuring for NATIVE but BOARD is selected/defined.
				This option does no purpose...])
      else
         dnl If no $BOARD and no $cross_compiling, it must mean that this
         dnl configure is for building a hosted kernel. Let build-system
         dnl know this, some instructions dnl are illegal on hosted systems as
         dnl kernel will not run in aprivileged mode (IRQ manipulation e.t.a.).

         HOSTED=1
			AC_SUBST(HOSTED)
			AC_DEFINE_UNQUOTED([TK_HOSTED],$HOSTED)
		fi
	fi

	AC_PROG_INSTALL

	ALL_C=$sources_c
	AC_SUBST(ALL_C)
	ALL_ASM=$sources_asm
	AC_SUBST(ALL_ASM)

	EXTRAS=$extras
	AC_SUBST(EXTRAS)

])
