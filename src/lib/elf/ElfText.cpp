// ---------------------------------------------------------------------------
// - ElfText.cpp                                                             -
// - iato:elf library - elf text section array class implementation          -
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

#include "ElfText.hpp"
#include "Exception.hpp"

namespace iato {

  // create a default text memory

  ElfText::ElfText (void) {
    reset ();
  }

  // add a memory if and only if it is an elf section

  void ElfText::add (Memory* mem) {
    if (!mem) return;
    ElfSection* scn = dynamic_cast <ElfSection*> (mem);
    addscn (scn);
  }


  // add a section to this elf text array

  void ElfText::addscn (ElfSection* scn) {
    if (!scn) return;
    ElfMemory::add (scn);
  }

  // return a section by index

  ElfSection* ElfText::getscn (const long index) const {
    ElfSection* scn = dynamic_cast <ElfSection*> (getmem (index));
    return scn;
  }
}
