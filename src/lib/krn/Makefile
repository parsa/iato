# -----------------------------------------------------------------------------
# - Makefile                                                                  -
# - iato:krn library - library makefile                                       -
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

SOURCES 	= KrnSig.cpp   \
		  Syscall.cpp  \
		  SysArgs.cpp  \
		  KrnUser.cpp  \
		  KrnProc.cpp  \
	   	  KrnExit.cpp  \
		  KrnMmap.cpp  \
		  KrnTime.cpp  \
		  KrnIoctl.cpp \
		  KrnFcntl.cpp \
		  KrnUname.cpp

NMLIB		= iakrn
ARLIB		= lib$(NMLIB).a
SOLIB		= lib$(NMLIB).so

CCINCLUDE	= -I. -I../elf -I../isa $(PLTINCS)

SOISA		= -L../isa -liaisa
SOELF		= -L../elf -liaelf
ARISA        	= ../isa/libiaisa.a
ARELF        	= ../elf/libiaelf.a

ARALL		= $(ARELF) $(ARISA)
SOALL		= $(SOELF) $(SOISA)

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
	 mkdir -p    $(HDRINSTALL)/krn
	 cp *.hpp    $(HDRINSTALL)/krn

distri:
	@/bin/rm -rf   ../../../bld/src/lib/krn
	@mkdir         ../../../bld/src/lib/krn
	@cp Makefile   ../../../bld/src/lib/krn
	@cp *.hpp      ../../../bld/src/lib/krn
	@cp *.hxx      ../../../bld/src/lib/krn
	@cp *.cpp      ../../../bld/src/lib/krn

clean:
	@/bin/rm -f *.o *.d *.a core *~
	@/bin/rm -f $(ARLIB) $(SOLIB)
