// ---------------------------------------------------------------------------
// - Ppag.cpp                                                                -
// - iato:mac library - per predicate history pred predictor implementation  -
// ---------------------------------------------------------------------------
// - (c) inria 2002-2004                                                     -
// ---------------------------------------------------------------------------
// - authors                                      Amaury Darsch    2002:2004 -
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
#include "Ppag.hpp"

namespace iato {

  // create a default pshare predictor

  Ppag::Ppag (void) : Predicate (RESOURCE_PPS) {
    d_type = "pshare";
    d_size = PR_LRSZ;
    d_usec = PP_UCFG;
    d_bhuo = PP_BHUO;
    p_htr  = new Htr[d_size];
    p_pht  = new Pht;
    reset ();
  }

  // create a pshare predictor by context

  Ppag::Ppag (Mtx* mtx) : Predicate (mtx,RESOURCE_PPS) {
    d_type = "pshare";
    d_size = mtx->getlong ("LR-PR-SIZE"); assert (d_size > 0);
    d_usec = mtx->getbool ("USE-CONFIDENCE-FLAG");
    d_bhuo = mtx->getbool ("BRANCH-HISTORY-UPDATE-ONLY");
    p_htr  = new Htr[d_size](mtx);
    p_pht  = new Pht (mtx);
    reset ();
  }
  
  // create a pshare predictor by context and name

  Ppag::Ppag (Mtx* mtx, const string& name) : Predicate (mtx, name) {
    d_type = "pshare";
    d_size = mtx->getlong ("LR-PR-SIZE"); assert (d_size > 0);
    d_usec = mtx->getbool ("USE-CONFIDENCE-FLAG");
    d_bhuo = mtx->getbool ("BRANCH-HISTORY-UPDATE-ONLY");
    p_htr  = new Htr[d_size](mtx);
    p_pht  = new Pht (mtx);
    reset ();
  }

  // destroy this predictor

  Ppag::~Ppag (void) {
    delete [] p_htr;
    delete p_pht;
  }

  // reset this branch predictor

  void Ppag::reset (void) {
    for (long i = 0; i < d_size; i++) p_htr[i].reset ();
    p_pht->reset ();
  }

  // report some resource information

  void Ppag::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "  resource type                : predicate predictor" << endl;
    cout << "  predictor type               : ppag" << endl;
    cout << "  predicate length             : " << d_size << endl;
    cout << "  htr size                     : " << p_htr->getsize () << endl;
    cout << "  pht size                     : " << p_pht->getsize () << endl;
    if (d_usec == true)
      cout << "  using confidence             : true" << endl;
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

  bool Ppag::isvalid (const t_octa ip, const long slot, 
			const long pred) const {
    // compute hash address
    t_octa addr = ip ^ p_htr[pred].gethist ();
    // check for valid pht entry
    return d_usec ? p_pht->isstrong (addr, slot) : true;
  }

  // compute the predicate value by index

  bool Ppag::compute (const t_octa ip, const long slot, 
			const long pred) const {
    // check for fixed predicate
    if (pred == 0) return true;
    // compute hash address
    t_octa addr = ip ^ p_htr[pred].gethist ();
    // get pht predicate
    return p_pht->istrue (addr,slot);
  }

  // update the predicate system by ip, slot, predicate and value

  void Ppag::update (const t_octa ip, const long slot, const long pred, 
		     const bool pval, const bool bflg) {
    // do nothing with fixed predicate
    if (pred == 0) return;
    // compute hash address
    t_octa addr = ip ^ p_htr[pred].gethist ();
    // update the pht
    p_pht->update (addr, slot, pval);
    // update the history
    if ((!d_bhuo | bflg) == true) p_htr[pred].update (pval);
  }
}
