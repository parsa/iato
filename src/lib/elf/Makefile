# -----------------------------------------------------------------------------
# - Makefile                                                                  -
# - iato:elf library - elf library makefile                                   -
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

SOURCES 	= Etx.cpp        \
		  ElfBsa.cpp     \
		  ElfBrk.cpp     \
		  ElfMap.cpp     \
		  ElfText.cpp    \
		  ElfLoad.cpp    \
		  ElfExec.cpp    \
		  ElfArgs.cpp    \
		  ElfEnvp.cpp    \
		  ElfStack.cpp   \
		  ElfTable.cpp   \
		  ElfImage.cpp   \
		  ElfKernel.cpp  \
		  ElfInterp.cpp  \
		  ElfMemory.cpp  \
		  ElfSection.cpp \
		  ElfSegment.cpp \
		  ElfChecker.cpp

NMLIB		= iaelf
ARLIB		= lib$(NMLIB).a
SOLIB		= lib$(NMLIB).so

CCINCLUDE	= -I. -I../isa $(PLTINCS)

SOISA		= -L../isa -liaisa
ARISA        	= ../isa/libiaisa.a

ARALL		= $(ARISA)
SOALL		= $(SOISA)

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
	 mkdir -p    $(HDRINSTALL)/elf
	 cp *.hpp    $(HDRINSTALL)/elf

distri:
	@/bin/rm -rf   ../../../bld/src/lib/elf
	@mkdir         ../../../bld/src/lib/elf
	@cp Makefile   ../../../bld/src/lib/elf
	@cp *.hpp      ../../../bld/src/lib/elf
	@cp *.hxx      ../../../bld/src/lib/elf
	@cp *.cpp      ../../../bld/src/lib/elf

clean:
	@/bin/rm -f *.o *.d *.a core *~
	@/bin/rm -f $(ARLIB) $(SOLIB)
