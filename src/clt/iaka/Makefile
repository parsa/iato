# -----------------------------------------------------------------------------
# - Makefile                                                                  -
# - iato:iaka client - iaka client makefile                                   -
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

SOURCES 	= Atx.cpp       \
		  Main.cpp      \
		  Option.cpp    \
		  Rename.cpp    \
		  Fetcher.cpp   \
		  Emulator.cpp

DEPENDS		= $(SOURCES:.cpp=.d)
OBJECTS 	= $(SOURCES:.cpp=.o)
CCINCLUDE	= -I. 
CCINCLUDE      += -I../../lib/mac 
CCINCLUDE      += -I../../lib/krn
CCINCLUDE      += -I../../lib/elf 
CCINCLUDE      += -I../../lib/isa
LIBIAISA	= ../../lib/isa/libiaisa.a
LIBIAELF	= ../../lib/elf/libiaelf.a
LIBIAKRN	= ../../lib/krn/libiakrn.a
LIBIAMAC	= ../../lib/mac/libiamac.a
LIBIAALL	= $(LIBIAMAC) $(LIBIAKRN) $(LIBIAELF) $(LIBIAISA)
include	      	../../../cnf/config.mak

all : iaka

-include $(DEPENDS)

%.o : %.cpp
	$(CC) $(CCFLAGS) $(CCDEFINES) $(CCINCLUDE) -o $@ -c $<

iaka : $(OBJECTS) $(LIBIAALL)
	$(CC) $(LKFLAGS) -o $@ $(OBJECTS) $(LIBIAALL) $(PLTLIBS)

install: iaka
	mkdir -p $(EXEINSTALL)
	cp iaka $(EXEINSTALL)
	chmod 755 $(EXEINSTALL)/iaka

distri:
	@/bin/rm -rf   ../../../bld/src/clt/iaka
	@mkdir         ../../../bld/src/clt/iaka
	@cp Makefile   ../../../bld/src/clt/iaka
	@cp *.hpp      ../../../bld/src/clt/iaka
	@cp *.cpp      ../../../bld/src/clt/iaka

clean:
	@/bin/rm -f *.o *.d *.a core *~
	@/bin/rm -f iaka
