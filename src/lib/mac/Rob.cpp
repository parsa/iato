// ---------------------------------------------------------------------------
// - Rob.cpp                                                                 -
// - iato:mac library - reorder buffer class implementation                  -
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

#include "Rob.hpp"
#include "Mac.hpp"
#include "Prn.hpp"

namespace iato {

  // the rob structure
  struct t_rob {
    // valid bit
    bool d_valid;
    // the mob index
    long   d_imob;
    // the iib index
    long   d_iiib;
    // the exec bit
    bool d_exec;
    // the cancel bit
    bool d_cnlf;
    // the irb index
    long d_iidx;
    // the sip speculative bit
    bool d_bsip;
    // the serialize bit
    bool d_srlz;
    //  the nop bit
    bool d_nopb;
    // the instruction ip
    t_octa d_ip;
    // the instruction slot
    long   d_slot;
    // the interrupt record
    Interrupt d_intr;
    // create a new rob entry
    t_rob (void) {
      reset ();
    }
    // reset this rob entry
    void reset (void) {
      d_valid = false;
      d_imob  = -1;
      d_iiib  = -1;
      d_exec  = false;
      d_cnlf  = false;
      d_iidx  = -1;
      d_bsip  = false;
      d_srlz  = false;
      d_nopb  = false;
      d_ip    = OCTA_0;
      d_slot  = 0;
      d_intr.reset ();
    }
  };

  // create a new rob
  
  Rob::Rob (void) : Resource (RESOURCE_ROB) {
    d_size = RB_SIZE;
    p_rbuf = new t_rob[d_size];
    reset ();
  }

  // create a new robwith a context
  
  Rob::Rob (Mtx* mtx) : Resource (RESOURCE_ROB) {
    d_size = mtx->getlong ("ROB-SIZE"); assert (d_size > 0);
    p_rbuf = new t_rob[d_size];
    reset ();
  }

  // destroy this rob

  Rob::~Rob (void) {
    delete [] p_rbuf;
  }

  // reset this rob

  void Rob::reset (void) {
    for (long i = 0; i < d_size; i++) p_rbuf[i].reset ();
    d_rlen = 0;
    d_iptr = 0;
    d_optr = 0;
  }

  // report this resource

