// ---------------------------------------------------------------------------
// - ElfEnvp.hpp                                                             -
// - iato:elf library - elf executable environment class definition          -
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

#ifndef  IATO_ELFENVP_HPP
#define  IATO_ELFENVP_HPP

#ifndef  IATO_ELFTABLE_HPP
#include "ElfTable.hpp"
#endif

namespace iato {
  using namespace std;

  /// The ElfEnvp class is a special class designed to build the memory
  /// image of the program environment. For a given program, a character
  /// block is built with the total size of the environment. This class
  /// is similar to the ElfArgs that manages the program arguments. As
  /// part of the ABI, the environment array is placed above the arguement
  /// array. That means that the first pointer is visible at argv[argc+1].
  /// Once again, this class is ABI sensitive. You have been warned.

  class ElfEnvp : public ElfTable {
  public:
    /// create a default elf environment
    ElfEnvp (void);

    /// create an argument block
    /// @param argc the number of arguments
    /// @param argv the argument vector
    ElfEnvp (const long argc, const char** argv);

  private:
    // make the copy constructor private
    ElfEnvp (const ElfEnvp&);
    // make the assignment operator private
    ElfEnvp& operator = (const ElfEnvp&);
  };
}

#endif
