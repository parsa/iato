// ---------------------------------------------------------------------------
// - Ghpp.cpp                                                                -
// - iato:mac library - ghpp predicate predictor class implementation        - 
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
#include "Ghpp.hpp"

namespace iato {

  // create a default ghpp predictor

  Ghpp::Ghpp (void) : Predicate (RESOURCE_PPS) {
    d_type = "ghpp";
    d_size = PR_LRSZ;
    p_htr  = new Htr;
    p_ppt  = new Perceptron[d_size];
    reset ();
  }

  // create a predictor by context

  Ghpp::Ghpp (Mtx* mtx) : Predicate (mtx,RESOURCE_PPS) {
    d_type = "ghpp";
    d_size = mtx->getlong ("LR-GR-SIZE"); assert (d_size > 0);
    p_htr  = new Htr (mtx);
    p_ppt  = new Perceptron[d_size] (mtx);
    reset ();
  }
  
  // create a predictor by context and name

  Ghpp::Ghpp (Mtx* mtx, const string& name) : Predicate (mtx, name) {
    d_type = "ghpp";
    d_size = mtx->getlong ("LR-GR-SIZE"); assert (d_size > 0);
    p_htr  = new Htr (mtx);
    p_ppt  = new Perceptron[d_size] (mtx);
    reset ();
  }

  // destroy this predictor

  Ghpp::~Ghpp (void) {
    delete p_htr;
    delete [] p_ppt;
  }

  // reset this branch predictor

  void Ghpp::reset (void) {
    p_htr->reset ();
    for (long i = 0; i < d_size; i++) p_ppt[i].reset ();
  }

  // report some resource information

  void Ghpp::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "  resource type                : predicate predictor" << endl;
    cout << "  predictor type               : " << "ghpp" << endl;
    cout << "  predictor size               : " << d_size << endl;
    cout << "  history size                 : " << p_htr->getsize ()   << endl;
    cout << "  perceptron size              : " << p_ppt[0].getsize () << endl;
    cout << "  perceptron weight bound      : " << p_ppt[0].getwbnd () << endl;
    cout << "  perceptron threshold         : " << p_ppt[0].getpthr () << endl;
    cout << "  perceptron confidence bound  : " << p_ppt[0].getpcbn () << endl;
  } 

  // return true if the predicate can be predicted

  bool Ghpp::isvalid (const t_octa ip, const long slot, 
		      const long pred) const {
    // get history mask
    t_octa hist = p_htr->gethist ();
    // compute perceptron index
    long index = pred % d_size;
    // check for validity
    return p_ppt[index].isvalid (hist);
  }

  // compute the predicate value by index

  bool Ghpp::compute (const t_octa ip, const long slot, 
			 const long pred) const {
    // check for fixed predicate
    if (pred == 0) return true;
    // get history mask
    t_octa hist = p_htr->gethist ();
    // compute perceptron index
    long index = pred % d_size;
    // compute value
    return p_ppt[index].istrue (hist);
  }

  // update the predicate system by ip, slot, predicate and value

  void Ghpp::update (const t_octa ip, const long slot, const long pred, 
		     const bool pval, const bool bflg) {
    // do nothing with fixed predicate
    if (pred == 0) return;
    // update history mask
    t_octa hist = p_htr->update (pval);
    // compute perceptron index
    long index = pred % d_size;
    // update the perceptron
    p_ppt[index].update (hist);
  }
}
