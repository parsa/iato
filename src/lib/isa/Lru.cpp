// ---------------------------------------------------------------------------
// - Lru.cpp                                                                 -
// - iato:isa library - least recently used matrix class implmentation       -
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

#include "Lru.hpp"

namespace iato {

  // create a lru matrix by size
  
  Lru::Lru (const long size) {
    d_size = size; assert (d_size > 0);
    p_vlru = new bool*[d_size];
    for (long i = 0; i < d_size; i++) p_vlru[i] = new bool[d_size];
    reset ();
  }

  // destroy this lru matrix

  Lru::~Lru (void) {
    for (long i = 0; i < d_size; i++) delete [] p_vlru[i];
    delete [] p_vlru;
  }

  // reset this matrix
  
  void Lru::reset (void) {
    for (long i = 0; i < d_size; i++) {
      bool* row = p_vlru[i];
      for (long j = 0; j < d_size; j++) row[j] = false;
    }
  }

  // update an access by index

  void Lru::update (const long index) {
    assert ((index >= 0) && (index < d_size));
    // set the row
    bool* row = p_vlru[index];
    for (long j = 0; j < d_size; j++) row[j] = true;
    // set the column
    for (long i = 0; i < d_size; i++) {
      row = p_vlru[i];
      row[index] = false;
    }
  }

  // return the lru index
  
  long Lru::getlru (void) const {
    long index = -1;
    long count = -1;
    for (long i = 0; i < d_size; i++) {
      // get the number of true elements
      bool* row = p_vlru[i];
      long  len = 0;
      for (long j = 0; j < d_size; j++) if (row[j] == true) len ++;
      // update index and counter
      if ((index == -1) || (len < count)) {
	index = i;
	count = len;	
      }
    }
    // return  least index
    return index;
  }

  // return the mru index

  long Lru::getmru (void) const {
    long index = -1;
    long count = -1;
    for (long i = 0; i < d_size; i++) {
      // get the number of true elements
      bool* row = p_vlru[i];
      long  len = 0;
      for (long j = 0; j < d_size; j++) if (row[j] == true) len ++;
      // update index and counter
      if ((index == -1) || (len > count)) {
	index = i;
	count = len;	
      }
    }
    // return most index
    return index;
  }
}
