// ---------------------------------------------------------------------------
// - Syscall.cpp                                                             -
// - iato:krn library - system call plugin class implementation              -
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

#include "KrnSig.hpp"
#include "Syscall.hpp"
#include "Syscall.hxx"
#include "KrnUser.hpp"
#include "KrnProc.hpp"
#include "KrnMmap.hpp"
#include "KrnTime.hpp"
#include "KrnIoctl.hpp"
#include "KrnFcntl.hpp"
#include "KrnUname.hpp"
#include "Exception.hpp"

namespace iato {
  using namespace std;

  // the system call resource name
  const string SCR = "SCR";
  // the break code for system call
  const t_octa SYS_BREAK = 0x0000000000100000ULL;

  // create a default system call plugin

  Syscall::Syscall (void) : Plugin (SCR) {
    p_rse = 0;
    p_rbk = 0;
    p_mem = 0;
  }

  // create a system call plugin with a memory

  Syscall::Syscall (ElfExec* mem) : Plugin (SCR) {
    p_rse = 0;
    p_rbk = 0;
    p_mem = mem;
  }

  // create a new plugin with a rse, register bank and system memory

  Syscall::Syscall (Rse* rse, Register* rbk, ElfExec* mem) : Plugin (SCR) {
    p_rse = rse;
    p_rbk = rbk;
    p_mem = mem;
  }

  // set the system call rse

  void Syscall::setrse (Rse* rse) {
    p_rse= rse;
  }

  // set the system call register bank

  void Syscall::setrbk (Register* rbk) {
    p_rbk = rbk;
  }

  // set the system call memory

  void Syscall::setmem (ElfExec* mem) {
    p_mem = mem;
  }

  // apply this plugin

  void Syscall::apply (void) {
    // get the syscall number
    long nr = sys_args_getnr (p_rbk);
    // dispatch system call
    switch (nr) {
    case NR_EXIT:
      krn_exit (p_rse, p_rbk);
      break;
    case NR_EXIT_GROUP:
      krn_exit_group (p_rse, p_rbk);
      break;
    case NR_BRK:
      krn_brk (p_rse, p_rbk, p_mem);
      break;
    case NR_MMAP:
      krn_mmap (p_rse, p_rbk, p_mem);
      break;
    case NR_MUNMAP:
      krn_munmap (p_rse, p_rbk, p_mem);
      break;
    case NR_IOCTL:
      krn_ioctl (p_rse, p_rbk, p_mem);
      break;
    case NR_FCNTL:
      krn_fcntl (p_rse, p_rbk);
      break;
    case NR_OPEN:
      krn_open (p_rse, p_rbk, p_mem);
      break;
    case NR_READ:
      krn_read (p_rse, p_rbk, p_mem);
      break;
    case NR_WRITE:
      krn_write (p_rse, p_rbk, p_mem);
      break;
    case NR_CLOSE:
      krn_close (p_rse, p_rbk);
      break;
    case NR_LSEEK:
      krn_lseek (p_rse, p_rbk);
      break;
    case NR_STAT:
      krn_stat (p_rse, p_rbk, p_mem);
      break;
    case NR_LSTAT:
      krn_lstat (p_rse, p_rbk, p_mem);
      break;
    case NR_FSTAT:
      krn_fstat (p_rse, p_rbk, p_mem);
      break;
    case NR_UNAME:
      krn_uname (p_rse, p_rbk, p_mem);
      break;
    case NR_GETUID:
      krn_getuid (p_rbk);
      break;
    case NR_GETEUID:
      krn_geteuid (p_rbk);
      break;
    case NR_GETGID:
      krn_getgid (p_rbk);
      break;
    case NR_GETEGID:
      krn_getegid (p_rbk);
      break;
    case NR_UNLINK:
      krn_unlink (p_rse, p_rbk, p_mem);
      break;
    case NR_LINK:
      krn_link (p_rse, p_rbk, p_mem);
      break;
    case NR_RENAME:
      krn_rename (p_rse, p_rbk, p_mem);
      break;
    case NR_MKDIR:
       krn_mkdir (p_rse, p_rbk, p_mem);
       break;
    case NR_RMDIR:
       krn_rmdir (p_rse, p_rbk, p_mem);
       break;
    case NR_KILL:
      krn_kill (p_rse, p_rbk, p_mem);
      break;
    case NR_RT_SIGACTION:
      krn_sigaction (p_rse, p_rbk, p_mem);
      break;
    case NR_RT_SIGPROCMASK:
      krn_sigprocmask (p_rse, p_rbk, p_mem);
      break;
    case NR_ACCESS:
      krn_access (p_rse, p_rbk, p_mem);
      break;
    case NR_GETTIMEOFDAY:
      krn_gettimeofday (p_rse, p_rbk, p_mem);
      break;
    case NR_TIMES:
      krn_times (p_rse, p_rbk, p_mem);
      break;
    case NR_GETPID:
      krn_getpid (p_rbk);
      break;
    case NR_GETPPID:
      krn_getppid (p_rbk);
      break;
    default:
      {
	ostringstream os;
	os << "unimplemented system call #=" << nr;
	throw Exception ("syscall-error", os.str ());
      }
      break;
    }
  }

  // apply this plugin with an interrupt

  void Syscall::apply (const Interrupt& vi) {
    if (vi.isvalid () == false) return;
    // make sure the interrupt code is valid
    assert (vi.getcode () == FAULT_IT_INST_BREAK);
    // get the break code
    if (vi.getiim () != SYS_BREAK) throw vi;
    // this is good - so process it
    apply ();
  }
}

