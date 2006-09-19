SUB_MODS:=lpc21xx
SUB_MODS_CONFIGURE := $(patsubst %, make configure -C %;, $(SUB_MODS))
.PHONY: all configure

all: configure

configure:
	rm -f config.*
	rm -f aclocal.m4 
	autoconf
	$(SUB_MODS_CONFIGURE)
