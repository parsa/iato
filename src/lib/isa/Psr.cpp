// ---------------------------------------------------------------------------
// - Psr.cpp                                                                 -
// - iato:isa library - processor status register class implementation       -
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

#include "Psr.hpp"
#include "Isa.hpp"
#include "Bits.hpp"
#include "Exception.hpp"

namespace iato {

  // the psr system mask reserved field mask
  const t_octa PSR_RVF_MASK = 0x0000000000011FC1ULL;
  // the psr reserved field mask
  const t_octa PSR_ALL_MASK = 0xFFFFC000F0011FC1ULL;

  // this procedure set the umr bit field in the psr
  static t_octa set_umr (const t_octa psr, const t_byte umr) {
    // build the umr mask and data
    t_octa mask = ~0x000000000000003ELL;
    t_octa data = (t_octa) (umr & 0x3E);
    // build the result by masking and oring
    t_octa result = (psr & mask) | data;
    return result;
  }

  // this procedure returns the umr as a byte field
  static t_byte get_umr (const t_octa psr) {
    t_byte result = ((t_byte) psr) & 0x03;
    return result;
  }

  // this procedure set the cpl bit field in the psr
  static t_octa set_cpl (const t_octa psr, const t_byte cpl) {
    // build the cpl mask and data
    t_octa mask = ~(0x0000000000000003LL << 32);
    t_octa data = ((t_octa) (cpl & 0x03)) << 32;
    // build the result by masking and oring
    t_octa result = (psr & mask) | data;
    return result;
  }

  // this procedure returns the cpl as a byte field
  static t_byte get_cpl (const t_octa psr) {
    t_byte result = ((t_byte) (psr >> 32)) & 0x03;
    return result;
  }

  // this procedure set the ri bit field in the psr
  static t_octa set_ri (const t_octa psr, const t_byte ri) {
    // build the cpl mask and data
    t_octa mask = ~(0x0000000000000003LL << 41);
    t_octa data = ((t_octa) (ri & 0x03)) << 41;
    // build the result by masking and oring
    t_octa result = (psr & mask) | data;
    return result;
  }

  // this procedure returns the ri as a byte field
  static t_byte get_ri (const t_octa psr) {
    t_byte result = ((t_byte) (psr >> 41)) & 0x03;
    return result;
  }
  
  // create a default psr

  Psr::Psr (void) {
    setpsr (DEF_PSR);
  }

  // create a psr with a value

  Psr::Psr (const t_octa value) {
    setpsr (value);
  }

  // copy construct this psr

  Psr::Psr (const Psr& that) {
    d_psr = that.d_psr;
  }

  // assign a psr to this one

  Psr& Psr::operator = (const Psr& that) {
    d_psr = that.d_psr;
    return *this;
  }

  // reset this psr

  void Psr::reset (void) {
    setpsr (DEF_PSR);
  }

  // set the psr value

  void Psr::setpsr (const t_octa psr) {
    d_psr = psr;
  }

  // return the current psr

  t_octa Psr::getpsr (void) const {
    return d_psr;
  }

  // set the umr value

  void Psr::setumr (const Umr& umr) {
    setfld (UM, umr.getumr ());
  }

  // get the umr value

  Umr Psr::getumr (void) const {
    Umr umr (getbyte (UM));
    return umr;
  }

  // set the psr field

