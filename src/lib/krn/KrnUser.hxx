// ---------------------------------------------------------------------------
// - KrnUser.hxx                                                             -
// - iato:krn library - user related system call argument definition         -
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

#ifndef  IATO_KRNUSER_HXX
#define  IATO_KRNUSER_HXX

// ---------------------------------------------------------------------------
// - linux based mapping                                                     -
// ---------------------------------------------------------------------------

#ifdef   OS_LINUX
#include <unistd.h>
#include <sys/types.h>
#endif

// ---------------------------------------------------------------------------
// - solaris base mapping                                                    -
// ---------------------------------------------------------------------------

#ifdef   OS_SOLARIS
#include <sys/types.h>
#include <unistd.h>
#endif

#endif
