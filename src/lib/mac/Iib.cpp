// ---------------------------------------------------------------------------
// - Iib.cpp                                                                 -
// - iato:mac library - instruction result buffer class implementation       -
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

#include "Iib.hpp"
#include "Mac.hpp"
#include "Prn.hpp"

namespace iato {

  // create a new buffer
  
  Iib::Iib (void) : Resource (RESOURCE_IIB) {
    d_size = IB_SIZE;
    d_ithr = d_size - BN_IWSZ * BN_SLSZ; assert (d_ithr > 0);
    p_vldb = new bool[d_size];
    p_intr = new Interrupt[d_size];
    reset ();
  }

  // create a new buffer with a context
  
  Iib::Iib (Mtx* mtx) : Resource (RESOURCE_IIB) {
    d_size = mtx->getlong ("IIB-SIZE"); assert (d_size > 0);
    d_ithr = d_size - mtx->getswsz ();  assert (d_ithr > 0);
    p_vldb = new bool[d_size];
    p_intr = new Interrupt[d_size];
    reset ();
  }

  // destroy this buffer

  Iib::~Iib (void) {
    delete [] p_vldb;
    delete [] p_intr;
  }

  // reset this buffer

  void Iib::reset (void) {
    d_ilen = 0;
    for (long i = 0; i < d_size; i++) {
      p_vldb[i] = false;
      p_intr[i].reset ();
    }
  }

  // partial flush this buffer

  void Iib::pflsh (void) {
    for (long i = 0; i < d_size; i++) {
      if (p_vldb[i] == false) continue;
      if (p_intr[i].isexec () == false) continue;
      clear (i);
    }
  }

  // report this resource

  void Iib::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "instruction interrupt buffer" << endl;
    cout << "\tiib size     \t\t: " << d_size << endl;
  }

  // return true if the buffer is empty

  bool Iib::isempty (void) const {
    return (d_ilen == 0);
  }

  // return true if the buffer is full

  bool Iib::isfull (void) const {
    return (d_ilen == d_size);
  }
  
  // return true if the buffer is at the threshold

  bool Iib::isthr (void) const {
    return (d_ilen >= d_ithr);
  }
  
  // check if the buffer entry is valid

  bool Iib::isvalid (const long index) const {
    assert ((index >= 0) && (index < d_size));
    return p_vldb[index];
  }

  // clear the buffer entry by index

  void Iib::clear (const long index) {
    assert ((index >= 0) && (index < d_size));
    p_vldb[index] = false;
    p_intr[index].reset ();
    if (d_ilen > 0) d_ilen--;
  }
  
  // allocate a new buffer entry

  long Iib::alloc (void) {
    // find a new entry
    for (long i = 0; i < d_size; i++) {
      if (p_vldb[i] == false) {
	p_vldb[i] = true;
	p_intr[i].reset ();
	d_ilen++;
	return i;
      }
    }
    return -1;
  }

  // allocate a new entry with an interrupt

  long Iib::alloc (const Interrupt& vi) {
    long index = alloc ();
    if (index != -1) setintr (index, vi);
    return index;
  }

  // get the buffer interrupt by index

  Interrupt Iib::getintr (const long index) const {
    assert ((index >= 0) && (index < d_size));
    assert (p_vldb[index] == true);
    return p_intr[index];
  }

  // set an interrupt by index

  void Iib::setintr (const long index, const Interrupt& vi) {
    assert ((index >= 0) && (index < d_size));
    assert (p_vldb[index] == true);
    p_intr[index] = vi;
  }

  // set an interrupt by index with an exec flag

  void Iib::setintr (const long index, const Interrupt& vi, const bool flag) {
    assert ((index >= 0) && (index < d_size));
    assert (p_vldb[index] == true);
    p_intr[index] = vi;
    p_intr[index].setexec (flag);
  }
}
