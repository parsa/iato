// ---------------------------------------------------------------------------
// - Disass.hpp                                                              -
// - iato:iaos client - text disassemble class definition                    -
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

#ifndef  IATO_DISASS_HPP
#define  IATO_DISASS_HPP

#ifndef  IATO_ELFIMAGE_HPP
#include "ElfImage.hpp"
#endif

namespace iato {

  /// The Disass class is a simple class that operates with an elf text
  /// object. Its purpose is to disassemble bundles and instructions.

  class Disass {
  private:
    /// the elf image
    ElfImage* p_elf;
    /// the address disassemble
    bool d_dadr;
    /// the bundle disassemble
    bool d_dbnd;
    /// the instruction disassemble
    bool d_dins;
    /// the force disassembly flag
    bool d_dfrc;

  public:
    /// create a new disassemble object with an image
    /// @param elf the elf image
    Disass (ElfImage* elf);

    /// set the report flags
    /// @param dadr the disassemble address flag
    /// @param dbnd the disassemble bundle flag
    /// @param dins the disassemble instruction flag
    /// @param dfrc the force disassembly flag
    void setflag (const bool dadr, const bool dbnd, const bool dins, 
		  const bool dfrc);

    /// report this class
    void report (void) const;

  private:
    // make the copy constructor private
    Disass (const Disass&);
    // make the assignment operator private
    Disass& operator = (const Disass&);
  };
}

#endif

