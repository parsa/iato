// ---------------------------------------------------------------------------
// - KrnIoctl.hpp                                                            -
// - iato:krn library - ioctl related system call definition                 -
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

#ifndef  IATO_KRNIOCTL_HPP
#define  IATO_KRNIOCTL_HPP

#ifndef  IATO_SYSARGS_HPP
#include "SysArgs.hpp"
#endif

namespace iato {

  /// ioctl system call
  void krn_ioctl (Rse* rse, Register* rbk, ElfExec* mem);
}

#endif
