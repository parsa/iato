// ---------------------------------------------------------------------------
// - Pht.cpp                                                                 -
// - iato:mac library - pattern history table class implementation           -
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
#include "Pht.hpp"

namespace iato {

  // create a default pht

  Pht::Pht (void) : Resource (RESOURCE_PHT) {
    d_size = PS_PHTS;
    p_vsct = new Sct[d_size];
    reset ();
  }

  // create a pht with a size

  Pht::Pht (const long size) : Resource (RESOURCE_PHT) {
    d_size = size; assert (d_size > 0);
    p_vsct = new Sct[d_size];
    reset ();
  }

  // create a pht with a context

  Pht::Pht (Mtx* mtx) : Resource (RESOURCE_PHT) {
    d_size = mtx->getlong ("PHT-SIZE"); assert (d_size > 0);
    p_vsct = new Sct[d_size];
    reset ();
  }

  // destroy this pht
  
  Pht::~Pht (void) {
    delete [] p_vsct;
  }

  // reset this pht
  
  void Pht::reset (void) {
    for (long i = 0; i < d_size; i++) p_vsct[i].reset ();
  }

  // return the pht size

  long Pht::getsize (void) const {
    return d_size;
  }

  // return a hashed index by address

  long Pht::hash (const t_octa addr) const {
    long result = addr % (t_octa) d_size;
    return result;
  }

  // return true if the prediction is strong by index

  bool Pht::isstrong (const long index) const {
    assert ((index >= 0) && (index < d_size));
    return p_vsct[index].isstrong ();
  }

  // return true if the prediction is strong by address

  bool Pht::isstrong (const t_octa addr) const {
    long index = hash (addr);
    return p_vsct[index].isstrong ();
  }

  // return true if the prediction is weak by index

  bool Pht::isweak (const long index) const {
    assert ((index >= 0) && (index < d_size));
    return p_vsct[index].isweak ();
  }

  // return true if the prediction is weak by address

  bool Pht::isweak (const t_octa addr) const {
    long index = hash (addr);
    return p_vsct[index].isweak ();
  }

  // return true if the prediction is true by index

  bool Pht::istrue (const long index) const {
    assert ((index >= 0) && (index < d_size));
    return p_vsct[index].istrue ();
  }

  // return true if the prediction is true by address

  bool Pht::istrue (const t_octa addr) const {
    long index = hash (addr);
    return p_vsct[index].istrue ();
  }

  // update the pht by index

  void Pht::update (const long index, const bool flag) {
    assert ((index >= 0) && (index < d_size));
    p_vsct[index].update (flag);
  }

  // update the pht by address

  void Pht::update (const t_octa addr, const bool flag) {
    long index = hash (addr);
    p_vsct[index].update (flag);
  }
}
