// ---------------------------------------------------------------------------
// - Utils.hpp                                                               -
// - iato:isa library - utility function definitions                         -
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

#ifndef  IATO_UTIL_HPP
#define  IATO_UTIL_HPP

#ifndef  IATO_TYPES_HPP
#include "Types.hpp"
#endif

namespace iato {
  using namespace std;

  /// These are the utilities functions that somehow expand some of the
  /// functionalities available in the STL. Typical utility function are
  /// string triming and data formating.

  /// @return an integer from a string
  long tolong (const char* s);

  /// @return a double from a string
  double todouble (const char* s);

  /// copy one byte string into another
  /// @param d the destination string
  /// @param s the source stirng
  void cpystr (t_byte* d, const t_byte* src);

  /// left trim a string
  /// @param s the string to trim left
  string ltrim (const string& s);

  /// right trim a string
  /// @param s the string to trim right
  string rtrim (const string& s);

  /// trim a string
  /// @param s the string to trim left and right
  string trim (const string& s);

  /// fill right a string with blank characters
  /// @param s the string to fill
  /// @param size the final size
  string rfill (const string& s, const long size);

  /// split a string according to separators
  /// @param str the string to split
  /// @param delimiters the string of delimiters
  /// @return the vector of strings
  vector<string> split (const string& str, const string& delimiters);

  /// @return the most significand bit index 
  long getmsbi (const t_octa value);

  /// @return a mask by size
  t_octa getsmsk (const long size);

  /// @return a mask by bit size
  t_octa getbmsk (const long size);

  /// read 'n' bytes from a descpriptor
  /// @param fd the file descriptor
  /// @param vptr the bytes returned container
  /// @param n the number of bytes to read
  /// @return the number of bytes read
  t_size readn (int fd, void* vptr, t_size n);
}

#endif
