// ---------------------------------------------------------------------------
// - Mob.cpp                                                                 -
// - iato:mac library - memory ordering buffer class implementation          -
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
#include "Mac.hpp"
#include "Mob.hpp"

namespace iato {

  // this function convert a memory request type to an address mask
  static t_octa get_addr_mask (const Mrt::t_mrtt type) {
    t_octa mask = OCTA_0;
    switch (type) {
    case Mrt::REQ_LD1:
    case Mrt::REQ_ST1:
      mask = 0x0000000000000000ULL;
      break;
    case Mrt::REQ_LD2:
    case Mrt::REQ_ST2:
      mask = 0x0000000000000001ULL;
      break;
    case Mrt::REQ_LD4:
    case Mrt::REQ_ST4:
    case Mrt::REQ_LDS:
    case Mrt::REQ_STS:
      mask = 0x0000000000000003ULL;
      break;
    case Mrt::REQ_LD8:
    case Mrt::REQ_ST8:
    case Mrt::REQ_LDD:
    case Mrt::REQ_STD:
    case Mrt::REQ_LDI:
    case Mrt::REQ_STI:
    case Mrt::REQ_LPS:
      mask = 0x0000000000000007ULL;
      break;
    case Mrt::REQ_LDE:
    case Mrt::REQ_STE:
    case Mrt::REQ_LDF:
    case Mrt::REQ_STF:
    case Mrt::REQ_LPD:
    case Mrt::REQ_LPI:
      mask = 0x000000000000000FULL;
      break;
    default:
      assert (false);
      break;
    }
    return mask;
  }

  // ordering buffer element
  struct t_mobe {
    // the valid bit
    bool d_valid;
    // the cancel bit
    bool d_cancel;
    // the load bit
    bool d_ildb;
    // the store bit
    bool d_istb;
    // the update bit
    bool d_updb;
    // the address mask
    t_octa d_mask;
    // the address
    t_octa d_addr;
    // initialize a buffer element
    t_mobe (void) {
      reset ();
    }
    // reset a buffer element
    void reset (void) {
      d_valid  = false;
      d_cancel = false;
      d_ildb   = false;
      d_istb   = false;
      d_updb   = false;
      d_mask   = OCTA_0;
      d_addr   = OCTA_0;
    }
    // check a load with an address and a size
    void chkld (const t_octa addr, const t_octa mask) {
      // do not check invalid, cancelled, store entry or updated entry
      if (d_valid  == false) return;
      if (d_cancel == true)  return;
      if (d_istb   == true)  return;
      if (d_updb   == false) return;
      // check if the address match this entry
      if ((d_addr & ~d_mask) == (addr & ~mask)) d_cancel = true;
    }
  };

  // create a new mob
  
  Mob::Mob (void) : Resource (RESOURCE_MOB) {
    d_size = SB_SIZE;
    d_bthr = BN_IWSZ * BN_SLSZ; assert (d_bthr > 0);
    p_mobe = new t_mobe[d_size];
    reset ();
  }

  // create a new mob with a context
  
  Mob::Mob (Mtx* mtx) : Resource (RESOURCE_MOB) {
    d_size = mtx->getlong ("MOB-SIZE"); assert (d_size > 0);
    d_bthr = mtx->getswsz ();  assert (d_bthr > 0);
    p_mobe = new t_mobe[d_size];
    reset ();
  }

  // destroy this mob

  Mob::~Mob (void) {
    delete [] p_mobe;
  }

  // reset this mob

  void Mob::reset (void) {
    for (long i = 0; i < d_size; i++) p_mobe[i].reset ();
    d_aidx = 0;
  }

  // report this resource

