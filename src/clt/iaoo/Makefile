# -----------------------------------------------------------------------------
# - Makefile                                                                  -
# - iato:iaoo client - iaoo client makefile                                   -
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

SOURCES 	= Stx.cpp       \
		  Main.cpp      \
		  Option.cpp    \
		  DlyStg.cpp    \
		  ResStg.cpp    \
		  IpgStg.cpp    \
		  DecStg.cpp    \
		  LrnStg.cpp    \
		  PrnStg.cpp    \
		  SlcStg.cpp    \
		  RegStg.cpp    \
		  ExeStg.cpp    \
		  WbkStg.cpp    \
		  CmtStg.cpp    \
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

all : iaoo

-include $(DEPENDS)

%.o : %.cpp
	$(CC) $(CCFLAGS) $(CCDEFINES) $(CCINCLUDE) -o $@ -c $<

iaoo : $(OBJECTS) $(LIBIAALL)
	$(CC) $(LKFLAGS) -o $@ $(OBJECTS) $(LIBIAALL) $(PLTLIBS)

install: iaoo
	 mkdir -p  $(EXEINSTALL)
	 cp iaoo   $(EXEINSTALL)
	 chmod 755 $(EXEINSTALL)/iaoo

distri:
	@/bin/rm -rf   ../../../bld/src/clt/iaoo
	@mkdir         ../../../bld/src/clt/iaoo
	@cp Makefile   ../../../bld/src/clt/iaoo
	@cp *.hpp      ../../../bld/src/clt/iaoo
	@cp *.cpp      ../../../bld/src/clt/iaoo

clean:
	@/bin/rm -f *.o *.d *.a core *~
	@/bin/rm -f iaoo
