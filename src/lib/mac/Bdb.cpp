// ---------------------------------------------------------------------------
// - Bdb.cpp                                                                 -
// - iato:mac library - bundle decoupling buffer class implementation        -
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
#include "Bdb.hpp"
#include "Exception.hpp"

namespace iato {

  // create a new buffer
  
  Bdb::Bdb (void) : Resource (RESOURCE_BDB) {
    d_dbsz = DB_SIZE;
    d_iwsz = BN_IWSZ;
    d_size = d_dbsz * d_iwsz;
    d_bthr = d_size - d_iwsz;
    p_bndl = new Bundle[d_size];
    reset ();
  }

  // create a new buffer with a context
  
  Bdb::Bdb (Mtx* mtx) : Resource (RESOURCE_BDB) {
    d_dbsz = mtx->getlong ("BDB-SIZE"); assert (d_dbsz > 0);
    d_iwsz = mtx->getiwsz (); assert (d_iwsz > 0);
    d_size = d_dbsz * d_iwsz;
    d_bthr = d_size - d_iwsz;
    p_bndl = new Bundle[d_size];
    reset ();
  }

  // destroy this buffer

  Bdb::~Bdb (void) {
    delete [] p_bndl;
  }

  // reset this buffer

  void Bdb::reset (void) {
    d_blen = 0;
    for (long i = 0; i < d_size; i++) p_bndl[i].reset ();
  }

  // report this resource

  void Bdb::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "bundle decoupling buffer" << endl;
    cout << "\tbdb size     \t\t: " << d_dbsz << endl;
    cout << "\twindow size  \t\t: " << d_iwsz << endl;
  }

  // return the buffer size

  long Bdb::getsize (void) const {
    return d_dbsz;
  }

  // return true if the buffer is empty

  bool Bdb::isempty (void) const {
    return (d_blen == 0);
  }

  // return true if the buffer is full

  bool Bdb::isfull (void) const {
    return (d_blen == d_size);
  }

  // return true if the buffer has reached the threshold

  bool Bdb::isthr (void) const {
    return (d_blen >= d_bthr);
  }

  // push a bundle in the buffer

  void Bdb::push (const Bundle& bndl) {
    // do not push invalid bundle
    if (bndl.isvalid () == false) return;
    // check that we are not full
    if (isfull () == true) {
      throw Exception ("bdb-error", "decoupling buffer is full");
    }
    p_bndl[d_blen++] = bndl;
  }

  // push back a bundle in the buffer

  void Bdb::back (const Bundle& bndl) {
    // do not push invalid bundle
    if (bndl.isvalid () == false) return;
    // check the that we are not full
    // check that we are not full
    if (isfull () == true) {
      throw Exception ("bdb-error", "decoupling buffer is full");
    }
    // rotate the buffer
    for (long i = d_blen; i > 0; i--) p_bndl[i] = p_bndl[i-1];
    // adjust length and save
    p_bndl[0] = bndl;
    d_blen++;
  }

  // pop a bundle from the buffer

  Bundle Bdb::pop (void) {
    // pop bundle in position 0
    Bundle result = p_bndl[0];
    // rotate bundle
    for (long i = 0; i < d_blen; i++) p_bndl[i] = p_bndl[i+1];
    // adjust length and return
    if (d_blen > 0) d_blen--;
    return result;
  }
}
