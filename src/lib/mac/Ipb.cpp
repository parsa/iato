// ---------------------------------------------------------------------------
// - Ipb.cpp                                                                 -
// - iato:mac library - issue port buffer class implementation               -
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
#include "Ipb.hpp"
#include "Exception.hpp"

namespace iato {
  using namespace std;

  // create a new buffer with a context

  Ipb::Ipb (Mtx* mtx) : Resource (RESOURCE_IPB) {
    d_mbsz = mtx->getlong ("NUMBER-M-UNITS"); assert (d_mbsz > 0);
    d_ibsz = mtx->getlong ("NUMBER-I-UNITS"); assert (d_ibsz > 0);
    d_fbsz = mtx->getlong ("NUMBER-F-UNITS"); assert (d_fbsz > 0);
    d_bbsz = mtx->getlong ("NUMBER-B-UNITS"); assert (d_bbsz > 0);
    p_mbuf = new (Slot*)[d_mbsz];
    p_ibuf = new (Slot*)[d_mbsz];
    p_fbuf = new (Slot*)[d_mbsz];
    p_bbuf = new (Slot*)[d_mbsz];
    for (long i = 0; i < d_mbsz; i++) p_mbuf[i] = 0;
    for (long i = 0; i < d_ibsz; i++) p_ibuf[i] = 0;
    for (long i = 0; i < d_fbsz; i++) p_fbuf[i] = 0;
    for (long i = 0; i < d_bbsz; i++) p_bbuf[i] = 0;
  }

  // create a new buffer with a context and a name

  Ipb::Ipb (Mtx* mtx, const string& name) : Resource (name) {
    d_mbsz = mtx->getlong ("NUMBER-M-UNITS"); assert (d_mbsz > 0);
    d_ibsz = mtx->getlong ("NUMBER-I-UNITS"); assert (d_ibsz > 0);
    d_fbsz = mtx->getlong ("NUMBER-F-UNITS"); assert (d_fbsz > 0);
    d_bbsz = mtx->getlong ("NUMBER-B-UNITS"); assert (d_bbsz > 0);
    p_mbuf = new (Slot*)[d_mbsz];
    p_ibuf = new (Slot*)[d_mbsz];
    p_fbuf = new (Slot*)[d_mbsz];
    p_bbuf = new (Slot*)[d_mbsz];
    for (long i = 0; i < d_mbsz; i++) p_mbuf[i] = 0;
    for (long i = 0; i < d_ibsz; i++) p_ibuf[i] = 0;
    for (long i = 0; i < d_fbsz; i++) p_fbuf[i] = 0;
    for (long i = 0; i < d_bbsz; i++) p_bbuf[i] = 0;
  }

  // destroy this buffer

  Ipb::~Ipb (void) {
    delete [] p_mbuf;
    delete [] p_ibuf;
    delete [] p_fbuf;
    delete [] p_bbuf;
  }

  // reset this buffer

  void Ipb::reset (void) {
    for (long i = 0; i < d_mbsz; i++) 
      if (p_mbuf[i]) p_mbuf[i]->reset ();
    for (long i = 0; i < d_ibsz; i++) 
      if (p_ibuf[i]) p_ibuf[i]->reset ();
    for (long i = 0; i < d_fbsz; i++) 
      if (p_fbuf[i]) p_fbuf[i]->reset ();
    for (long i = 0; i < d_bbsz; i++) 
      if (p_bbuf[i]) p_bbuf[i]->reset ();
  }

  // report this reosurce

  void Ipb::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "issue port buffer" << endl;
    cout << "\tM slots      \t\t: " << d_mbsz << endl;
    cout << "\tI slots      \t\t: " << d_ibsz << endl;
    cout << "\tF slots      \t\t: " << d_fbsz << endl;
    cout << "\tB slots      \t\t: " << d_bbsz << endl;
  }

  // return a buffer size by unit

  long Ipb::getsize (t_unit unit) const {
    long   size = 0;
    switch (unit) {
    case MUNIT:
      size = d_mbsz;
      break;
    case IUNIT:
      size = d_ibsz;
      break;
    case FUNIT:
      size = d_fbsz;
      break;
    case BUNIT:
      size = d_bbsz;
      break;
    default:
      assert (false);
      break;
    }
    return size;
  }

  // add a slot in this buffer

  void Ipb::add (Slot* slot) {
    // check for a valid slot
    if (!slot) return;
    // get valid buffer
    t_unit unit = slot->getunit ();
    long   size = 0;
    Slot** sbuf = 0;
    switch (unit) {
    case MUNIT:
      size = d_mbsz;
      sbuf = p_mbuf;
      break;
    case IUNIT:
      size = d_ibsz;
      sbuf = p_ibuf;
      break;
    case FUNIT:
      size = d_fbsz;
      sbuf = p_fbuf;
      break;
    case BUNIT:
      size = d_bbsz;
      sbuf = p_bbuf;
      break;
    default:
      assert (false);
      break;
    }
    // find valid slot and update
    for (long i = 0; i < size; i++) {
      if (sbuf[i]) continue;
      sbuf[i] = slot;
      slot->setspos (i);
      return;
    }
    throw Exception ("ipb-error", "port buffer is full");
  }

  // find a free slot by unit

  long Ipb::find (t_unit unit) const {
    // initialize size and buffer pointer
    long   size = 0;
    Slot** sbuf = 0;
    switch (unit) {
    case MUNIT:
      size = d_mbsz;
      sbuf = p_mbuf;
      break;
    case IUNIT:
      size = d_ibsz;
      sbuf = p_ibuf;
      break;
    case FUNIT:
      size = d_fbsz;
      sbuf = p_fbuf;
      break;
    case BUNIT:
      size = d_bbsz;
      sbuf = p_bbuf;
      break;
    default:
      assert (false);
      break;
    }
    // find free slot
    for (long i = 0; i < size; i++) {
      if (!sbuf[i]) continue;
      if (sbuf[i]->isfree () == true) return i;
    }
    return -1;
  }

  // set an instruction by unit and slot index

  void Ipb::setinst (t_unit unit, const long slot, const Ssi& ssi) {
    // initialize size and buffer pointer
    long   size = 0;
    Slot** sbuf = 0;
    switch (unit) {
    case MUNIT:
      size = d_mbsz;
      sbuf = p_mbuf;
      break;
    case IUNIT:
      size = d_ibsz;
      sbuf = p_ibuf;
      break;
    case FUNIT:
      size = d_fbsz;
      sbuf = p_fbuf;
      break;
    case BUNIT:
      size = d_bbsz;
      sbuf = p_bbuf;
      break;
    default:
      assert (false);
      break;
    }
    // check valid slot
    assert ((slot >= 0) && (slot < size));
    // set instruction
    if (sbuf[slot]) sbuf[slot]->setinst (ssi);
  }

  // return an instruction by unit and index

  Ssi Ipb::getinst (t_unit unit, const long slot) const {
    // initialize size and buffer pointer
    long   size = 0;
    Slot** sbuf = 0;
    switch (unit) {
    case MUNIT:
      size = d_mbsz;
      sbuf = p_mbuf;
      break;
    case IUNIT:
      size = d_ibsz;
      sbuf = p_ibuf;
      break;
    case FUNIT:
      size = d_fbsz;
      sbuf = p_fbuf;
      break;
    case BUNIT:
      size = d_bbsz;
      sbuf = p_bbuf;
      break;
    default:
      assert (false);
      break;
    }
    // check valid slot
    assert ((slot >= 0) && (slot < size));
    // get instruction
    Ssi result;
    if (sbuf[slot]) result = sbuf[slot]->getinst ();
    return result;
  }
}
