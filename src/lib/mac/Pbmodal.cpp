// ---------------------------------------------------------------------------
// - Pbmodal.cpp                                                             -
// - iato:mac library - address based predicator class implementation        -
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
#include "Pbmodal.hpp"

namespace iato {

  // create a default pbmodal predictor

  Pbmodal::Pbmodal (void) : Predicate (RESOURCE_PPS) {
    d_type = "pbmodal";
    d_usec = PP_UCFG;
    p_pht  = new Pht;
    reset ();
  }

  // create a pbmodal predictor by context

  Pbmodal::Pbmodal (Mtx* mtx) : Predicate (mtx, RESOURCE_PPS) {
    d_type = "pbmodal";
    d_usec = mtx->getbool ("USE-CONFIDENCE-FLAG");
    p_pht  = new Pht;
    reset ();
  }
  
  // create a pbmodal predictor by context and name

  Pbmodal::Pbmodal (Mtx* mtx, const string& name) : Predicate (mtx, name) {
    d_type = "pbmodal";
    d_usec = mtx->getbool ("USE-CONFIDENCE-FLAG");
    p_pht  = new Pht;
    reset ();
  }

  // destroy this predictor

  Pbmodal::~Pbmodal (void) {
    delete p_pht;
  }

  // reset this branch predictor

  void Pbmodal::reset (void) {
    p_pht->reset ();
  }

  // report some resource information

  void Pbmodal::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "  resource type                : predicate predictor" << endl;
    cout << "  predictor type               : pbmodal" << endl;
    cout << "  pht size                     : " << p_pht->getsize () << endl;
    if (d_usec == true)
      cout << "  using confidence             : true" << endl;
    else
      cout << "  using confidence             : false" << endl;
  }

  // return true if the predicate can be predicted

  bool Pbmodal::isvalid (const t_octa ip, const long slot, 
			 const long pred) const {
    if (pred == 0) return true;
    return d_usec ? p_pht->isstrong (ip >> 4) : true;
  }

  // compute the predicate value by index

  bool Pbmodal::compute (const t_octa ip, const long slot, 
			 const long pred) const {
    // check for fixed predicate
    if (pred == 0) return true;
    // get pht predicate
    return p_pht->istrue (ip >> 4);
  }

  // update the predicate system by ip, slot, predicate and value

  void Pbmodal::update (const t_octa ip, const long slot, const long pred, 
			const bool pval, const t_octa phst) {
    // do nothing with fixed predicate
    if (pred == 0) return;
    // update the pht
    p_pht->update (ip >> 4, pval);
  }
}
