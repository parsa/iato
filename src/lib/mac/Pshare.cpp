// ---------------------------------------------------------------------------
// - Pshare.cpp                                                              -
// - iato:mac library - global history pred predictor class implementation   -
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
#include "Pshare.hpp"

namespace iato {

  // create a default pshare predictor

  Pshare::Pshare (void) : Predicate (RESOURCE_PPS) {
    d_type = "pshare";
    d_usec = PP_UCFG;
    p_htr  = new Htr;
    p_pht  = new Pht;
    reset ();
  }

  // create a pshare predictor by context

  Pshare::Pshare (Mtx* mtx) : Predicate (mtx,RESOURCE_PPS) {
    d_type = "pshare";
    d_usec = mtx->getbool ("USE-CONFIDENCE-FLAG");
    p_htr  = new Htr (mtx);
    p_pht  = new Pht (mtx);
    reset ();
  }
  
  // create a pshare predictor by context and name

  Pshare::Pshare (Mtx* mtx, const string& name) : Predicate (mtx, name) {
    d_type = "pshare";
    d_usec = mtx->getbool ("USE-CONFIDENCE-FLAG");
    p_htr  = new Htr (mtx);
    p_pht  = new Pht (mtx);
    reset ();
  }

  // destroy this predictor

  Pshare::~Pshare (void) {
    delete p_htr;
    delete p_pht;
  }

  // reset this branch predictor

  void Pshare::reset (void) {
    p_htr->reset ();
    p_pht->reset ();
  }

  // report some resource information

  void Pshare::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "  resource type                : predicate predictor" << endl;
    cout << "  predictor type               : pshare" << endl;
    cout << "  htr size                     : " << p_htr->getsize () << endl;
    cout << "  pht size                     : " << p_pht->getsize () << endl;
    if (d_usec == true)
      cout << "  using confidence             : true"  << endl;
    else
      cout << "  using confidence             : false" << endl;
  }

  // return true if the predicate can be predicted

  bool Pshare::isvalid (const t_octa ip, const long slot, 
			const long pred) const {
    // compute hash address
    t_octa addr = (ip + slot) ^ p_htr->gethist ();
    // check for valid pht entry
    return d_usec ? p_pht->isstrong (addr) : true;
  }

  // set the predictor history

  void Pshare::setphst (const t_octa phst) {
    p_htr->sethist (d_phst = phst);
  }
  
  // return the predictor history

  t_octa Pshare::getphst (void) const {
    return p_htr->gethist ();
  }

  // compute the predicate value by index

  bool Pshare::compute (const t_octa ip, const long slot, 
			const long pred) const {
    // check for fixed predicate
    if (pred == 0) return true;
    // compute hash address
    t_octa addr = (ip + slot) ^ p_htr->gethist ();
    // get pht predicate
    bool result = p_pht->istrue (addr);
    // update the speculative history
    p_htr->update (result);
    // return prediction
    return result;
  }

  // update the predicate system by ip, slot, predicate and value

  void Pshare::update (const t_octa ip, const long slot, const long pred, 
		       const bool pval, const t_octa phst) {
    // do nothing with fixed predicate
    if (pred == 0) return;
    // compute hash address
    t_octa addr = (ip + slot) ^ phst;
    // update the pht
    p_pht->update (addr, pval);
  }
}
