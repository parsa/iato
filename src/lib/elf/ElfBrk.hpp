// ---------------------------------------------------------------------------
// - ElfBrk.hpp                                                              -
// - iato:elf library - elf breakable load segment class definition          -
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

#ifndef  IATO_ELFBRK_HPP
#define  IATO_ELFBRK_HPP

#ifndef  IATO_ELFLOAD_HPP
#include "ElfLoad.hpp"
#endif

#ifndef  IATO_ELFKERNEL_HPP
#include "ElfKernel.hpp"
#endif

namespace iato {

  /// The ElfBrk class is a specialized elf load object that maintain a break
  /// limit for further memory allocation. Each time a new segment is added
  /// the break limit is changed, as to reflect the change in memory access.
  /// Increasing the break limit is equivalent to increase the segment size
  /// of the containing segment. Decreasing the break limit can result in
  /// segment removal.

  class ElfBrk : public ElfLoad {
  private:
    /// the page size
    t_octa d_psize;
    /// the memory mode
    bool   d_mode;
    /// the break limit address
    t_octa d_brkta;

  public:
    /// create a new breakable memory
    ElfBrk (void);
 
    /// create a new breakable memory by parameters
    /// @param ekp the elf kernel parameters
    ElfBrk (ElfKernel* ekp);
    
    /// add a segment to this memory
    /// @param seg  the segment to add
    void addseg (ElfSegment* seg);

    /// change the break limit address
    /// @param addr the break limit to change
    void setbrkta (const t_octa addr);

    /// @return the break limit address
    t_octa getbrkta (void) const;

  private:
    // make the copy constructor private
    ElfBrk (const ElfBrk&);
    // make the assignment operator private
    ElfBrk& operator = (const ElfBrk&);
  };
}

#endif
