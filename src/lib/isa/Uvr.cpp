// ---------------------------------------------------------------------------
// - Uvr.cpp                                                                 -
// - iato:isa library - universal value representation class implementation  -
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

#include "Uvr.hpp"

namespace iato {
  
  // create a default uvr

  Uvr::Uvr (void) {
    reset ();
  }

  // copy construct this uvr

  Uvr::Uvr (const Uvr& that) {
    d_type = that.d_type;
    d_bval = that.d_bval;
    d_oval = that.d_oval;
    d_rval = that.d_rval;
  }

  // assign a uvr to this one

  Uvr& Uvr::operator = (const Uvr& that) {
    d_type = that.d_type;
    d_bval = that.d_bval;
    d_oval = that.d_oval;
    d_rval = that.d_rval;
    return *this;
  }

  // reset this uvr

  void Uvr::reset (void) {
    d_type = NAV;
    d_bval = false;
    d_oval = OCTA_0;
    d_rval = 0.0L;
  }

  // return true if the uvr is valid

  bool Uvr::isvalid (void) const {
    if (d_type == NAV) return false;
    return true;
  }

  // return the uvr type

  Uvr::t_uvrt Uvr::gettype (void) const {
    return d_type;
  }

  // set a boolean value

  void Uvr::setbval (const bool bval) {
    d_type = BBV;
    d_bval = bval;
    d_oval = OCTA_0;
    d_rval = 0.0L;
  }

  // get a boolean value

  bool Uvr::getbval (void) const {
    assert ((d_type == ONV) || (d_type == BBV));
    return d_bval;
  }

  // set an octa value

  void Uvr::setoval (const t_octa oval) {
    d_type = OBV;
    d_bval = false;
    d_oval = oval;
    d_rval = 0.0L;
  }

  // get an octa value

  t_octa Uvr::getoval (void) const {
    assert ((d_type == OBV) || (d_type == ONV));
    return d_oval;
  }

  // set a dual value

  void Uvr::setdval (const t_octa oval, const bool bval) {
    d_type = ONV;
    d_oval = oval;
    d_bval = bval;
    d_rval = 0.0L;
  }

  // set a real value

  void Uvr::setrval (const t_real rval) {
    d_type = RBV;
    d_bval = false;
    d_oval = OCTA_0;
    d_rval = rval;
  }

  // get a real value

  t_real Uvr::getrval (void) const {
    assert (d_type == RBV);
    return d_rval;
  }
}
