# -----------------------------------------------------------------------------
# - Makefile                                                                  -
# - iato:lib library - all library makefile                                   -
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

all : build

build:
	${MAKE} -C isa all
	${MAKE} -C elf all
	${MAKE} -C krn all
	${MAKE} -C mac all
	${MAKE} -C ecu all

install:
	${MAKE} -C isa install
	${MAKE} -C elf install
	${MAKE} -C krn install
	${MAKE} -C mac install
	${MAKE} -C ecu install

distri:
	@/bin/rm -rf   ../../bld/src/lib
	@mkdir         ../../bld/src/lib
	@cp Makefile   ../../bld/src/lib
	${MAKE} -C isa distri
	${MAKE} -C elf distri
	${MAKE} -C krn distri
	${MAKE} -C mac distri
	${MAKE} -C ecu distri

clean:
	${MAKE} -C isa clean
	${MAKE} -C elf clean
	${MAKE} -C krn clean
	${MAKE} -C mac clean
	${MAKE} -C ecu clean
	@/bin/rm -f *.o *.d *.a core *~
