// ---------------------------------------------------------------------------
// - KrnSig.hxx                                                              -
// - iato:krn library - signal related system call argument definition       -
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

#ifndef  IATO_KRNSIG_HXX
#define  IATO_KRNSIG_HXX

#ifndef  IATO_EXCEPTION_HPP
#include "Exception.hpp"
#endif

// ---------------------------------------------------------------------------
// - linux 2.4 - gnu libc 2                                                  -
// ---------------------------------------------------------------------------

namespace iato {
  const int KRN_SIG_BLOCK   = 0x00;
  const int KRN_SIG_UNBLOCK = 0x01;
  const int KRN_SIG_SETMASK = 0x02;
}

// ---------------------------------------------------------------------------
// - linux based mapping                                                     -
// ---------------------------------------------------------------------------

#ifdef   OS_LINUX
#include <errno.h>
#include <signal.h>
#include <sys/types.h>

namespace iato {
  
  inline static int map_sigprocmask_request (const int req) {
    return req;
  }
}
#endif

// ---------------------------------------------------------------------------
// - solaris base mapping                                                    -
// ---------------------------------------------------------------------------

#ifdef   OS_SOLARIS
#include <errno.h>
#include <signal.h>
#include <sys/types.h>

namespace iato {
  
  inline static int map_sigprocmask_request (const int req) {
    int result = 0;
    if (req == SIG_BLOCK) result = KRN_SIG_BLOCK;
    else if (req == SIG_UNBLOCK) result = KRN_SIG_UNBLOCK;
    else if (req == SIG_SETMASK) result = KRN_SIG_SETMASK;
    else throw Exception ("syscall-error", "unexisting sigprocmask request");
    return result;
  }
}
#endif

#endif
