// ---------------------------------------------------------------------------
// - Urf.cpp                                                                 -
// - iato:mac library - universal register file class implementation         -
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
#include "Urf.hpp"

namespace iato {

  // this procedure performs an initial rat and trb initialization
  static void init_rat_trb (t_lreg lreg, Rat* rat, Trb* trb) {
    long size = rat->getsize (lreg);
    if (size == 0) return;
    for (long i = 0; i < size; i++) {
      long vnum = trb->initial ();
      rat->setmap (lreg, i, vnum);
    }
  }

  // create a default urf

  Urf::Urf (void) : Resource (RESOURCE_URF) {
    p_rat = new Rat;
    p_trb = new Trb;
    p_urb = new Urb;
    reset ();
  }

  // create a urf by context

  Urf::Urf (Mtx* mtx) : Resource (RESOURCE_URF) {
    p_rat = new Rat (mtx);
    p_trb = new Trb (mtx);
    p_urb = new Urb (mtx);
    reset ();
  }

  // create a urf by context and name

  Urf::Urf (Mtx* mtx, const string& name) : Resource (name) {
    p_rat = new Rat (mtx);
    p_trb = new Trb (mtx);
    p_urb = new Urb (mtx);
    reset ();
  }

  // destroy this urf

  Urf::~Urf (void) {
    delete p_rat;
    delete p_trb;
    delete p_urb;
  }

  // reset this urf

  void Urf::reset (void) {
    flush ();
  }

  // flush this urf

  void Urf::flush (void) {
    // reset rat and trb
    p_rat->reset ();
    p_trb->reset ();
    p_urb->reset ();
    // initialize translation and map
    init_rat_trb (GREG, p_rat, p_trb);
    init_rat_trb (FREG, p_rat, p_trb);
    init_rat_trb (PREG, p_rat, p_trb);
    init_rat_trb (BREG, p_rat, p_trb);
    init_rat_trb (AREG, p_rat, p_trb);
    init_rat_trb (CREG, p_rat, p_trb);
  }

  // partial flush this urf

  void Urf::pflsh (void) {
    p_trb->pflsh ();
    p_urb->reset ();;
  }

  // report this resource

