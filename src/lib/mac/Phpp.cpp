// ---------------------------------------------------------------------------
// - Phpp.cpp                                                                -
// - iato:mac library - phpp predicate predictor class implementation        - 
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
#include "Phpp.hpp"

namespace iato {

  // create a default phpp predictor

  Phpp::Phpp (void) : Predicate (RESOURCE_PPS) {
    d_type = "phpp";
    d_size = PR_LRSZ;
    p_htr  = new Htr[d_size];
    p_ppt  = new Perceptron[d_size];
    reset ();
  }

  // create a predictor by context

  Phpp::Phpp (Mtx* mtx) : Predicate (mtx,RESOURCE_PPS) {
    d_type = "phpp";
    d_size = mtx->getlong ("LR-PR-SIZE"); assert (d_size > 0);
    p_htr  = new Htr[d_size] (mtx);
    p_ppt  = new Perceptron[d_size] (mtx);
    reset ();
  }
  
  // create a predictor by context and name

  Phpp::Phpp (Mtx* mtx, const string& name) : Predicate (mtx, name) {
    d_type = "phpp";
    d_size = mtx->getlong ("LR-PR-SIZE"); assert (d_size > 0);
    p_htr  = new Htr[d_size] (mtx);
    p_ppt  = new Perceptron[d_size] (mtx);
    reset ();
  }

  // destroy this predictor

  Phpp::~Phpp (void) {
    delete [] p_htr;
    delete [] p_ppt;
  }

  // reset this branch predictor

  void Phpp::reset (void) {
    for (long i = 0; i < d_size; i++) {
      p_htr[i].reset ();
      p_ppt[i].reset ();
    }
  }

  // report some resource information

  void Phpp::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "  resource type                : predicate predictor" << endl;
    cout << "  predictor type               : " << "phpp" << endl;
    cout << "  predictor size               : " << d_size << endl;
    cout << "  history size                 : " << p_htr->getsize ()   << endl;
    cout << "  perceptron size              : " << p_ppt[0].getsize () << endl;
    cout << "  perceptron weight bound      : " << p_ppt[0].getwbnd () << endl;
    cout << "  perceptron threshold         : " << p_ppt[0].getpthr () << endl;
    cout << "  perceptron confidence bound  : " << p_ppt[0].getpcbn () << endl;
  } 

  // return true if the predicate can be predicted

  bool Phpp::isvalid (const t_octa ip, const long slot, 
		      const long pred) const {
    // compute access index
    long index = pred % d_size;
    // get history mask
    t_octa hist = p_htr[index].gethist ();
    // check for validity
    return p_ppt[index].isvalid (hist);
  }

  // compute the predicate value by index

  bool Phpp::compute (const t_octa ip, const long slot, 
		      const long pred) const {
    // check for fixed predicate
    if (pred == 0) return true;
    // compute access index
    long index = pred % d_size;
    // get history mask
    t_octa hist = p_htr[index].gethist ();
    // compute value
    return p_ppt[index].istrue (hist);
  }

  // update the predicate system by ip, slot, predicate and value

  void Phpp::update (const t_octa ip, const long slot, const long pred, 
		     const bool pval, const bool bflg) {
    // do nothing with fixed predicate
    if (pred == 0) return;
    // compute access index
    long index = pred % d_size;
    // update history mask
    t_octa hist = p_htr[index].update (pval);
    // update the perceptron
    p_ppt[index].update (hist);
  }
}
