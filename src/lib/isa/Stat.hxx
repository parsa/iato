// ---------------------------------------------------------------------------
// - Stat.hxx                                                                -
// - iato:isa library - isa stat collection dependant function definitions   -
// ---------------------------------------------------------------------------
// - (c) inria 2002-2004                                                     -
// ---------------------------------------------------------------------------
// - authors                                      Amaury Darsch    2002:2004 -
// -                                              Pierre Villalon  2002:2003 -
// -                                              Andre  Seznec    2002:2004 -
// ---------------------------------------------------------------------------
// - This program  is  free software;  you can redistribute it and/or modify -
// - it  under  the  terms  of the GNU  General Public License version 2, as -
// - published by the Free Software Foundation.                              -
// -                                                                         -
// - This  program  is  distributed  in  the  hope  that it  will be useful, -
// - but   without  any  warranty; without  even  the  implied  warranty  of -
// - merchantability or fitness for a particular purpose.                    -
// -                                                                         -
// - See the GNU General Public License version 2 for more details           -
// ---------------------------------------------------------------------------

#ifndef IATO_STAT_HXX
#define IATO_STAT_HXX

// ---------------------------------------------------------------------------
// - linux configuration                                                     -
// ---------------------------------------------------------------------------

#ifdef   OS_LINUX
#include <ctime>
#endif

// ---------------------------------------------------------------------------
// - sparc/solaris configuration                                             -
// ---------------------------------------------------------------------------

#ifdef   OS_SOLARIS
#include <ctime>
#endif

#endif
