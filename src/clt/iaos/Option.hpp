// ---------------------------------------------------------------------------
// - Option.hpp                                                              -
// - iato:iaos client - option class definition                              -
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

#ifndef  IATO_OPTION_HPP
#define  IATO_OPTION_HPP

#ifndef  IATO_TYPES_HPP
#include "Types.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Options class is a simple class that handle the "iaos" program
  /// options. The basic options are the program name to query the info,
  /// eventually a section name and some control flags.

  class Option {
  private:
    /// the program name
    string d_name;
    /// the elf header flag
    bool   d_ehdr;
    /// the elf exec flag
    bool   d_eexc;
    /// the elf text flag
    bool   d_etxt;
    /// the verify bundle flag
    bool   d_vbnd;
    /// the verify instruction flag
    bool   d_vins;
    /// the disassemble address
    bool   d_dadr;
    /// the disassemble bundle
    bool   d_dbnd;
    /// the disassemble instruction
    bool   d_dins;
    /// force disassembly in case of error
    bool   d_dfrc;
    /// the statistic flag
    bool   d_sflg;
    /// the bundle distribution flag
    bool   d_bflg;
    /// the instruction distribution flag
    bool   d_iflg;
    /// the nop flag
    bool   d_nflg;

  public:
    /// create a new option from the argument list
    /// @param argc the number of arguments
    /// @param argv the vector of arguments
    Option (int argc, char** argv);

    /// @return the program name
    string getname (void) const {
      return d_name;
    }

    /// @return the elf header flag
    bool getehdr (void) const {
      return d_ehdr;
    }

    /// @return the elf exec flag
    bool geteexc (void) const {
      return d_eexc;
    }

    /// @return the elf text flag
    bool getetxt (void) const {
      return d_etxt;
    }

    /// @return the verify bundle
    bool getvbnd (void) const {
      return d_vbnd;
    }

    /// @return the verify instruction
    bool getvins (void) const {
      return d_vins;
    }

    /// @return the disassemble address
    bool getdadr (void) const {
      return d_dadr;
    }

    /// @return the disassemble bundle
    bool getdbnd (void) const {
      return d_dbnd;
    }

    /// @return the disassemble instruction
    bool getdins (void) const {
      return d_dins;
    }

    /// @return the force disassemble flag
    bool getdfrc (void) const {
      return d_dfrc;
    }

    /// @return the stat flag
    bool getsflg (void) const {
      return d_sflg;
    }

    /// @return the bundle flag
    bool getbflg (void) const {
      return d_bflg;
    }

    /// @return the instruction flag
    bool getiflg (void) const {
      return d_iflg;
    }

    /// @return the nop flag
    bool getnflg (void) const {
      return d_nflg;
    }

  private:
    // make the copy constructor private
    Option (const Option&);
    // make the assignment operator private
    Option& operator = (const Option&);
  };
}

#endif

