// ---------------------------------------------------------------------------
// - ElfArgs.hpp                                                             -
// - iato:elf library - elf executable arguments class definition            -
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

#ifndef  IATO_ELFARGS_HPP
#define  IATO_ELFARGS_HPP

#ifndef  IATO_ELFTABLE_HPP
#include "ElfTable.hpp"
#endif

namespace iato {
  using namespace std;

  /// The ElfArgs class is a special class designed to build the memory
  /// image of the program arguments. For a given program, a character
  /// block is built with the total size of the arguments. Note that we
  /// are using C strings, that means the trailing null character is
  /// also included. Then the block is padded to align it with a 8
  /// bytes boundary. Note that we store also the number of arguments
  /// and we use both data to initialize the program stack. This kind
  /// of operation is heavily ABI sensitive. You have been warned ...

  class ElfArgs : public ElfTable {
  private:
    /// the program name
    string  d_name;

  public:
    /// create an argument block by name
    /// @param name the program name
    ElfArgs (const string& name);

    /// create an argument block by arguments
    /// @param argc the number of arguments
    /// @param argv the argument vector
    ElfArgs (const long argc, const char** argv);

    /// create an argument block by name and arguments
    /// @param name the program name
    /// @param argv the program arguments
    ElfArgs (const string& name, const vector<string>& argv);


    /// @return the program name
    string getname (void) const;

  private:
    // make the copy constructor private
    ElfArgs (const ElfArgs&);
    // make the assignment operator private
    ElfArgs& operator = (const ElfArgs&);
  };
}

#endif
