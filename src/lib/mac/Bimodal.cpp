// ---------------------------------------------------------------------------
// - Bimodal.cpp                                                             -
// - iato:mac library - bimodal branch prediction class implementation       -
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
#include "Bimodal.hpp"

namespace iato {
  // this procedure compute the absolute value of a long
  static inline long abs (const long x) {
    return x > 0 ? x : -x;
  }

  // create a default bimodal predictor

  Bimodal::Bimodal (void) : Branch (RESOURCE_BPS) {
    p_pht = new Pht;
    p_btb = new Btb;
    reset ();
  }

  // create a branch prediction by context

  Bimodal::Bimodal (Mtx* mtx) : Branch (mtx,RESOURCE_BPS) {
    p_pht = new Pht (mtx);
    p_btb = new Btb (mtx);
    reset ();
  }
  
  // create a branch prediction by context and name

  Bimodal::Bimodal (Mtx* mtx, const string& name) : Branch (mtx, name) {
    p_pht = new Pht (mtx);
    p_btb = new Btb (mtx);
    reset ();
  }

  // destroy this predictor

  Bimodal::~Bimodal (void) {
    delete p_pht;
    delete p_btb;
  }

  // reset this branch predictor

  void Bimodal::reset (void) {
    p_pht->reset ();
    p_btb->reset ();
  }

  // report some resource information

  void Bimodal::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: branch predictor" << endl;
    cout << "\tpredictor type \t\t: " << "bimodal" << endl;
    cout << "\tpht size       \t\t: " << p_pht->getsize () << endl;
    cout << "\tbtb size       \t\t: " << p_btb->getsize () << endl;
  }

  // return true if the branch is predicted taken

  bool Bimodal::istaken (const t_octa cip, const long slot) const {
    return p_pht->istrue (cip >> 4);
  }

  // return true if the branch can be predicted

  bool Bimodal::ispredict (const t_octa cip, const long slot) const {
    if (p_btb->isvalid (cip) == false) return false;
    return istaken (cip, slot);
  }

  // predict the next ip from the current ip and slot

  t_octa Bimodal::predict (const t_octa cip, const long slot) {
    // compute next ip for safety
    t_octa nip = cip + BN_BYSZ;
    // check in the btb to see if the address is available
    // if not return the next ip (use at your own risk)
    if (p_btb->isvalid (cip) == false) return nip;
    // get the target address from the btb
    return p_btb->gettrg (cip);
  }

  // update the branch system with an ip and next ip

  void Bimodal::update (const t_octa cip, const long slot, 
			const bool btk, const t_octa nip) {
    // update the pht by address
    p_pht->update (cip >> 4, btk);
    // update the btb
    if (btk == true) p_btb->update (cip, nip);
  }
}
