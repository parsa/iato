// ---------------------------------------------------------------------------
// - Fetcher.cpp                                                             -
// - iato:ecu library - instruction fetch class implementation               -
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

#include "Ecu.hpp"
#include "Urn.hpp"
#include "Fetcher.hpp"

namespace iato {
  using namespace std;

  // create a default fetcher

  Fetcher::Fetcher (void) : Resource (RESOURCE_FTC) {
    d_dflg = FT_DISF;
    d_iwsz = 1;
    p_bndl = new Bundle[d_iwsz];
    p_imem = 0;
    reset ();
  }

  // create a fetcher with a context

  Fetcher::Fetcher (Utx* utx) : Resource (RESOURCE_FTC) {
    d_dflg = utx->getbool ("FETCHER-DISPERSE-FLAG");
    d_iwsz = d_dflg ? utx->getiwsz () : 1;
    p_bndl = new Bundle[d_iwsz];
    p_imem = 0;
    reset ();
  }

  // destroy this fetcher

  Fetcher::~Fetcher (void) {
    delete [] p_bndl;
  }

  // reset this fetcher

  void Fetcher::reset (void) {
    for (long i = 0; i < d_iwsz; i++) p_bndl[i].reset ();
  }

  // bind the instruction memory

  void Fetcher::bind (Memory* imem) {
    p_imem = imem;
  }

  // return true if the fetcher is empty

  bool Fetcher::isempty (void) const {
    for (long i = 0; i < d_iwsz; i++) {
      if (p_bndl[i].isvalid () == true) return false;
    }
    return true;
  }

  // fill the fetcher at a certain ip

  void Fetcher::fill (const t_octa ip) {
    t_octa bip = ip;
    for (long i = 0; i < d_iwsz; i++) {
      // check for valid bundle
      if (p_bndl[i].isvalid () == true) continue;
      // reset and fill
      p_bndl[i].reset  ();
      p_bndl[i].setbip (bip);
      for (long j = 0; j < BN_BYSZ; j++) {
	p_bndl[i].push (p_imem->readexec (bip++));
      }
    }
  }

  // pack the bundle array by shifting down

  void Fetcher::pack (void) {
    for (long i = 1; i < d_iwsz; i++) {
      if (p_bndl[i-1].isvalid () == true) break;
      p_bndl[i-1] = p_bndl[i];
      p_bndl[i].reset ();
    }
  }

  // return the next available bundle

  Bundle Fetcher::getbndl (void) {
    // pack the array
    pack ();
    // grab first bundle and check
    Bundle bndl = p_bndl[0];
    p_bndl[0].reset ();
    // check for valid bundle
    if (bndl.isvalid () == false) {
      reset ();
      return bndl;
    }
    // check for non disperse bundle
    if ((d_dflg == false) && (bndl.isvalid () == true)) {
      reset ();
      return bndl;
    }
    // compute stop bit and marking
    for (long i = 0; i < BN_SLSZ; i++) {
      Instr inst;
      try {
	inst = bndl.getinstr (i);
      } catch (...) {
	inst.reset ();
      }
      // check for validity and get slot
      if (inst.isvalid () == false) continue;
      long slot = inst.getslot ();
      if ((slot == 2) && 
	  ((inst.getstop () == true) || (inst.isbr () == true))) {
	reset ();
	break;
      }
      // check for stop bit
      if (inst.getstop () == true) {
	// restaure bundle, since isvalid does not check for vsb
	p_bndl[0] = bndl;
	// clear bundle slots
	for (long j = 0;   j <= slot;   j++) p_bndl[0].setvsb (j, false);
	for (long j = i+1; j < BN_SLSZ; j++) bndl.setvsb      (j, false);
	break;
      }
    }
    return bndl;
  }
}
