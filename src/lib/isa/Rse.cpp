// ---------------------------------------------------------------------------
// - Rse.cpp                                                                 -
// - iato:isa library - rse class implementation                             -
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

#include "Isa.hpp"
#include "Rse.hpp"
#include "Lrn.hpp"
#include "Interrupt.hpp"
#include "Memory.hpp"
#include "Register.hpp"

namespace iato {

  static const t_octa BSP_MASK = 0x00000000000001F8ULL;

  static inline long nxtgr (const long reg, const long ngr) {
    assert (reg >= GR_STBS);
    assert (reg < ngr);
    long result = reg + 1;
    return (result >= ngr) ? GR_STBS : result;
  }

  static inline long prvgr (const long reg, const long ngr) {
    assert (reg >= GR_STBS);
    assert (reg < ngr);
    long result = reg - 1;
    return (result < GR_STBS) ? (ngr - 1) : result;
  }

  static inline long dstgr (const long from, const long to, const long ngr) {
    assert (from >= GR_STBS);
    assert (from < ngr);
    assert (to   >= GR_STBS);
    assert (to   < ngr);
    if (from <= to) return to - from;
    return (ngr - from) + (to - GR_STBS);
  }

  static inline t_octa bsp_add (const t_octa bsp, const t_octa nreg) {
    const t_octa slot = (bsp & BSP_MASK) >> 3;
    return ((bsp >> 3) + nreg + ((slot + nreg) / 63)) << 3;
  }

  static inline t_octa bsp_sub (const t_octa bsp, const t_octa nreg) {
    const t_octa slot = (bsp & BSP_MASK) >> 3;
    return ((bsp >> 3) - nreg - ((62 - slot + nreg) / 63)) << 3;
  }


  // -------------------------------------------------------------------------
  // - rse state section                                                     -
  // -------------------------------------------------------------------------

  // create a default rse state

  Rse::State::State (void) {
    p_rbk = 0;
    p_mem = 0;
    setngr (GR_LRSZ);
  }

  // create a rse state a register size

  Rse::State::State (const long ngr) {
    p_rbk = 0;
    p_mem = 0;
    setngr (ngr);
  }

  // create a rse state a register size and a cfm

  Rse::State::State (const long ngr, const Cfm& cfm) {
    p_rbk = 0;
    p_mem = 0;
    setngr (ngr);
    setcfm (cfm);
  }

  // bind the rse state with a register bank and a backing store memory

  void Rse::State::bind (Memory* mem, Register* rbk) {
    p_mem = mem;
    p_rbk = rbk;
  }

  // reset this rse state

  void Rse::State::reset (void) {
    d_sof = DEF_SOF;
    d_sol = DEF_SOL;
    d_sor = DEF_SOR;
    d_rgr = DEF_RGR;
    d_rfr = DEF_RFR;
    d_rpr = DEF_RPR;
    d_bof = GR_STBS;
    d_ldr = GR_STBS;
    d_str = GR_STBS;
    d_dty = 0;
  }

  // compare this rse state with another one

  bool Rse::State::operator == (const Rse::State& that) const {
    if (d_sof != that.d_sof) return false;
    if (d_sol != that.d_sol) return false;
    if (d_sor != that.d_sor) return false;
    if (d_rgr != that.d_rgr) return false;
    if (d_rfr != that.d_rfr) return false;
    if (d_rpr != that.d_rpr) return false;
    if (d_bof != that.d_bof) return false;
    return true;
  }

  /// set the state ngr fter a reset

  void Rse::State::setngr (const long ngr) {
    reset ();
    d_ngr = ngr;
    assert ((d_ngr > 0) && ((d_ngr % 16) == 0));
  }

  // return the cfm associated with this state

  Cfm Rse::State::getcfm (void) const {
    Cfm cfm;
    cfm.setfld (Cfm::SOF, d_sof);
    cfm.setfld (Cfm::SOL, d_sol);
    cfm.setfld (Cfm::SOR, d_sor >> 3);
    cfm.setrrb (Cfm::RGR, d_rgr);
    cfm.setrrb (Cfm::RFR, d_rfr);
    cfm.setrrb (Cfm::RPR, d_rpr);
    return cfm;
  }

  // set the rse state with a cfm 

