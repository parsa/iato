// ---------------------------------------------------------------------------
// - Watchdog.cpp                                                            -
// - iato:mac library - instruction result buffer class implementation       -
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

#include "Mac.hpp"
#include "Prn.hpp"
#include "Watchdog.hpp"
#include "Exception.hpp"

namespace iato {

  // create a new watchdog
  
  Watchdog::Watchdog (void) : Resource (RESOURCE_WDG) {
    d_time = WD_TIME;
    reset ();
  }

  // create a new watchdog with a context
  
  Watchdog::Watchdog (Mtx* mtx) : Resource (RESOURCE_WDG) {
    d_time = mtx->getlong ("WATCHDOG-TIME");
    reset ();
  }

  // reset this watchdog

  void Watchdog::reset (void) {
    d_wcnt = d_time;
  }

  // report this resource

  void Watchdog::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "system watchdog" << endl;
    cout << "\twatchdog time\t\t: " << d_time << endl;
  }

  // beat this watchdog

  void Watchdog::notify (void) {
    if (--d_wcnt <= 0) {
      reset ();
      throw Exception ("watchdog-error", "watchdog alarm activated");
    }
  }
}
