// ---------------------------------------------------------------------------
// - Htr.cpp                                                                 -
// - iato:mac library - history register implementation                      -
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

#include "Htr.hpp"
#include "Mac.hpp"
#include "Utils.hpp"

namespace iato {

  // create a default htr

  Htr::Htr (void) {
    d_size = PS_HTRS;
    d_mask = ~getbmsk (d_size);
    reset ();
  }

  // create a htr by size

  Htr::Htr (const long size) {
    d_size = size;
    assert ((d_size > 0) && (d_size <= (long) (8*sizeof (t_octa))));
    d_mask = ~getbmsk (d_size);
    reset ();
  }

  // create a htr by context

  Htr::Htr (Mtx* mtx) {
    d_size = mtx->getlong ("HTR-SIZE");
    assert ((d_size > 0) && (d_size <= (long) (8*sizeof (t_octa))));
    d_mask = ~getbmsk (d_size);
    reset ();
  }

  // copy construct this htr

  Htr::Htr (const Htr& that) {
    d_size = that.d_size;
    d_mask = that.d_mask;
    d_hist = that.d_hist;
  }

  // reset this htr
  
  void Htr::reset (void) {
    d_hist = OCTA_0;
  }

  // assign a htr to this one

  Htr& Htr::operator = (const Htr& that) {
    d_size = that.d_size;
    d_mask = that.d_mask;
    d_hist = that.d_hist;
    return *this;
  }

  // return the htr size

  long Htr::getsize (void) const {
    return d_size;
  }

  // return the htr value

  t_octa Htr::gethist (void) const {
    return d_hist;
  }

  // update the htr by flag

  t_octa Htr::update (const bool flag) {
    t_octa val = d_hist << 1;
    if (flag == true) val |= 0x0000000000000001ULL;
    d_hist = val & d_mask;
    return d_hist;
  }
}
