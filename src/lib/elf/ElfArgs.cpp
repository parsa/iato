// ---------------------------------------------------------------------------
// - ElfArgs.cpp                                                             -
// - iato:elf library - elf executable arguments class implementation        -
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

#include "ElfArgs.hpp"
#include "Exception.hpp"

namespace iato {
  using namespace std;

  // create an argument block by name only

  ElfArgs::ElfArgs (const string& name) {
    add (d_name = name);
  }
  
  // create an argument block by program argument

  ElfArgs::ElfArgs (const long argc, const char** argv) {
    // check for good arguments
    if ((argc <= 0) || (!argv)) {
      throw Exception ("elf-error", "invalid elf program arguments");
    }
    // set name and character table
    d_name = argv[0];
    for (long i = 0; i < argc; i++) add (argv[i]);
  }
 
  // create an argument block by name and arguments

  ElfArgs::ElfArgs (const string& name, const vector<string>& argv) {
    // save program name
    add (d_name = name);
    // save arguments
    long size = argv.size ();
    for (long i = 0; i < size; i++) add (argv[i]);
  }

  // return the program name

  string ElfArgs::getname (void) const {
    return d_name;
  }
}
