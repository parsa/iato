// ---------------------------------------------------------------------------
// - KrnTime.hxx                                                             -
// - iato:krn library - time related system call argument definition         -
// ---------------------------------------------------------------------------
// - (c) inria 2002-2004                                                /    -
//                       florian paumier                                     -
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
// ---------------------------------------------------------------------------

#ifndef  IATO_KRNTIME_HXX
#define  IATO_KRNTIME_HXX

#ifndef  IATO_EXCEPTION_HPP
#include "Exception.hpp"
#endif

namespace iato {

  // the kernel timeval structure for gettimeofday
  struct krn_timeval {
    t_octa d_tv_sec;          // seconds 
    t_octa d_tv_usec;         // microseconds
  };
  
  // the kernel tms structure for times
  struct krn_tms {
    t_octa d_tms_utime;      // user CPU time
    t_octa d_tms_stime;      // system CPU time
    t_octa d_tms_cutime;     // user CPU time of dead children
    t_octa d_tms_cstime;     // system CPU time of dead children
  };
  
// ---------------------------------------------------------------------------
// - linux based mapping                                                     -
// ---------------------------------------------------------------------------

#ifdef   OS_LINUX
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>
#endif

// ---------------------------------------------------------------------------
// - solaris base mapping                                                    -
// ---------------------------------------------------------------------------

#ifdef   OS_SOLARIS
#include <errno.h>
#include <sys/time.h>
#include <sys/times.h>
#endif
}
#endif
