// ---------------------------------------------------------------------------
// - KrnMmap.cpp                                                             -
// - iato:krn library - mmap related system call implementation              -
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

#include "KrnMmap.hpp"
#include "KrnMmap.hxx"

namespace iato {

  // this procedure computes the local protection from the mmap protection
  static t_byte mmap_prot (const t_octa prot) {
    t_byte result = Memory::PROT_NO;
    if ((prot & KRN_MMAP_READ)  == KRN_MMAP_READ)  result |= Memory::PROT_RD;
    if ((prot & KRN_MMAP_WRITE) == KRN_MMAP_WRITE) result |= Memory::PROT_WR;
    if ((prot & KRN_MMAP_EXEC)  == KRN_MMAP_EXEC)  result |= Memory::PROT_EX;
    return result;
  }

  // this procedure computes the segment type
  static ElfSegment::t_stype mmap_type (const t_octa type) {
    ElfSegment::t_stype result = ElfSegment::PRIVATE;
    if ((type & KRN_MMAP_PRIVATE) == KRN_MMAP_PRIVATE) 
      result = ElfSegment::PRIVATE;
    if ((type & KRN_MMAP_SHARED) == KRN_MMAP_SHARED) 
      result = ElfSegment::SHARED;
    return result;
  }

  // mmap system call
  
  void krn_mmap (Rse* rse, Register* rbk, ElfExec* mem) {
    // get mmap parameters
    t_long size = sys_args_getoval (1, rse, rbk);
    t_octa prot = sys_args_getoval (2, rse, rbk);
    t_octa flag = sys_args_getoval (3, rse, rbk);
    // get the elf mappable memory
    ElfMap* mapm = mem->getmapm ();
    // prepare for map failed
    t_octa result = (t_octa) -1;
    // compute segment protection
    t_byte sprt = mmap_prot (prot);
    // compute segment type
    ElfSegment::t_stype stype = mmap_type (flag);
    // check for anonymous map
    if ((flag & KRN_MMAP_ANON) == KRN_MMAP_ANON) {
      result = mapm->mmap (size, sprt, stype);
    } else {
      assert (false);
    }
    // set syscall result
    if (result == (t_octa) -1) {
      sys_args_seterrn (result, OCTA_0, rbk);
    } else {
      sys_args_setretn (result, rbk);
    }
  }

  // munmap system call
  
  void krn_munmap (Rse* rse, Register* rbk, ElfExec* mem) {
    // get mmap parameters
    t_octa addr = sys_args_getoval (0, rse, rbk);
    t_long size = sys_args_getoval (1, rse, rbk);
    // get the elf mappable memory
    ElfMap* mapm = mem->getmapm ();
    // unmap the memory
    bool status = mapm->munmap (addr, size);
    // set syscall result
    if (status == false) {
      sys_args_seterrn ((t_octa) -1, KRN_EINVAL, rbk);
    } else {
      sys_args_setretn (0, rbk);
    }
  }
}
