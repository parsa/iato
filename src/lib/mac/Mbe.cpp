// ---------------------------------------------------------------------------
// - Mbe.cpp                                                                 -
// - iato:mac library - memory bypass element class implementation           -
// ---------------------------------------------------------------------------
// - (c) inria 2002-2004                                                     -
// ---------------------------------------------------------------------------
// - authors                                      Amaury Darsch    2002:2004 -
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

#include "Mbe.hpp"

namespace iato {

  // create a default mbe

  Mbe::Mbe (void) {
    reset ();
  }


  // reset this element

  void Mbe::reset (void) {
    d_eix = -1;
    d_mrt.reset ();
  }

  // set the element index

  void Mbe::seteix (const long eix) {
    d_eix = eix;
  }

  // get the element index

  long Mbe::geteix (void) const {
    return d_eix;
  }

  // set the element by mrt

  void Mbe::setmrt (const Mrt& mrt) {
    d_mrt = mrt;
  }

  // get the element mrt

  Mrt Mbe::getmrt (void) const {
    return d_mrt;
  }

  // set the element by mrt and index

  void Mbe::setmrt (const Mrt& mrt, const long eix) {
    d_eix = eix;
    d_mrt = mrt;
  }

  // grab the mrt

  Mrt Mbe::grabmrt (void) {
    Mrt result = d_mrt;
    reset ();
    return result;
  }
}
