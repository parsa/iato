// ---------------------------------------------------------------------------
// - Scoreboard.cpp                                                          -
// - iato:mac library - register scoreboarding class implementation          -
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
#include "Scoreboard.hpp"

namespace iato {
  using namespace std;

  // -------------------------------------------------------------------------
  // - instruction scoreboard                                                -
  // -------------------------------------------------------------------------
  
  // the instruction scoreboard structure
  struct t_isb {
    // the instruction
    Ssi d_ssi;
    // the interrupt
    Interrupt d_svi;
    // the instruction state
    bool d_iss[Scoreboard::MAX_STATES];
    // create a default entry
    t_isb (void) {
      reset ();
    }
    // reset this entry
    void reset (void) {
      d_ssi.reset ();
      d_svi.reset ();
      for (long i = 0; i < Scoreboard::MAX_STATES; i++) d_iss[i] = false;
    }
    // return true if the entry is valid
    bool isvalid (void) {
      return (d_ssi.isvalid () || d_svi.isvalid ());
    }
    // allocate a new entry
    void alloc (const Ssi& ssi, const long ridx) {
      assert (ssi.isvalid () == true);
      d_ssi = ssi;
      d_svi.reset ();
      d_ssi.setrix (ridx);
    }
    // allocate a new interrupt
    void alloc (const Interrupt& svi) {
      assert (svi.isvalid () == true);
      d_ssi.reset ();
      d_svi = svi;
      d_iss[Scoreboard::INTERP] = true;
    }
    // set an instruction state
    void setstate (Scoreboard::t_state state) {
      if (d_ssi.isvalid () == false) return;
      d_iss[state] = true;
    }
    // return an instruction state
    bool getstate (Scoreboard::t_state state) const {
      return d_iss[state];
    }
  };

  // create a default instruction scoreboard

  Scoreboard::Is::Is (void) {
    d_size = IS_SIZE;
    p_visb = new t_isb[d_size];
    reset ();
  }

  // create a new instruction scoreboard by context

  Scoreboard::Is::Is (Mtx* mtx) {
    d_size = mtx->getlong ("ISB-SIZE"); assert (d_size > 0);
    p_visb = new t_isb[d_size];
    reset ();
  }

  // destroy this instruction scoreboard

  Scoreboard::Is::~Is (void) {
    delete [] p_visb;
  }

  // reset this instruction scoreboard

  void Scoreboard::Is::reset (void) {
    for (long i = 0; i < d_size; i++) p_visb[i].reset ();
    d_iptr = 0;
    d_optr = 0;
    d_ilen = 0;
  }

  // return true if the buffer if empty

  bool Scoreboard::Is::isempty (void) const {
    return (d_ilen == 0);
  }

  // return true if the buffer if full

  bool Scoreboard::Is::isfull (void) const {
    return (d_ilen == d_size);
  }

  // return true if the index is the current pop index
  
  bool Scoreboard::Is::ispop (const long index) const {
    assert ((index >= 0) && (index < d_size));
    return (d_optr == index);
  }

  // allocate a new instruction

  long Scoreboard::Is::alloc (const Ssi& ssi) {
    // make sure we have a valid entry
    assert (d_ilen < d_size);
    assert (p_visb[d_iptr].isvalid () == false);
    // allocate the new entry
    p_visb[d_iptr].alloc (ssi, d_iptr);
    // adjust index and result
    long result = d_iptr;
    d_iptr = ++d_iptr % d_size;
    d_ilen++;
    return result;
  }

  // allocate a new interrupt

  void Scoreboard::Is::alloc (const Interrupt& svi) {
    // check for valid interrupt
    if (svi.isvalid () == false) return;
    // make sure we have a valid entry
    assert (d_ilen < d_size);
    assert (p_visb[d_iptr].isvalid () == false);
    // allocate the new entry
    p_visb[d_iptr].alloc (svi);
    // adjust indexes
    d_iptr = ++d_iptr % d_size;
    d_ilen++;
  }

  // clean a register scoreboard

  bool Scoreboard::Is::clean (void) {
    // check if we are noty empty
    if (isempty () == true) return false;
    // check for current write-back
    if (getstate (d_optr, WRBACK) == false) return false;
    // clean current entry
    p_visb[d_optr].reset ();
    d_optr = ++d_optr % d_size;
    d_ilen--;
    return true;
  }

  // set the instruction state

  void Scoreboard::Is::setstate (const long ridx, t_state state) {
    assert ((ridx >= 0) && (ridx < d_size));
    p_visb[ridx].setstate (state);
  }
 
  // get the instruction state by index

  bool Scoreboard::Is::getstate (const long ridx, t_state state) const {
    assert ((ridx >= 0) && (ridx < d_size));
    return p_visb[ridx].getstate (state);
  }

