// ---------------------------------------------------------------------------
// - Sct.hpp                                                                 -
// - iato:mac library - saturating 2 bits counter class implementation       -
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

#include "Sct.hpp"

namespace iato {

   // create a default counter
  
  Sct::Sct  (void) {
    d_sinit = STRONG_F;
    d_state = STRONG_F;
  }

  // create a counter with an initial state

  Sct::Sct (t_sct state) {
    d_sinit = state;
    d_state = state;
  }

  // copy construct this counter

  Sct::Sct (const Sct& that) {
    d_sinit = that.d_sinit;
    d_state = that.d_state;
  }

  // reset this counter
  
  void Sct::reset (void) {
    d_state = d_sinit;
  }

  // assign a counter to this one

  Sct& Sct::operator = (const Sct& that) {
    d_sinit = that.d_sinit;
    d_state = that.d_state;
    return *this;
  }

  // return true if the state is strong
  
  bool Sct::isstrong (void) const {
    bool result = false;
    switch (d_state) {
    case STRONG_F:
    case STRONG_T:
      result = true;
      break;
    default:
      result = false;
      break;
    }
    return result;
  }

  // return true if the state is weak
  
  bool Sct::isweak (void) const {
    bool result = false;
    switch (d_state) {
    case WEAKLY_F:
    case WEAKLY_T:
      result = true;
      break;
    default:
      result = false;
      break;
    }
    return result;
  }

  // return true if the state is true
  
  bool Sct::istrue (void) const {
    bool result = false;
    switch (d_state) {
    case WEAKLY_T:
    case STRONG_T:
      result = true;
      break;
    default:
      result = false;
      break;
    }
    return result;
  }

  // update a counter with a flag
  
  void Sct::update (const bool flag) {
    switch (d_state) {
    case STRONG_F:
      d_state = flag ? WEAKLY_F : STRONG_F;
      break;
    case WEAKLY_F:
      d_state = flag ? WEAKLY_T : STRONG_F;
      break;
    case WEAKLY_T:
      d_state = flag ? STRONG_T : WEAKLY_F;
      break;
    case STRONG_T:
      d_state = flag ? STRONG_T : WEAKLY_T;
      break;
    }
  };
}
