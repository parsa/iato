// ---------------------------------------------------------------------------
// - Utx.cpp                                                                 -
// - iato:ecu library - utility extension context class implementation       -
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

#include "Ecu.hpp"
#include "Utx.hpp"

namespace iato {

  // create a new context

  Utx::Utx (void) {
    reset ();
  }

  // reset this context to a default

  void Utx::reset (void) {
    // reset base context
    Mtx::reset ();
    // update architecture
    update (DEF_ARCH);
  }

  // update this context with a particular architecture

  void Utx::update (const t_arch arch) {
    // update base system
    Mtx::update (arch);
    // update utility resources
    d_bmap["FETCHER-DISPERSE-FLAG"] = FT_DISF;
  }
}
