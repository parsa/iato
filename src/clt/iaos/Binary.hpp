// ---------------------------------------------------------------------------
// - Binary.hpp                                                              -
// - iato:iaos client - elf binary statistic class definition                -
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

#ifndef  IATO_ELFBINARY_HPP
#define  IATO_ELFBINARY_HPP

#ifndef  IATO_ELFIMAGE_HPP
#include "ElfImage.hpp"
#endif

namespace iato {

  /// The Binary class is a simple class that compute statistics from
  /// an elf image. The statistics are related only to the elf file
  /// format. Information include header info, file type, number of
  /// section and more. The behavior of the report method is controlled
  /// by various flags

  class Binary {
  private:
    /// the elf image
    ElfImage* p_elf;
    /// the header flag
    bool d_ehdr;
    /// the exec flag
    bool d_eexc;
    /// the text flag
    bool d_etxt;

  public:
    /// create a new elf stat with an image
    /// @param elf the elf image
    Binary (ElfImage* elf);

    /// set the report flags
    /// @param ehdr the header flag
    /// @param eexc the exec flag
    /// @param etxt the text flag
    void setflag (const bool ehdr, const bool eexc, const bool etxt);

    /// report this class
    void report (void) const;

  private:
    // make the copy constructor private
    Binary (const Binary&);
    // make the assignment operator private
    Binary& operator = (const Binary&);
  };
}

#endif

