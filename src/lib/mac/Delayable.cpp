// ---------------------------------------------------------------------------
// - Delayable.cpp                                                           -
// - iato:mac library - abstract delayable object class implementation       -
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

#include "Delayable.hpp"

namespace iato {

  // create a default delayable

  Delayable::Delayable (void) {
    d_dlat = 1;
    d_dcnt = d_dlat;
  }  
  // create a delayable by name

  Delayable::Delayable (const string& name) : Runnable (name) {
    d_dlat = 1;
    d_dcnt = d_dlat;
  }

  // reset this delayable interface

  void Delayable::reset (void) {
    d_dcnt = d_dlat;
  }

  // flush this delayable interface

  void Delayable::flush (void) {
    d_dcnt = d_dlat;
  }

  // run this delayable interface

  void Delayable::run (void) {
    if (--d_dcnt <= 0) {
      d_dcnt = d_dlat;
      activate ();
    }
  }

  // set the delayable latency

  void Delayable::setdlat (const long dlat) {
    assert (dlat > 0);
    d_dlat = dlat;
  }

  // return the delayable latency

  long Delayable::getdlat (void) const {
    return d_dlat;
  }
}