  // -------------------------------------------------------------------------
  // - register scoreboard                                                   -
  // -------------------------------------------------------------------------

  // create a default register scoreboard

  Scoreboard::Rs::Rs (void) {
    d_gnum = GR_LRSZ;
    d_fnum = FR_LRSZ;
    d_pnum = PR_LRSZ;
    d_bnum = BR_LRSZ;
    d_anum = AR_LRSZ;
    d_cnum = CR_LRSZ;
    p_glok = new bool[d_gnum];
    p_flok = new bool[d_fnum];
    p_plok = new bool[d_pnum];
    p_blok = new bool[d_bnum];
    p_alok = new bool[d_anum];
    p_clok = new bool[d_cnum];
    reset ();
  }

  // create a register scoreboard with a context

  Scoreboard::Rs::Rs (Mtx* mtx) {
    d_gnum = mtx->getlong ("LR-GR-SIZE"); assert (d_gnum > 0);
    d_fnum = mtx->getlong ("LR-FR-SIZE"); assert (d_fnum > 0);
    d_pnum = mtx->getlong ("LR-PR-SIZE"); assert (d_pnum > 0);
    d_bnum = mtx->getlong ("LR-BR-SIZE"); assert (d_bnum > 0);
    d_anum = mtx->getlong ("LR-AR-SIZE"); assert (d_anum > 0);
    d_cnum = mtx->getlong ("LR-CR-SIZE"); assert (d_cnum > 0);
    p_glok = new bool[d_gnum];
    p_flok = new bool[d_fnum];
    p_plok = new bool[d_pnum];
    p_blok = new bool[d_bnum];
    p_alok = new bool[d_anum];
    p_clok = new bool[d_cnum];
    reset ();
  }
  
  // destroy this register scoreboard

  Scoreboard::Rs::~Rs (void) {
    delete [] p_glok;
    delete [] p_flok;
    delete [] p_plok;
    delete [] p_blok;
    delete [] p_alok;
    delete [] p_clok;
  }

  // reset the register scoreboard

  void Scoreboard::Rs::reset (void) {
    for (long i = 0; i < d_gnum; i++) p_glok[i] = false;
    for (long i = 0; i < d_fnum; i++) p_flok[i] = false;
    for (long i = 0; i < d_pnum; i++) p_plok[i] = false;
    for (long i = 0; i < d_bnum; i++) p_blok[i] = false;
    for (long i = 0; i < d_anum; i++) p_alok[i] = false;
    for (long i = 0; i < d_cnum; i++) p_clok[i] = false;
  }

  // return a true if a register is marked

  bool Scoreboard::Rs::ismarked (const Rid& rid) const {
    if (rid.isvalid () == false) return false;
    // extract rid ifo
    t_lreg lreg = rid.gettype ();
    long   lnum = rid.getlnum ();
    if (lnum < 0) return false;
    // process from the register type
    bool result = false;
    switch (lreg) {
    case GREG:
      result = lnum < d_gnum ? p_glok[lnum] : false;
      break;
    case FREG:
      result = lnum < d_fnum ? p_flok[lnum] : false;
      break;
    case PREG:
      result = lnum < d_pnum ? p_plok[lnum] : false;
      break;
    case BREG:
      result = lnum < d_bnum ? p_blok[lnum] : false;
      break;
    case AREG:
      result = lnum < d_anum ? p_alok[lnum] : false;
      break;
    case CREG:
      result = lnum < d_cnum ? p_clok[lnum] : false;
      break;
    default:
      break;
    }
    return result;
  }

  // set a register scoreboard by rid

  void Scoreboard::Rs::mark (const Rid& rid, const bool flag) {
    if (rid.isvalid () == false) return;
    // extract rid info
    t_lreg lreg = rid.gettype ();
    long   lnum = rid.getlnum ();
    assert (lnum >= 0);
    // process from the register type
    switch (lreg) {
    case GREG:
      assert (lnum < d_gnum);
      p_glok[lnum] = flag;
    case FREG:
      assert (lnum < d_fnum);
      p_flok[lnum] = flag;
    case PREG:
      assert (lnum < d_pnum);
      p_plok[lnum] = flag;
    case BREG:
      assert (lnum < d_bnum);
      p_blok[lnum] = flag;
    case AREG:
      assert (lnum < d_anum);
      p_alok[lnum] = flag;
    case CREG:
      assert (lnum < d_cnum);
      p_clok[lnum] = flag;
    default:
      assert (false);
      break;
    }
  }

  // -------------------------------------------------------------------------
  // - scoreboard                                                            -
  // -------------------------------------------------------------------------


  // create a default scoreboard

  Scoreboard::Scoreboard (void) : Resource (RESOURCE_PSB) {
    p_is   = new Scoreboard::Is;
    p_rs   = new Scoreboard::Rs;
    d_swsz = 1;
    reset ();
  }