  void Rob::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "reorder buffer" << endl;
    cout << "\trob size     \t\t: " << d_size << endl;
  }

  // return true if the rob is empty

  bool Rob::isempty (void) const {
    return (d_rlen == 0) ? true : false;
  }

  // return true if the rob is full

  bool Rob::isfull (void) const {
    return (d_rlen >= d_size) ? true : false;
  }

  // check if the rob entry is valid

  bool Rob::isvalid (const long ridx) const {
    assert ((ridx >= 0) && (ridx < d_size));
    return p_rbuf[ridx].d_valid;
  }

  // check if the current rob entry is valid

  bool Rob::isvalid (void) const {
    return isvalid (d_optr);
  }

  // check if the current rob entry is valid

  bool Rob::ispop (void) const {
    if (p_rbuf[d_optr].d_valid == false) return false;
    if (p_rbuf[d_optr].d_exec  == true)  return true;
    if (p_rbuf[d_optr].d_srlz  == true)  return true;
    if (p_rbuf[d_optr].d_intr.isvalid () == true) return true;
    return false;
  }

  // return true if the current rob entry is a serialization
  
  bool Rob::issrlz (void) const {
    if (p_rbuf[d_optr].d_valid == false) return false;
    return p_rbuf[d_optr].d_srlz;
  }

  // return true if the current rob entry is a nop
  
  bool Rob::isnop (void) const {
    if (p_rbuf[d_optr].d_valid == false) return false;
    return p_rbuf[d_optr].d_nopb;
  }

  // return true if the current rob entry is an interrupt
  
  bool Rob::isintr (void) const {
    if (p_rbuf[d_optr].d_valid == false) return false;
    return p_rbuf[d_optr].d_intr.isvalid ();
  }

  // return true of the current rob entry has been cancelled
  
  bool Rob::iscancel (void) const {
    if (p_rbuf[d_optr].d_valid == false) return false;
    return p_rbuf[d_optr].d_cnlf;
  }

  // allocate a new rob entry with serialize and nop bit

  void Rob::alloc (const t_octa ip, const long slot, const bool srlz,
		   const bool nopb) {
    assert (d_rlen < d_size);
    assert (p_rbuf[d_iptr].d_valid == false);
    // update the rob record
    p_rbuf[d_iptr].reset ();
    p_rbuf[d_iptr].d_valid = true;
    p_rbuf[d_iptr].d_srlz  = srlz;
    if (nopb == true) {
      p_rbuf[d_iptr].d_nopb  = nopb;
      p_rbuf[d_iptr].d_exec  = true;
    }
    p_rbuf[d_iptr].d_ip    = ip;
    p_rbuf[d_iptr].d_slot  = slot;
    // update rob input index
    d_iptr = ++d_iptr % d_size;
    d_rlen++;
  }

  // allocate a new rob entry

  long Rob::alloc (const Instr& inst, const long imob, const long iiib) {
    assert (d_rlen < d_size);
    assert (p_rbuf[d_iptr].d_valid == false);
    assert (inst.isvalid () == true);
    // update the rob record
    p_rbuf[d_iptr].reset ();
    p_rbuf[d_iptr].d_valid = true;
    p_rbuf[d_iptr].d_imob  = imob;
    p_rbuf[d_iptr].d_iiib  = iiib;
    p_rbuf[d_iptr].d_ip    = inst.getiip  ();
    p_rbuf[d_iptr].d_slot  = inst.getslot ();
    // update rob input index
    long result = d_iptr;
    d_iptr = ++d_iptr % d_size;
    d_rlen++;
    return result;
  }

  // allocate a new rob entry with an interrupt

  long Rob::alloc (const Interrupt& vi) {
    assert (d_rlen < d_size);
    assert (p_rbuf[d_iptr].d_valid == false);
    assert (vi.isvalid () == true);
    // allocate the new rob entry
    p_rbuf[d_iptr].reset ();
    p_rbuf[d_iptr].d_valid = true;
    p_rbuf[d_iptr].d_intr  = vi;
    long result = d_iptr;
    d_iptr = ++d_iptr % d_size;
    d_rlen++;
    return result;
  }

  // pop the latest irb index and clean

  long Rob::pop (void) {
    assert (ispop () == true);
    long result = p_rbuf[d_optr].d_iidx;
    // clean this entry
    p_rbuf[d_optr].reset ();
    d_optr = ++d_optr % d_size;
    d_rlen--;
    return result;
  }
 
  // pop the latest serialize and clean

  void Rob::spop (void) {
    assert (issrlz () == true);
    // clean this entry
    p_rbuf[d_optr].reset ();
    d_optr = ++d_optr % d_size;
    d_rlen--;
  }

  // pop the latest nop and clean

  void Rob::npop (void) {
    assert (isnop () == true);
    // clean this entry
    p_rbuf[d_optr].reset ();
    d_optr = ++d_optr % d_size;
    d_rlen--;
  }

  // pop the latest interrupt and clean

  Interrupt Rob::ipop (void) {
    assert (isintr () == true);
    Interrupt vi = p_rbuf[d_optr].d_intr;
    // clean this entry
    p_rbuf[d_optr].reset ();
    d_optr = ++d_optr % d_size;
    d_rlen--;
    return vi;
  }

  // return the latest instruction ip

  t_octa Rob::getiip (void) const {
    assert (p_rbuf[d_optr].d_valid == true);
    return p_rbuf[d_optr].d_ip;
  }

  // return the latest instruction slot

  long Rob::getslot (void) const {
    assert (p_rbuf[d_optr].d_valid == true);
    return p_rbuf[d_optr].d_slot;
  }

  // return the latest mob index

  long Rob::getimob (void) const {
    assert (p_rbuf[d_optr].d_valid == true);
    return p_rbuf[d_optr].d_imob;
  }

  // return the latest iib index

  long Rob::getiiib (void) const {
    assert (p_rbuf[d_optr].d_valid == true);
    return p_rbuf[d_optr].d_iiib;
  }

  // return the latest sip speculative bit

  bool Rob::getbsip (void) const {
    assert (p_rbuf[d_optr].d_valid == true);
    return p_rbuf[d_optr].d_bsip;
  }

  // set the exec flag by index

  void Rob::setexe (const long ridx, const bool flag) {
    assert ((ridx >= 0) && (ridx < d_size));
    assert (p_rbuf[ridx].d_valid == true);
    p_rbuf[ridx].d_exec = flag;
  }

  // set the cancel flag by index

  void Rob::setcnlf (const long ridx, const bool flag) {
    assert ((ridx >= 0) && (ridx < d_size));
    assert (p_rbuf[ridx].d_valid == true);
    p_rbuf[ridx].d_cnlf = flag;
  }

  // set the irb idex

  void Rob::setirb (const long ridx, const long index) {
    assert ((ridx >= 0) && (ridx < d_size));
    assert (p_rbuf[ridx].d_valid == true);
    p_rbuf[ridx].d_iidx = index;
  }

  // set the interrupt entry

  void Rob::setintr (const long ridx, const Interrupt& vi) {
    assert ((ridx >= 0) && (ridx < d_size));
    assert (p_rbuf[ridx].d_valid == true);
    p_rbuf[ridx].d_intr = vi;
  }

  // set the sip speculative bit entry

  void Rob::setbsip (const long ridx, const bool bsip) {
    assert ((ridx >= 0) && (ridx < d_size));
    assert (p_rbuf[ridx].d_valid == true);
    p_rbuf[ridx].d_bsip = bsip;
  }
}
