// ---------------------------------------------------------------------------
// - Segment.cpp                                                             -
// - iato:isa library - memory segment class implementation                  -
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

#include "Segment.hpp"
#include "Interrupt.hpp"

namespace iato {

  // create a new segment

  Segment::Segment (void) {
    d_size = 0;
    d_base = 0;
    p_data = 0;
    reset ();
  }

  // create a new segment instance by size

  Segment::Segment (const t_long size) {
    assert (size > 0);
    d_size = size;
    p_data = new t_byte[d_size];
    d_base = 0;
    reset ();
  }

  // destroy this segment
  
  Segment::~Segment (void) {
    delete [] p_data;
  }

  // reset this segment
  
  void Segment::reset (void) {
    if (d_size == 0) return;
    for (t_long i = 0; i < d_size; i++) p_data[i] = '\000';
  }

  // return the segment size

  t_long Segment::getsize (void) const {
    return d_size;
  }

  // set the segment base adress

  void Segment::setbase (const t_octa addr) {
    d_base = addr;
  }

  // return the segment base address

  t_octa Segment::getbase (void) const {
    return d_base;
  }

  // initialize this segment by copy

  void Segment::setdata (const long blen, t_byte* buf) {
    if (blen <= 0) return;
    if (d_size != 0) delete [] p_data;
    d_size = blen;
    p_data = new t_byte[d_size];
    for (long i = 0; i < d_size; i++) p_data[i] = buf[i];
  }

  // initialize this segment by mapping

  void Segment::mapdata (const long blen, t_byte* buf) {
    if (blen <= 0) return;
    if (d_size != 0) delete [] p_data;
    d_size = blen;
    p_data = buf;
  }

  // return true if the address is valid

  bool Segment::isvalid (const t_octa addr) const {
    t_octa ma = addr - d_base;
    if ((addr < d_base) || (ma >= (t_octa) d_size)) return false;
    return true;
  }

  // read a byte from this segment

  t_byte Segment::readbyte (const t_octa addr) const {
    // check protection
    if ((d_prot & PROT_RD) != PROT_RD) {
      ostringstream os;
      os << "read data access rights fault at 0x";
      os << hex << setw (16) << setfill ('0') << addr;
      throw Interrupt (FAULT_IT_DATA_ACRGT, os.str ());
    }
    // check access
    t_octa ma = addr - d_base;
    if ((addr < d_base) || (ma >= (t_octa) d_size)) {
      ostringstream os;
      os << "data page not present at 0x";
      os << hex << setw (16) << setfill ('0') << addr;
      throw Interrupt (FAULT_IT_DATA_PAGE, os.str ());
    }
    return p_data[ma];
  }

  // write a byte at a certain address

  void Segment::writebyte (const t_octa addr, const t_byte byte) {
    // check protection
    if ((d_prot & PROT_WR) != PROT_WR){
      ostringstream os;
      os << "write data access rights fault at 0x";
      os << hex << setw (16) << setfill ('0') << addr;
      throw Interrupt (FAULT_IT_DATA_ACRGT, os.str ());
    }
    // check access
    t_octa ma = addr - d_base;
    if ((addr < d_base) || (ma >= (t_octa) d_size)) {
      ostringstream os;
      os << "data page not present at 0x";
      os << hex << setw (16) << setfill ('0') << addr;
      throw Interrupt (FAULT_IT_DATA_PAGE, os.str ());
    }
    p_data[ma] = byte;
  }
}
