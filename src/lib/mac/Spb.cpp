// ---------------------------------------------------------------------------
// - Spb.cpp                                                                 -
// - iato:mac library - issue port buffer class implementation               -
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
#include "Spb.hpp"
#include "Exception.hpp"

namespace iato {
  using namespace std;

  // create a new buffer with a context

  Spb::Spb (Mtx* mtx) : Resource (RESOURCE_SPB) {
    d_mbsz = mtx->getlong ("NUMBER-M-UNITS"); assert (d_mbsz > 0);
    d_ldsn = mtx->getlong ("LD-SLOT-M-UNIT"); assert (d_ldsn > 0);
    d_stsn = mtx->getlong ("ST-SLOT-M-UNIT"); assert (d_stsn > 0);
    assert (d_ldsn + d_stsn <= d_mbsz);
    d_ibsz = mtx->getlong ("NUMBER-I-UNITS"); assert (d_ibsz > 0);
    d_fbsz = mtx->getlong ("NUMBER-F-UNITS"); assert (d_fbsz > 0);
    d_bbsz = mtx->getlong ("NUMBER-B-UNITS"); assert (d_bbsz > 0);
    p_mbuf = new Slot*[d_mbsz];
    p_ibuf = new Slot*[d_ibsz];
    p_fbuf = new Slot*[d_fbsz];
    p_bbuf = new Slot*[d_bbsz];
    for (long i = 0; i < d_mbsz; i++) p_mbuf[i] = 0;
    for (long i = 0; i < d_ibsz; i++) p_ibuf[i] = 0;
    for (long i = 0; i < d_fbsz; i++) p_fbuf[i] = 0;
    for (long i = 0; i < d_bbsz; i++) p_bbuf[i] = 0;
  }

  // create a new buffer with a context and a name

  Spb::Spb (Mtx* mtx, const string& name) : Resource (name) {
    d_mbsz = mtx->getlong ("NUMBER-M-UNITS"); assert (d_mbsz > 0);
    d_ldsn = mtx->getlong ("LD-SLOT-M-UNIT"); assert (d_ldsn > 0);
    d_stsn = mtx->getlong ("ST-SLOT-M-UNIT"); assert (d_stsn > 0);
    assert (d_ldsn + d_stsn <= d_mbsz);
    d_ibsz = mtx->getlong ("NUMBER-I-UNITS"); assert (d_ibsz > 0);
    d_fbsz = mtx->getlong ("NUMBER-F-UNITS"); assert (d_fbsz > 0);
    d_bbsz = mtx->getlong ("NUMBER-B-UNITS"); assert (d_bbsz > 0);
    p_mbuf = new Slot*[d_mbsz];
    p_ibuf = new Slot*[d_ibsz];
    p_fbuf = new Slot*[d_fbsz];
    p_bbuf = new Slot*[d_bbsz];
    for (long i = 0; i < d_mbsz; i++) p_mbuf[i] = 0;
    for (long i = 0; i < d_ibsz; i++) p_ibuf[i] = 0;
    for (long i = 0; i < d_fbsz; i++) p_fbuf[i] = 0;
    for (long i = 0; i < d_bbsz; i++) p_bbuf[i] = 0;
  }

  // destroy this buffer

  Spb::~Spb (void) {
    delete [] p_mbuf;
    delete [] p_ibuf;
    delete [] p_fbuf;
    delete [] p_bbuf;
  }

  // reset this buffer

  void Spb::reset (void) {
    for (long i = 0; i < d_mbsz; i++) 
      if (p_mbuf[i]) p_mbuf[i]->reset ();
    for (long i = 0; i < d_ibsz; i++) 
      if (p_ibuf[i]) p_ibuf[i]->reset ();
    for (long i = 0; i < d_fbsz; i++) 
      if (p_fbuf[i]) p_fbuf[i]->reset ();
    for (long i = 0; i < d_bbsz; i++) 
      if (p_bbuf[i]) p_bbuf[i]->reset ();
    d_intr.reset ();
    d_iioi = -1;
  }

  // report this reosurce

  void Spb::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "issue port buffer" << endl;
    cout << "\tM slots      \t\t: " << d_mbsz << endl;
    cout << "\tI slots      \t\t: " << d_ibsz << endl;
    cout << "\tF slots      \t\t: " << d_fbsz << endl;
    cout << "\tB slots      \t\t: " << d_bbsz << endl;
  }

  // return a buffer size by unit

  long Spb::getsize (t_unit unit) const {
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

  void Spb::add (Slot* slot) {
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
    throw Exception ("spb-error", "port buffer is full");
  }

  // return true if all slot are free

  bool Spb::isfree (void) const {
    for (long i = 0; i < d_mbsz; i++)
      if (p_mbuf[i]->isfree () == false) return false;
    for (long i = 0; i < d_ibsz; i++)
      if (p_ibuf[i]->isfree () == false) return false;
    for (long i = 0; i < d_fbsz; i++)
      if (p_fbuf[i]->isfree () == false) return false;
    for (long i = 0; i < d_bbsz; i++)
      if (p_bbuf[i]->isfree () == false) return false;
    return true;
  }
  
  // find a free slot by unit

  long Spb::find (const t_unit unit, const bool stb, const bool ldb) const {
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
    // find free slot - with M unit we check for store and load slots
    // in this approach, we force the store to be in the lower slots
    for (long i = 0; i < size; i++) {
      if (!sbuf[i]) continue;
      if (sbuf[i]->isfree () == true) {
	// check for store
	if ((unit == MUNIT) && (stb == true) && (i >= d_stsn)) return -1;
	if ((unit == MUNIT) && (ldb == true) && (i <  d_stsn)) continue;
	return i;
      }
    }
    return -1;
  }

  // set an instruction by unit and slot index

  void Spb::setinst (t_unit unit, const long slot, const Ssi& ssi) {
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

  Ssi Spb::getinst (t_unit unit, const long slot) const {
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

  // return true if the port buffer has been interrupted

  bool Spb::isintr (void) const {
    return d_intr.isvalid ();
  }

  // set the offending interrupt and index

  void Spb::setintr (const Interrupt& vi, const long iioi) {
    d_intr = vi;
    d_iioi = iioi;
  }
}
