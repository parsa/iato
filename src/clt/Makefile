# -----------------------------------------------------------------------------
# - Makefile                                                                  -
# - iato:clt library - all clients makefile                                   -
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
	${MAKE} -C iaos all
	${MAKE} -C iata all
	${MAKE} -C iaka all
	${MAKE} -C iaoo all

install:
	${MAKE} -C iaos install
	${MAKE} -C iata install
	${MAKE} -C iaka install
	${MAKE} -C iaoo install

distri:
	@/bin/rm -rf   ../../bld/src/clt
	@mkdir         ../../bld/src/clt
	${MAKE} -C iaos distri
	${MAKE} -C iaka distri
	${MAKE} -C iaoo distri
	${MAKE} -C iata distri

clean:
	${MAKE} -C iaos clean
	${MAKE} -C iata clean
	${MAKE} -C iaka clean
	${MAKE} -C iaoo clean
	@/bin/rm -f *.o *.d *.a core *~
