// ---------------------------------------------------------------------------
// - Umr.cpp                                                                 -
// - iato:isa library - user mask register class implementation              -
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

#include "Umr.hpp"
#include "Isa.hpp"
#include "Bits.hpp"

namespace iato {

  // create a default umr

  Umr::Umr (void) {
    setumr (DEF_UMR);
  }

  // create a umr with a value

  Umr::Umr (const t_byte value) {
    setumr (value);
  }

  // copy construct this umr

  Umr::Umr (const Umr& that) {
    d_umr = that.d_umr;
  }

  // assign a umr to this one

  Umr& Umr::operator = (const Umr& that) {
    d_umr = that.d_umr;
    return *this;
  }

  // reset this umr

  void Umr::reset (void) {
    setumr (DEF_UMR);
  }

  // set the umr value

  void Umr::setumr (const t_byte umr) {
    d_umr = umr & 0x3e;
  }

  // return the current umr

  t_byte Umr::getumr (void) const {
    return d_umr & 0x3e;
  }

  // set the umr field

  void Umr::setfld (t_field fld, const bool val) {
    switch (fld) {
    case BE:
      d_umr = bsetbyte (d_umr, 1, val);
      break;
    case UP:
      d_umr = bsetbyte (d_umr, 2, val);
      break;
    case AC:
      d_umr = bsetbyte (d_umr, 3, val);
      break;
    case MFL:
      d_umr = bsetbyte (d_umr, 4, val);
      break;
    case MFH:
      d_umr = bsetbyte (d_umr, 5, val);
      break;
    }
  }

  // return a umr field value

  bool Umr::getfld (t_field fld) const {
    bool result = false;
    switch (fld) {
    case BE:
      result = bsetget (d_umr, 1);
      break;
    case UP:
      result = bsetget (d_umr, 2);
      break;
    case AC:
      result = bsetget (d_umr, 3);
      break;
    case MFL:
      result = bsetget (d_umr, 4);
      break;
    case MFH:
      result = bsetget (d_umr, 5);
      break;
    }
    return result;
  }
}
