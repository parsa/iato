// ---------------------------------------------------------------------------
// - KrnUname.cpp                                                            -
// - iato:krn library - uname related system call implementation             -
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

#include "Utils.hpp"
#include "KrnUname.hpp"
#include "KrnUname.hxx"

namespace iato {

  // uname system call
  
  void krn_uname (Rse* rse, Register* rbk, ElfExec* mem) {
    
    // get the node name from the running machine
    struct utsname uts;
    int status = uname (&uts);
    if (status == -1) {
      sys_args_seterrn (status, errno, rbk);
      return;
    }
    // the kernel result structue
    union {
      t_byte      kbk[sizeof(krn_utsname)];
      krn_utsname utk;
    };
    // zero the whole structure
    for (long i = 0; i < (long) sizeof(krn_utsname); i++) kbk[i] = BYTE_0;
    // get result address structure
    t_octa uptr = sys_args_getoval (0, rse, rbk);
    // map result to structure
    cpystr (utk.sysname,  KRN_UTSNAME_SYSNAME);
    cpystr (utk.nodename, (const t_byte*) uts.nodename);
    cpystr (utk.release,  KRN_UTSNAME_RELEASE);
    cpystr (utk.version,  KRN_UTSNAME_VERSION);
    cpystr (utk.machine,  KRN_UTSNAME_MACHINE);
    // copy structure, no endian worry here
    sys_args_setdata (uptr, kbk, sizeof (utk), mem);
    // set syscall result
    sys_args_setretn (OCTA_0, rbk);
  }
}
