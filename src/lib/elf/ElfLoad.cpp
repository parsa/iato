// ---------------------------------------------------------------------------
// - ElfLoad.cpp                                                             -
// - iato:elf library - elf load segment array class implementation          -
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

#include "ElfLoad.hpp"
#include "Exception.hpp"

namespace iato {

  // create a default load memory

  ElfLoad::ElfLoad (void) {
    reset ();
  }

  // add a memory if and only if it is a load segment

  void ElfLoad::add (Memory* mem) {
    if (!mem) return;
    ElfSegment* seg = dynamic_cast <ElfSegment*> (mem);
    addseg (seg);
  }


  // add a segment to this elf load array

  void ElfLoad::addseg (ElfSegment* seg) {
    if (!seg) return;
    ElfMemory::add (seg);
  }

  // return a segment by index

  ElfSegment* ElfLoad::getseg (const long index) const {
    ElfSegment* seg = dynamic_cast <ElfSegment*> (getmem (index));
    return seg;
  }
}
