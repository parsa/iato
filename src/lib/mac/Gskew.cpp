// ---------------------------------------------------------------------------
// - Gskew.cpp                                                               -
// - iato:mac library - skewed branch predictor class implementation         -
// ---------------------------------------------------------------------------
// - (c) inria 2002-2004                                                     -
// ---------------------------------------------------------------------------
// - author                                       Andre  Seznec    2002:2004 -
// - adapted for iato by                          Amaury Darsch    2002:2003 -
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

#include "Prn.hpp"
#include "Gskew.hpp"

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

  // create a default gskew predictor

  Gskew::Gskew (void) : Branch (RESOURCE_BPS) {
    p_htr = new Htr;
    p_btb = new Btb;
    p_pht.reserve(3);
    for (size_t i = 0; i < 3; ++i)
    {
      p_pht.emplace_back(new Pht());
    }
    reset ();
  }

  // create a branch prediction by context

  Gskew::Gskew (Mtx* mtx) : Branch (mtx,RESOURCE_BPS) {
    p_htr = new Htr    (mtx);
    p_btb = new Btb    (mtx);
    p_pht.reserve(3);
    for (size_t i = 0; i < 3; ++i)
    {
      p_pht.emplace_back(new Pht(mtx));
    }
    reset ();
  }
  
  // create a branch prediction by context and name

  Gskew::Gskew (Mtx* mtx, const string& name) : Branch (mtx, name) {
    p_htr = new Htr    (mtx);
    p_btb = new Btb    (mtx);
    p_pht.reserve(3);
    for (size_t i = 0; i < 3; ++i)
    {
      p_pht.emplace_back(new Pht(mtx));
    }
    reset ();
  }

  // destroy this predictor

  Gskew::~Gskew (void) {
    delete    p_htr;
    delete    p_btb;
    for (Pht* i : p_pht)
    {
      delete i;
    }
  }

  // reset this branch predictor

  void Gskew::reset (void) {
    p_htr->reset   ();
    p_btb->reset   ();
    p_pht[0]->reset ();
    p_pht[1]->reset ();
    p_pht[2]->reset ();
  }

  // report some resource information

  void Gskew::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: branch predictor" << endl;
    cout << "\tpredictor type \t\t: " << "gskew" << endl;
    cout << "\thtr size       \t\t: " << p_htr->getsize   () << endl;
    cout << "\tbtb size       \t\t: " << p_btb->getsize   () << endl;
    cout << "\tpht size       \t\t: " << p_pht[0]->getsize () << endl;
  }

  // return true if the branch is predicted taken

  bool Gskew::istaken (const t_octa cip) const {
    // adjust the address and get the history
    t_octa addr = cip >> 4;
    t_octa hist = p_htr->gethist ();
    // compute pht indexes
    t_octa A0 = F0 (addr, hist);
    t_octa A1 = F1 (addr, hist);
    t_octa A2 = F2 (addr, hist);
    // get pht flags
    bool p0 = p_pht[0]->istrue (A0);
    bool p1 = p_pht[1]->istrue (A1);
    bool p2 = p_pht[2]->istrue (A2);
    // compute majority
    return majority (p0, p1, p2);
  }

  // return true if the branch can be predicted

  bool Gskew::ispredict (const t_octa cip) const {
    // compute taken branch and check btb if taken
    bool result = istaken (cip);
    if ((result == true) && (p_btb->isvalid (cip) == false)) return false;
    // update the speculative history
    p_htr->update (result);
    // return the result
    return result;
  }

  // set the predictor history

  void Gskew::sethist (const t_octa hist) {
    p_htr->sethist (d_hist = hist);
  }
  
  // return the predictor history

  t_octa Gskew::gethist (void) const {
    return p_htr->gethist ();
  }

  // predict the next ip from the current ip

  t_octa Gskew::predict (const t_octa cip) {
    // compute next ip for safety
    t_octa nip = cip + BN_BYSZ;
    // check in the btb to see if the address is available
    // if not return the next ip (use at your own risk)
    if (p_btb->isvalid (cip) == false) return nip;
    // get the target address from the btb
    return p_btb->gettrg (cip);
  }

  // update the branch system with an ip and next ip

  void Gskew::update (const t_octa cip, const bool btk, const t_octa nip, 
		      const t_octa hst) {
    // adjust the address
    t_octa addr = cip >> 4;
    // compute pht indexes
    t_octa A0 = F0 (addr, hst);
    t_octa A1 = F1 (addr, hst);
    t_octa A2 = F2 (addr, hst);
    // update the pht by address
    p_pht[0]->update (A0, btk);
    p_pht[1]->update (A1, btk);
    p_pht[2]->update (A2, btk);
    // update the btb
    if (btk == true) p_btb->update (cip, nip);
  }
}
