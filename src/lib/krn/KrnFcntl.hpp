// ---------------------------------------------------------------------------
// - KrnFcntl.hpp                                                            -
// - iato:krn library - fcntl related system call definition                 -
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

#ifndef  IATO_KRNFCNTL_HPP
#define  IATO_KRNFCNTL_HPP

#ifndef  IATO_SYSARGS_HPP
#include "SysArgs.hpp"
#endif

namespace iato {

  /// fcntl system call
  void krn_fcntl  (Rse* rse, Register* rbk);
  // open system call
  void krn_open   (Rse* rse, Register* rbk, ElfExec* mem);
  // read system call
  void krn_read   (Rse* rse, Register* rbk, ElfExec* mem);
  /// write system call
  void krn_write  (Rse* rse, Register* rbk, ElfExec* mem);
  /// close system call
  void krn_close  (Rse* rse, Register* rbk);
  /// lseek system call
  void krn_lseek  (Rse* rse, Register* rbk);
  /// stat system call
  void krn_stat   (Rse* rse, Register* rbk, ElfExec* mem);
  /// lstat system call
  void krn_lstat  (Rse* rse, Register* rbk, ElfExec* mem);
  /// fstat system call
  void krn_fstat  (Rse* rse, Register* rbk, ElfExec* mem);
  /// unlink system call
  void krn_unlink (Rse* rse, Register* rbk, ElfExec* mem);
  /// link system call
  void krn_link   (Rse* rse, Register* rbk, ElfExec* mem);
  /// rename system call
  void krn_rename (Rse* rse, Register* rbk, ElfExec* mem);
  /// mkdir system call
  void krn_mkdir  (Rse* rse, Register* rbk, ElfExec* mem);
  /// rmdir system call
  void krn_rmdir  (Rse* rse, Register* rbk, ElfExec* mem);
  /// access system call
  void krn_access (Rse* rse, Register* rbk, ElfExec* mem);
}

#endif
