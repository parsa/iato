// ---------------------------------------------------------------------------
// - Disperse.cpp                                                            -
// - iato:mac library - bundle dispersion class implementation               -
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
#include "Disperse.hpp"

namespace iato {

  // this procedure checks that a bundle has a at least one branch type
  // in the template, as to define an implicit stop bit
  static bool is_br_tmpl (const t_byte tmpl) {
    bool result = false;
    switch (tmpl) {
    case Bundle::BN_MxIxBx:
    case Bundle::BN_MxIxBs:
    case Bundle::BN_MxBxBx:
    case Bundle::BN_MxBxBs:
    case Bundle::BN_BxBxBx:
    case Bundle::BN_BxBxBs:
    case Bundle::BN_MxMxBx:
    case Bundle::BN_MxMxBs:
    case Bundle::BN_MxFxBx:
    case Bundle::BN_MxFxBs:
      result = true;
      break;
    default:
      result = false;
      break;
    }
    return result;
  }

  // this procedure check that a template has a stop bit in the last slot
  static bool is_st_tmpl (const t_byte tmpl) {
    bool result = false;
    switch (tmpl) {
    case Bundle::BN_MxIxIs:
    case Bundle::BN_MxIsIs:
    case Bundle::BN_MxLxXs:
    case Bundle::BN_MxMxIs:
    case Bundle::BN_MsMxIs:
    case Bundle::BN_MxFxIs:
    case Bundle::BN_MxMxFs:
    case Bundle::BN_MxIxBs:
    case Bundle::BN_MxBxBs:
    case Bundle::BN_BxBxBs:
    case Bundle::BN_MxMxBs:
    case Bundle::BN_MxFxBs:
      result = true;
      break;
    default:
      result = false;
      break;
    }
    return result;
  }

  // create a default dispersal engine

  Disperse::Disperse (void) : Resource (RESOURCE_BDS) {
    p_ipb = 0;
    p_iib = 0;
    p_mob = 0;
    p_psb = 0;
    reset ();
  }

  // create a dispersal engine by name

  Disperse::Disperse (const string& name) : Resource (name) {
    p_ipb = 0;
    p_iib = 0;
    p_mob = 0;
    p_psb = 0;
    reset ();
  }

  // create a dispersal engine by context

  Disperse::Disperse (Mtx* mtx) : Resource (RESOURCE_BDS) {
    p_ipb = 0;
    p_iib = 0;
    p_mob = 0;
    p_psb = 0;
    reset ();
  }
  
  // create a dispersal engine by context and name

  Disperse::Disperse (Mtx* mtx, const string& name) : Resource (name) {
    p_ipb = 0;
    p_iib = 0;
    p_mob = 0;
    p_psb = 0;
    reset ();
  }

  // reset this dispersal engine

  void Disperse::reset (void) {
    if (p_ipb) p_ipb->reset ();
    if (p_iib) p_iib->reset ();
    if (p_mob) p_mob->reset ();
    if (p_psb) p_psb->reset ();
  }

  // report some resource information

  void Disperse::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: dispersal engine" << endl;
    cout << "\tdisperse type\t\t: default" << endl;
  }

  // this procedure return true if the bundle must be pushed back

  bool Disperse::isback (const Bundle& bndl) const {
    // check for valid bundle
    if (bndl.isvalid () == false) return false;
    // check if all instruction are invalidated
    for (long i = 0; i < BN_SLSZ; i++) {
      try {
	Instr inst = bndl.getinstr (i);
	if (inst.isvalid () == true) return true;
      } catch (...) {
	return true;
      }
    }
    return false;
  }

  // this procedure returns true if a bundle as an implicit stop bit
  // i.e a next bundle is not required for dispersal

  bool Disperse::isnext (const Bundle& bndl) const {
    // check for valid bundle
    if (bndl.isvalid () == false) return false;
    // check if we have a branch template
    t_byte tmpl = bndl.gettmpl ();
    if (is_br_tmpl (tmpl) == true) return false;
    // check if we have a stop bit at the last slot
    if (is_st_tmpl (tmpl) == true) return false;
    // nothing prevents to continue
    return true;
  }

  // disperse a bundle into an issue port buffer

  void Disperse::expand (Bundle& bndl) {
    // check for valid buffer and bundle
    if (bndl.isvalid () == false) return;
    // loop in the bundle and disperse
    for (long i = 0; i < BN_SLSZ; i++) {
      // get the instruction
      Ssi ssi;
      try {
	ssi = bndl.getinstr (i);
      } catch (const Interrupt& vi) {
	// mark interrupt in the scoreboard
	p_psb->alloc (vi);
	return;
      }
      if (ssi.isvalid () == false) continue;
      // get the instruction slot unit
      t_unit unit = ssi.getsunit ();
      // find the first available slot
      long slot = p_ipb->find (unit);
      // if invalid, then terminate the expansion
      if (slot == -1) return;
      // allocate the interrupt buffer entry
      long iidx = p_iib->alloc (); assert (iidx != -1);
      ssi.setiib (iidx);
      // allocate memory ordering buffer entry
      bool ildb = ssi.getldb ();
      bool istb = ssi.getstb ();
      if ((ildb == true) || (istb == true)) {
	long imob = p_mob->alloc (ildb, istb); assert (imob != -1);
	ssi.setmob (imob);
      }
      // allocate scoreboard entry
      long ridx = p_psb->alloc (ssi); assert (ridx != -1);
      ssi.setrix (ridx);
      // the slot is valid, so assign the instruction
      p_ipb->setinst (unit, slot, ssi);      
      // clear the instruction in the bundle
      slot = ssi.getslot ();
      bndl.setvsb (slot, false);
      // check if we have a stop bit but not at slot 2
      if (ssi.getstop () == true) return;
    }
  }

  // bind the disperse resource
  
  void Disperse::bind (Ipb* ipb, Iib* iib, Mob* mob, Scoreboard* psb) {
    p_ipb = ipb;
    p_iib = iib;
    p_mob = mob;
    p_psb = psb;
  } 
}