  // create a new scoreboard with a context

  Scoreboard::Scoreboard (Mtx* mtx) : Resource (RESOURCE_PSB) {
    p_is   = new Scoreboard::Is (mtx);
    p_rs   = new Scoreboard::Rs (mtx);
    d_swsz = mtx->getiwsz () * BN_SLSZ; assert (d_swsz > 0);
    reset ();
  }

  // create a new scoreboard with a context and a name

  Scoreboard::Scoreboard (Mtx* mtx, const string& name) : Resource (name) {
    p_is   = new Scoreboard::Is (mtx);
    p_rs   = new Scoreboard::Rs (mtx);
    d_swsz = mtx->getiwsz () * BN_SLSZ; assert (d_swsz > 0);
    reset ();
  }

  // destroy this scoreboard

  Scoreboard::~Scoreboard (void) {
    delete p_is;
    delete p_rs;
  }

  // reset this scoreboard

  void Scoreboard::reset (void) {
    p_is->reset ();
    p_rs->reset ();
  }

  // report this resource

  void Scoreboard::report (void) const {
    Resource::report ();
    cout << "\tresource type\t\t: " << "register scoreboard" << endl;
  }

  // allocate a new entry in the instruction scoreboard

  long Scoreboard::alloc (const Ssi& ssi) {
    return p_is->alloc (ssi);
  }

  // allocate a new interrupt

  void Scoreboard::alloc (const Interrupt& svi) {
    p_is->alloc (svi);
  }

  // clean the scoreboard with write-backed instruction
  // the maximum number is the instruction window size

  void Scoreboard::clean (void) {
    for (long i = 0; i < d_swsz; i++) {
      if (p_is->clean () == false) break;
    }
  }

  // set the instruction state by index

  void Scoreboard::setstate (const long ridx, t_state state) {
    p_is->setstate (ridx, state);
  }

  // set the instruction state by instruction

  void Scoreboard::setstate (const Ssi& ssi, t_state state) {
    if (ssi.isvalid () == false) return;
    long ridx = ssi.getrix ();
    p_is->setstate (ridx, state);
  }

  // get the instruction state by index

  bool Scoreboard::getstate (const long ridx, t_state state) const {
    return p_is->getstate (ridx, state);
  }

  // get the instruction state by instruction

  bool Scoreboard::getstate (const Ssi& ssi, t_state state) const {
    if (ssi.isvalid () == false) return false;
    long ridx = ssi.getrix ();
    return p_is->getstate (ridx, state);
  }

  // complete an instruction by marking write-backed and clean

  void Scoreboard::retire (const Ssi& ssi) {
    // check for valid instruction
    if (ssi.isvalid () == false) return;
    // marked write-backed
    setstate (ssi, WRBACK);
    // clean scoreboard entries
    clean ();
  }

  // return a true if a register is marked

  bool Scoreboard::ismarked (const Rid& rid) const {
    return p_rs->ismarked (rid);
  }

  // set a register scoreboard by rid

  void Scoreboard::mark (const Rid& rid, const bool flag) {
    p_rs->mark (rid, flag);
  }

  // check if an instruction has a hazard

  bool Scoreboard::ishazard (const Ssi& ssi) const {
    // do nothing with invalid instruction
    if (ssi.isvalid () == false) return false;
    // check predicate first
    Rid pred = ssi.getpnum ();
    if (ismarked (pred) == true) return true;
    // check source registers
    for (long i = 0; i < IA_MSRC; i++) {
      Rid sreg = ssi.getsnum (i);
      if (ismarked (sreg) == true) return true;
    }
    // check destination registers
    for (long i = 0; i < IA_MDST; i++) {
      Rid dreg = ssi.getdnum (i);
      if (ismarked (dreg) == true) return true;
    }
    // no hazard at all
    return false;
  }

  // check if an instruction is holding
  // a holding condition is defined as an instruction that requires
  // synchronisation. In that case, the output pointer must be equal to the 
  // instruction pointer and not be write-backed

  bool Scoreboard::isholding (const Ssi& ssi) const {
    // check for valid instruction
    if (ssi.isvalid () == false) return false;
    // check for non write-backed instruction
    long ridx = ssi.getrix ();
    if (getstate (ridx, WRBACK) == true) return false;
    // check for interrupted instruction
    if (getstate (ridx, INTERP) == true) {
      if (p_is->ispop (ridx) == false) return true;
    }    
    // check for pipe flush by speculation
    if (getstate (ridx, PFSPEC) == true) {
      if (p_is->ispop (ridx) == false) return true;
    }
    // check for pipe flush by branch taken
    if (getstate (ridx, PFBRTK) == true) {
      if (p_is->ispop (ridx) == false) return true;
    }
    // no holding condition
    return false;
  }
}
