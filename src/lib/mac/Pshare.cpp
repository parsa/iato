// ---------------------------------------------------------------------------
// - Pshare.cpp                                                             -
// - iato:mac library - global history predicate predictor class definition  -
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
    d_bhuo = PP_BHUO;
    p_htr  = new Htr;
    p_pht  = new Pht;
    reset ();
  }

  // create a pshare predictor by context

  Pshare::Pshare (Mtx* mtx) : Predicate (mtx,RESOURCE_PPS) {
    d_type = "pshare";
    d_usec = mtx->getbool ("USE-CONFIDENCE-FLAG");
    d_bhuo = mtx->getbool ("BRANCH-HISTORY-UPDATE-ONLY");
    p_htr  = new Htr (mtx);
    p_pht  = new Pht (mtx);
    reset ();
  }
  
  // create a pshare predictor by context and name

  Pshare::Pshare (Mtx* mtx, const string& name) : Predicate (mtx, name) {
    d_type = "pshare";
    d_usec = mtx->getbool ("USE-CONFIDENCE-FLAG");
    d_bhuo = mtx->getbool ("BRANCH-HISTORY-UPDATE-ONLY");
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
    if (d_bupd == true)
      cout << "  using branch update          : true"  << endl;
    else
      cout << "  using branch update          : false" << endl;
    if (d_bhuo == true)
      cout << "  branch history update only   : true"  << endl;
    else
      cout << "  branch history update only   : false" << endl;
  }

  // return true if the predicate can be predicted

  bool Pshare::isvalid (const t_octa ip, const long slot, 
			const long pred) const {
    // compute hash address
    t_octa addr = (ip >> 4) ^ p_htr->gethist ();
    // check for valid pht entry
    return d_usec ? p_pht->isstrong (addr) : true;
  }

  // compute the predicate value by index

  bool Pshare::compute (const t_octa ip, const long slot, 
			const long pred) const {
    // check for fixed predicate
    if (pred == 0) return true;
    // compute hash address
    t_octa addr = (ip >> 4) ^ p_htr->gethist ();
    // get pht predicate
    return p_pht->istrue (addr);
  }

  // update the predicate system by ip, slot, predicate and value

  void Pshare::update (const t_octa ip, const long slot, const long pred, 
		       const bool pval, const bool bflg) {
    // do nothing with fixed predicate
    if (pred == 0) return;
    // compute hash address
    t_octa addr = (ip >> 4) ^ p_htr->gethist ();
    // update according to branch only flag
    if (d_bhuo == true) {
      if (bflg == true) {
	p_htr->update (pval);
      } else {
	p_pht->update (addr, pval);
      }
    } else {
      // update the pht
      p_pht->update (addr, pval);
      // update the history
      p_htr->update (pval);
    }
  }
}
