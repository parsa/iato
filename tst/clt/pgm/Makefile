# -----------------------------------------------------------------------------
# - Makefile                                                                  -
# - iato client testers - test makefile                                       -
# -----------------------------------------------------------------------------
# - (c) inria[caps] 2002:2004                                                 -
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

SOURCES 	= p_0001.c \
		  p_0002.c \
		  p_0003.c \
		  p_0004.c \
		  p_0005.c \
		  p_0006.c \
		  p_0007.c \
		  p_0008.c \
		  p_0009.c \
		  p_0010.c \
		  p_0011.c \
		  p_0013.c \
		  p_0014.c \
		  p_0015.c \
		  p_0016.c

OBJECTS 	= $(SOURCES:.c=.o)
TARGETS		= $(basename $(OBJECTS))
IAOS		= ../../../src/clt/iaos/iaos -v
IAKA		= ../../../src/clt/iaka/iaka -G 256
IAOO		= ../../../src/clt/iaoo/iaoo -G 256
include 	  ../../../cnf/config.mak

all : test

%.o : %.c
	$(XCC) $(XCCFLAGS) $(XCCDEFINES) -o $@ -c $<

$(TARGETS) : % : %.o
	$(XLK) -o $@ $@.o

pgm : $(TARGETS)

$(TARGETS:%=%.iaka): %.iaka : %
	$(IAKA) $< >> /dev/null

iaka : $(TARGETS:%=%.iaka)
.PHONY: iaka

$(TARGETS:%=%.iaoo): %.iaoo : %
	$(IAOO) $< >> /dev/null

iato : $(TARGETS:%=%.iaoo)
.PHONY: iaoo

test : iaka iaoo
.PHONY: test

distri:
	@/bin/rm -rf   ../../../bld/tst/clt/pgm
	@mkdir         ../../../bld/tst/clt/pgm
	@cp Makefile   ../../../bld/tst/clt/pgm
	@cp *.c        ../../../bld/tst/clt/pgm

clean:
	@/bin/rm -f *.o *.d *.a core *~
	@/bin/rm -f $(TARGETS)
