# -----------------------------------------------------------------------------
# - Makefile                                                                  -
# - iato top level makefile                                                   -
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

all : debug

debug:
	${MAKE} -C src all

world:
	${MAKE} -C src all CCMODE=OPTIMIZED

test:
	${MAKE} -C tst all


install:
	${MAKE} -C src install

distri:
	@/bin/rm -rf bld
	@mkdir bld
	${MAKE} -C cnf distri
	${MAKE} -C src distri
	${MAKE} -C tst distri
	mv  bld iato-src-1.6
	tar cvf iato-src-1.6.tar iato-src-1.6
	gzip iato-src-1.6.tar	
	@/bin/rm -rf iato-src-1.6

clean:
	${MAKE} -C cnf clean
	${MAKE} -C src clean
	${MAKE} -C tst clean
	@/bin/rm -f *.o *.d *.a core *~
	@/bin/rm -rf bld iato-src-1.6 iato-src-1.6.tar iato-src-1.6.tar.gz
