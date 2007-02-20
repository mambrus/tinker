#SUB_MODS:=gnu_arm gnu_i386 gnu_bfin gnu_powerpc
SUB_MODS:=$(shell cat make.subdirs)
SUB_MODS_CONFIGURE := $(patsubst %, make configure -C %;, $(SUB_MODS))
.PHONY: all configure

all: 
	make -f Makefile-gnu all

configure:
	rm -f config.*
	rm -f install-sh
	rm -rf autom4te.cache
	rm -f Makefile-gnu.in
	cp ../Makefile-gnu.in .
	#rm -f aclocal.m4 
	autoconf -I ../
	#aclocal
	$(SUB_MODS_CONFIGURE)