  void Psr::setfld (const t_field fld, const bool val) {
    switch (fld) {
    case IC:
      d_psr = bsetocta (d_psr, 13, val);
      break;
    case IB:
      d_psr = bsetocta (d_psr, 14, val);
      break;
    case PK:
      d_psr = bsetocta (d_psr, 15, val);
      break;
    case DT:
      d_psr = bsetocta (d_psr, 17, val);
      break;
    case DFL:
      d_psr = bsetocta (d_psr, 18, val);
      break;
    case DFH:
      d_psr = bsetocta (d_psr, 19, val);
      break;
    case SP:
      d_psr = bsetocta (d_psr, 20, val);
      break;
    case PP:
      d_psr = bsetocta (d_psr, 21, val);
      break;
    case DI:
      d_psr = bsetocta (d_psr, 22, val);
      break;
    case SI:
      d_psr = bsetocta (d_psr, 23, val);
      break;
    case DB:
      d_psr = bsetocta (d_psr, 24, val);
      break;
    case LP:
      d_psr = bsetocta (d_psr, 25, val);
      break;
    case TB:
      d_psr = bsetocta (d_psr, 26, val);
      break;
    case RT:
      d_psr = bsetocta (d_psr, 27, val);
      break;
    case IS:
      d_psr = bsetocta (d_psr, 34, val);
      break;
    case MC:
      d_psr = bsetocta (d_psr, 35, val);
      break;
    case IT:
      d_psr = bsetocta (d_psr, 36, val);
      break;
    case ID:
      d_psr = bsetocta (d_psr, 37, val);
      break;
    case DA:
      d_psr = bsetocta (d_psr, 38, val);
      break;
    case DD:
      d_psr = bsetocta (d_psr, 39, val);
      break;
    case SS:
      d_psr = bsetocta (d_psr, 40, val);
      break;
    case ED:
      d_psr = bsetocta (d_psr, 43, val);
      break;
    case BN:
      d_psr = bsetocta (d_psr, 44, val);
      break;
    case IA:
      d_psr = bsetocta (d_psr, 45, val);
      break;
    default:
      throw Exception ("psr-error", "invalid boolean field to set");
      break;
    }
  }

  // set the psr byte field

  void Psr::setfld (const t_field fld, const t_byte val) {
    switch (fld) {
    case UM:
      d_psr = set_umr (d_psr, val);
      break;
    case CPL:
      d_psr = set_cpl (d_psr, val);
      break;
    case RI:
      d_psr = set_ri  (d_psr, val);
      break;
    default:
      throw Exception ("psr-error", "invalid byte field to set");
      break;
    }
  }

  // return a psr boolean field value

  bool Psr::getfld (const t_field fld) const {
    bool result = false;
    switch (fld) {
    case IC:
      result = bsetget (d_psr, 13);
      break;
    case IB:
      result = bsetget (d_psr, 14);
      break;
    case PK:
      result = bsetget (d_psr, 15);
      break;
    case DT:
      result = bsetget (d_psr, 17);
      break;
    case DFL:
      result = bsetget (d_psr, 18);
      break;
    case DFH:
      result = bsetget (d_psr, 19);
      break;
    case SP:
      result = bsetget (d_psr, 20);
      break;
    case PP:
      result = bsetget (d_psr, 21);
      break;
    case DI:
      result = bsetget (d_psr, 22);
      break;
    case SI:
      result = bsetget (d_psr, 23);
      break;
    case DB:
      result = bsetget (d_psr, 24);
      break;
    case LP:
      result = bsetget (d_psr, 25);
      break;
    case TB:
      result = bsetget (d_psr, 26);
      break;
    case RT:
      result = bsetget (d_psr, 27);
      break;
    case IS:
      result = bsetget (d_psr, 34);
      break;
    case MC:
      result = bsetget (d_psr, 35);
      break;
    case IT:
      result = bsetget (d_psr, 36);
      break;
    case ID:
      result = bsetget (d_psr, 37);
      break;
    case DA:
      result = bsetget (d_psr, 38);
      break;
    case DD:
      result = bsetget (d_psr, 39);
      break;
    case SS:
      result = bsetget (d_psr, 40);
      break;
    case ED:
      result = bsetget (d_psr, 43);
      break;
    case BN:
      result = bsetget (d_psr, 44);
      break;
    case IA:
      result = bsetget (d_psr, 45);
      break;
    default:
      throw Exception ("psr-error", "invalid boolean field to get");
      break;
    }
    return result;
  }

  // return a psr byte field value

  t_byte Psr::getbyte (const t_field fld) const {
    t_byte result = 0x00;
    switch (fld) {
    case UM:
      result = get_umr (d_psr);
      break;
    case CPL:
      result = get_cpl (d_psr);
      break;
    case RI:
      result = get_ri (d_psr);
      break;
    default:
      throw Exception ("psr-error", "invalid byte field to get");
      break;
    }
    return result;
  }

  // return true if the value marks a reserved field

  bool Psr::isrvfd (const t_trvfd fd, const t_octa value) const {
    bool result = false;
    switch (fd) {
    case UMB:
    case SMB:
      result = ((value & PSR_RVF_MASK) == OCTA_0) ? false : true;
      break;
    case ALL:
      result = ((value & PSR_ALL_MASK) == OCTA_0) ? false : true;
      break;
    default:
      assert (false);
      break;
    }
    return result;
  }
}
