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
    d_usec = PP_UCFG;
    p_mpp  = Predicate::mkpr (PP_HYPM);
    p_spp  = Predicate::mkpr (PP_HYPS);
    p_msp  = Predicate::mkpr (PP_HYMP);
    reset ();
  }

  // create a hybrid predictor by context

  Hypp::Hypp (Mtx* mtx) : Predicate (mtx,RESOURCE_PPS) {
    d_type = "hypp";
    d_usec = mtx->getbool ("USE-CONFIDENCE-FLAG");
    string mpp = mtx->getstr ("HYBRID-MASTER-PREDICTOR");
    assert (mpp != d_type);
    string spp = mtx->getstr ("HYBRID-SLAVE-PREDICTOR");
    assert (spp != d_type);
    string msp = mtx->getstr ("HYBRID-META-PREDICTOR");
    assert (msp != d_type);
    p_mpp  = Predicate::mkpr (mpp, mtx);
    p_spp  = Predicate::mkpr (spp, mtx);
    p_msp  = Predicate::mkpr (msp, mtx);
    reset ();
  }
  
  // create a pshare predictor by context and name

  Hypp::Hypp (Mtx* mtx, const string& name) : Predicate (mtx, name) {
    d_type = "hypp";
    d_usec = mtx->getbool ("USE-CONFIDENCE-FLAG");
    string mpp = mtx->getstr ("HYBRID-MASTER-PREDICTOR");
    assert (mpp != d_type);
    string spp = mtx->getstr ("HYBRID-SLAVE-PREDICTOR");
    assert (spp != d_type);
    string msp = mtx->getstr ("HYBRID-META-PREDICTOR");
    assert (msp != d_type);
    p_mpp  = Predicate::mkpr (mpp, mtx);
    p_spp  = Predicate::mkpr (spp, mtx);
    p_msp  = Predicate::mkpr (msp, mtx);
    reset ();
  }

  // destroy this predictor

  Hypp::~Hypp (void) {
    delete p_mpp;
    delete p_spp;
    delete p_msp;
  }

  // reset this branch predictor

  void Hypp::reset (void) {
    p_mpp->reset ();
    p_spp->reset ();
    p_msp->reset ();
  }

  // report some resource information

  void Hypp::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "  resource type                : predicate predictor" << endl;
    cout << "  predictor type               : hybrid" << endl;
    p_mpp->report ();
    p_spp->report ();
    p_msp->report ();
  }

  // return true if the predicate can be predicted

  bool Hypp::isvalid (const t_octa ip, const long slot, 
		      const long pred) const {
    return d_usec ? p_msp->isvalid (ip, slot, pred) : true;
  }

  // compute the predicate value with the selector

  bool Hypp::compute (const t_octa ip, const long slot, 
		      const long pred) const {
    // compute meta selector
    bool meta = p_msp->compute (ip, slot, pred);
    // get result from selector
    if (meta == true) {
      return p_mpp->compute (ip, slot, pred);
    } else {
      return p_spp->compute (ip, slot, pred);
    }
  }

  // update the predicate system by ip, slot, predicate and value

  void Hypp::update (const t_octa ip, const long slot, const long pred, 
		     const bool pval, const bool bflg) {
    p_mpp->update (ip, slot, pred, pval, bflg);
    p_spp->update (ip, slot, pred, pval, bflg);
    p_msp->update (ip, slot, pred, pval, bflg);
  }
}
