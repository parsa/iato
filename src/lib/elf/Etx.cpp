// ---------------------------------------------------------------------------
// - Etx.cpp                                                                 -
// - iato:elf library - elf context class implementation                     -
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

#include "Etx.hpp"
#include "Abi.hpp"

namespace iato {

  // create a new context

  Etx::Etx (void) {
    reset ();
  }
  // reset this context

  void Etx::reset (void) {
    // reset base context
    Ctx::reset ();
    // update archietcture
    update (DEF_ARCH);
  }

  // update this context with a particular architecture

  void Etx::update (const t_arch arch) {
    // updqte base system
    Ctx::update (arch);
    // set default elf information
    d_lmap["PAGE-SHIFT"]         = PAGE_SHFT;
    d_lmap["STACK-SIZE"]         = VSTK_SIZE;
    d_lmap["BACKING-STORE-SIZE"] = VBSA_SIZE;
  }
}
