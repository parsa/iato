// ---------------------------------------------------------------------------
// - Msi.cpp                                                                 -
// - iato:mac library - memory synchro interface class implementation        -
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

#include "Prn.hpp"
#include "Msi.hpp"

namespace iato {

  // create a default memory logic

  Msi::Msi (void) : Resource (RESOURCE_MSI) {
    p_mpr = new Mpr;
  }

  // create a memory logic by context

  Msi::Msi (Mtx* mtx) : Resource (RESOURCE_MSI) {
    p_mpr = new Mpr (mtx);
  }

  // create a memory logic by context and name

  Msi::Msi (Mtx* mtx, const string& name) : Resource (name) {
    p_mpr = new Mpr (mtx);
  }
 
  // reset this memory logic

  void Msi::reset (void) {
    if (p_mpr) p_mpr->reset ();
  }

  // report some resource information

  void Msi::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: memory synchro interface" << endl;
  }

  // bind a memory request port

  void Msi::bind (Mta* mta) {
    p_mpr->bind (mta);
  }

  // request a memory transaction by instruction and result

  void Msi::preset (const Ssi& inst, Result& resl) {
    // check for valid result
    if (inst.isvalid () == false) return;
    if (resl.isvalid () == false) return;
    // chek for load
    if (inst.getldb () == false) return;
    // get the mrt and check
    Mrt mrt = resl.getmrt ();
    if (mrt.isvalid () == false) return;
    // preset the mpr
    p_mpr->preset (mrt);
    // eventually update the result
    if (p_mpr->istack () == true) {
      Mrt tmrt = p_mpr->getmrt ();
      resl.update (tmrt);
    }
  }

  // update a memory transaction by instruction and result

  void Msi::update (const Ssi& inst, const Result& resl) {
    // check for valid result
    if (inst.isvalid () == false) return;
    if (resl.isvalid () == false) return;
    // chek for store
    if (inst.getstb () == false) return;
    // get the mrt and check
    Mrt mrt = resl.getmrt ();
    if (mrt.isvalid () == false) return;
    // update the port
    p_mpr->update (mrt);
  }
}
