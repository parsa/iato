// ---------------------------------------------------------------------------
// - ElfSegment.hpp                                                          -
// - iato:elf library - elf segment class definition                         -
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

#ifndef  IATO_ELFSEGMENT_HPP
#define  IATO_ELFSEGMENT_HPP

#ifndef  IATO_SEGMENT_HPP
#include "Segment.hpp"
#endif

namespace iato {
  using namespace std;

  /// The ElfSegment class is a simple class that holds the data associated
  /// with a particular elf segment. The class cannot be constructed directly.
  /// Unlike a section, a segment does not have a name. Block segment are
  /// built from the elf file, from loadable segments.
  
  class ElfSegment : public Segment {
  public:
    /// the segment type
    enum t_stype {PRIVATE, SHARED};

  private:
    /// the segment type
    t_stype d_type;

  public:
    /// create an elf segment by base and size
    /// @param size the segment size
    /// @param base the base address
    ElfSegment (const t_long size, const t_octa base);

    /// create an elf segment by base and size and type
    /// @param size the segment size
    /// @param base the base address
    /// @param type the segment type
    ElfSegment (const t_long size, const t_octa base, t_stype type);

    /// create a new elf by segment and mode
    /// @param fid the elf file id
    /// @param seg the elf segment header
    /// @param mode the endian mode
    ElfSegment (const int fid, void* seg, const bool mode);

  private:
    // make the copy constructor private
    ElfSegment (const ElfSegment&);
    // make the assignment operator private
    ElfSegment& operator = (const ElfSegment&);
  };
}

#endif
