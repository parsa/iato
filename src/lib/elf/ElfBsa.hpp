// ---------------------------------------------------------------------------
// - ElfBsa.hpp                                                              -
// - iato:elf library - elf backing store area class definition              -
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

#ifndef  IATO_ELFBSA_HPP
#define  IATO_ELFBSA_HPP

#ifndef  IATO_SEGMENT_HPP
#include "Segment.hpp"
#endif

#ifndef  IATO_ELFKERNEL_HPP
#include "ElfKernel.hpp"
#endif

namespace iato {

  /// The ElfBsa class is a simple segment that represents the backing
  /// store area. The class is nothing more than a constructor. The good
  /// news is that it can be constructed from the ElfKernel object.

  class ElfBsa : public Segment {
  public:
    /// create a new default bsa
    ElfBsa (void);

    /// create a new bsa by context
    /// @param ekp the elf kernel parameters
    ElfBsa (ElfKernel* ekp);

  private:
    // make the copy constructor private
    ElfBsa (const ElfBsa&);
    // make the assignment operator private
    ElfBsa& operator = (const ElfBsa&);
  };
}

#endif
