# -----------------------------------------------------------------------------
# - Makefile                                                                  -
# - iato:iaio client - iaio client makefile                                   -
# -----------------------------------------------------------------------------
# - (c) inria 2002-2004                                                       -
# -----------------------------------------------------------------------------
# - authors                                        Amaury Darsch    2002:2004 -
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

SOURCES 	= Stx.cpp       \
		  Main.cpp      \
		  Option.cpp    \
		  IpgStg.cpp    \
		  RotStg.cpp    \
		  ExpStg.cpp    \
		  RenStg.cpp    \
		  DlyStg.cpp    \
		  ResStg.cpp    \
		  RegStg.cpp    \
		  ExeStg.cpp    \
		  DetStg.cpp    \
		  Fp1Stg.cpp    \
		  Fp2Stg.cpp    \
		  Fp3Stg.cpp    \
		  Fp4Stg.cpp    \
		  BobStg.cpp    \
		  BedStg.cpp    \
		  WrbStg.cpp    \
		  FwbStg.cpp    \
		  Resteer.cpp   \
		  RseLogic.cpp  \
		  Processor.cpp \
		  Simulator.cpp

DEPENDS		= $(SOURCES:.cpp=.d)
OBJECTS 	= $(SOURCES:.cpp=.o)
CCINCLUDE	= -I. -I../../lib/mac -I../../lib/krn -I../../lib/elf \
		  -I../../lib/isa
LIBIAISA	= ../../lib/isa/libiaisa.a
LIBIAELF	= ../../lib/elf/libiaelf.a
LIBIAKRN	= ../../lib/krn/libiakrn.a
LIBIAMAC	= ../../lib/mac/libiamac.a
LIBIAALL	= $(LIBIAMAC) $(LIBIAKRN) $(LIBIAELF) $(LIBIAISA)
include	      	../../../cnf/config.mak

all : iaio

-include $(DEPENDS)

%.o : %.cpp
	$(CC) $(CCFLAGS) $(CCDEFINES) $(CCINCLUDE) -o $@ -c $<

iaio : $(OBJECTS) $(LIBIAALL)
	$(CC) $(LKFLAGS) -o $@ $(OBJECTS) $(LIBIAALL) $(PLTLIBS)

distri:
	@/bin/rm -rf   ../../../bld/src/clt/iaio
	@mkdir         ../../../bld/src/clt/iaio
	@cp Makefile   ../../../bld/src/clt/iaio
	@cp *.hpp      ../../../bld/src/clt/iaio
	@cp *.cpp      ../../../bld/src/clt/iaio

clean:
	@/bin/rm -f *.o *.d *.a core *~
	@/bin/rm -f iaio
