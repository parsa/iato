# -----------------------------------------------------------------------------
# - Makefile                                                                  -
# - iato:iata client - iata client makefile                                   -
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
		  Prstat.cpp    \
		  Printing.cpp  \
		  Generate.cpp  \
		  Statistic.cpp \
		  Filtering.cpp

DEPENDS		= $(SOURCES:.cpp=.d)
OBJECTS 	= $(SOURCES:.cpp=.o)
CCINCLUDE	= -I. -I../../lib/mac -I../../lib/krn -I../../lib/elf \
		  -I../../lib/isa
LIBIAISA	= ../../lib/isa/libiaisa.a
LIBIAELF	= ../../lib/elf/libiaelf.a
LIBIAKRN	= ../../lib/krn/libiakrn.a
LIBIAMAC	= ../../lib/mac/libiamac.a
LIBIAALL	= $(LIBIAMAC) $(LIBIAKRN) $(LIBIAELF) $(LIBIAISA)
include		../../../cnf/config.mak

all : iata

-include $(DEPENDS)

%.o : %.cpp
	$(CC) $(CCFLAGS) $(CCDEFINES) $(CCINCLUDE) -o $@ -c $<

iata : $(OBJECTS) $(LIBIAALL)
	$(CC) -o $@ $(OBJECTS) $(LIBIAALL) $(STDLIB)

install: iata
	 mkdir -p  $(EXEINSTALL)
	 cp iata   $(EXEINSTALL)
	 chmod 755 $(EXEINSTALL)/iata

distri:
	@/bin/rm -rf   ../../../bld/src/clt/iata
	@mkdir         ../../../bld/src/clt/iata
	@cp Makefile   ../../../bld/src/clt/iata
	@cp *.hpp      ../../../bld/src/clt/iata
	@cp *.cpp      ../../../bld/src/clt/iata

clean:
	@/bin/rm -f *.o *.d *.a core *~
	@/bin/rm -f iata
