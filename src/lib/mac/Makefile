# -----------------------------------------------------------------------------
# - Makefile                                                                  -
# - iato:mac library - library makefile                                       -
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

SOURCES 	= Mtx.cpp         \
		  Ssi.cpp         \
		  Dsi.cpp         \
		  Rat.cpp         \
		  Bdb.cpp         \
		  Spb.cpp         \
		  Rob.cpp         \
		  Eiq.cpp         \
		  Eib.cpp         \
		  Iib.cpp         \
		  Irb.cpp         \
		  Gcs.cpp         \
		  Trb.cpp         \
		  Urb.cpp         \
		  Urf.cpp         \
		  Bpe.cpp         \
		  Bpn.cpp         \
		  Mbe.cpp         \
		  Mbn.cpp         \
		  Mob.cpp         \
		  Stb.cpp         \
		  Sct.cpp         \
		  Htr.cpp         \
		  Pht.cpp         \
		  Btb.cpp         \
		  Mta.cpp         \
		  Mpr.cpp         \
		  Mli.cpp         \
		  Msi.cpp         \
		  Hma.cpp         \
		  Rib.cpp         \
		  Slot.cpp        \
		  Stage.cpp       \
		  Cache.cpp       \
		  Gskew.cpp       \
		  Pskew.cpp       \
		  Branch.cpp      \
		  Gshare.cpp      \
		  Pforce.cpp      \
		  Pshare.cpp      \
		  Detect.cpp      \
		  Hazard.cpp      \
		  System.cpp      \
		  Bimodal.cpp     \
		  Pimodal.cpp     \
		  Pbmodal.cpp     \
		  Restart.cpp     \
		  Station.cpp     \
		  Disperse.cpp    \
		  RseStack.cpp    \
		  Runnable.cpp    \
		  Pipeline.cpp    \
		  Pipelane.cpp    \
		  Weakable.cpp    \
		  Delayable.cpp   \
		  Predicate.cpp   \
		  Scoreboard.cpp  \
		  CacheBlock.cpp  \
		  CacheDirect.cpp

NMLIB		= iamac
ARLIB		= lib$(NMLIB).a
SOLIB		= lib$(NMLIB).so

CCINCLUDE	= -I. -I../krn -I../elf -I../isa $(PLTINCS)

SOISA		= -L../isa -liaisa
SOELF		= -L../elf -liaelf
SOKRN		= -L../krn -liakrn
ARISA        	= ../isa/libiaisa.a
ARELF        	= ../elf/libiaelf.a
ARKRN        	= ../krn/libiakrn.a

ARALL		= $(ARKRN) $(ARELF) $(ARISA)
SOALL		= $(SOKRN) $(SOELF) $(SOISA)

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
	 mkdir -p    $(HDRINSTALL)/mac
	 cp *.hpp    $(HDRINSTALL)/mac

distri:
	@/bin/rm -rf   ../../../bld/src/lib/mac
	@mkdir         ../../../bld/src/lib/mac
	@cp Makefile   ../../../bld/src/lib/mac
	@cp *.hpp      ../../../bld/src/lib/mac
	@cp *.cpp      ../../../bld/src/lib/mac

clean:
	@/bin/rm -f *.o *.d *.a core *~
	@/bin/rm -f $(ARLIB) $(SOLIB)
