// ---------------------------------------------------------------------------
// - SysArgs.hpp                                                             -
// - iato:krn library - system call argument function definition             -
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

#ifndef  IATO_SYSARGS_HPP
#define  IATO_SYSARGS_HPP

#ifndef  IATO_RSE_HPP
#include "Rse.hpp"
#endif

#ifndef  IATO_ELFEXEC_HPP
#include "ElfExec.hpp"
#endif

#ifndef  IATO_REGISTER_HPP
#include "Register.hpp"
#endif

namespace iato {

  /// This file contains the system call argument mapping procedure.
  /// The system call arguments are placed in the stacked registers. The
  /// rse is used to remap the logical register number to the physical
  /// register number. The argument index starts at 0. The logical register
  /// number starts at gr[32]. The result is placed in register gr[10]. The
  /// system call number is stored in gr[15].

  /// @return the system call number
  long sys_args_getnr (Register* rbk);

  /// get a string from a memory
  /// @param addr the base string address
  /// @param mem the memory to use
  t_byte* sys_args_getstr (const t_octa addr, Memory* mem);

  /// set a data buffer in memory
  /// @param base the base pointer address
  /// @param buf the buffer to copy
  /// @param size the buffer size
  /// @param mem  the meory where to write
  void sys_args_setdata (const t_octa base,const t_byte* buf,const long size,
			 Memory* mem);

  /// set the system call register result
  /// @param resl the result value
  /// @param rbk  the register bank
  void sys_args_setretn (const t_octa resl, Register* rbk);

  /// set the system call error result
  /// @param resl the result value
  /// @param errn the error value
  /// @param rbk  the register bank
  void sys_args_seterrn (const t_octa resl, const t_octa errn, Register* rbk);

  /// get an argument by index
  /// @param argi the argument index
  /// @param rse  the register stack engine
  /// @param rbk  the register bank
  t_octa sys_args_getoval (const long argi, Rse* rse, Register* rbk);

  /// compare two arguments by index
  /// @param argx the argument index
  /// @param argy the argument index
  /// @param rse  the register stack engine
  /// @param rbk  the register bank
  bool sys_args_compare (const long argx, const long argy, 
			 Rse* rse, Register* rbk);  
}

#endif
