// ---------------------------------------------------------------------------
// - ReqBuf.cpp                                                              -
// - iato:mac library - port request buffer class implementation             -
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

#include "Prn.hpp"
#include "ReqBuf.hpp"

namespace iato {

  // create a new request buffer
  
  ReqBuf::ReqBuf (void) : Resource (RESOURCE_PRB) {
    reset ();
  }

  // create a new request buffer with a context
  
  ReqBuf::ReqBuf (Mtx* mtx) : Resource (RESOURCE_PRB) {
    reset ();
  }

  // reset this buffer

  void ReqBuf::reset (void) {
    long size = d_vprq.size ();
    for (long i = 0; i < size; i++) d_vprq[i]->reset ();
  }

  // flush this buffer

  void ReqBuf::flush (void) {
    long size = d_vprq.size ();
    for (long i = 0; i < size; i++) d_vprq[i]->reset ();
  }

  // report this resource

  void ReqBuf::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "port request buffer" << endl;
    cout << "\tsize         \t\t: " << d_vprq.size () << endl;
  }

  // return the buffer length

  long ReqBuf::length (void) const {
    return d_vprq.size ();
  }

  // get a port request by index

  PortReq* ReqBuf::get (const long index) const {
    assert ((index >= 0) && (index < length ()));
    return d_vprq[index];
  }

  // add a new port request to this buffer

  void ReqBuf::add (PortReq* preq) {
    if (preq) d_vprq.push_back (preq);
  }

  // bind a port acknowledger to this buffer

  void ReqBuf::bind (Weakable* pack) {
    if (!pack) return;
    long size = d_vprq.size ();
    for (long i = 0; i < size; i++) d_vprq[i]->bind (pack);
  }
}