  void Rse::State::setcfm (const Cfm& cfm) {
    d_sof = cfm.getfld (Cfm::SOF);
    d_sol = cfm.getfld (Cfm::SOL);
    d_sor = cfm.getfld (Cfm::SOR) << 3;
    d_rgr = cfm.getrrb (Cfm::RGR);
    d_rfr = cfm.getrrb (Cfm::RFR);
    d_rpr = cfm.getrrb (Cfm::RPR);
    // make sure all rotating are within range
    assert ((d_rgr <= 0) && (-d_rgr <= d_sor));
    assert ((d_rfr <= 0) && (-d_rfr <= FR_RTSZ));
    assert ((d_rpr <= 0) && (-d_rpr <= PR_RTSZ));
  }

  // get the state bottom of frame

  long Rse::State::getbof (void) const {
    return d_bof;
  }

  // set the state with a bottom of frame

  void Rse::State::setbof (const long val) {
    assert (val >= GR_STBS);
    d_bof = val;
  }

  // check a cfm against the current state

  bool Rse::State::chkcfm (const Cfm& cfm) const {
    if (d_sof != cfm.getfld (Cfm::SOF)) return false;
    if (d_sol != cfm.getfld (Cfm::SOL)) return false;
    if (d_sor != cfm.getfld (Cfm::SOR) << 3) return false;
    if (d_rgr != cfm.getrrb (Cfm::RGR)) return false;
    if (d_rfr != cfm.getrrb (Cfm::RFR)) return false;
    if (d_rpr != cfm.getrrb (Cfm::RPR)) return false;
    return true;
  }

  // map a general register

  long Rse::State::mapgr (const long lnum, const bool rwf) const {
    assert ((lnum >= 0) && (lnum < GR_LRSZ));
    // static register are not mapped
    if (lnum < GR_STBS) return lnum;
    // make sure the frame has been allocated
    if ((rwf == true) && ((lnum - GR_STBS) >= d_sof)) {
      string msg = "out of frame register renaming";
      throw Interrupt (FAULT_IT_OPER_LEGAL, msg);
    }
    // compute rotating index    
    long result = lnum - GR_STBS;
    if ((d_sor != 0) && (result < d_sor)) {
      result += d_rgr;
      if (result < 0) result += d_sor;
    }
    // index relative to the frame
    result += d_bof;
    result = (result >= d_ngr) ? (result - d_ngr) + GR_STBS : result;
    assert ((result >= GR_STBS) && (result < d_ngr));
    return result;
  }

  // map a floating register

  long Rse::State::mapfr (const long lnum) const {
    assert ((lnum >= 0) && (lnum < FR_LRSZ));
    // static register are not mapped
    if (lnum < FR_RTBS) return lnum;
    long result = lnum + d_rfr;
    // if we extend above the register file, then wrap around the base
    if (result < FR_RTBS) result += FR_RTSZ;
    assert ((result >= FR_RTBS) && (result < FR_LRSZ));
    return result;
  }

  // map a predicate register

  long Rse::State::mappr (const long lnum) const {
    assert ((lnum >= 0) && (lnum < PR_LRSZ));
    // static register are not mapped
    if (lnum < PR_RTBS) return lnum;
    long result = lnum + d_rpr;
    // if we extend above the register file, then wrap around the base
    if (result < PR_RTBS) result += PR_RTSZ;
    assert ((result >= PR_RTBS) && (result < PR_LRSZ));
    return result;
  }

  // update the state with an alloc instruction

  void Rse::State::alloc (const Cfm& cfm) {
    // update the cfm
    setcfm (cfm);
    // make sure all rename are 0
    if (d_sor != 0) {
      if ((d_rgr != 0) || (d_rfr != 0) || (d_rpr != 0)) {
	string msg = "non zero rrb during alloc";
	throw Interrupt (FAULT_IT_RESV_RGFD, msg);
      }
    }
  }

  // update the state with a call instruction

  void Rse::State::call (const Cfm& cfm) {
    // compute bottom of frame and dirty
    d_bof += d_sol;
    d_dty += d_sol;
    d_bof = (d_bof >= d_ngr) ? (d_bof - d_ngr) + GR_STBS : d_bof;
    // set the cfm
    setcfm (cfm);
  }

  // update the state with a return instruction

