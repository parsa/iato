// ---------------------------------------------------------------------------
// - KrnIoctl.cpp                                                            -
// - iato:krn library - ioctl related system call implementation             -
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

#include "Bits.hpp"
#include "KrnIoctl.hpp"
#include "KrnIoctl.hxx"

namespace iato {

  // this procedure implements the functionality of the TCGETS ioctl
  static int ioctl_tcgets (Rse* rse, Register* rbk, ElfExec* mem) {
    // get the first argument
    int fd  = (int) sys_args_getoval (0, rse, rbk);
    // prepare for local call
    termios ut;
    int status = ioctl (fd, TCGETS, &ut);
    if (status != 0) return status;
    // map to memory data
    union {
      t_byte      btr[sizeof(krn_termios)];
      krn_termios ktr;
    };
    // zero the whole structure
    for (long i = 0; i < (long) sizeof(krn_termios); i++) btr[i] = BYTE_0;
    map_termios (ktr, ut);
    // copy result into memory
    t_octa kptr = sys_args_getoval (2, rse, rbk);
    sys_args_setdata (kptr, btr, sizeof (krn_termios), mem);
    // done
    return status;
  }

  // ioctl system call
  
  void krn_ioctl (Rse* rse, Register* rbk, ElfExec* mem) {
    // initialize result and arguments
    int result = -1;
    // get the request
    t_octa req = sys_args_getoval  (1, rse, rbk);
    t_octa ctl = map_ioctl_request (req);
    // process the request
    switch (ctl) {
    case TCGETS:
      result = ioctl_tcgets (rse, rbk, mem);
      break;
    default:
      result = -1;
      break;
    }
    // set syscall result
    if (result != 0) {
      sys_args_seterrn (result, errno, rbk);
    } else {
      sys_args_setretn (OCTA_0, rbk);
    }
  }
}
