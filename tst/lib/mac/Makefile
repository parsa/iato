# -----------------------------------------------------------------------------
# - Makefile                                                                  -
# - iato:mac testers - test makefile                                          -
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

SOURCES 	= t_cdirect.cpp

OBJECTS 	= $(SOURCES:.cpp=.o)
TESTERS		= $(basename $(OBJECTS))
CCINCLUDE      	= -I. -I../../../src/lib/mac -I../../../src/lib/elf
CCINCLUDE      += -I../../../src/lib/isa $(PLTINCS)
LIBIAISA	= ../../../src/lib/isa/libiaisa.a
LIBIAELF	= ../../../src/lib/elf/libiaelf.a
LIBIAMAC	= ../../../src/lib/mac/libiamac.a
IATOLIBS	= $(LIBIAMAC) $(LIBIAELF) $(LIBIAISA) $(PLTLIBS)
include 	  ../../../cnf/config.mak

all : test

%.o : %.cpp
	$(CC) $(CCFLAGS) $(CCDEFINES) $(CCINCLUDE) -o $@ -c $<

$(TESTERS) : % : %.o
	$(CC) -o $@ $@.o $(IATOLIBS)

pgm : $(TESTERS)

$(TESTERS:%=%.run): %.run : %
	@echo "testing :" $<
	@./$<

test : $(TESTERS:%=%.run)
.PHONY: test

distri:
	@/bin/rm -rf   ../../../bld/tst/lib/mac
	@mkdir         ../../../bld/tst/lib/mac
	@cp Makefile   ../../../bld/tst/lib/mac
	@cp *.cpp      ../../../bld/tst/lib/mac

clean:
	@/bin/rm -f *.o *.d *.a core *~
	@/bin/rm -f $(TESTERS)
