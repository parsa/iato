// ---------------------------------------------------------------------------
// - ElfStack.hpp                                                            -
// - iato:elf library - elf stack class definition                           -
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

#ifndef  IATO_ELFSTACK_HPP
#define  IATO_ELFSTACK_HPP

#include <string>

#ifndef  IATO_SEGMENT_HPP
#include "Segment.hpp"
#endif

#ifndef  IATO_ELFKERNEL_HPP
#include "ElfKernel.hpp"
#endif

namespace iato {

  /// The ElfStack class is a simple segment that represents the process
  /// stack. The stack can be initialized by default or by context.
  /// The  ABI specifies that the stack grows towards lower address. 
  /// Initially, the stack is created with a certain default size. After 
  /// some elf initialization, the initialized stack pointer indicates the 
  /// current stack pointer. Normally it should not be changed after this. 
  /// The initialized stack pointer can be set to initialize the processor 
  /// with the current stack pointer (sp). The stack is initialized with the 
  /// read/write/execute permission.

  class ElfStack : public Segment {
  private:
    /// the initialized sp
    t_octa d_stkva;
    /// kernel-provided top-of-stack before layout
    t_octa d_initva;
    /// address of argc/argv block (for r11)
    t_octa d_argva;
    /// base address of the TLS/TCB blob
    t_octa d_tlsva;
    /// reserved TLS size
    long   d_tlssz;
    /// argc value installed on the stack
    long   d_argc;
    /// pointer to argv vector on the stack
    t_octa d_argvp;
    /// pointer to envp vector on the stack
    t_octa d_envpp;
    /// pointer to synthesized boot param block
    t_octa d_bootva;
    /// pointer to synthesized command-line string
    t_octa d_cmdva;
    /// reserved space consumed by loader data
    long   d_loader_off;
    
    /// reserve/zero the TLS region and adjust the initial SP
    void inittls (void);
    /// reserve loader-only space near the stack base
    t_octa allocloader (const long size, const long align);

  public:
    /// create a new default stack
    ElfStack (void);

    /// create a new stack by parameters
    /// @param ekp the elf kernel parameters
    ElfStack (ElfKernel* ekp);

    /// set the elf stack address
    void setstkva (const t_octa addr);

    /// @return the elf stack address
    t_octa getstkva (void) const;

    /// set the argument vector base pointer (argc slot)
    void setargva (const t_octa addr);

    /// @return the argument vector base pointer
    t_octa getargva (void) const;

    /// @return the TLS base pointer reserved for r13
    t_octa gettlsva (void) const;

    /// record the argc value
    void setargc (const long argc);

    /// @return the argc value
    long getargc (void) const;

    /// record the argv vector pointer
    void setargv (const t_octa addr);

    /// @return the argv vector pointer
    t_octa getargv (void) const;

    /// record the envp vector pointer
    void setenvp (const t_octa addr);

    /// @return the envp vector pointer
    t_octa getenvp (void) const;

    /// ensure the ia64 boot param block exists and return its address
    t_octa installbootparam (const std::string& cmdline);

    /// @return the ia64 boot param block address
    t_octa getbootparam (void) const;

    /// @return the kernel-provided initial stack address
    t_octa getinitva (void) const;

    /// @return the address where argc lives (`sp + VSTK_PPAD`)
    t_octa getstackend (void) const;

    /// print the stack arguments after initialization
    void pargs (void) const;

  private:
    // make the copy constructor private
    ElfStack (const ElfStack&);
    // make the assignment operator private
    ElfStack& operator = (const ElfStack&);
  };
}

#endif
