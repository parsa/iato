// ---------------------------------------------------------------------------
// - Pskew.cpp                                                              -
// - iato:mac library - global history pred predictor class implementation   -
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
#include "Pskew.hpp"

namespace iato {

  // This procedure implements the H function from PARLE 93 paper
  // this function operates with 64 bits representation
  static inline t_octa H (const t_octa v) {
    t_octa msb = (v ^ (v << 63)) & 0x8000000000000000ULL;
    t_octa res = (v & 0x7FFFFFFFFFFFFFFFULL) >> 1;
    return msb|res;
  }

  // This procedure implements the inverse H function from PARLE 93 paper
  // this function operates with 64 bits representation
  static inline t_octa Hi (const t_octa v) {
    t_octa lsb = ((v >> 63) ^ (v >> 62)) & OCTA_1;
    t_octa res = v << 1;
    return res|lsb;
  }

  // these procedures implement the skewing functions - with these 64 bits
  // arguments, x is the address and y is the history
  static inline t_octa F0 (const t_octa x, const t_octa y) {
    return H (x) ^ Hi (y) ^ y;
  }
  static inline t_octa F1 (const t_octa x, const t_octa y) {
    return H (x) ^ Hi (y) ^ x;
  }
  static inline t_octa F2 (const t_octa x, const t_octa y) {
    return Hi (x) ^ H (y) ^ y;
  }

  // this function computes the majority flag
  static bool majority (const bool p0, const bool p1, const bool p2) {
    return (p0 & p1) | (p0 & p2) | (p1 & p2);
  }

  // create a default pskew predictor

  Pskew::Pskew (void) : Predicate (RESOURCE_PPS) {
    d_type = "pskew";
    d_usec = PP_UCFG;
    p_htr  = new Htr;
    p_pht.reserve(3);
    for (size_t i = 0; i < 3; ++i)
    {
      p_pht.emplace_back(new Pht());
    }
    reset ();
  }

  // create a pskew predictor by context

  Pskew::Pskew (Mtx* mtx) : Predicate (mtx,RESOURCE_PPS) {
    d_type = "pskew";
    d_usec = mtx->getbool ("USE-CONFIDENCE-FLAG");
    p_htr  = new Htr    (mtx);
    p_pht.reserve(3);
    for (size_t i = 0; i < 3; ++i)
    {
      p_pht.emplace_back(new Pht(mtx));
    }
    reset ();
  }
  
  // create a pskew predictor by context and name

  Pskew::Pskew (Mtx* mtx, const string& name) : Predicate (mtx, name) {
    d_type = "pskew";
    d_usec = mtx->getbool ("USE-CONFIDENCE-FLAG");
    p_htr  = new Htr    (mtx);
    p_pht.reserve(3);
    for (size_t i = 0; i < 3; ++i)
    {
      p_pht.emplace_back(new Pht(mtx));
    }
    reset ();
  }

  // destroy this predictor

  Pskew::~Pskew (void) {
    delete    p_htr;
    for (Pht* i : p_pht)
    {
      delete i;
    }
  }

  // reset this branch predictor

  void Pskew::reset (void) {
    p_htr->reset   ();
    p_pht[0]->reset ();
    p_pht[1]->reset ();
    p_pht[2]->reset ();
  }

  // report some resource information

  void Pskew::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "  resource type                : predicate predictor" << endl;
    cout << "  predictor type               : pskew" << endl;
    cout << "  htr size                     : " << p_htr->getsize () << endl;
    cout << "  pht size                     : " << p_pht[0]->getsize () << endl;
    if (d_usec == true)
      cout << "  using confidence             : true"  << endl;
    else
      cout << "  using confidence             : false" << endl;
  }

  // return true if the predicate can be predicted

  bool Pskew::isvalid (const t_octa ip, const long slot, 
		       const long pred) const {
    // adjust the address and get the history
    t_octa addr = ip + slot;
    t_octa hist = p_htr->gethist ();
    // compute pht indexes
    t_octa A0 = F0 (addr, hist);
    t_octa A1 = F1 (addr, hist);
    t_octa A2 = F2 (addr, hist);
    // get pht flags
    bool p0 = d_usec ? p_pht[0]->isstrong (A0) : true;
    bool p1 = d_usec ? p_pht[1]->isstrong (A1) : true;
    bool p2 = d_usec ? p_pht[2]->isstrong (A2) : true;
    // compute majority
    return majority (p0, p1, p2);
  }

  // set the predictor history

  void Pskew::setphst (const t_octa phst) {
    p_htr->sethist (d_phst = phst);
  }
  
  // return the predictor history

  t_octa Pskew::getphst (void) const {
    return p_htr->gethist ();
  }

  // compute the predicate value by index

  bool Pskew::compute (const t_octa ip, const long slot, 
		       const long pred) const {
    // check for fixed predicate
    if (pred == 0) return true;
    // adjust the address and get the history
    t_octa addr = ip + slot;
    t_octa hist = p_htr->gethist ();
    // compute pht indexes
    t_octa A0 = F0 (addr, hist);
    t_octa A1 = F1 (addr, hist);
    t_octa A2 = F2 (addr, hist);
    // get pht flags
    bool p0 = d_usec ? p_pht[0]->istrue (A0) : true;
    bool p1 = d_usec ? p_pht[1]->istrue (A1) : true;
    bool p2 = d_usec ? p_pht[2]->istrue (A2) : true;
    // compute majority
    bool result = majority (p0, p1, p2);
    // update speculative history
    p_htr->update (result);
    // return prediction
    return result;
  }

  // update the predicate system by ip, slot, predicate and value

  void Pskew::update (const t_octa ip, const long slot, const long pred, 
		      const bool pval, const t_octa phst) {
    // do nothing with fixed predicate
    if (pred == 0) return;
    // adjust the address and get the history
    t_octa addr = ip + slot;
    // compute pht indexes
    t_octa A0 = F0 (addr, phst);
    t_octa A1 = F1 (addr, phst);
    t_octa A2 = F2 (addr, phst);    
    // update the pht
    p_pht[0]->update (A0, pval);
    p_pht[1]->update (A1, pval);
    p_pht[2]->update (A2, pval);
  }
}
