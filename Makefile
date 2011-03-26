#/***************************************************************************
# *   Copyright (C) 2006 by Michael Ambrus                                  *
# *   michael.ambrus@maquet.com                                             *
# *                                                                         *
# *   This program is free software; you can redistribute it and/or modify  *
# *   it under the terms of the GNU General Public License as published by  *
# *   the Free Software Foundation; either version 2 of the License, or     *
# *   (at your option) any later version.                                   *
# *                                                                         *
# *   This program is distributed in the hope that it will be useful,       *
# *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
# *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
# *   GNU General Public License for more details.                          *
# *                                                                         *
# *   You should have received a copy of the GNU General Public License     *
# *   along with this program; if not, write to the                         *
# *   Free Software Foundation, Inc.,                                       *
# *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
# ***************************************************************************/

#This is the top-level generic makefile in a multi build multi system.

MODULES:=  .
CONFMODULES:=  src bsp

THIS_MODULE=DUMMY_MODULE
CPU_T=DUMMY_CPU_TYPE
USR_DEFINES:=DUMMY_DEF
export TINKER_SRC_DIR = $(shell pwd)

# TOOLS could be either 'gnu' or any othe supported tool-chain.
# If TOOLS is not set, the build system assumes each sub-module 
# will know which tol-chain to use

TOOLS := gnu
#TOOLS := keil

ifdef TOOLS
   CLEAN_MODS      := $(patsubst %, make -f Makefile-$(TOOLS) clean -C %;, $(MODULES))
   CLEANALL_MODS   := $(patsubst %, make -f Makefile-$(TOOLS) cleanall -C %;, $(MODULES))
   INSTALL_MODS    := $(patsubst %, make -f Makefile-$(TOOLS) install -C %;, $(MODULES))
   FLASHIT_MODS    := $(patsubst %, make -f Makefile-$(TOOLS) flashit -C %;, $(MODULES))
   CONSOLE_MODS    := $(patsubst %, make -f Makefile-$(TOOLS) console -C %;, $(MODULES))
   NEXT_MAKEALL    := -f Makefile-$(TOOLS)
else
   CLEAN_MODS      := $(patsubst %, make clean -C %;, $(MODULES))
   CLEANALL_MODS   := $(patsubst %, make cleanall -C %;, $(MODULES))
   INSTALL_MODS    := $(patsubst %, make install -C %;, $(MODULES))
   FLASHIT_MODS    := $(patsubst %, make flashit -C %;, $(MODULES))
   CONSOLE_MODS    := $(patsubst %, make console -C %;, $(MODULES))
   NEXT_MAKEALL    :=
endif

#Note: 'make configure' always assumes a GNU (or UNIX like) build host
CONFIGURE_MODS  := $(patsubst %, make configure -C %;, $(CONFMODULES))

.PHONY: modules $(MODULES) clean cleanall configure install flashit console cleanhard cleanconfigure

all: modules

cleanhard:
	find -iname "*.d" -exec rm '{}' ';'
	find -iname "*.o" -exec rm '{}' ';'
	find -iname "*~" -exec rm '{}' ';'
	rm -f .installed-*
	@echo "======================================================"
	@echo "<<- All is hard-cleaned from garbage! ->>"
	@echo "======================================================"


modules: $(MODULES)
	@echo "======================================================"
	@echo "<<-        ALL MODULES "ALL" BUILT!                ->>"
	@echo "======================================================"

clean:
	$(CLEAN_MODS)
	FS=$$(find lib/ -type f | grep -v README); for F in $$FS; do echo $$F; done
	@echo "======================================================"
	@echo "<<-           ALL MODULES CLEANED!                 ->>"
	@echo "======================================================"
	
cleanall:
	$(CLEANALL_MODS)
	@echo "======================================================"
	@echo "<<-        ALL MODULES DEEP CLEANED!               ->>"
	@echo "======================================================"	

install:
	$(INSTALL_MODS)
	@echo "======================================================"
	@echo "<<-        ALL MODULES INSTALLED!                  ->>"
	@echo "======================================================"

flashit:
	$(FLASHIT_MODS)
	@echo "======================================================"
	@echo "<<-         ALL MODULES: Flashed!!                 ->>"
	@echo "======================================================"

console:
	$(CONSOLE_MODS)
	@echo "======================================================"
	@echo "<<-         ALL MODULES: Flashed!!                 ->>"
	@echo "======================================================"

$(MODULES):
	@echo "======================================================"
	@echo "<<-            ENTERING MODULE $@                  ->>"
	@echo "======================================================"
	make $(NEXT_MAKEALL) -C  $@

configure:
	rm -f config.*
	rm -f install-sh
	rm -f .installed-*
	rm -rf autom4te.cache
	autoheader
	#rm -f aclocal.m4 
	autoconf -I kernel
	#aclocal
	$(CONFIGURE_MODS)
	@echo "Configure scripts created. Now run './configure [options]'. For example: "
	@echo "./configure -C --host=bfin-hixs-elf BOARD=generic"
	@echo "./configure -C --host=powerpc-eabisim MCPU=860 BOARD=generic"
	@echo "./configure -C --host=arm-hixs-elf MCPU=arm7tdmi BOARD=lpc21xx"
	@echo "==========================================================="

cleanconfigure:
	rm -f confdefs.h
	rm -f include/tinker/config.h.in
	find -name temp -exec rm -rf '{}' ';'
	find . -name "automake-*" -exec rm '{}' ';'
	find . -iname configure -exec rm '{}' ';'
	find . -name 'Makefile-gnu' -exec rm '{}' ';'
	find . -regex '.*config\.[^.]+$$' -exec rm '{}' ';'
	find . -name 'install-sh' -exec rm '{}' ';'
	find . -regex '.*\(bsp\|src\).*Makefile-gnu\.[^.]+$$' -exec rm '{}' ';'
	FS=$$(find . -iname Makefile | egrep -v '^..Makefile' | \
       egrep -v '^..src.Makefile' | egrep -v '^..bsp.Makefile'); \
	   for F in $$FS; do rm $$F; done
	( find . -name 'autom4te.cache' -exec rm -rf '{}' ';' 2>/dev/null )
