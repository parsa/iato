// ---------------------------------------------------------------------------
// - Operand.cpp                                                             -
// - iato:isa library - instruction operand value class implementation       -
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

#include "Operand.hpp"

namespace iato {

  // create a default operand

  Operand::Operand (void) {
    reset ();
  }

  // copy construct this operand

  Operand::Operand (const Operand& that) {
    for (long i = 0; i < IA_MSRC; i++) {
      d_valid[i] = that.d_valid[i];
      d_orid[i]  = that.d_orid[i];
      d_bval[i]  = that.d_bval[i];
      d_oval[i]  = that.d_oval[i];
      d_rval[i]  = that.d_rval[i];
    }
  }

  // assign an operand to this one

  Operand& Operand::operator = (const Operand& that) {
    for (long i = 0; i < IA_MSRC; i++) {
      d_valid[i] = that.d_valid[i];
      d_orid[i]  = that.d_orid[i];
      d_bval[i]  = that.d_bval[i];
      d_oval[i]  = that.d_oval[i];
      d_rval[i]  = that.d_rval[i];
    }
    return *this;
  }
   

  // reset this operand

  void Operand::reset (void) {
    for (long i = 0; i < IA_MSRC; i++) {
      d_valid[i] = false;
      d_orid[i].reset ();
      d_bval[i]  = false;
      d_oval[i]  = 0;
    }
  }

  // check if the operand is valid

  bool Operand::isvalid (void) const {
    for (long i = 0; i < IA_MSRC; i++) {
      if ((d_orid[i].isvalid () == true) && (d_valid[i] == false)) 
	return false;
    }
    return true;
  }

  // set the operand value by index and uvr

  void Operand::setuvr (const long index, const Uvr& uvr) {
    assert (uvr.isvalid () == true);
    switch (uvr.gettype ()) {
    case Uvr::OBV:
      setoval (index, uvr.getoval ());
      break;
    case Uvr::ONV:
      setoval (index, uvr.getoval ());
      setbval (index, uvr.getbval ());
      break;
    case Uvr::BBV:
      setbval (index, uvr.getbval ());
      break;
    case Uvr::RBV:
      setrval (index, uvr.getrval ());
      break;
    default:
      assert (false);
      break;
    }
  }
}
