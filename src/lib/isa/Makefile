# -----------------------------------------------------------------------------
# - Makefile                                                                  -
# - iato:isa library - isa library makefile                                   -
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

SOURCES 	= Ip.cpp          \
		  Uvr.cpp         \
		  Ctx.cpp         \
		  Env.cpp         \
		  Rid.cpp         \
		  Cfm.cpp         \
		  Pfs.cpp         \
		  Umr.cpp         \
		  Psr.cpp         \
		  Rse.cpp         \
		  Irt.cpp         \
		  Lru.cpp         \
		  Mrt.cpp         \
		  Rpm.cpp         \
		  Fpsr.cpp        \
		  Huge.cpp        \
		  Real.cpp        \
		  Bits.cpp        \
		  Stat.cpp        \
		  Alat.cpp        \
		  Utils.cpp       \
		  Instr.cpp       \
		  Bundle.cpp      \
		  Result.cpp      \
		  Plugin.cpp      \
		  Record.cpp      \
		  Tracer.cpp      \
		  Memory.cpp      \
		  Filter.cpp	  \
		  Segment.cpp     \
		  Checker.cpp     \
		  Operand.cpp     \
		  Adecode.cpp     \
		  Mdecode.cpp     \
		  Idecode.cpp     \
		  Fdecode.cpp     \
		  Bdecode.cpp     \
		  Xdecode.cpp     \
		  Urecode.cpp     \
		  Arecode.cpp     \
		  Mrecode.cpp     \
		  Irecode.cpp     \
		  Frecode.cpp     \
		  Brecode.cpp     \
		  Xrecode.cpp     \
		  MemLogic.cpp    \
		  Aexecute.cpp    \
		  Mexecute.cpp    \
		  Iexecute.cpp    \
		  Fexecute.cpp    \
		  Bexecute.cpp    \
		  Resource.cpp    \
		  Register.cpp    \
		  Exception.cpp   \
		  Interrupt.cpp

NMLIB		= iaisa
ARLIB		= lib$(NMLIB).a
SOLIB		= lib$(NMLIB).so

CCINCLUDE	= -I. $(PLTINCS)

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

$(ARLIB) : $(OBJECTS)
	ar rc $@ $(OBJECTS)

$(SOLIB) : $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS)

$(NMLIB) : $(ARLIB) $(SOLIB)

install: $(NMLIB)
	 mkdir -p    $(LIBINSTALL)
	 cp $(ARLIB) $(LIBINSTALL)
	 cp $(SOLIB) $(LIBINSTALL)
	 mkdir -p    $(HDRINSTALL)/isa
	 cp *.hpp    $(HDRINSTALL)/isa

distri:
	@/bin/rm -rf   ../../../bld/src/lib/isa
	@mkdir         ../../../bld/src/lib/isa
	@cp Makefile   ../../../bld/src/lib/isa
	@cp *.hpp      ../../../bld/src/lib/isa
	@cp *.hxx      ../../../bld/src/lib/isa
	@cp *.cpp      ../../../bld/src/lib/isa

clean:
	@/bin/rm -f *.o *.d *.a core *~
	@/bin/rm -f $(ARLIB) $(SOLIB)
