// ---------------------------------------------------------------------------
// - ElfExec.hpp                                                             -
// - iato:elf library - elf executable memory class definition               -
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

#ifndef  IATO_ELFEXEC_HPP
#define  IATO_ELFEXEC_HPP

#ifndef  IATO_ELFBRK_HPP
#include "ElfBrk.hpp"
#endif

#ifndef  IATO_ELFBSA_HPP
#include "ElfBsa.hpp"
#endif

#ifndef  IATO_ELFMAP_HPP
#include "ElfMap.hpp"
#endif

#ifndef  IATO_ELFSTACK_HPP
#include "ElfStack.hpp"
#endif

namespace iato {

  /// The ElfExec class is a memory object that represent the various
  /// portions of a memory process. It is built with a breakable 
  /// memory, a backing store area, a stack and a mappable area. To be
  /// effectively constructed, the class needs the ElfKernel object
  /// in order to grab the stack and backing store parameters. The breakable
  /// memory can be added at any time.

  class ElfExec : public ElfMemory {
  private:
    /// the elf kernel parameters
    ElfKernel* p_ekp;
    /// the breakable memory
    ElfBrk* p_brk;
    /// the stack
    ElfStack* p_stk;
    /// the backing store area
    ElfBsa* p_bsa;
    /// the mappable area
    ElfMap* p_map;

  public:
    /// create an elf executable memory
    /// @param ekp the elf kernel parameters
    ElfExec (ElfKernel* ekp);

    /// set the breakable memory
    /// @param brk the breakable memory to set
    void setbrkm (ElfBrk* brk);

    /// @return the elf breakable memory
    ElfBrk* getbrkm (void) const;
 
    /// @return the elf stack memory
    ElfStack* getstkm (void) const;

    /// @return the elf bsa memory
    ElfBsa* getbsam (void) const;

    /// @return the elf mappable memory
    ElfMap* getmapm (void) const;

  private:
    // make the copy constructor private
    ElfExec (const ElfExec&);
    // make the assignment operator private
    ElfExec& operator = (const ElfExec&);
  };
}

#endif
