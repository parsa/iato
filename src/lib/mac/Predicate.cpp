// ---------------------------------------------------------------------------
// - Predicate.cpp                                                           -
// - iato:mac library - base predicate prediction class implementation       -
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
#include "Mspp.hpp"
#include "Hypp.hpp"
#include "Pshare.hpp"
#include "Pimodal.hpp"
#include "Pbmodal.hpp"
#include "Exception.hpp"

namespace iato {
  
  // this procedure create a new predicate predictor by name
  Predicate* Predicate::mkpr (const string& name) {
    // check for default
    if ((name.length () == 0) || (name == "none")) return new Predicate;
    // check for pimodal
    if (name == "pimodal") return new Pimodal;
    // check for pimodal
    if (name == "pbmodal") return new Pbmodal;
    // check for pshare
    if (name == "pshare") return new Pshare;
    // check for mspp
    if (name == "mspp") return new Mspp;
    // check for hypp
    if (name == "hypp") return new Hypp;
    // not found
    string msg = "invalid predicate predictor name ";
    throw Exception ("context-error", msg + name);
  }

  // this procedure create a new predicate predictor by name and context
  Predicate* Predicate::mkpr (const string& name, Mtx* mtx) {
    // check for default
    if ((name.length () == 0) || (name == "none")) return new Predicate (mtx);
    // check for pimodal
    if (name == "pimodal") return new Pimodal (mtx);
    // check for pbmodal
    if (name == "pbmodal") return new Pbmodal (mtx);
    // check for pshare
    if (name == "pshare") return new Pshare (mtx);
    // check for mspp
    if (name == "mspp") return new Mspp (mtx);
    // check for hypp
    if (name == "hypp") return new Hypp (mtx);
    // not found
    string msg = "invalid predicate predictor name ";
    throw Exception ("context-error", msg + name);
  }

  // this procedure create a new predicate predictor by context
  Predicate* Predicate::mkpr (Mtx* mtx) {
    // get the predictor name
    string name = mtx->getstr ("PREDICATE-PREDICTOR-TYPE");
    return Predicate::mkpr (name, mtx);
  }

  // create a default predicate predictor

  Predicate::Predicate (void) : Resource (RESOURCE_PPS) {
    d_type = "none";
    d_bupd = PP_BUPD;
    reset ();
  }

  // create a predicate predictor by name

  Predicate::Predicate (const string& name) : Resource (name) {
    d_type = "none";
    d_bupd = PP_BUPD;
    reset ();
  }

  // create a predicate predictor by context

  Predicate::Predicate (Mtx* mtx) : Resource (RESOURCE_PPS) {
    d_type = "none";
    d_bupd = mtx->getbool ("PREDICATE-BRANCH-UPDATE");
    reset ();
  }
  
  // create a predicate predictor by context and name

  Predicate::Predicate (Mtx* mtx, const string& name) : Resource (name) {
    d_type = "none";
    d_bupd = mtx->getbool ("PREDICATE-BRANCH-UPDATE");
    reset ();
  }

  // reset this predicate prediction

  void Predicate::reset (void) {
  }

  // return the predictor name

  string Predicate::gettype (void) const {
    return d_type;
  }

  // report some resource information

  void Predicate::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: predicate predictor" << endl;
    cout << "\tpredictor type \t\t: " << "default" << endl;
    if (d_bupd == true)
      cout << "\tuse br update  \t\t: " << "true" << endl;
    else
      cout << "\tuse br update  \t\t: " << "false" << endl;
  }

  // return true if the predicate can be predicted

  bool Predicate::isvalid (const t_octa ip, const long slot, 
			   const long pred) const {
    return false;
  }

  // return true if the instruction predicate can be predicted

  bool Predicate::ispredict (const Instr& inst) const {
    if (inst.isvalid () == false) return false;
    t_octa ip = inst.getiip  ();
    long slot = inst.getslot ();
    Rid  prid = inst.getpnum (); assert (prid.isvalid () == true);
    long pred = prid.getpnum ();
    return (pred == 0) ? false : isvalid (ip, slot, pred);
  }

  // predict the predicate value by index

  bool Predicate::compute (const t_octa ip, const long slot, 
			   const long pred) const {
    // check for fixed predicate
    if (pred == 0) return true;
    // invalid access
    ostringstream os;
    os << "cannot predict predicate " << pred << " at 0x";
    os << hex << setw (16) << setfill ('0') << ip << dec;
    os << " slot " << slot;
    throw Exception ("predicate-error", os.str ());
  }

  // predict the instruction predicate

  bool Predicate::predict (const Instr& inst) const {
    assert (inst.isvalid () == true);
    t_octa ip = inst.getiip  ();
    long slot = inst.getslot ();
    Rid  prid = inst.getpnum (); assert (prid.isvalid () == true);
    long pred = prid.getpnum ();
    return (pred == 0) ? true : compute (ip, slot, pred);
  }

  // update the predicate system by ip, slot, predicate and value

  void Predicate::update (const t_octa ip, const long slot, const long pred,
			  const bool pval, const bool bflg) {
    // do nothing with fixed predicate
    if (pred == 0) return;
  }

  // update the prediction system by instruction and value

  void Predicate::markpp (const Instr& inst, const bool pval) {
    // check for valid instruction
    if (inst.isvalid () == false) return;
    // check for branch update
    if ((d_bupd == false) && (inst.isbr () == true)) return;
    // grab instruction info
    t_octa ip = inst.getiip  ();
    long slot = inst.getslot ();
    Rid  prid = inst.getpnum ();
    long pred = prid.getpnum ();
    // and now update the system
    update (ip, slot, pred, pval, inst.isbr ());
  }

  // update the prediction system by instruction and result

  void Predicate::markpp (const Instr& inst, const Result& resl) {
    // check for valid instruction
    if (inst.isvalid () == false) return;
    // check for valid result
    if (resl.isvalid () == false) return;
    // check for branch update
    if ((d_bupd == false) && (inst.isbr () == true)) return;
    // grab instruction info
    t_octa ip = inst.getiip  ();
    long slot = inst.getslot ();
    // iterate in the result and update
    for (long i = 0; i < IA_MDST; i++) {
      // check for predicate
      if (resl.isreg (i, PREG) == false) continue;
      // get the predicate index and value
      Rid  prid = resl.getrid  (i);
      long pred = prid.getpnum ();
      bool pval = resl.getbval (i);
      // and now update the system
      update (ip, slot, pred, pval, inst.isbr ());
    }
  }
}
