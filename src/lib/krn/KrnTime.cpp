// ---------------------------------------------------------------------------
// - KrnTime.cpp                                                             -
// - iato:krn library - time related system call implementation              -
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

#include "Bits.hpp"
#include "KrnTime.hpp"
#include "KrnTime.hxx"

namespace iato {

  void krn_gettimeofday (Rse* rse, Register* rbk, ElfExec* mem) {
    // get the struct timeval pointer argument
    t_octa tvadr = sys_args_getoval (0, rse, rbk);
    // process system call
    timeval tv;
    t_long result = gettimeofday (&tv, NULL);
    // process system call
    if (result != -1) {
      sys_args_setretn (result, rbk);
    } else {
      sys_args_seterrn (result, errno, rbk);
    }
    // the kernel argument structure
    union {
      krn_timeval  ktv;
      t_byte       btv[sizeof(krn_timeval)];
    };
    // zero the whole structure
    for (long i = 0; i < (long) sizeof(krn_timeval); i++) btv[i] = BYTE_0;
    // fill in the structure
    ktv.d_tv_sec  = lfixocta (tv.tv_sec);
    ktv.d_tv_usec = lfixocta (tv.tv_usec);
    sys_args_setdata (tvadr, btv, sizeof (ktv), mem);
  }

  void krn_times (Rse* rse, Register* rbk, ElfExec* mem) {
    // get the structure tms pointer argument
    t_octa bufadr = sys_args_getoval (0, rse, rbk);
    // process system call
    tms buf;
    clock_t result = times (&buf);
    // process system call
    if (result != -1) {
      sys_args_setretn (result, rbk);
    } else {
      sys_args_seterrn (result, errno, rbk);
    }
    // the kernel argument structure
    union {
      krn_tms      kbuf;
      t_byte       bbuf[sizeof(krn_tms)];
    };
    // zero the whole structure
    for (long i = 0; i < (long) sizeof(krn_tms); i++) bbuf[i] = BYTE_0;
    // fill in the structure
    kbuf.d_tms_utime  = lfixocta (buf.tms_utime);
    kbuf.d_tms_stime  = lfixocta (buf.tms_stime);
    kbuf.d_tms_cutime = lfixocta (buf.tms_cutime);
    kbuf.d_tms_cstime = lfixocta (buf.tms_cstime);
    sys_args_setdata (bufadr, bbuf, sizeof (kbuf), mem);
  }
 }
