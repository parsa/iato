// ---------------------------------------------------------------------------
// - ElfInterp.hpp                                                           -
// - iato:elf library - elf default ia64 interpreter class definition        -
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

#ifndef  IATO_ELFINTERP_HPP
#define  IATO_ELFINTERP_HPP

#ifndef  IATO_ELFARGS_HPP
#include "ElfArgs.hpp"
#endif

#ifndef  IATO_ELFENVP_HPP
#include "ElfEnvp.hpp"
#endif

#ifndef  IATO_ELFSTACK_HPP
#include "ElfStack.hpp"
#endif

#ifndef  IATO_ELFKERNEL_HPP
#include "ElfKernel.hpp"
#endif

namespace iato {
  using namespace std;

  /// The ElfInterp class is the elf interpreter. As definied by the ABI,
  /// the interpreter is responsible to setup the memory image so that
  /// a program can be executed. The memory image setup include, among
  /// other things, the preparation of the stack (with arguments), the
  /// got and plt setup, relocation and many other stuff. This implementation
  /// is the default one. It makes some assumption about the process image
  /// and mimic a standard IA64 ld.so implementation. Although this might
  /// vary from one implementation to another, it is unlikelly to change
  /// unless the libc is changed dramatically. Note that in real life, the
  /// elf interpreter is part of the libc. This means that program compiled
  /// with a special library might not work. Again, this is unlikelly to 
  /// happen.

  class ElfInterp {
  private:
    /// the emulated interpreter
    string d_interp;
    /// the kernel parameters
    ElfKernel* p_kernel;
    /// the program header address
    t_octa d_phdr;
    /// the program header size
    t_octa d_phent;
    /// the number of program segments
    t_octa d_phnum;

  public:
    /// create a default interpreter
    ElfInterp (void);

    /// create an interpreter by name
    /// @param name the emulated name
    ElfInterp (const string& name);

    /// destroy this interpreter
    virtual ~ElfInterp (void);

    /// bind the kernel parameters
    /// @param ekp the elf kernel parameters
    virtual void bind (ElfKernel* ekp);

    /// bind the arguments in a memory
    /// @param args the program argument
    /// @param envp the program environment
    /// @param stk  the elf stack
    virtual void bind (ElfArgs* args, ElfEnvp* envp, ElfStack* stk) const;

    /// @return true if the interpreter is supported
    static bool isvalid (const string& interp);

    /// set the program header info at once
    /// @param phdr  the program header address
    /// @param phent the program header size
    /// @param phnum the number of program header
    void setph (const t_octa phdr, const t_octa phent, const t_octa phnum);

  private:
    // make the copy constructor private
    ElfInterp (const ElfInterp&);
    // make the assignment operator private
    ElfInterp& operator = (const ElfInterp&);
  };
}

#endif
