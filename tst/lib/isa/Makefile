# -----------------------------------------------------------------------------
# - Makefile                                                                  -
# - iato:tst:lib:isa testers - test makefile                                  -
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

SOURCES 	= t_ip.cpp	\
		  t_rid.cpp	\
		  t_cfm.cpp	\
		  t_uvr.cpp	\
	  	  t_umr.cpp	\
		  t_psr.cpp	\
		  t_bits.cpp	\
		  t_huge.cpp	\
		  t_real.cpp	\
		  t_alat.cpp	\
		  t_fpsr.cpp	\
		  t_bundle.cpp

OBJECTS 	= $(SOURCES:.cpp=.o)
TESTERS		= $(basename $(OBJECTS))
CCINCLUDE      	= -I. -I../../../src/lib/isa $(PLTINCS)
LIBIAISA	= ../../../src/lib/isa/libiaisa.a
include 	  ../../../cnf/config.mak

all : test

%.o : %.cpp
	$(CC) $(CCFLAGS) $(CCDEFINES) $(CCINCLUDE) -o $@ -c $<

$(TESTERS) : % : %.o
	$(CC) -o $@ $@.o $(LIBIAISA) $(PLTLIBS)

pgm : $(TESTERS)

$(TESTERS:%=%.run): %.run : %
	@echo "testing :" $<
	@./$<

test : $(TESTERS:%=%.run)
.PHONY: test

distri:
	@/bin/rm -rf   ../../../bld/tst/lib/isa
	@mkdir         ../../../bld/tst/lib/isa
	@cp Makefile   ../../../bld/tst/lib/isa
	@cp *.cpp      ../../../bld/tst/lib/isa

clean:
	@/bin/rm -f *.o *.d *.a core *~
	@/bin/rm -f $(TESTERS)
