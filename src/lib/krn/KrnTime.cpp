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

  namespace {
    // ensure we can read a timespec-sized buffer from guest memory
    static bool touch_timespec (const t_octa addr, ElfExec* mem) {
      if ((!mem) || (addr == OCTA_0)) return false;
      for (long i = 0; i < (long) sizeof (krn_timespec); i++) {
	mem->readbyte (addr + i);
      }
      return true;
    }

    // write a host timespec into guest memory
    static void store_timespec (const t_octa addr,
				const t_octa sec,
				const t_octa nsec,
				ElfExec* mem) {
      if ((!mem) || (addr == OCTA_0)) return;
      union {
	krn_timespec kts;
	t_byte       bts[sizeof (krn_timespec)];
      };
      for (long i = 0; i < (long) sizeof (krn_timespec); i++) bts[i] = BYTE_0;
      kts.d_tv_sec  = lfixocta (sec);
      kts.d_tv_nsec = lfixocta (nsec);
      sys_args_setdata (addr, bts, sizeof (kts), mem);
    }

    // shared helper to emulate a completed sleep
    static bool emulate_sleep (const t_octa reqp, const t_octa remp,
			       ElfExec* mem) {
      if (!touch_timespec (reqp, mem)) return false;
      if (remp != OCTA_0) {
	store_timespec (remp, OCTA_0, OCTA_0, mem);
      }
      return true;
    }
  }

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

  void krn_clock_gettime (Rse* rse, Register* rbk, ElfExec* mem) {
    t_octa clkid = sys_args_getoval (0, rse, rbk);
    t_octa tsptr = sys_args_getoval (1, rse, rbk);
    if ((tsptr == OCTA_0) || (mem == nullptr)) {
      sys_args_seterrn ((t_octa) -1, EFAULT, rbk);
      return;
    }
    timespec ts;
    int result = clock_gettime (static_cast<clockid_t>(clkid), &ts);
    if (result != 0) {
      sys_args_seterrn (result, errno, rbk);
      return;
    }
    store_timespec (tsptr,
		    static_cast<t_octa>(ts.tv_sec),
		    static_cast<t_octa>(ts.tv_nsec),
		    mem);
    sys_args_setretn (OCTA_0, rbk);
  }

  void krn_nanosleep (Rse* rse, Register* rbk, ElfExec* mem) {
    t_octa reqp = sys_args_getoval (0, rse, rbk);
    t_octa remp = sys_args_getoval (1, rse, rbk);
    if (!emulate_sleep (reqp, remp, mem)) {
      sys_args_seterrn ((t_octa) -1, EFAULT, rbk);
      return;
    }
    sys_args_setretn (OCTA_0, rbk);
  }

  void krn_clock_nanosleep (Rse* rse, Register* rbk, ElfExec* mem) {
    // arguments: clockid, flags, req, rem
    sys_args_getoval (0, rse, rbk); // clockid (ignored)
    sys_args_getoval (1, rse, rbk); // flags   (ignored)
    t_octa reqp = sys_args_getoval (2, rse, rbk);
    t_octa remp = sys_args_getoval (3, rse, rbk);
    if (!emulate_sleep (reqp, remp, mem)) {
      sys_args_seterrn ((t_octa) -1, EFAULT, rbk);
      return;
    }
    sys_args_setretn (OCTA_0, rbk);
  }
 }
