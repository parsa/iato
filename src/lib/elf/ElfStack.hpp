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
