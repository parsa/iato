// ---------------------------------------------------------------------------
// - ElfTable.hpp                                                            -
// - iato:elf library - elf character table class definition                 -
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

#ifndef  IATO_ELFTABLE_HPP
#define  IATO_ELFTABLE_HPP

#ifndef  IATO_TYPES_HPP
#include "Types.hpp"
#endif

namespace iato {
  using namespace std;

  /// The ElfTable class is a simple class that manages a vector of
  /// strings and produces a block of characters associated with
  /// a character block pointer relocated at a certain address. A typical
  /// example is the argument string for a program which is a table
  /// of string pointers. In order to operate, the string vector must
  /// be filled with the string values. The "getargb" methods returns a
  /// character block which is padded modulo the abi alignement. The 
  /// "getargs" method returns the complete block size. The "getargv" method
  /// returns a block of pointers relocated at a certain address. The "getargc"
  /// method returns the number of pointers (aka the number of strings) in the
  /// argument vector. Obviously, adding a string between various getxxxx calls
  /// will result in inconsistent tables.

  class ElfTable {
  private:
    /// the string vector
    vector<string> d_svec;

  public:
    /// create an empty character table
    ElfTable (void);

    /// destroy this character table
    virtual ~ElfTable (void);

    /// reset this argument table
    virtual void reset (void);

    /// add a new string to the table
    /// @param s the string to add
    void add (const string& s);

    /// @return the number of arguments
    virtual long getargc (void) const;

    /// @return the argument block size
    virtual long getargs (void) const;

    /// @return the argument block array
    virtual t_byte* getargb (void) const;

    /// relocate the argument vector at a certain address
    /// @param addr the base address to relocate
    virtual t_octa* getargv (const t_octa addr) const;

  private:
    // make the copy constructor private
    ElfTable (const ElfTable&);
    // make the assignment operator private
    ElfTable& operator = (const ElfTable&);
  };
}

#endif
