// ---------------------------------------------------------------------------
// - Mli.cpp                                                                 -
// - iato:mac library - memory logic interface class implementation          -
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

#include "Prn.hpp"
#include "Mli.hpp"

namespace iato {

  // create a default memory logic

  Mli::Mli (void) : Resource (RESOURCE_MLI) {
    p_mpr = new Mpr;
    p_mob = 0;
  }

  // create a memory logic by context

  Mli::Mli (Mtx* mtx) : Resource (RESOURCE_MLI) {
    p_mpr = new Mpr (mtx);
    p_mob = 0;
  }

  // create a memory logic by context and name

  Mli::Mli (Mtx* mtx, const string& name) : Resource (name) {
    p_mpr = new Mpr (mtx);
    p_mob = 0;
  }
 
  // reset this memory logic

  void Mli::reset (void) {
    if (p_mpr) p_mpr->reset ();
    if (p_mob) p_mob->reset ();
  }

  // report some resource information

  void Mli::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: memory logic interface" << endl;
  }

  // bind a memory request port

  void Mli::bind (Mta* mta, Mob* mob) {
    p_mpr->bind (mta);
    p_mob = mob;
  }

  // request a memory transaction by instruction and result

  void Mli::preset (const Dsi& inst, Result& resl) {
    // check for valid result
    if (inst.isvalid () == false) return;
    if (resl.isvalid () == false) return;
    // chek for load
    if (inst.getldb () == false) return;
    // get the mrt and check
    Mrt mrt = resl.getmrt ();
    if (mrt.isvalid () == false) return;
    // preset the mob and mpr
    p_mob->preset (inst.getmob (), mrt);
    p_mpr->preset (mrt);
    // eventually update the result
    if (p_mpr->istack () == true) resl.update (p_mpr->getmrt ());	
  }

  // update a memory transaction by instruction and result

  void Mli::update (const Dsi& inst, const Result& resl) {
    // check for valid result
    if (inst.isvalid () == false) return;
    if (resl.isvalid () == false) return;
    // chek for store
    if (inst.getstb () == false) return;
    // get the mrt and check
    Mrt mrt = resl.getmrt ();
    if (mrt.isvalid () == false) return;
    // update the mob
    p_mob->update (inst.getmob (), mrt);
    // update the port
    p_mpr->update (mrt);
  }
}
