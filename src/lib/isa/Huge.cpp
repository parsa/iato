// ---------------------------------------------------------------------------
// - Huge.cpp                                                                -
// - iato:isa library - 128 bits huge unsigned integer class implementation  -
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

#include "Huge.hpp"
#include "Bits.hpp"

namespace iato {

  // this procedure assign an octa into a huge buffer
  static void fill_huge (t_byte* buf, const t_octa value) {
    // the octa/bytes representation
    union {
      t_octa oval;
      t_byte bval[8];
    };
    oval = lfixocta (value);
    // set the value
    for (long i = 0; i < 8; i++)                 buf[i] = bval[i];
    for (long i = 8; i < t_huge::HUGE_SIZE; i++) buf[i] = 0x00;
  }

  // this procedure assign two octa into a huge buffer
  static void fill_huge (t_byte* buf, const t_octa hip, const t_octa lop) {
    // the octa/bytes representation
    union {
      t_octa d_oval;
      t_byte d_bval[8];
    } high, low;
    low.d_oval  = lfixocta (lop);
    high.d_oval = lfixocta (hip); 
    // set the value
    for (long i = 0; i < 8; i++) {
      buf[i]   = low.d_bval[i];
      buf[i+8] = high.d_bval[i];
    }
  }

  // this function shift by byte a huge buffer
  static void shl_byte (t_byte* buf, const long shl) {
    for (long i = 0; i < shl; i++) {
      for (long j = t_huge::HUGE_SIZE - 1; j > 0; j--) {
	buf[j] = buf[j-1];
      }
      buf[0] = 0x00;
    }
  }

  // this function multiply a huge buffer with a byte value
  static void mul_byte (t_byte* dst, const t_byte* src, const t_byte x) {
    t_word wval = 0x0000;
    for (long i = 0; i < t_huge::HUGE_SIZE; i++) {
      wval += (t_word) src[i] * (t_word) x;
      dst[i] = (t_byte) (wval & 0x00ff);
      wval = (wval & 0xff00) >> 8;
    }
  }

  // create a default huge value

  t_huge::t_huge (void) {
    for (long i = 0; i < HUGE_SIZE; i++) d_hval[i] = 0x00;
  }

  // create a huge value with an octa

  t_huge::t_huge (const t_octa value) {
    fill_huge (d_hval, value);
  }

  // create a huge value with two octa

  t_huge::t_huge (const t_octa hip, const t_octa lop) {
    fill_huge (d_hval, lop);
    union{
      t_octa oval;
      t_byte bval[8];
    };
    oval = lfixocta (hip);
    for (long j = 8; j < HUGE_SIZE; j++)
      d_hval[j] = bval[j-8];
  }

  // copy construct this huge value

  t_huge::t_huge (const t_huge& that) {
    for (long i = 0; i < HUGE_SIZE; i++) d_hval[i] = that.d_hval[i];
  }

  // assign an octa to this huge value

  t_huge& t_huge::operator = (const t_octa value) {
    fill_huge (d_hval, value);
    return *this;
  }

  // assign a huge value to this huge value

  t_huge& t_huge::operator = (const t_huge& that) {
    for (long i = 0; i < HUGE_SIZE; i++) d_hval[i] = that.d_hval[i];
    return *this;
  }

  // add a huge value with another one

  t_huge t_huge::operator + (const t_huge& x) const {
    t_huge result;
    t_word wval = 0x0000;
    for (long i = 0; i < HUGE_SIZE; i++) {
      wval += (t_word) d_hval[i] + (t_word) x.d_hval[i];
      result.d_hval[i] = (t_byte) (wval & 0x00FF);
      wval = (wval & 0xFF00) >> 8;
    }
    return result;
  }

  // multiply a huge value with another one

  t_huge t_huge::operator * (const t_huge& x) const {
    t_huge result;
    t_huge line;
    for (long i = 0; i < HUGE_SIZE; i++) {
      mul_byte (line.d_hval, d_hval, x.d_hval[i]);
      shl_byte (line.d_hval, i);
      result = result + line;
    }
    return result;
  }

  // shift right one huge

  t_huge t_huge::operator >> (const t_octa shc) const {
    t_huge result(OCTA_0);
    if (shc < 64){
      t_octa d_oval[2];
      d_oval[0]  = getlow () >> shc;
      d_oval[1]  = gethigh () >> shc;
      d_oval[0] |= gethigh () << (64 - shc);
      fill_huge (result.d_hval, d_oval[1], d_oval[0]);
    }
    else if (shc < 128){
      fill_huge (result.d_hval, gethigh () >> (shc - 64));
    }
    return result;
  }

  // set the high part of a huge

  void t_huge::sethigh (const t_octa value) {
    fill_huge (d_hval, value, getlow ());
  }

  // return the lowest part of the huge number

  t_octa t_huge::getlow (void) const {
    // the octa/bytes representation
    union {
      t_octa oval;
      t_byte bval[8];
    };
    for (long i = 0; i < 8; i++) bval[i] = d_hval[i];
    return lfixocta (oval);
  }

  // return the highest part of the huge number

  t_octa t_huge::gethigh (void) const {
    // the octa/bytes representation
    union {
      t_octa oval;
      t_byte bval[8];
    };
    for (long i = 0; i < 8; i++) bval[i] = d_hval[i+8];
    return lfixocta (oval);
  }
}
