# -----------------------------------------------------------------------------
# - Makefile                                                                  -
# - iato:isa testers - test makefile                                          -
# -----------------------------------------------------------------------------
# - (c) inria 2002-2004                                                       -
# -----------------------------------------------------------------------------
# - authors                                        Amaury Darsch    2002:2004 -
# -                                                Pierre Villalon  2002:2003 -
# -                                                Andre  Seznec    2002:2004 -
# -----------------------------------------------------------------------------
# - This program  is  free  software;  you can redistribute  it and/or modify -
# - it  under  the  terms  of  the  GNU  General Public License version 2, as -
# - published by the Free Software Foundation.                                -
# -                                                                           -
# - This  program  is  distributed  in  the  hope  that  it  will be  useful, -
# - but   without  any  warranty;  without  even  the   implied  warranty  of -
# - merchantability or fitness for a particular purpose.                      -
# -                                                                           -
# - See the GNU General Public License version 2 for more details             -
# -----------------------------------------------------------------------------

SOURCES 	= T01.s \
		  T02.s \
		  T03.s \
		  T04.s \
		  T05.s \
		  T06.s \
		  T07.s \
		  T08.s \
		  T09.s \
		  T10.s \
		  T11.s \
		  T12.s \
		  T13.s \
		  F10.s

OBJECTS 	= $(SOURCES:.s=.o)
TARGETS		= $(basename $(OBJECTS))
LDSCRPT		= ../../../cnf/iato.lds
IAOS		= ../../../src/clt/iaos/iaos -v
IAKA		= ../../../src/clt/iaka/iaka -c
IAOO		= ../../../src/clt/iaoo/iaoo -c
IAIO		= ../../../src/clt/iaio/iaio -c
include 	  ../../../cnf/config.mak

all : test

%.o : %.s
	$(XAS) -o $@ $<

$(TARGETS) : % : %.o xit.o
	$(XLD) -T $(LDSCRPT) -o $@ $@.o

pgm : $(TARGETS)

$(TARGETS:%=%.iaos): %.iaos : %
	$(IAOS) $< >> /dev/null

iaos : $(TARGETS:%=%.iaos)
.PHONY: iaos

$(TARGETS:%=%.iaka): %.iaka : %
	$(IAKA) $< >> /dev/null

iaka : $(TARGETS:%=%.iaka)
.PHONY: iaka

$(TARGETS:%=%.iaio): %.iaio : %
	$(IAIO) $< >> /dev/null

iaio : $(TARGETS:%=%.iaio)
.PHONY: iaio

$(TARGETS:%=%.iaoo): %.iaoo : %
	$(IAOO) $< >> /dev/null

iaoo : $(TARGETS:%=%.iaoo)
.PHONY: iaoo

test : iaos iaka iaio iaoo
.PHONY: test

distri:
	@/bin/rm -rf   ../../../bld/tst/clt/asm
	@mkdir         ../../../bld/tst/clt/asm
	@cp Makefile   ../../../bld/tst/clt/asm
	@cp *.s        ../../../bld/tst/clt/asm

clean:
	@/bin/rm -f *.o *.d *.a core *~
	@/bin/rm -f $(TARGETS)
