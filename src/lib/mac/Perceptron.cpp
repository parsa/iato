// ---------------------------------------------------------------------------
// - Perceptron.cpp                                                          -
// - iato:mac library - perceptron element class implementation              -
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
#include "Bits.hpp"
#include "Perceptron.hpp"

namespace iato {

  // this function compute the absolute value
  static inline long abs (const long x) {
    return x > 0 ? x : -x;
  }

  // create a default perceptron

  Perceptron::Perceptron (void) {
    d_size = PP_PSIZ;
    p_whts = new long[d_size];
    d_wbnd = PP_PBND;
    d_pthr = PP_PTHR;
    d_pcbn = PP_PCBN;
  }

  // create a perceptron with a context

  Perceptron::Perceptron (Mtx* mtx) {
    d_size = mtx->getlong ("PERCEPTRON-SIZE"); assert (d_size > 0);
    p_whts = new long[d_size];
    d_wbnd = mtx->getlong ("PERCEPTRON-WEIGHT-BOUND");
    d_pthr = mtx->getlong ("PERCEPTRON-THRESHOLD");
    d_pcbn = mtx->getlong ("PERCEPTRON-CONFIDENCE");
  }

  // destroy this perceptron

  Perceptron::~Perceptron (void) {
    delete [] p_whts;
  }

  // reset this perceptron

  void Perceptron::reset (void) {
    for (long i = 0; i < d_size; i++) p_whts[i] = 0;
  }

  // return the perceptron size

  long Perceptron::getsize (void) const {
    return d_size;
  }

  // return the perceptron weight bound

  long Perceptron::getwbnd (void) const {
    return d_wbnd;
  }

  // return the perceptron threshold

  long Perceptron::getpthr (void) const {
    return d_pthr;
  }

  // return the perceptron confidence bound

  long Perceptron::getpcbn (void) const {
    return d_pcbn;
  }

  // return true if the perceptron is value is valid

  bool Perceptron::isvalid (const t_octa hist) const {
    long pval = abs (compute (hist));
    return pval > d_pcbn;
  }

  // return true if the perceptron is true

  bool Perceptron::istrue (const t_octa hist) const {
    long pval = compute (hist);
    return pval > 0;
  }

  // compute the perceptron value

  long Perceptron::compute (const t_octa hist) const {
    long result = 0;
    // compute result with hist
    for (long i = 0; i < d_size; i++) {
      bool hbit = bsetget (hist, i);
      if (hbit == true) result += p_whts[i];
    }
    return result;
  }

  // update the perceptron - bit 0 contains the new value

  void Perceptron::update (const t_octa hist) {
    // get the bit 0 and perceptron value
    bool flag = bsetget (hist, 0);
    if ((isvalid (hist) == true) && (istrue (hist) == flag)) return;
    // if the perceptron value is bigger than the threshold
    // the training is stopped
    if (abs (compute (hist)) > d_pthr) return;
    // update the perceptron
    for (long i = 0; i < d_size; i++) {
      bool hbit = bsetget (hist, i);
      p_whts[i] = hbit ? p_whts[i] + 1 : p_whts[i] - 1;
      if (p_whts[i] >  d_wbnd) p_whts[i] =  d_wbnd;
      if (p_whts[i] < -d_wbnd) p_whts[i] = -d_wbnd;
    }
  }
}