  void Urf::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "universal register file" << endl;
    p_rat->report ();
    p_trb->report ();
    p_urb->report ();
  }

  // return the associated rat

  Rat* Urf::getrat (void) const {
    return p_rat;
  }

  // return the associated trb

  Trb* Urf::gettrb (void) const {
    return p_trb;
  }

  // return the associated urb

  Urb* Urf::geturb (void) const {
    return p_urb;
  }

  // return true if a trb entry is ready

  bool Urf::isready (const Rid& rid) const {
    // if not valid, in some sense it is ready
    if (rid.isvalid () == false) return true;
    // if the virtual bit is not set, the rid is ready
    if (rid.getvbit () == false) return true;
    long vnum = rid.getvnum ();
    return p_trb->isready (vnum);
  }

  // return true if an operand is ready
  bool Urf::isready (const Operand& oprd) const {
    // iterate through the rid
    for (long i = 0; i < IA_MSRC; i++) {
      // discard already valid value
      if (oprd.isvalid (i) == true) continue;
      // get the rid and check validity
      Rid rid = oprd.getrid (i);
      if (isready (rid) == false) return false;
    }
    return true;
  }

  // evaluate a rid in the urf

  Uvr Urf::eval (const Rid& rid) const {
    Uvr result;
    if (rid.isvalid () == false) return result;
    // check the virtual bit, as to decide if a translation ocuurs
    if (rid.getvbit () == false) return result;
    // get the virtual number and check the trb for urb access
    long vnum = rid.getvnum ();
    if (p_trb->isready (vnum) == false) return result;
    bool tbit = p_trb->gettbit (vnum);
    // if the tbit is true, the value is available in the urb
    if (tbit == true) {
      long tnum = p_trb->gettnum (vnum);
      result    = p_urb->getuvr  (tnum);
    }
    return result;
  }

  // evaluate an operand in the urf

  void Urf::eval (Operand& oprd) const {
    // iterate through the rid
    for (long i = 0; i < IA_MSRC; i++) {
      // discard already valid value
      if (oprd.isvalid (i) == true) continue;
      // get the rid and check validity
      Rid rid = oprd.getrid (i);
      if (rid.isvalid () == false) continue;
      // check the virtual bit, as to decide if a translation occurs
      if (rid.getvbit () == false) continue;
      // get the virtual number and check the trb for urb access
      long vnum = rid.getvnum ();
      bool tbit = p_trb->gettbit (vnum);
      // if the tbit is true, the operand value is available in the urb
      if (tbit == true) {
	long tnum = p_trb->gettnum (vnum);
	Uvr  uvr  = p_urb->getuvr  (tnum);
	oprd.setuvr (i, uvr);
      }
    }
  }

  // reroute a rid in the urf

  void Urf::reroute (const Rid& rid) {
    // check for valid rid and non renamed
    if (rid.isvalid () == false) return;
    if (rid.islrnm  () == false) return;
    if (rid.getvbit () == false) return;
    // get the old virtual number and check for ready
    long onum = rid.getonum ();
    if (p_trb->isready (onum) == false) return;
    // get the virtual number to update
    long vnum = rid.getvnum ();
    // the old trb is ready, so check for translation
    // if the translation bit is false, the value is available
    // in the register file, so we just mark the value as ready
    // without translation
    bool tbit = p_trb->gettbit (onum);
    if (tbit == false) {
      p_trb->setrdy (vnum);
    } else {
      // the translation bit is true, so we can get the old value
      long tnum = p_trb->gettnum (onum);
      Uvr uvr   = p_urb->getuvr  (tnum);
      // allocate a new urb entry
      tnum = p_urb->alloc ();
      assert (tnum != -1);
      // update trb and urb
      p_trb->settnum (vnum, tnum, true);
      p_urb->setuvr  (tnum, uvr);
    }
  }

  // update the urf with a result by rerouting registers

  void Urf::reroute (const Result& resl) {
    // iterate through all result
    for (long i = 0; i < IA_MDST; i++) {
      // check for none or reroute operation
      Result::t_rop rop = resl.getrop (i);
      if ((rop != Result::ROP_NOP) && (rop != Result::REG_RRT)) continue;
      // get the rid reroute it
      Rid rid = resl.getrid (i);
      reroute (rid);
    }
  }

  // update the urf with a result

  void Urf::update (const Result& resl) {
    // check for valid result
    if (resl.isvalid () == false) return;
    // iterate through all result
    for (long i = 0; i < IA_MDST; i++) {
      // get the rid and check for translation
      Rid rid = resl.getrid (i);
      if (rid.isvalid () == false) continue;
      // do not update logical renamed
      if (rid.islrnm  () == false) continue;
      // check for rerouting
      if (resl.getrop (i) == Result::REG_RRT) {
	reroute (rid);
	continue;
      }
      // check for update
      if (rid.isvalid () == false) continue;
      if (rid.getvbit () == false) continue;
      // get the virtual number and check the trb for urb access
      long vnum = rid.getvnum ();
      // get the uvr result
      Uvr uvr = resl.getuvr (i);
      // allocate a new urb entry
      long tnum = p_urb->alloc ();
      assert (tnum != -1);
      // update trb and urb
      p_trb->settnum (vnum, tnum, true);
      p_urb->setuvr  (tnum, uvr);
    }
  }

  // clean the urf with a result

  void Urf::clean (const Result& resl) {
    // check for valid result
    if (resl.isvalid () == false) return;
    // iterate through all result
    for (long i = 0; i < IA_MDST; i++) {
      // get the virtual number and check
      Rid rid = resl.getrid (i);
      if (rid.isvalid () == false) continue;
      if (rid.islrnm  () == false) continue;
      // do not clean, untranslated register
      if (rid.getvbit () == false) continue;
      // get the virtual register to clean
      long vnum = rid.getvnum ();
      long onum = rid.getonum ();
      long tnum = p_trb->clean (vnum, onum);
      // free the urb
      if (tnum != -1) p_urb->clean (tnum);
    }
  }

  // cancel the urf with a result by updating the trb

  void Urf::cancel (const Result& resl) {
    // iterate through all rid
    for (long i = 0; i < IA_MDST; i++) {
      // get result type and check
      Result::t_rop rop = resl.getrop (i);
      assert ((rop == Result::ROP_NOP) || (rop == Result::REG_RRT));
      // get the rid and check for translation
      Rid rid = resl.getrid (i);
      if (rid.isvalid () == false) continue;
      // do not cancel untranslated register
      if (rid.getvbit () == false) continue;
      // get the virtual number and cancel
      long vnum = rid.getvnum ();
      long onum = rid.getonum ();
      long tnum = p_trb->clean (vnum, onum);
      // free the urb
      if (tnum != -1) p_urb->clean (tnum);
    }
  }
}
