// ---------------------------------------------------------------------------
// - KrnProc.cpp                                                             -
// - iato:krn library - process related system call implementation           -
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

#include "KrnProc.hpp"
#include "KrnExit.hpp"

namespace iato {

  // exit system call

  void krn_exit (Rse* rse, Register* rbk) {
    t_octa status = sys_args_getoval (0, rse, rbk);
    throw KrnExit (status);
  }

  // brk system call

  void krn_brk (Rse* rse, Register* rbk, ElfExec* mem) {
    // double check arguments
    if ((!rse) || (!rbk) || (!mem)) {
      sys_args_seterrn ((t_octa) -1, OCTA_0, rbk);
      return;
    };
    // get the break memory
    ElfBrk* brkm = mem->getbrkm ();
    if (!brkm) {
      sys_args_seterrn ((t_octa) -1, OCTA_0, rbk);
      return;
    }
    // get the new break value  and set
    t_octa brk = sys_args_getoval (0, rse, rbk);
    brkm->setbrkta (brk);
    // return the new break value - if it was null return the current one
    sys_args_setretn (brkm->getbrkta (), rbk);
  }
}
