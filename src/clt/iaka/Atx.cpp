// ---------------------------------------------------------------------------
// - Atx.hpp                                                                 -
// - iato:iaka client - application context class implementation             -
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

#include "Eio.hpp"
#include "Atx.hpp"

namespace iato {

  // create a new context

  Atx::Atx (void) {
    reset ();
  }

  // reset this context

  void Atx::reset (void) {
    Utx::reset ();
    // set maximu cycle count and pipeline refill
    setlong ("MAXIMUM-CYCLE-COUNT",      EM_MXCC);
    setlong ("BRANCH-PIPELINE-REFILL",   EM_PFIL);
    // set tracer only mode
    setbool ("TRACER-BRANCH-ONLY",       EM_BOFL);
    setbool ("TRACER-PREDICATE-ONLY",    EM_POFL);
    setbool ("TRACER-PREDICATE-TARGETS", EM_PGFL);
  }
}
