// ---------------------------------------------------------------------------
// - Pimodal.cpp                                                             -
// - iato:mac library - pimodal predicate prediction class implementation    - 
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
#include "Pimodal.hpp"

namespace iato {

  // create a default pimodal predictor

  Pimodal::Pimodal (void) : Predicate (RESOURCE_PPS) {
    d_type = "pimodal";
    d_size = PR_LRSZ;
    d_usec = PP_UCFG;
    p_pht  = new Pht (d_size);
    reset ();
  }

  // create a pimodal predictor by context

  Pimodal::Pimodal (Mtx* mtx) : Predicate (mtx,RESOURCE_PPS) {
    d_type = "pimodal";
    d_size = mtx->getlong ("LR-PR-SIZE"); assert (d_size > 0);
    d_usec = mtx->getbool ("USE-CONFIDENCE-FLAG");
    p_pht  = new Pht (d_size);
    reset ();
  }
  
  // create a pimodal predictor by context and name

  Pimodal::Pimodal (Mtx* mtx, const string& name) : Predicate (mtx, name) {
    d_type = "pimodal";
    d_size = mtx->getlong ("LR-PR-SIZE"); assert (d_size > 0);
    d_usec = mtx->getbool ("USE-CONFIDENCE-FLAG");
    p_pht  = new Pht (d_size);
    reset ();
  }

  // destroy this predictor

  Pimodal::~Pimodal (void) {
    delete p_pht;
  }

  // reset this branch predictor

  void Pimodal::reset (void) {
    p_pht->reset ();
  }

  // report some resource information

  void Pimodal::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "  resource type                : predicate predictor" << endl;
    cout << "  predictor type               : pimodal" << endl;
    cout << "  pht size                     : " << d_size << endl;
    if (d_usec == true)
      cout << "  using confidence             : true" << endl;
    else
      cout << "  using confidence             : false" << endl;
  }

  // return true if the predicate can be predicted

  bool Pimodal::isvalid (const t_octa ip, const long slot, 
			 const long pred) const {
    return d_usec ? p_pht->isstrong (pred) : true;
  }

  // compute the predicate value by index

  bool Pimodal::compute (const t_octa ip, const long slot, 
			 const long pred) const {
    // check for fixed predicate
    if (pred == 0) return true;
    // get pht predicate
    return p_pht->istrue (pred);
  }

  // update the predicate system by ip, slot, predicate and value

  void Pimodal::update (const t_octa ip, const long slot, const long pred, 
			const bool pval, const t_octa phst) {
    // do nothing with fixed predicate
    if (pred == 0) return;
    // update the pht
    p_pht->update (pred, pval);
  }
}
