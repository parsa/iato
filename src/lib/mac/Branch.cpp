// ---------------------------------------------------------------------------
// - Branch.cpp                                                              -
// - iato:mac library - base branch prediction class implementation          -
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
#include "Gskew.hpp"
#include "Gshare.hpp"
#include "Bimodal.hpp"
#include "Exception.hpp"

namespace iato {

  // this procedure compute the absolute value of a long
  static inline long abs (const long x) {
    return x > 0 ? x : -x;
  }

  // this procedure create a new branch prediction system by name
  Branch* Branch::mkbr (Mtx* mtx) {
    // get the predictor name
    string name = mtx->getstr ("BRANCH-PREDICTOR-TYPE");
    // check for default
    if ((name.length () == 0) || (name == "none")) return new Branch (mtx);
    // check for bimodal
    if (name == "bimodal") return new Bimodal (mtx);
    // check for gshare
    if (name == "gshare") return new Gshare (mtx);
    // check for gskew
    if (name == "gskew") return new Gskew (mtx);    
    // not found
    string msg = "invalid branch predictor name ";
    throw Exception ("context-error", msg + name);
  }

  // create a default branch predictor

  Branch::Branch (void) : Resource (RESOURCE_BPS) {
    reset ();
  }

  // create a branch predictor by name

  Branch::Branch (const string& name) : Resource (name) {
    reset ();
  }

  // create a branch predictor by context

  Branch::Branch (Mtx* mtx) : Resource (RESOURCE_BPS) {
    reset ();
  }
  
  // create a branch predictor by context and name

  Branch::Branch (Mtx* mtx, const string& name) : Resource (name) {
    reset ();
  }

  // reset this branch prediction

  void Branch::reset (void) {
    d_hist = OCTA_0;
  }

  // report some resource information

  void Branch::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: branch predictor" << endl;
    cout << "\tpredictor type \t\t: " << "default" << endl;
  }

  // return true if the branch is predicted taken

  bool Branch::istaken (const t_octa addr) const {
    return false;
  }

  // return true if the branch can be predicted

  bool Branch::ispredict (const t_octa addr) const {
    return false;
  }

  // set the predictor history

  void Branch::sethist (const t_octa hist) {
    d_hist = hist;
  }

  // return the predictor history

  t_octa Branch::gethist (void) const {
    return d_hist;
  }

  // compute the next ip from the current ip and the window size

  t_octa Branch::nextip (const t_octa cip, const long ws) {
    return cip + abs (ws);
  }

  // predict the next ip by ip

  t_octa Branch::predict (const t_octa cip) {
    ostringstream os;
    os << "cannot predict next ip at 0x";
    os << hex << setw (16) << setfill ('0') << cip;
    throw Exception ("branch-error", os.str ());
  }

  // update the branch system with an ip and next ip

  void Branch::update (const t_octa cip, const bool btk, const t_octa nip, 
		       const t_octa hst) {
    d_hist = hst;
  }

  // update the branch system with an instruction, result and taken flag

  void Branch::markbr (const Instr& inst, const Result& resl, const bool btk) {
    // check for valid instruction
    if (inst.isvalid () == false) return;
    // check for branches
    if (inst.isbr () == false) return;
    // get the instruction ip
    t_octa cip = inst.getiip  ();
    // get the result ip
    t_octa rip = resl.isvalid () ? resl.getrip () : inst.getsip ();
    // get the history
    t_octa hst = inst.gethist ();
    // update the branch system, note that the cancel flag is inverted
    update (cip, btk, rip, hst);
  }
}
