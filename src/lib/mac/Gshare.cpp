// ---------------------------------------------------------------------------
// - Gshare.cpp                                                              -
// - iato:mac library - global history predictor class implementation        -
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
#include "Gshare.hpp"

namespace iato {

  // create a default gshare predictor

  Gshare::Gshare (void) : Branch (RESOURCE_BPS) {
    p_htr = new Htr;
    p_pht = new Pht;
    p_btb = new Btb;
    reset ();
  }

  // create a branch prediction by context

  Gshare::Gshare (Mtx* mtx) : Branch (mtx,RESOURCE_BPS) {
    p_htr = new Htr (mtx);
    p_pht = new Pht (mtx);
    p_btb = new Btb (mtx);
    reset ();
  }
  
  // create a branch prediction by context and name

  Gshare::Gshare (Mtx* mtx, const string& name) : Branch (mtx, name) {
    p_htr = new Htr (mtx);
    p_pht = new Pht (mtx);
    p_btb = new Btb (mtx);
    reset ();
  }

  // destroy this predictor

  Gshare::~Gshare (void) {
    delete p_htr;
    delete p_pht;
    delete p_btb;
  }

  // reset this branch predictor

  void Gshare::reset (void) {
    p_htr->reset ();
    p_pht->reset ();
    p_btb->reset ();
  }

  // report some resource information

  void Gshare::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: branch predictor" << endl;
    cout << "\tpredictor type \t\t: " << "gshare" << endl;
    cout << "\thtr size       \t\t: " << p_htr->getsize () << endl;
    cout << "\tpht size       \t\t: " << p_pht->getsize () << endl;
    cout << "\tbtb size       \t\t: " << p_btb->getsize () << endl;
  }

  // return true if the branch is predicted taken

  bool Gshare::istaken (const t_octa cip, const long slot) const {
    // compute hash address
    t_octa addr = (cip >> 4) ^ p_htr->gethist ();
    // get the taken flag
    return p_pht->istrue (addr);
  }

  // return true if the branch can be predicted

  bool Gshare::ispredict (const t_octa cip, const long slot) const {
    if (p_btb->isvalid (cip) == false) return false;
    return istaken (cip, slot);
  }

  // predict the next ip from the current ip and slot

  t_octa Gshare::predict (const t_octa cip, const long slot) {
    // compute next ip for safety
    t_octa nip = cip + BN_BYSZ;
    // check in the btb to see if the address is available
    // if not return the next ip (use at your own risk)
    if (p_btb->isvalid (cip) == false) return nip;
    // get the target address from the btb
    return p_btb->gettrg (cip);
  }

  // update the branch system with an ip and next ip

  void Gshare::update (const t_octa cip, const long slot, 
			const bool btk, const t_octa nip) {
    // compute hash address
    t_octa addr = (cip >> 4) ^ p_htr->gethist ();
    // update the pht by address
    p_pht->update (addr, btk);
    // update the branch history
    p_htr->update (btk);
    // update the btb
    if (btk == true) p_btb->update (cip, nip);
  }
}
