// ---------------------------------------------------------------------------
// - Bits.hpp                                                                -
// - iato:isa library - machine format and bit field function definitions    -
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

#ifndef  IATO_BITS_HPP
#define  IATO_BITS_HPP

#ifndef  IATO_CONFIG_HXX
#include "Config.hxx"
#endif

namespace iato {

  /// The Bits utilities are a set of functions that manipulates the
  /// data format from one endian representation to another. A global
  /// variable called MA_MODE, if set to true indicates that the running
  /// machine is in big endian format. Other function permits to swap
  /// type as well as masking field.

  /// this global variable indicates if the runtime operation are
  /// carried with a big endian machine.
  extern const bool MA_MODE;

  /// swap bytes in a word
  /// @param value the word to swap
  t_word swapword (const t_word value);

  /// swap bytes in a quad
  /// @param value the word to swap
  t_quad swapquad (const t_quad value);

  /// swap bytes in an octa
  /// @param value the word to swap
  t_octa swapocta (const t_octa value);

  /// fix a word in little endian form
  /// @param value the value to fix
  t_word lfixword (const t_word value);
 
  /// fix a quad in little endian form
  /// @param value the value to fix
  t_quad lfixquad (const t_quad value);
  
  /// fix an octa in little endian form
  /// @param value the value to fix
  t_octa lfixocta (const t_octa value);

  /// set a bit in a byte at a certain position
  /// @param value the byte to set
  /// @param pos   the bit position
  /// @param bval the bit value to set
  t_byte bsetbyte (const t_byte value, const long pos, const bool bval);

  /// set a bit in an octa at a certain position
  /// @param value the octa to set
  /// @param pos   the bit position
  /// @param bval  the bit value to set
  t_octa bsetocta (const t_octa value, const long pos, const bool bval);

  /// get a bit in a byte at a certain position
  /// @param value the byte to use during get
  /// @param pos   the bit position
  bool bsetget (const t_byte value, const long pos);

  /// get a bit in an octa at a certain position
  /// @param value the octa to use during get
  /// @param pos   the bit position
  bool bsetget (const t_octa value, const long pos);
}

#endif