  void Rse::State::retn (const Cfm& cfm) {
    // set the cfm
    setcfm (cfm);
    // recompute bottom of frame and number of dirty
    d_bof -= d_sol;
    d_dty -= d_sol;
    d_bof  = (d_bof < GR_STBS) ? (d_ngr - (GR_STBS - d_bof)) : d_bof;
  }

  // update the state with a loop instruction

  void Rse::State::loop (const Cfm& cfm) {
    // set the cfm
    setcfm (cfm);
  }

  // map an rid by doing register renaming

  Rid Rse::State::maprid (const Rid& rid, const bool rwf) const {
    // check for valid rid
    if (rid.isvalid () == false) return rid;
    // check for valid renaming
    if (rid.islrnm  () == false) return rid;
    // rename ny mapping
    Rid    prid = rid;
    t_lreg lreg = rid.gettype ();
    long   lnum = rid.getlnum ();
    switch (lreg) {
    case GREG:
    case NREG:
      prid.setpnum (mapgr (lnum, rwf));
      break;
    case FREG:
      prid.setpnum (mapfr (lnum));
      break;
    case PREG:
      prid.setpnum (mappr (lnum));
      break;
    default:
      break;
    }
    return prid;
  }

  // check if the rse needs to spill registers

  void Rse::State::spill (void) {
    // no backing store binding -> legacy behavior (best-effort check)
    if ((p_mem == 0) || (p_rbk == 0)) {
      assert ((d_bof + d_sof) <= d_ngr);
      return;
    }
    // compute stacked register file size and spill capacity
    const long psz = d_ngr - GR_STBS;
    assert (d_sof >= 0);
    assert (d_sof <= psz);
    long cap = psz - d_sof;
    if (cap < 0) cap = 0;
    // compute desired number of dirty registers kept in the physical file
    long want = d_dty;
    if (want > cap) want = cap;
    // current number of dirty registers in the physical file
    long cur = dstgr (d_ldr, d_bof, d_ngr);
    // spill oldest dirty registers until we meet the target
    while (cur > want) {
      // get the spill address (bspstore) and rnat
      t_octa bsps = p_rbk->getoval (AREG, AR_BSPS);
      t_octa rnat = p_rbk->getoval (AREG, AR_RNAT);
      // if we ever land on an rnat slot, flush it and move on
      t_octa slot = (bsps & BSP_MASK) >> 3;
      if (slot == 63) {
	p_mem->writeocta (bsps, rnat);
	rnat = OCTA_0;
	bsps = bsps + 8;
	slot = (bsps & BSP_MASK) >> 3;
      }
      assert (slot < 63);
      // spill the physical register at d_ldr to backing store memory
      const t_octa oval = p_rbk->getoval (GREG, d_ldr);
      const bool   nval = p_rbk->getbval (NREG, d_ldr);
      p_mem->writeocta (bsps, oval);
      // update rnat bit for this slot
      const t_octa bit = static_cast<t_octa>(1ULL) << slot;
      rnat = nval ? (rnat | bit) : (rnat & ~bit);
      // if this was the last slot in the group, store the rnat slot too
      if (slot == 62) {
	p_mem->writeocta (bsps + 8, rnat);
	rnat = OCTA_0;
      }
      // advance the bspstore and save rnat
      bsps = bsp_add (bsps, 1);
      p_rbk->write (AREG, AR_BSPS, bsps);
      p_rbk->write (AREG, AR_RNAT, rnat);
      // advance the load/store register pointer
      d_ldr = nxtgr (d_ldr, d_ngr);
      d_str = d_ldr;
      cur--;
    }
  }

  // check if the rse needs to fill registers

