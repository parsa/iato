// ---------------------------------------------------------------------------
// - Pfs.cpp                                                                 -
// - iato:isa library - previous frame state class implementation            -
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

#include "Pfs.hpp"

namespace iato {

  // create a default pfs

  Pfs::Pfs (void) {
    d_pfs = 0x0000000000000000LL;
  }

  // create a pfs with a value

  Pfs::Pfs (const t_octa value) {
    setpfs (value);
  }

  // copy construct this pfs

  Pfs::Pfs (const Pfs& that) {
    d_pfs = that.d_pfs;
  }

  // assign a pfs to this one

  Pfs& Pfs::operator = (const Pfs& that) {
    d_pfs = that.d_pfs;
    return *this;
  }

  // reset this pfs

  void Pfs::reset (void) {
    d_pfs = 0x0000000000000000LL;
  }

  // set the pfs value

  void Pfs::setpfs (const t_octa pfs) {
    d_pfs = pfs;
  }

  // return the current pfs

  t_octa Pfs::getpfs (void) const {
    return d_pfs;
  }

  // this procedure returns the pfs mask by type
  static inline t_octa get_pfs_mask (Pfs::t_field fld) {
    t_octa mask = 0x0000000000000000LL;
    switch (fld) {
    case Pfs::PFM:
      mask = ((t_octa) 0x0000003FFFFFFFFFLL);
      break;
    case Pfs::PEC:
      mask = ((t_octa) 0x3F) << 52;
      break;
    case Pfs::PPL:
      mask = ((t_octa) 0x03) << 62;
      break;
    }
    return mask;
  }

  // set the pfs field

  void Pfs::setfld (t_field fld, t_octa val) {
    // mask pfs value
    t_octa mask = ~get_pfs_mask (fld);
    // compute value shift
    t_octa fval = val;
    switch (fld) {
    case PFM:
      break;
    case PEC:
      fval = fval << 52;
      break;
    case PPL:
      fval = fval << 62;
      break;
    }
    // update pfs value
    d_pfs = (d_pfs & mask) | fval;
  }

  // return a pfs field value

  t_octa Pfs::getfld (t_field fld) const {
    t_octa mask = get_pfs_mask (fld);
    t_octa fval = d_pfs & mask;
    switch (fld) {
    case PFM:
      break;
    case PEC:
      fval = fval >> 52;
      break;
    case PPL:
      fval = fval >> 62;
      break;
    }
    return fval;
  }
}
