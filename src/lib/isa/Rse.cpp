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

namespace iato {


  // -------------------------------------------------------------------------
  // - rse state section                                                     -
  // -------------------------------------------------------------------------

  // create a default rse state

  Rse::State::State (void) {
    setngr (GR_LRSZ);
  }

  // create a rse state a register size

  Rse::State::State (const long ngr) {
    setngr (ngr);
  }

  // create a rse state a register size and a cfm

  Rse::State::State (const long ngr, const Cfm& cfm) {
    setngr (ngr);
    setcfm (cfm);
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
    // spilling is checked as a condition
    assert ((d_bof + d_sof) <= d_ngr);
  }

  // check if the rse needs to fill registers

  void Rse::State::fill (void) {
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

  bool Rse::validate (const Cfm& cfm) const {
    State state (d_ngr, cfm);
    return (d_state == state);
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
