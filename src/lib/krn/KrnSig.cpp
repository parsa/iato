// ---------------------------------------------------------------------------
// - KrnSig.cpp                                                              -
// - iato:krn library - signal related system call implementation            -
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

#include "KrnSig.hpp"
#include "KrnSig.hxx"
#include "Exception.hpp"

namespace iato {

  // sigaction system call - simply ignored at this stage

  void krn_sigaction (Rse* rse, Register* rbk, Memory* mem) {
    t_long result = 0;
    if (result == -1) {
      sys_args_seterrn (result, errno, rbk);
    } else {
      sys_args_setretn (result, rbk);
    }
  }

  // sigproc mask system call - simply ignored at this stage

  void krn_sigprocmask (Rse* rse, Register* rbk, Memory* mem) {
    t_long result = 0;
    // t_octa oldset = sys_args_getoval (2, rse, rbk);
    //    assert (oldset != OCTA_0);
    // set syscall result
    if (result == 0) {
      sys_args_setretn (result, rbk);
    } else {
      sys_args_seterrn (result, errno, rbk);
    }
  }
  
  // kill system call

  void krn_kill (Rse* rse, Register* rbk, Memory* mem) {
    throw Exception ("kill-error", "unsupported kill call");
  }
}
