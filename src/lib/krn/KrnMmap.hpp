// ---------------------------------------------------------------------------
// - KrnMmap.hpp                                                             -
// - iato:krn library - mmap releated system call definition                 -
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

#ifndef  IATO_KRNMMAP_HPP
#define  IATO_KRNMMAP_HPP

#ifndef  IATO_SYSARGS_HPP
#include "SysArgs.hpp"
#endif

namespace iato {

  /// mmap system call
  void krn_mmap (Rse* rse, Register* rbk, ElfExec* mem);

  /// munmap system call
  void krn_munmap (Rse* rse, Register* rbk, ElfExec* mem);
}

#endif
