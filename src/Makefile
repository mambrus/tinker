
HAS_SUBDIRS_FILE:=$(shell if test -f make.subdirs; then echo yes; else echo no; fi)

ifeq ($(HAS_SUBDIRS_FILE),yes)
	SUB_MODS:=$(shell cat make.subdirs)
else
	SUB_MODS:=$(shell find . -maxdepth 2 -mindepth 2 -iname configure.in -exec dirname '{}' ';')
endif

SUB_MODS_CONFIGURE := $(patsubst %, make configure -C %;, $(SUB_MODS))
.PHONY: all info configure configure_this configure_subs clean cleanhard

#Colorization tool detection
ifeq ($(VIM),)
   GRCAT = $(shell which grcat)
endif
ifneq ($(GRCAT),)
   USE_GRCAT = $(shell ls gcc.grc 2>/dev/null)
endif

ifneq ($(USE_GRCAT),)
all:
	( make -f Makefile-gnu all 2>&1 ) | grcat gcc.grc

clean:
	( make -f Makefile-gnu clean 2>&1 ) | grcat gcc.grc

else
all:
	make -f Makefile-gnu all

clean:
	make -f Makefile-gnu clean
endif

cleanhard:
	find -iname "*.d" -exec rm '{}' ';'
	find -iname "*.o" -exec rm '{}' ';'
	find -iname "*~" -exec rm '{}' ';'


info:
	@echo
	@echo "================================================================================"
	@echo "---- $$PWD ----"
	@echo "================================================================================"

configure_this:
	rm -f config.*
	rm -f install-sh
	rm -rf autom4te.cache
	rm -f Makefile-gnu.in
	cp $(MAKE_ROOTDIR)/Makefile-gnu.in .
	autoconf -I $(MAKE_ROOTDIR)
	if test -n "$(SUB_MODS)"; then for SUBDIR in $(SUB_MODS); do cp $(MAKE_ROOTDIR)/bsp/Makefile $$SUBDIR/; done; fi


configure_subs:
	$(SUB_MODS_CONFIGURE)

configure: info configure_this configure_subs
