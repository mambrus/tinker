
HAS_SUBDIRS_FILE:=$(shell if test -f make.subdirs; then echo yes; else echo no; fi)

ifeq ($(HAS_SUBDIRS_FILE),yes)
	SUB_MODS:=$(shell cat make.subdirs)
else
	SUB_MODS:=
endif

SUB_MODS_CONFIGURE := $(patsubst %, make configure -C %;, $(SUB_MODS))
.PHONY: all info configure configure_this configure_subs cleanhard

all: 
	make -f Makefile-gnu all

clean: 
	make -f Makefile-gnu clean

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
	cp $(TINKER_SRC_DIR)/Makefile-gnu.in .
	autoconf -I $(TINKER_SRC_DIR)
	if test -n "$(SUB_MODS)"; then for SUBDIR in $(SUB_MODS); do cp $(TINKER_SRC_DIR)/src/Makefile $$SUBDIR/; done; fi
	

configure_subs:
	$(SUB_MODS_CONFIGURE)

configure: info configure_this configure_subs
