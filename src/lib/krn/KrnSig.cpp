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

  namespace {
    const long KRN_SIG_MAX = 64;
    struct SigActionSlot {
      bool          d_valid;
      KrnSigAction  d_action;
      SigActionSlot (void) : d_valid (false), d_action () {}
    };
    static SigActionSlot g_sigactions[KRN_SIG_MAX + 1];
    static t_octa g_sigmask = 0;

    static inline size_t sanitize_sigset_size (const size_t sz) {
      if (sz == 0) return sizeof (t_octa);
      return (sz > sizeof (t_octa)) ? sizeof (t_octa) : sz;
    }

    static t_octa read_sigset (Memory* mem, const t_octa addr,
			       const size_t sz) {
      if ((!mem) || (addr == OCTA_0)) return 0;
      const size_t rsize = sanitize_sigset_size (sz);
      t_octa value = 0;
      for (size_t i = 0; i < rsize; ++i) {
	t_byte byte = mem->readbyte (addr + i);
	value |= static_cast<t_octa>(byte) << (i * 8);
      }
      return value;
    }

    static void write_sigset (Memory* mem, const t_octa addr,
			      const size_t sz, const t_octa value) {
      if ((!mem) || (addr == OCTA_0)) return;
      union {
	t_octa oval;
	t_byte bval[sizeof (t_octa)];
      } data;
      data.oval = value;
      const size_t wsize = sanitize_sigset_size (sz);
      sys_args_setdata (addr, data.bval, wsize, mem);
    }

    static void read_sigaction (Memory* mem, const t_octa addr,
				const size_t sigset_size,
				KrnSigAction& dst) {
      if ((!mem) || (addr == OCTA_0)) {
	dst = KrnSigAction ();
	return;
      }
      const t_octa step = sizeof (t_octa);
      dst.d_handler  = mem->readocta (addr);
      dst.d_flags    = mem->readocta (addr + step);
      dst.d_restorer = mem->readocta (addr + (step * 2));
      dst.d_mask     = read_sigset (mem, addr + (step * 3), sigset_size);
    }

    static void write_sigaction (Memory* mem, const t_octa addr,
				 const size_t sigset_size,
				 const KrnSigAction& src) {
      if ((!mem) || (addr == OCTA_0)) return;
      const t_octa step = sizeof (t_octa);
      mem->writeocta (addr, src.d_handler);
      mem->writeocta (addr + step, src.d_flags);
      mem->writeocta (addr + (step * 2), src.d_restorer);
      write_sigset (mem, addr + (step * 3), sigset_size, src.d_mask);
    }
  }

  void krn_sigaction (Rse* rse, Register* rbk, Memory* mem) {
    long signum = (long) sys_args_getoval (0, rse, rbk);
    t_octa actp = sys_args_getoval (1, rse, rbk);
    t_octa oact = sys_args_getoval (2, rse, rbk);
    size_t sigset_size = sanitize_sigset_size (
	static_cast<size_t>(sys_args_getoval (3, rse, rbk)));
    if ((signum <= 0) || (signum > KRN_SIG_MAX)) {
      sys_args_seterrn ((t_octa) -1, EINVAL, rbk);
      return;
    }
    if (oact != OCTA_0) {
      KrnSigAction installed =
	g_sigactions[signum].d_valid ?
	g_sigactions[signum].d_action :
	KrnSigAction ();
      write_sigaction (mem, oact, sigset_size, installed);
    }
    if (actp != OCTA_0) {
      KrnSigAction incoming;
      read_sigaction (mem, actp, sigset_size, incoming);
      g_sigactions[signum].d_action = incoming;
      g_sigactions[signum].d_valid  = true;
    }
    sys_args_setretn (OCTA_0, rbk);
  }

  void krn_sigprocmask (Rse* rse, Register* rbk, Memory* mem) {
    long how = (long) sys_args_getoval (0, rse, rbk);
    t_octa setp = sys_args_getoval (1, rse, rbk);
    t_octa oldp = sys_args_getoval (2, rse, rbk);
    size_t sigset_size = sanitize_sigset_size (
	static_cast<size_t>(sys_args_getoval (3, rse, rbk)));
    if (oldp != OCTA_0) {
      write_sigset (mem, oldp, sigset_size, g_sigmask);
    }
    if (setp != OCTA_0) {
      t_octa newmask = read_sigset (mem, setp, sigset_size);
      switch (map_sigprocmask_request (how)) {
      case KRN_SIG_BLOCK:
	g_sigmask |= newmask;
	break;
      case KRN_SIG_UNBLOCK:
	g_sigmask &= ~newmask;
	break;
      case KRN_SIG_SETMASK:
	g_sigmask = newmask;
	break;
      default:
	sys_args_seterrn ((t_octa) -1, EINVAL, rbk);
	return;
      }
    }
    sys_args_setretn (OCTA_0, rbk);
  }
  
  // kill system call

  void krn_kill (Rse* rse, Register* rbk, Memory* mem) {
    throw Exception ("kill-error", "unsupported kill call");
  }
}
