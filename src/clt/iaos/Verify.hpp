// ---------------------------------------------------------------------------
// - Verify.hpp                                                              -
// - iato:iaos client - text verify class definition                         -
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

#ifndef  IATO_VERIFY_HPP
#define  IATO_VERIFY_HPP

#ifndef  IATO_ELFIMAGE_HPP
#include "ElfImage.hpp"
#endif

namespace iato {

  /// The Verify class is a simple class that operates with an elf text
  /// object. It purpose is to verify that bundles and instructions can
  /// be decoded correctly. In case of error, the offending address is
  /// is reported.

  class Verify {
  private:
    /// the elf image
    ElfImage* p_elf;
    /// the bundle verify
    bool d_vbnd;
    // the instruction verify
    bool d_vins;

  public:
    /// create a new verify object with an image
    /// @param elf the elf image
    Verify (ElfImage* elf);

    /// set the report flags
    /// @param vbnd the verify bundle flag
    /// @param vins the verify instruction flag
    void setflag (const bool vbnd, const bool vins);

    /// verify this class
    void verify (void) const;

  private:
    // make the copy constructor private
    Verify (const Verify&);
    // make the assignment operator private
    Verify& operator = (const Verify&);
  };
}

#endif

