// ---------------------------------------------------------------------------
// - Hypp.cpp                                                                -
// - iato:mac library - hybrid predicate predictor implementation            -
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
#include "Hypp.hpp"

namespace iato {

  // create a default hybrid predictor

  Hypp::Hypp (void) : Predicate (RESOURCE_PPS) {
    d_type = "hypp";
    p_mpp  = Predicate::mkpr (PP_HYPM);
    p_spp  = Predicate::mkpr (PP_HYPS);
    reset ();
  }

  // create a hybrid predictor by context

  Hypp::Hypp (Mtx* mtx) : Predicate (mtx,RESOURCE_PPS) {
    d_type = "hypp";
    string mpp = mtx->getstr ("HYBRID-MASTER-PREDICTOR");
    assert (mpp != d_type);
    string spp = mtx->getstr ("HYBRID-SLAVE-PREDICTOR");
    assert (spp != d_type);
    p_mpp  = Predicate::mkpr (mpp, mtx);
    p_spp  = Predicate::mkpr (spp, mtx);
    reset ();
  }
  
  // create a pshare predictor by context and name

  Hypp::Hypp (Mtx* mtx, const string& name) : Predicate (mtx, name) {
    d_type = "hypp";
    string mpp = mtx->getstr ("HYBRID-MASTER-PREDICTOR");
    assert (mpp != d_type);
    string spp = mtx->getstr ("HYBRID-SLAVE-PREDICTOR");
    assert (spp != d_type);
    p_mpp  = Predicate::mkpr (mpp, mtx);
    p_spp  = Predicate::mkpr (spp, mtx);
    reset ();
  }

  // destroy this predictor

  Hypp::~Hypp (void) {
    delete p_mpp;
    delete p_spp;
  }

  // reset this branch predictor

  void Hypp::reset (void) {
    p_mpp->reset ();
    p_spp->reset ();
  }

  // report some resource information

  void Hypp::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "  resource type                : predicate predictor" << endl;
    cout << "  predictor type               : hybrid" << endl;
    p_mpp->report ();
    p_spp->report ();
  }

  // return true if the predicate can be predicted

  bool Hypp::isvalid (const t_octa ip, const long slot, 
		      const long pred) const {
    // get master result
    bool mflg = p_mpp->isvalid (ip, slot, pred);
    if (mflg == false) return false;
    // get slave result
    bool sflg = p_spp->isvalid (ip, slot, pred);
    if (sflg == false) return false;
    // compute the value
    bool mval = p_mpp->compute (ip, slot, pred);
    bool sval = p_spp->compute (ip, slot, pred);
    if (mval != sval) return false;
    // ok, both agree
    return true;
  }

  // compute the predicate value with the master

  bool Hypp::compute (const t_octa ip, const long slot, 
		      const long pred) const {
    return p_mpp->compute (ip, slot,pred);
  }

  // update the predicate system by ip, slot, predicate and value

  void Hypp::update (const t_octa ip, const long slot, const long pred, 
		     const bool pval, const bool bflg) {
    p_mpp->update (ip, slot, pred, pval, bflg);
    p_spp->update (ip, slot, pred, pval, bflg);
  }
}
