// ---------------------------------------------------------------------------
// - ElfEnvp.cpp                                                             -
// - iato:elf library - elf executable environment class implementation      -
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

#include "ElfEnvp.hpp"
#include "Exception.hpp"

namespace iato {

  // create a default elf environment

  ElfEnvp::ElfEnvp (void) {
    add ("LANG=C");
  }

  // create an argument block by program argument

  ElfEnvp::ElfEnvp (const long argc, const char** argv) {
    // check for good arguments
    if ((argc <= 0) || (!argv)) {
      throw Exception ("elf-error", "invalid elf program arguments");
    }
    // get first argument pointer
    const char** envp = &argv[argc+1];
    while (*envp != 0) {
      if (*envp++) add (*envp);
    }
  }
}