  void Rse::State::fill (void) {
    // no backing store binding -> nothing to do
    if ((p_mem == 0) || (p_rbk == 0)) return;
    // compute stacked register file size and fill capacity
    const long psz = d_ngr - GR_STBS;
    assert (d_sof >= 0);
    assert (d_sof <= psz);
    long cap = psz - d_sof;
    if (cap < 0) cap = 0;
    // desired number of dirty registers kept in the physical file
    long want = d_dty;
    if (want > cap) want = cap;
    // current number of dirty registers in the physical file
    long cur = dstgr (d_ldr, d_bof, d_ngr);
    // fill oldest spilled registers until we meet the target
    while (cur < want) {
      // extend the in-register dirty region
      d_ldr = prvgr (d_ldr, d_ngr);
      d_str = d_ldr;
      // get the bspstore and rnat
      t_octa bsps = p_rbk->getoval (AREG, AR_BSPS);
      t_octa rnat = p_rbk->getoval (AREG, AR_RNAT);
      const t_octa slot = (bsps & BSP_MASK) >> 3;
      // if we cross a group boundary, load the rnat slot for the previous group
      if (slot == 0) {
	const t_octa rnaddr = bsps - 8;
	rnat = p_mem->readocta (rnaddr);
	p_rbk->write (AREG, AR_RNAT, rnat);
      }
      // move bspstore back by one register slot (skipping rnat slots)
      bsps = bsp_sub (bsps, 1);
      const t_octa nslot = (bsps & BSP_MASK) >> 3;
      assert (nslot < 63);
      // load register value from backing store
      const t_octa oval = p_mem->readocta (bsps);
      const bool   nval = ((rnat >> nslot) & 0x1ULL) != 0;
      p_rbk->write (GREG, d_ldr, oval);
      p_rbk->write (NREG, d_ldr, nval);
      // update bspstore
      p_rbk->write (AREG, AR_BSPS, bsps);
      cur++;
    }
  }

  // dump the rse state (for debug)

  void Rse::State::dump (const string& prefix) const {
    cerr << prefix;
    cerr << "  bof = " << d_bof;
    cerr << ", sof = " << d_sof;
    cerr << ", sol = " << d_sol;
    cerr << ", sor = " << d_sor;
    cerr << ", rgr = " << d_rgr;
    cerr << ", rfr = " << d_rfr;
    cerr << ", rpr = " << d_rpr << endl;
  }

  // -------------------------------------------------------------------------
  // - rse section                                                           -
  // -------------------------------------------------------------------------

  
  // create a new default register stack engine

  Rse::Rse (void) : Resource (RESOURCE_RSE) {
    d_ngr = GR_LRSZ;
    reset ();
  }

  // create a register stack engine by context

  Rse::Rse (Ctx* ctx) : Resource (RESOURCE_RSE) {
    d_ngr = ctx->getlong ("LR-GR-SIZE"); assert (d_ngr > 0);
    reset ();
  }

  // create a register stack engine by context and name

  Rse::Rse (Ctx* ctx, const string& name) : Resource (name) {
    d_ngr = ctx->getlong ("LR-GR-SIZE"); assert (d_ngr > 0);
    reset ();
  }

  // reset the rse

  void Rse::reset(void){
    d_state.setngr (d_ngr);
  }

  // flush this rse

  void Rse::flush (void) {
  }

  // bind the backing store memory and register bank for spill/fill

  void Rse::bind (Memory* mem, Register* rbk) {
    d_state.bind (mem, rbk);
  }

  // set the rse state by state

  void Rse::setste (const State& state) {
    d_state = state;
  }

  // return the current rse state

  Rse::State Rse::getste (void) const {
    return d_state;
  }

  // set the speculative rse state by state

  void Rse::setsst (const State& state) {
    d_state = state;
  }

  // return the speculative rse state

  Rse::State Rse::getsst (void) const {
    return d_state;
  }

  // validate a state against the rse state

  bool Rse::validate (const State& state) const {
    return (d_state == state);
  }

  // validate a cfm against the rse state

  bool Rse::validate (const Cfm& cfm) const {
    return d_state.chkcfm (cfm);
  }

  // rename an instruction by doing rid mapping
  
  void Rse::rename (Instr& inst) const {
    // check for valid
    if (inst.isvalid () == false) return;
    // rename the predicate operand
    Rid prnum = inst.getpnum ();
    Rid phnum = d_state.maprid (prnum, false);
    inst.setpnum (phnum);
    // rename the source operands
    for (long i = 0; i < IA_MSRC; i++) {
      Rid snum = inst.getsnum (i);
      Rid pnum = d_state.maprid (snum, false);
      inst.setsnum (i, pnum);
    }
    // rename the destination operands
    for (long i = 0; i < IA_MDST; i++) {
      Rid dnum = inst.getdnum (i);
      Rid pnum = d_state.maprid (dnum, true);
      inst.setdnum (i, pnum);
    }
  }
}
