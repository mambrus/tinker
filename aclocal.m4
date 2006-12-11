dnl This function replaces the need of automake to get some files that autoconf
dnl needs. TinKer does not use automake because of it's version incompability
dnl problems. However, aoutoconf seems to have some dependancy towards automake
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

	dnl The following seems needed to reninitialize the search-patch
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
		DEPMAKE=$enableval,
		DEPMAKE="Makefile-gnu Makefile"
	)
	AC_ARG_ENABLE(cplusplus,
		AS_HELP_STRING([--enable-cplusplus],[Build - Enable/disable using C++ for compilation and linking]),
		USECPLUSPLUS=$enableval,
		USECPLUSPLUS="no"
	)
	AC_ARG_ENABLE(makeopts,
		AS_HELP_STRING([--enable-makeopts],[Build - Pass specified make options to recursive makes (=arg). Options worth considering are -S, -w]),
		MAKEOPTS=$enableval,
		MAKEOPTS=""
	)

	AC_SUBST(DEPMAKE)
	AC_SUBST(MAKEOPTS)
])


dnl Mega everything. This function reaally needs to be broken up in bits...
AC_DEFUN([TINKER_CONFIGURE],
[

	TINKER_PATH=$1
	AC_SUBST(TINKER_PATH)


	dnl if called from kernel directory create the AM prerequisits 
	dnl All other should use AC_CONFIG_AUX_DIR
	if test $1 == "."; then
		TINKER_AM_PREREQ
	fi

	TINKER_OPTIONS_BUILD

	dnl Find and set the C compiler. The result of this will affect the $CC env. var in the make
	dnl files. Further more the TK_CPLUSPLUS macro in config.h will reflec the following:
	dnl * Undefined. Whomever intended to build TinKer did *not* intend TinKer to be build with g++
	dnl * Set but "FALSE". Whomever intended to build TinKer *did* intend TinKer to be build 
	dnl    with g++, but the test failed and the build system falls back to gcc
	dnl * Set but "TRUE". Whomever intended to build TinKer *did* intend TinKer to be build 
	dnl    with g++ and TinKer was built that way.
	dnl
	dnl Note: it's easy to mix up the two macros G++ and C++ because they look very similar
	dnl.They indicate the same logic, but their contents are very different.
	AC_PROG_CC	
	if test $USECPLUSPLUS != no; then		
		dnl Tries to use g++ as instructed. Should fall back on gcc if c++ is not available
		AC_PROG_CXX
		AC_PROG_CC(g++)		
		AC_LANG_CPLUSPLUS
		dnl The TK_CPLUSPLUS macro in config.h will be set accordingly to the result
		AC_DEFINE_UNQUOTED([TK_CPLUSPLUS],$GXX)
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

	dnl Deduct the system call API based on TinKer's convention of asuming
	dnl the vendor part will tell us.
	if test $host_vendor == "unknown"; then
		SYSTEM="default"
	else
		SYSTEM=$host_vendor
	fi

	AC_SUBST(SYSTEM)
	AC_SUBST(host_alias)
	AC_DEFINE_UNQUOTED([TK_ALIAS_HOST],$host_alias)


	dnl What is this?
	AC_PROG_MAKE_SET 

	AC_HEADER_STDC
	AC_CHECK_FUNCS(clock)
	AC_CHECK_FUNCS(time)
	AC_CHECK_FUNCS(times)
	AC_CHECK_FUNCS(gettimeofday)
	AC_CHECK_FUNCS(settimeofday)

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

	if test $USECPLUSPLUS == no; then
		TOOLDIR=$(echo $GCC_PATH | sed -e "s/\/bin\/\+$CC//")
	else
		AC_PATH_TOOL([GXX_PATH], [g++],     [:])
		TOOLDIR=$(echo $GXX_PATH | sed -e "s/\/bin\/\+$CXX//")
	fi

	AC_SUBST(TOOLDIR)
	AC_DEFINE_UNQUOTED([TK_TOOLDIR],$TOOLDIR)

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
	AC_DEFINE_UNQUOTED([TK_XCOMPILE],$XCOMPILE)
	AC_DEFINE_UNQUOTED([TK_ARCH],$ARCH)
	AC_DEFINE_UNQUOTED([TK_ABI],$ABI)

	GCC_VERSION=$($CC -v 2>&1 | grep version | sed -e 's/gcc version //' | cut -f1 -d " ")
	AC_SUBST(GCC_VERSION)
	AC_DEFINE_UNQUOTED([TK_GCC_VERSION],$GCC_VERSION)

	dnl Do not accept default autoconf CFLAGS. Tinker will not run with them (not on ARM tdmi at leat)
	CFLAGS=""
	AC_SUBST(CFLAGS)

	AC_ARG_VAR(BOARD, [Selects which board to build TinKer BSP for. Valid values depend on each BSP and coorespond to a sub-directory in that structure])
	AC_DEFINE_UNQUOTED([TK_BOARD],$BOARD)

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
	AC_DEFINE_UNQUOTED([TK_DCPU],$MCPU)


	dnl these shell values are used in our scripts. Set them to defaut values that correspond the the
	dnl values in each values AC_ARG_ENABLE section
	enable_itc=yes
	enable_ptimer=no
	enable_kmem=no
	enable_pthread=yes
	enable_posix_rt=yes

	enable_builtin_sorting=yes

	dnl Components config part. Use enable-feature/disable-feature without arguments
	dnl Note: Default values are set here.
	dnl ----------------------------------------------------------------------------
	AC_ARG_ENABLE(itc,
		AS_HELP_STRING([--enable-itc],[ITC - Enable/disable Inter thread communication component (native API)]),
		AC_DEFINE_UNQUOTED([TK_COMP_ITC],$enableval),
		AC_DEFINE_UNQUOTED([TK_COMP_ITC],yes)
	)
	AC_ARG_ENABLE(ptimer,
		AS_HELP_STRING([--enable-ptimer],[PTIMER - Enable/disable Preemptive timer component]),
		AC_DEFINE_UNQUOTED([TK_COMP_PTIMER],$enableval),
		AC_DEFINE_UNQUOTED([TK_COMP_PTIMER],no)
	)
	AC_ARG_ENABLE(kmem,
		AS_HELP_STRING([--enable-kmem],[KMEM - Enable/disable Kernel memory manager component]),
		AC_DEFINE_UNQUOTED([TK_COMP_KMEM],$enableval),
		AC_DEFINE_UNQUOTED([TK_COMP_KMEM],no)
	)
	AC_ARG_ENABLE(pthread,
		AS_HELP_STRING([--enable-pthread],[PTHRED - Enable/disable POSIX 1003.1c threads component]),
		AC_DEFINE_UNQUOTED([TK_COMP_PTHREAD],$enableval),
		AC_DEFINE_UNQUOTED([TK_COMP_PTHREAD],yes)
	)
	AC_ARG_ENABLE(posix_rt,
		AS_HELP_STRING([--enable-posix_rt],[POSIX_RT - Enable/disable POSIX 1003.1b queues, semaphores component enabled]),
		AC_DEFINE_UNQUOTED([TK_COMP_POSIX_RT],$enableval),
		AC_DEFINE_UNQUOTED([TK_COMP_POSIX_RT],yes)
	)

	dnl Package config part. (usage, tuning and misc) Use enable-feature/disable-feature (arguments optional)
	dnl NOTE: Default values are set here, but *might get overridden in target specific sub-trees*
	dnl ----------------------------------------------------------------------------
	AC_ARG_ENABLE(builtin_sorting,
		AS_HELP_STRING([--enable-builtin_sorting],[PACKAGE - Use builtin search/sort functions (overrides default for target)]),
		AC_DEFINE_UNQUOTED([TK_USE_BUILTIN_SORT],$enableval),
		AC_DEFINE_UNQUOTED([TK_USE_BUILTIN_SORT],yes)
	)

	dnl Configurable entities (TinKer tuning defines)
	dnl NOTE: Default values *NOT* set here, These are supposed to be set in target specific sub-trees
	dnl   which goes in each configure.in in such case.
	dnl ---------------------------------------------
	AC_ARG_ENABLE(min_stack,
		AS_HELP_STRING([--enable-min_stack=<val>],[Maximum size of a stack for a thread ]),
		AC_DEFINE_UNQUOTED([TK_MINIMUM_STACK_SIZE],$enableval))
	AC_ARG_ENABLE(norm_stack,
		AS_HELP_STRING([--enable-norm_stack=<val>],[Stacksize used when stack-size is omitted ]),
		AC_DEFINE_UNQUOTED([TK_NORMAL_STACK_SIZE],$enableval))
	AC_ARG_ENABLE(max_threads,
		AS_HELP_STRING([--enable-max_threads=<val>],[Maximum number of threds your system could have ]),
		AC_DEFINE_UNQUOTED([TK_MAX_THREADS],$enableval))
	AC_ARG_ENABLE(max_prio,
		AS_HELP_STRING([--enable-max_prio=<val>],[Numer of priorities for you system  (recomended value: 3 or 16)]),
		AC_DEFINE_UNQUOTED([TK_MAX_PRIO_LEVELS],$enableval))
	AC_ARG_ENABLE(that_prio,
		AS_HELP_STRING([--enable-that_prio=<val>],[Number of threads that can be sceduled at each priority ]),
		AC_DEFINE_UNQUOTED([TK_MAX_THREADS_AT_PRIO],$enableval))
	AC_ARG_ENABLE(thename_len,
		AS_HELP_STRING([--enable-thename_len=<val>],[Lengts of the string in TCB that holds the name of the thread ]),
		AC_DEFINE_UNQUOTED([TK_THREAD_NAME_LEN],$enableval))
	AC_ARG_ENABLE(blocked_q,
		AS_HELP_STRING([--enable-blocked_q=<val>],[Maximum nuber of threads blocked on any ITC primitive, i.e. not only Q ]),
		AC_DEFINE_UNQUOTED([TK_MAX_BLOCKED_ON_Q],$enableval))
	AC_ARG_ENABLE(max_q,
		AS_HELP_STRING([--enable-max_q=<val>],[Maximum number of any ITC primitive, i.e not only Q ]),
		AC_DEFINE_UNQUOTED([TK_MAX_NUM_Q],$enableval))

	dnl Other configurable features
	dnl ---------------------------------------------
	AC_ARG_ENABLE(dispatch,
		AS_HELP_STRING([--enable-dispatch=<val>],[Main type of dispatching. MIXED or EXCLUSIVE (default is MIXED]),
		AC_DEFINE_UNQUOTED([TK_DISPATCH],$enableval),
		AC_DEFINE_UNQUOTED([TK_DISPATCH],MIXED)
	)

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

	dnl The following test will handle defaults but also permit specific argument values
	if test "$DEPMAKE" == yes; then
		DEPMAKE="Makefile-gnu Makefile"
	elif test "$DEPMAKE" == no; then
		DEPMAKE=""
	fi
	AC_SUBST(DEPMAKE)

	AC_PROG_INSTALL

	ALL_C=$sources_c
	AC_SUBST(ALL_C)
	ALL_ASM=$sources_asm
	AC_SUBST(ALL_ASM)

	EXTRAS=$extras
	AC_SUBST(EXTRAS)

])
