// ---------------------------------------------------------------------------
// - ElfLoad.hpp                                                             -
// - iato:elf library - elf load segment array class definition              -
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

#ifndef  IATO_ELFLOAD_HPP
#define  IATO_ELFLOAD_HPP

#ifndef  IATO_ELFSEGMENT_HPP
#include "ElfSegment.hpp"
#endif

#ifndef  IATO_ELFMEMORY_HPP
#include "ElfMemory.hpp"
#endif

namespace iato {

  /// The ElfLoad class is a specialized elf memory object that is built from 
  /// an elf file. The class acts as an array of elf segments built with load 
  /// flag. This class is similar to the elf text section array. Note that
  /// globally, the load array has all protections enabled.

  class ElfLoad : public ElfMemory {
  public:
    /// create a default load memory
    ElfLoad (void);

    /// add a memory only if it is an elf segment
    /// @param mem the memory to add
    void add (Memory* mem);

    /// add a segment to this memory
    /// @param seg  the segment to add
    virtual void addseg (ElfSegment* seg);

    /// @return a segment by index
    ElfSegment* getseg (const long index) const;

  private:
    // make the copy constructor private
    ElfLoad (const ElfLoad&);
    // make the assignment operator private
    ElfLoad& operator = (const ElfLoad&);
  };
}

#endif
