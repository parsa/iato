// ---------------------------------------------------------------------------
// - Stx.hpp                                                                 -
// - iato:iaio client - simulation context class implementation              -
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

#include "Soo.hpp"
#include "Stx.hpp"

namespace iato {

  // create a new context

  Stx::Stx (void) {
    reset ();
  }

  // reset this context

  void Stx::reset (void) {
    Mtx::reset ();
    // simulator parameters
    setlong ("MAXIMUM-CYCLE-COUNT",   SM_MXCC);
  }
}
