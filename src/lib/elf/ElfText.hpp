// ---------------------------------------------------------------------------
// - ElfText.hpp                                                             -
// - iato:elf library - elf text section array class definition              -
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

#ifndef  IATO_ELFTEXT_HPP
#define  IATO_ELFTEXT_HPP

#ifndef  IATO_ELFSECTION_HPP
#include "ElfSection.hpp"
#endif

#ifndef  IATO_ELFMEMORY_HPP
#include "ElfMemory.hpp"
#endif

namespace iato {

  /// The ElfText class is a specialized elf memory object that is built from 
  /// an elf file. The class acts as an array of elf sections built with text 
  /// section. The associated section are those that have the text associated
  /// flag sets. Note that such section are built generally with read/execute 
  /// flag only.

  class ElfText : public ElfMemory {
  public:
    /// create a default memory
    ElfText (void);

    /// add a memory only if is an elf section
    /// @param mem the memory to add
    void add (Memory* mem);

    /// add a section to this memory
    /// @param scn  the section to add
    virtual void addscn (ElfSection* scn);

    /// @return a section by index
    ElfSection* getscn (const long index) const;

  private:
    // make the copy constructor private
    ElfText (const ElfText&);
    // make the assignment operator private
    ElfText& operator = (const ElfText&);
  };
}

#endif
