SUB_MODS:=gnu_arm gnu_i386
SUB_MODS_CONFIGURE := $(patsubst %, make configure -C %;, $(SUB_MODS))
.PHONY: all configure

all: configure

configure:
	rm -f config.*
	rm -f aclocal.m4 
	autoconf
	$(SUB_MODS_CONFIGURE)
