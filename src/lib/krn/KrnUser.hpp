// ---------------------------------------------------------------------------
// - KrnUser.hpp                                                             -
// - iato:krn library - user releated system call definition                 -
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

#ifndef  IATO_KRNUSER_HPP
#define  IATO_KRNUSER_HPP

#ifndef  IATO_SYSARGS_HPP
#include "SysArgs.hpp"
#endif

namespace iato {

  /// getuid system call
  void krn_getuid  (Register* rbk);
  /// geteuid system call
  void krn_geteuid (Register* rbk);
  /// getgid system call
  void krn_getgid  (Register* rbk);
  /// getegid system call
  void krn_getegid (Register* rbk);
  // getpid system call
  void krn_getpid  (Register* rbk);
  // getpid system call
  void krn_getppid (Register* rbk);
}

#endif
