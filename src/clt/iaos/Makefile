# -----------------------------------------------------------------------------
# - Makefile                                                                  -
# - iato:iaos client - iaos client makefile                                   -
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

SOURCES 	= Main.cpp      \
		  Option.cpp    \
		  Binary.cpp    \
		  Verify.cpp    \
		  Disass.cpp    \
		  Statistic.cpp

DEPENDS		= $(SOURCES:.cpp=.d)
OBJECTS 	= $(SOURCES:.cpp=.o)
CCINCLUDE	= -I. -I../../lib/elf -I../../lib/isa
LIBIAISA	= ../../lib/isa/libiaisa.a
LIBIAELF	= ../../lib/elf/libiaelf.a
STDLIB		= -lelf
include		../../../cnf/config.mak

all : iaos

-include $(DEPENDS)

%.o : %.cpp
	$(CC) $(CCFLAGS) $(CCDEFINES) $(CCINCLUDE) -o $@ -c $<

iaos : $(OBJECTS) $(LIBIAELF) $(LIBIAISA)
	$(CC) -o $@ $(OBJECTS) $(LIBIAELF) $(LIBIAISA) $(STDLIB)

install: iaos
	 mkdir -p  $(EXEINSTALL)
	 cp iaos   $(EXEINSTALL)
	 chmod 755 $(EXEINSTALL)/iaos

distri:
	@/bin/rm -rf   ../../../bld/src/clt/iaos
	@mkdir         ../../../bld/src/clt/iaos
	@cp Makefile   ../../../bld/src/clt/iaos
	@cp *.hpp      ../../../bld/src/clt/iaos
	@cp *.cpp      ../../../bld/src/clt/iaos

clean:
	@/bin/rm -f *.o *.d *.a core *~
	@/bin/rm -f iaos
