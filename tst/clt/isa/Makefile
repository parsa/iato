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

SOURCES 	= A01.s \
		  A02.s \
		  A03.s \
		  A05.s \
		  A06.s \
		  A07.s \
		  A08.s \
		  A9b.s \
		  A09.s \
		  A10.s \
		  M01.s \
		  M02.s \
		  M03.s \
		  M04.s \
		  M05.s \
		  M06.s \
		  M07.s \
		  M08.s \
		  M09.s \
		  M10.s \
		  M11.s \
		  M12.s \
		  M13.s \
		  M14.s \
		  M15.s \
		  M18.s \
		  M19.s \
		  M20.s \
		  M24.s \
		  M29.s \
		  M30.s \
		  M31.s \
		  M32.s \
		  M33.s \
		  M34.s \
		  I01.s	\
		  I02.s \
		  I03.s \
		  I04.s \
		  I05.s \
		  I06.s \
		  I07.s \
		  I08.s \
		  I09.s \
		  I10.s \
		  I11.s \
		  I12.s \
		  I13.s \
		  I14.s \
		  I15.s \
		  I16.s \
		  I17.s \
		  I21.s \
		  I22.s \
	          I23.s \
	          I24.s \
	          I25.s \
	          I26.s \
	          I27.s \
	          I28.s \
	          I29.s \
		  F01.s \
		  F02.s \
		  F03.s \
		  F04.s \
		  F05.s \
		  F07.s \
		  F08.s \
		  F09.s \
		  F10.s \
		  F11.s \
		  X02.s \
		  B01.s \
	   	  B06.s \
		  B07.s

SRCEXTS		= F06.s \
		  F12.s \
		  F13.s \
		  F14.s \
		  F15.s \
		  I19.s \
		  M16.s \
		  M17.s \
		  M21.s \
		  M22.s \
		  M23.s \
		  M25.s \
		  M26.s \
		  M27.s \
		  M28.s \
		  M35.s \
		  M36.s \
		  M38.s \
		  M39.s \
		  M40.s \
		  M41.s \
		  M42.s \
		  M43.s \
		  M44.s \
		  M45.s \
		  M46.s \
		  X01.s


OBJECTS 	= $(SOURCES:.s=.o)
OBJEXTS		= $(SRCEXTS:.s=.o)
TARGETS		= $(basename $(OBJECTS))
TRGEXTS		= $(basename $(OBJEXTS))
LDSCRPT		= ../../../cnf/iato.lds
IAOS		= ../../../src/clt/iaos/iaos -v
IAKA		= ../../../src/clt/iaka/iaka -c
IAIO		= ../../../src/clt/iaio/iaio -c
IAOO		= ../../../src/clt/iaoo/iaoo -c
include 	  ../../../cnf/config.mak

all : test

%.o : %.s
	$(XAS) -o $@ $<

$(TARGETS) : % : %.o xit.o
	$(XLD) -T $(LDSCRPT) -o $@ $@.o

$(TRGEXTS) : % : %.o xit.o
	$(XLD) -T $(LDSCRPT) -o $@ $@.o

pgm : $(TARGETS) $(TRGEXTS)

$(TARGETS:%=%.iaos): %.iaos : %
	$(IAOS) $< >> /dev/null

$(TRGEXTS:%=%.iaos): %.iaos : %
	$(IAOS) $< >> /dev/null

iaos : $(TARGETS:%=%.iaos) $(TRGEXTS:%=%.iaos)
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

iato : $(TARGETS:%=%.iaoo)
.PHONY: iaoo

test : iaos iaka iaio iaoo
.PHONY: test

distri:
	@/bin/rm -rf   ../../../bld/tst/clt/isa
	@mkdir         ../../../bld/tst/clt/isa
	@cp Makefile   ../../../bld/tst/clt/isa
	@cp *.s        ../../../bld/tst/clt/isa

clean:
	@/bin/rm -f *.o *.d *.a core *~
	@/bin/rm -f $(TARGETS) $(TRGEXTS)
