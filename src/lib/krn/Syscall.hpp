// ---------------------------------------------------------------------------
// - Syscall.hpp                                                             -
// - iato:krn library - system call plugin class definition                  -
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

#ifndef  IATO_SYSCALL_HPP
#define  IATO_SYSCALL_HPP

#ifndef  IATO_RSE_HPP
#include "Rse.hpp"
#endif

#ifndef  IATO_PLUGIN_HPP
#include "Plugin.hpp"
#endif

#ifndef  IATO_ELFEXEC_HPP
#include "ElfExec.hpp"
#endif

#ifndef  IATO_REGISTER_HPP
#include "Register.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Syscall class is a special plugin designed to handle system calls.
  /// The plugin is constructed with the rse, the register bank and the
  /// system memory. By convention, the system call number is stored in gr[15]
  /// and the system call parameters are in the standard registers starting
  /// at gr[32]. The rse is part of this plugin in order to find the right
  /// register for a given argument. Upon completion, the return value
  /// is store in gr[10]. Generally a -1 value indicates an error. The 
  /// plugin can directly work with virtual interrupt. In that case, the
  /// system expect that the immediate break value is 0x10000 as defined
  /// by the IA64 ISA. Other value rethrow the interrupt.

  class Syscall : public Plugin {
  private:
    /// the register stack engine
    Rse*      p_rse;
    /// the register bank
    Register* p_rbk;
    /// the system memory
    ElfExec*  p_mem;
    
  public:
    /// create a default syscall
    Syscall (void);

    /// create a syscall plugin with a memory
    /// @param mem the memory image
    Syscall (ElfExec* mem);

    /// create a plugin with a rse, register bank and memory
    /// @param rse the register stack engine
    /// @param rbk the register bank
    /// @param mem the executable memory
    Syscall (Rse* rse, Register* rbk, ElfExec* mem);

    /// set the system call rse object
    /// @param rse the rse object
    void setrse (Rse* rse);

    /// set the system call register bank object
    /// @param rbk the register bank object
    void setrbk (Register* rbk);

    /// set the system call memory object
    /// @param mem the memory object
    void setmem (ElfExec* mem);

    /// apply this syscall plugin
    void apply (void);

    /// apply this syscall plugin with an interrupt
    /// @param vi the virtual interrupt
    void apply (const Interrupt& vi);

  private:
    // make the copy constructor private
    Syscall (const Syscall &);
    // make the assignment operator private
    Syscall& operator = (const Syscall&);
  };
}

#endif
