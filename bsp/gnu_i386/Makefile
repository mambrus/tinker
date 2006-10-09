SUB_MODS:=
SUB_MODS_CONFIGURE := $(patsubst %, make configure -C %;, $(SUB_MODS))
.PHONY: all configure

all: 
	make -f Makefile-gnu all

configure:
	rm -f config.*
	rm -f aclocal.m4 
	autoconf
	$(SUB_MODS_CONFIGURE)
