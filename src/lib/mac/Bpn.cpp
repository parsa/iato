// ---------------------------------------------------------------------------
// - Bpn.cpp                                                                 -
// - iato:mac library - bypass network class implementation                  -
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

#include "Bpn.hpp"
#include "Prn.hpp"

namespace iato {

  // create an empty bypass
  
  Bpn::Bpn (void) : Resource (RESOURCE_BPN) {
    reset ();
  }

  // create an empty bypass by context
  
  Bpn::Bpn (Mtx* mtx) : Resource (RESOURCE_BPN) {
    reset ();
  }

  // create an empty bypass by context and name
  
  Bpn::Bpn (Mtx* mtx, const string& name) : Resource (name) {
    reset ();
  }

  // reset this bypass network

  void Bpn::reset (void) {
    long size = d_vbpe.size ();
    for (long i = 0; i < size; i++) d_vbpe[i]->reset ();
  }

  // report this resource

  void Bpn::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "bypass network" << endl;
  }

  // add a bypass element

  void Bpn::add (Bpe* bpe) {
    if (bpe) d_vbpe.push_back (bpe);
  }

  // evaluate an rid in the bypass network

  Uvr Bpn::eval (const Rid& rid) const {
    Uvr result;
    if (rid.isvalid () == false) return result;
    // get the number of bpe
    long size = d_vbpe.size ();
    // iterate in the bpe(s)
    for (long j = 0; j < size; j++) {
      long index = d_vbpe[j]->find (rid);
      if (index != -1) {
	result = d_vbpe[j]->getuvr (index);
	break;
      }
    }
    return result;
  }

  // evaluate an operand in the bypass network

  void Bpn::eval (Operand& oprd) const {
    // get the number of bpe
    long size = d_vbpe.size ();
    // iterate in the operand
    for (long i = 0; i < IA_MSRC; i++) {
      if (oprd.isvalid (i) == true) continue;
      Rid rid = oprd.getrid (i);
      if (rid.isvalid () == false) continue;
      // iterate in the bpe(s)
      for (long j = 0; j < size; j++) {
	long index = d_vbpe[j]->find (rid);
	if (index == -1) continue;
	Uvr uvr = d_vbpe[j]->getuvr (index);
	// set the uvr value and break since value is set
	assert (oprd.isvalid (i) == false);
	oprd.setuvr (i, uvr);
	break;
      }
    }
  }

  // update a predicate from the bypass bynetwork

  void Bpn::predup (Ssi& inst) const {
    // check for valid instruction
    if (inst.isvalid () == false) return;
    // update the instruction predicate
    Rid pred = inst.getpnum ();
    Uvr puvr = eval (pred);
    if (puvr.isvalid () == true) {
      assert (puvr.gettype () == Uvr::BBV);
      inst.setcnlf (!puvr.getbval ());
    }
  }

  // update a predicate from the bypass bynetwork

  void Bpn::predup (Ssi& inst, Operand& oprd) const {
    // check for valid instruction
    if (inst.isvalid () == false) return;
    // update the instruction predicate
    predup (inst);
    // eventually update the operand predicate
    for (long i = 0; i < IA_MSRC; i++) {
      // check for a predicate rid
      Rid orid = oprd.getrid (i);
      if (orid.isvalid () == false) continue;
      if (orid.gettype () != PREG)  continue;
      // evaluate the predicate
      Uvr ouvr = eval (orid);
      if (ouvr.isvalid () == true) {
	assert (ouvr.gettype () == Uvr::BBV);
	oprd.setuvr (i, ouvr);
      }
    }
  }

  // update an operand in the bypass network

  void Bpn::update (Operand& oprd) const {
    // get the number of bpe
    long size = d_vbpe.size ();
    // iterate in the operand
    for (long i = 0; i < IA_MSRC; i++) {
      Rid rid = oprd.getrid (i);
      if (rid.isvalid () == false) continue;
      // iterate in the bpe(s)
      for (long j = 0; j < size; j++) {
	long index = d_vbpe[j]->find (rid);
	if (index == -1) continue;
	Uvr uvr = d_vbpe[j]->getuvr (index);
	oprd.setuvr (i, uvr);
	break;
      }
    }
  }

  // update a result for an instruction that has explicit rid pair

  void Bpn::update (const Ssi& ssi, Result& resl) const {
    // check for valid instruction
    if (ssi.isvalid () == false) return;
    // iterate with rid pair
    for (long i = 0; i < IA_MRPM; i++) {
      // get the rpm and check
      Rpm rpm = ssi.getrrpm (i);
      if (rpm.isvalid () == false) continue;
      // get the rids and evaluate
      Rid src = rpm.getsrc ();
      Rid dst = rpm.getdst ();
      Uvr uvr = eval (src);
      // get the function mapping and update
      Rpm::t_rfm rfm = rpm.getrfm ();
      if (rfm) {
	resl.upduval (dst, rfm (uvr));
      } else {
	resl.upduval (dst, uvr);
      }	
    }
  }

  // clear a bypass element by rid

  void Bpn::clear (const Rid& rid) {
    long size = d_vbpe.size ();
    for (long i = 0; i < size; i++) d_vbpe[i]->clear (rid);
  }

  // clear a bypass element by result

  void Bpn::clear (const Result& resl) {
    for (long i = 0; i < IA_MDST; i++) {
      Rid rid = resl.getrid (i);
      clear (rid);
    }
  }
}
