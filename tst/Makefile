# -----------------------------------------------------------------------------
# - Makefile                                                                  -
# - iato:tst - all test makefile                                              -
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
	${MAKE} -C lib all
	${MAKE} -C clt all

pgm:
	${MAKE} -C lib pgm
	${MAKE} -C clt pgm

distri:
	@/bin/rm -rf   ../bld/tst
	@mkdir         ../bld/tst
	@cp Makefile   ../bld/tst
	${MAKE} -C lib distri
	${MAKE} -C clt distri

clean:
	${MAKE} -C lib clean
	${MAKE} -C clt clean
	@/bin/rm -f *.o *.d *.a core *~
