# -----------------------------------------------------------------------------
# - Makefile                                                                  -
# - iato:clt - all libraries test makefile                                    -
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

all : test

test:
	${MAKE} -C isa all
	${MAKE} -C elf all
	${MAKE} -C mac all

pgm:
	${MAKE} -C isa pgm
	${MAKE} -C elf pgm
	${MAKE} -C mac pgm

distri:
	@/bin/rm -rf   ../../bld/tst/lib
	@mkdir         ../../bld/tst/lib
	@cp Makefile   ../../bld/tst/lib
	${MAKE} -C isa distri
	${MAKE} -C elf distri
	${MAKE} -C mac distri

clean:
	${MAKE} -C isa clean
	${MAKE} -C elf clean
	${MAKE} -C mac clean
	@/bin/rm -f *.o *.d *.a core *~
