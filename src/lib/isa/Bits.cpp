// ---------------------------------------------------------------------------
// - Bits.cpp                                                                -
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

#include "Bits.hpp"

namespace iato {

  // this procedure returns the host endian mode as true for big endian
  // that is most significant bit first (msb).
  static bool get_host_mode (void) {
    union {
      t_byte bval[2];
      t_word wval;
    };
    wval = 0xff00;
    if (bval[0] == 0xff) {
      assert (bval[1] == 0x00);
      return true;
    }
    assert ((bval[0] == 0x00) && (bval[1] == 0xff));
    return false;
  }

  // fix the global machine endian mode
  const bool MA_MODE = get_host_mode ();

  // swap bytes in a word

  t_word swapword (const t_word value) {
    // build structure and initialize
    union {
      t_byte bval[2];
      t_word wval;
    };
    wval = value;
    // swap bytes and return
    t_byte tmp = bval[0]; bval[0] = bval[1]; bval[1] = tmp;
    return wval;
  }

  // swap bytes in a quad

  t_quad swapquad (const t_quad value) {
    // build structure and initialize
    union {
      t_byte bval[4];
      t_quad qval;
    };
    qval = value;
    // swap bytes and return
    t_byte tmp0 = bval[0]; bval[0] = bval[3]; bval[3] = tmp0;
    t_byte tmp1 = bval[1]; bval[1] = bval[2]; bval[2] = tmp1;
    return qval;
  }

  // swap bytes in an octa

  t_octa swapocta (const t_octa value) {
    // build structure and initialize
    union {
      t_byte bval[8];
      t_octa oval;
    };
    oval = value;
    // swap bytes and return
    t_byte tmp0 = bval[0]; bval[0] = bval[7]; bval[7] = tmp0;
    t_byte tmp1 = bval[1]; bval[1] = bval[6]; bval[6] = tmp1;
    t_byte tmp2 = bval[2]; bval[2] = bval[5]; bval[5] = tmp2;
    t_byte tmp3 = bval[3]; bval[3] = bval[4]; bval[4] = tmp3;
    return oval;
  }

  // set a bit in a byte at a certain position

  t_byte bsetbyte (const t_byte value, const long pos, const bool bval) {
    // adjust bit position
    unsigned long idx = pos % (sizeof (t_byte) * 8);
    assert ((idx >= 0) && (idx < (sizeof (t_byte) * 8)));
    // compute mask
    t_byte mask = 0x01;
    mask = ~(mask << idx);
    // compute value
    t_byte data = (bval == true) ? 0x01 : 0x00;
    data = data << idx;
    // compute result by masking and oring
    t_byte result = (value & mask) | data;
    return result;
  }

  // fix a word - swap if big endian or do nothing

  t_word lfixword (const t_word value) {
    return MA_MODE ? swapword (value) : value;
  }
  
  // fix a quad - swap if big endian or do nothing

  t_quad lfixquad (const t_quad value) {
    return MA_MODE ? swapquad (value) : value;
  }

  // fix an octa  - swap if big endian or do nothing

  t_octa lfixocta (const t_octa value) {
    return MA_MODE ? swapocta (value) : value;
  }

  // set a bit in an octa at a certain position

  t_octa bsetocta (const t_octa value, const long pos, const bool bval) {
    // adjust bit position
    unsigned long idx = pos % (sizeof (t_octa) * 8);
    assert ((idx >= 0) && (idx < (sizeof (t_octa) * 8)));
    // compute mask
    t_octa mask = OCTA_1;
    mask = ~(mask << idx);
    // compute value
    t_octa data = (bval == true) ? OCTA_1 : OCTA_0;
    data = data << idx;
    // compute result by masking and oring
    t_octa result = (value & mask) | data;
    return result;
  }

  // get a bit at a certain position

  bool bsetget (const t_byte value, const long pos) {
    // adjust bit position
    unsigned long idx = pos % (sizeof (t_byte) * 8);
    assert ((idx >= 0) && (idx < (sizeof (t_byte) * 8)));
    // compute mask
    t_byte mask = 0x01;
    mask = mask << idx;
    // compute value by masking ans shifting
    t_byte data = (value & mask) >> idx;
    return (data == 0x01) ? true : false;
  }

  // get a bit at a certain position

  bool bsetget (const t_octa value, const long pos) {
    // adjust bit position
    unsigned long idx = pos % (sizeof (t_octa) * 8);
    assert ((idx >= 0) && (idx < (sizeof (t_octa) * 8)));
    // compute mask
    t_octa mask = OCTA_1;
    mask = mask << idx;
    // compute value by masking ans shifting
    t_octa data = (value & mask) >> idx;
    return (data == OCTA_1) ? true : false;
  }
}
