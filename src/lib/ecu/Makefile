# -----------------------------------------------------------------------------
# - Makefile                                                                  -
# - iato:ecu library - library makefile                                       -
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

SOURCES 	= Utx.cpp     \
		  Mapper.cpp  \
		  Fetcher.cpp \
		  Renamer.cpp 

NMLIB		= iaecu
ARLIB		= lib$(NMLIB).a
SOLIB		= lib$(NMLIB).so

CCINCLUDE	= -I. -I../mac -I../krn -I../elf -I../isa $(PLTINCS)

SOISA		= -L../isa -liaisa
SOELF		= -L../elf -liaelf
SOKRN		= -L../krn -liakrn
SOMAC		= -L../mac -liamac
ARISA        	= ../isa/libiaisa.a
ARELF        	= ../elf/libiaelf.a
ARKRN        	= ../krn/libiakrn.a
ARMAC        	= ../mac/libiamac.a

ARALL		= $(ARMAC) $(ARKRN) $(ARELF) $(ARISA)
SOALL		= $(SOMAC) $(SOKRN) $(SOELF) $(SOISA)

# ---------------------------------------------------------------------------
# - private section                                                         -
# ---------------------------------------------------------------------------

DEPENDS		= $(SOURCES:.cpp=.d)
OBJECTS 	= $(SOURCES:.cpp=.o)
include 	../../../cnf/config.mak

all : $(NMLIB)

-include $(DEPENDS)

%.o : %.cpp
	$(CC) $(CCFLAGS) $(CCDEFINES) $(CCINCLUDE) -o $@ -c $<

$(ARLIB) : $(OBJECTS) $(ARALL)
	ar rc $@ $(OBJECTS)

$(SOLIB) : $(OBJECTS) $(ARALL)
	$(CC) -shared -o $@ $(OBJECTS) $(SOALL)

$(NMLIB) : $(ARLIB) $(SOLIB)

install: $(NMLIB)
	 mkdir -p    $(LIBINSTALL)
	 cp $(ARLIB) $(LIBINSTALL)
	 cp $(SOLIB) $(LIBINSTALL)
	 mkdir -p    $(HDRINSTALL)/ecu
	 cp *.hpp    $(HDRINSTALL)/ecu

distri:
	@/bin/rm -rf   ../../../bld/src/lib/ecu
	@mkdir         ../../../bld/src/lib/ecu
	@cp Makefile   ../../../bld/src/lib/ecu
	@cp *.hpp      ../../../bld/src/lib/ecu
	@cp *.cpp      ../../../bld/src/lib/ecu

clean:
	@/bin/rm -f *.o *.d *.a core *~
	@/bin/rm -f $(ARLIB) $(SOLIB)
