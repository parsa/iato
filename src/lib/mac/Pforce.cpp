// ---------------------------------------------------------------------------
// - Pforce.cpp                                                              -
// - iato:mac library - forced predicate prediction class implementation     - 
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
#include "Pforce.hpp"

namespace iato {

  // create a default pforce predictor

  Pforce::Pforce (void) : Predicate (RESOURCE_PPS) {
    d_type = "pforce";
    d_fval = PP_FVAL;
  }

  // create a pforce predictor by context

  Pforce::Pforce (Mtx* mtx) : Predicate (mtx,RESOURCE_PPS) {
    d_type = "pforce";
    d_fval = mtx->getbool ("PREDICATE-FORCE-VALUE");
  }
  
  // create a pforce predictor by context and name

  Pforce::Pforce (Mtx* mtx, const string& name) : Predicate (mtx, name) {
    d_type = "pforce";
    d_fval = mtx->getbool ("PREDICATE-FORCE-VALUE");
  }

  // report some resource information

  void Pforce::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "  resource type                : predicate predictor" << endl;
    cout << "  predictor type               : pforce" << endl;
    if (d_fval == true)
      cout << "  forcing value                : true" << endl;
    else
      cout << "  forcing value                : false" << endl;
  }

  // return true if the predicate can be predicted

  bool Pforce::isvalid (const t_octa ip, const long slot,
			const long pred) const {
    return true;
  }

  // compute the predicate value by index

  bool Pforce::compute (const t_octa ip, const long slot, 
			 const long pred) const {
    // check for fixed predicate
    if (pred == 0) return true;
    // get pht predicate
    return d_fval;
  }
}
