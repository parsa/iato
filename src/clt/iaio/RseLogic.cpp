// ---------------------------------------------------------------------------
// - RseLogic.hpp                                                            -
// - iato:mac library - rse control logic class implementation               -
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

#include "RseLogic.hpp"

namespace iato {

  // create a default rse logic

  RseLogic::RseLogic (void) {
    p_rstk = new RseStack;
    reset ();
  }

  // create a rse logic with a context

  RseLogic::RseLogic (Mtx* mtx) : Rse (mtx) {
    p_rstk = new RseStack (mtx);
    reset ();
  }

  // reset this rse logic

  void RseLogic::reset (void) {
    Rse::reset ();
    p_rstk->reset ();
    d_spste = d_state;
  }

  // flush this rse logic

  void RseLogic::flush (void) {
    Rse::flush ();
    p_rstk->reset ();
    d_spste = d_state;
  }

  // report this resource

  void RseLogic::report (void) const {
    Resource::report ();
    cout << "\tresource type\t\t: << speculative rse logic" << endl;
  }

  // set the speculative rse state by state

  void RseLogic::setsst (const State& state) {
    d_spste = state;
  }

  // return the speculative rse state

  Rse::State RseLogic::getsst (void) const {
    return d_spste;
  }

  // rename an instruction with the speculative state

  void RseLogic::rename (Instr& inst) const {
    // check for valid
    if (inst.isvalid () == false) return;
    // rename the predicate operand
    Rid prnum = inst.getpnum ();
    Rid phnum = d_spste.maprid (prnum, false);
    inst.setpnum (phnum);
    // rename the source operands
    for (long i = 0; i < IA_MSRC; i++) {
      Rid snum = inst.getsnum (i);
      Rid pnum = d_spste.maprid (snum, false);
      inst.setsnum (i, pnum);
    }
    // rename the destination operands
    for (long i = 0; i < IA_MDST; i++) {
      Rid dnum = inst.getdnum (i);
      Rid pnum = d_spste.maprid (dnum, true);
      inst.setdnum (i, pnum);
    }
  }

  // preset the rse with an instruction

  void RseLogic::preset (const Instr& inst) {
    // check for valid instruction
    if (inst.isvalid () == false) return;
    t_iopc iopc = inst.getiopc ();
    // check for alloc
    if (iopc == M_ALLOC) {
      // get alloc arguments
      t_byte sof = inst.getimmv(0);
      t_byte sol = inst.getimmv(1);
      t_byte sor = inst.getimmv(2);
      // get the cfm and update it
      Cfm cfm = d_spste.getcfm ();
      cfm.alloc (sof, sol, sor);
      // in speculative mode - clear the rrb's
      cfm.clrrrb ();
      // update the rse state
      d_spste.alloc (cfm);
    }
  }

  // post set the rse with an instruction

  void RseLogic::aftset (const Instr& inst) {
    // check for valid instruction
    if (inst.isvalid () == false) return;
  }

  // update the rse state with a result

  void RseLogic::update (const Result& resl) {
    // check for valid result
    if (resl.isvalid () == false) return;
    // loop in the result
    for (long i = 0; i < IA_MDST; i++) {
      Cfm cfm = OCTA_0;
      switch (resl.getrop (i)) {
      case Result::RSE_ALC:
	cfm = resl.getoval (i);
	d_state.alloc (cfm);
	d_state.spill ();
	break;
      case Result::RSE_CAL:
	cfm = resl.getoval (i);
	d_state.call (cfm);
	break;
      case Result::RSE_RET:
	cfm = resl.getoval (i);
	d_state.retn (cfm);
	break;
      case Result::RSE_LOP:
	cfm = resl.getoval (i);
	d_state.loop (cfm);
	break;
      case Result::RSE_RRB:
	cfm = resl.getoval (i);
	d_state.setcfm (cfm);
	break;
      default:
	break;
      }
    }
  }
}
