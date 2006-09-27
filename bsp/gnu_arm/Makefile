SUB_MODS:=lpc21xx
SUB_MODS_CONFIGURE := $(patsubst %, make configure -C %;, $(SUB_MODS))
.PHONY: all configure

all: configure

configure:
	rm -f config.*
	rm -f install-sh
	rm -rf autom4te.cache
	rm -f Makefile-gnu.in
	cp ../../Makefile-gnu.in .	
	#rm -f aclocal.m4 
	autoconf -I ../../
	#aclocal
	$(SUB_MODS_CONFIGURE)
