// ---------------------------------------------------------------------------
// - Btb.cpp                                                                 -
// - iato:mac library - branch target buffer class implementation            -
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
#include "Btb.hpp"
#include "Exception.hpp"

namespace iato {

  // the btb structure
  struct s_btb {
    bool   d_valid; // valid bit
    t_octa d_addr;  // source address
    t_octa d_targ;  // target address
    // simple constructor
    s_btb (void) {
      reset ();
    }
    // reset this btb entry
    void reset (void) {
      d_valid = false;
      d_addr  = OCTA_0;
      d_targ  = OCTA_0;
    }
    // return true if the address is valid
    bool isvalid (const t_octa addr) const {
      if (d_valid == false) return false;
      return (d_addr == addr);
    }
    // update a btb entry
    void update (const t_octa addr, const t_octa targ) {
      d_valid = true;
      d_addr  = addr;
      d_targ  = targ;
    }
  };

  // create a default btb

  Btb::Btb (void) {
    d_size = BP_BTBS;
    p_vbtb = new s_btb[d_size];
    reset ();
  }

  // create a btb with a size

  Btb::Btb (const long size) {
    d_size = size; assert (d_size > 0);
    p_vbtb = new s_btb[d_size];
    reset ();
  }

  // create a btb with a context

  Btb::Btb (Mtx* mtx) {
    d_size = mtx->getlong ("BTB-SIZE"); assert (d_size > 0);
    p_vbtb = new s_btb[d_size];
    reset ();
  }

  // destroy this btb
  
  Btb::~Btb (void) {
    delete [] p_vbtb;
  }

  // reset this btb
  
  void Btb::reset (void) {
    for (long i = 0; i < d_size; i++) p_vbtb[i].reset ();
  }

  // return the btb size

  long Btb::getsize (void) const {
    return d_size;
  }

  // this function returns true if the address is valid

  bool Btb::isvalid (const t_octa addr) const {
    // compute table index
    long index = addr % (t_octa) d_size;
    return p_vbtb[index].isvalid (addr);
  }

  // get the btb target by address

  t_octa Btb::gettrg (const t_octa addr) const {
    // compute table index
    long index = addr % (t_octa) d_size;
    // check for valid access
    if (p_vbtb[index].isvalid (addr) == true) return p_vbtb[index].d_targ;
    // invalid access
    ostringstream os;
    os << "invalid btb target address address access at 0x";
    os << hex << setw (16) << setfill ('0') << addr;
    throw Exception ("btb-error", os.str ());
  }

  // update the btb with an address and a target
  
  void Btb::update (const t_octa addr, const t_octa targ) {
    // compute table index
    long index = addr % (t_octa) d_size;
    // update the btb
    p_vbtb[index].update (addr, targ);
  }
}