  void Mob::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "memory ordering buffer" << endl;
    cout << "\tbuffer size  \t\t: " << d_size << endl;
  }

  // return true if the mob is empty

  bool Mob::isempty (void) const {
    for (long i = 0; i < d_size; i++) {
      if (p_mobe[i].d_valid == true) return false;
    }
    return true;
  }

  // return true if the mob has reach a threshold

  bool Mob::isthr (void) const {
    for (long i = 0; i < d_bthr; i++) {
      long midx = (d_aidx + i) % d_size;
      if (p_mobe[midx].d_valid == true) return true;
    }
    return false;
  }

  // check if the mob entry is valid

  bool Mob::isvalid (const long index) const {
    assert ((index >= 0) && (index < d_size));
    return p_mobe[index].d_valid;
  }

  // check if the mob entry has been cancelled

  bool Mob::iscancel (const long index) const {
    assert ((index >= 0) && (index < d_size));
    assert (p_mobe[index].d_valid == true);
    return p_mobe[index].d_cancel;
  }

  // clear the mob entry by index

  void Mob::clear (const long index) {
    assert ((index >= 0) && (index < d_size));
    p_mobe[index].reset ();
  }
  
  // allocate a new mob entry

  long Mob::alloc (const bool ildb, const bool istb) {
    if (p_mobe[d_aidx].d_valid == true) return -1;
    // update the element entry
    p_mobe[d_aidx].reset ();
    p_mobe[d_aidx].d_valid = true;
    p_mobe[d_aidx].d_ildb  = ildb;
    p_mobe[d_aidx].d_istb  = istb;
    // recompute alloc index
    long result = d_aidx;
    d_aidx = ++d_aidx % d_size;
    return result;
  }

  // preset a load by index and mrt

  void Mob::preset (const long index, const Mrt& mrt) {
    // check for valid mrt
    if (mrt.isvalid () == false) return;
    assert ((index >= 0) && (index < d_size));
    assert (p_mobe[index].d_valid  == true);
    assert (p_mobe[index].d_ildb   == true);
    assert (p_mobe[index].d_updb   == false);
    // update the load info
    p_mobe[index].d_cancel = false;
    p_mobe[index].d_updb   = true;
    p_mobe[index].d_addr   = mrt.getaddr ();
    p_mobe[index].d_mask   = get_addr_mask (mrt.gettype ());
  }

  // update a store by index and mrt

  void Mob::update (const long index, const Mrt& mrt) {
    // check for valid mrt
    if (mrt.isvalid () == false) return;
    assert ((index >= 0) && (index < d_size));
    assert (p_mobe[index].d_valid  == true);
    assert (p_mobe[index].d_cancel == false);
    assert (p_mobe[index].d_istb   == true);
    assert (p_mobe[index].d_updb   == false);
    // update the store info
    t_octa addr = mrt.getaddr ();
    p_mobe[index].d_updb = true;
    p_mobe[index].d_addr = addr;
    p_mobe[index].d_mask = get_addr_mask (mrt.gettype ());
    // eventually cancel all load entries that have the same address
    for (long  i = 0; i < d_size; i++) {
      long lidx = (index + i) % d_size;
      if (lidx == d_aidx) break;
      // check a load with an address and size
      p_mobe[lidx].chkld (addr, get_addr_mask (mrt.gettype ()));
    }
  }

  // process a memory request by index and mrt

  void Mob::process (const long index, const Mrt& mrt) {
    if (mrt.isvalid () == false) return;
    switch (mrt.gettype ()) {
    case Mrt::REQ_LD1:
    case Mrt::REQ_LD2:
    case Mrt::REQ_LD4:
    case Mrt::REQ_LD8:
    case Mrt::REQ_LDS:
    case Mrt::REQ_LDD:
    case Mrt::REQ_LDE:
    case Mrt::REQ_LDF:
    case Mrt::REQ_LDI:
      preset (index, mrt);
      break;
    case Mrt::REQ_ST1:
    case Mrt::REQ_ST2:
    case Mrt::REQ_ST4:
    case Mrt::REQ_ST8:
    case Mrt::REQ_STS:
    case Mrt::REQ_STD:
    case Mrt::REQ_STE:
    case Mrt::REQ_STF:
    case Mrt::REQ_STI:
      update (index, mrt);
      break;
    default:
      break;
    }
  }
}
