// ---------------------------------------------------------------------------
// - Rename.hpp                                                              -
// - iato:iaka client - rse renaming class implementation                    -
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

#include "Rename.hpp"

namespace iato {
  
  // create a default rse rename

  Rename::Rename (void) {
    p_mem = 0;
    p_rbk = 0;
  }

  // create a rse rename with a context

  Rename::Rename (Atx* atx) : Rse (atx) {
    p_mem = 0;
    p_rbk = 0;
  }

  // bind the memory and register bank to this rse

  void Rename::bind (Memory* mem, Register* rbk) {
    p_mem = mem;
    p_rbk = rbk;
  }

  // preset the rse with an instruction

  void Rename::preset (const Instr& inst) {
    // check for valid instruction
    if (inst.isvalid () == false) return;
    // check for alloc
    if (inst.getiopc () == M_ALLOC) {
      // get alloc arguments
      long sof = inst.getimmv (0);
      long sol = inst.getimmv (1);
      long sor = inst.getimmv (2);
      // get the current cfm and update it
      Cfm cfm = d_state.getcfm ();
      cfm.setfld (Cfm::SOF, sof);
      cfm.setfld (Cfm::SOL, sol);
      cfm.setfld (Cfm::SOR, sor);
      // update the rse state
      d_state.alloc (cfm);
    }
  }

  // after set the rse state with an instruction

  void Rename::aftset (const Instr& inst) {
    // check for valid instruction
    if (inst.isvalid () == false) return;
    // check for spilling
    d_state.spill ();
  }

  // update the rse state with a result

  void Rename::update (const Result& resl) {
    // check for valid result
    if (resl.isvalid () == false) return;
    // loop in the result
    for (long i = 0; i < IA_MDST; i++) {
      Cfm cfm = OCTA_0;
      switch (resl.getrop (i)) {
      case Result::RSE_ALC:
	cfm = resl.getoval (i);
	assert (d_state.chkcfm (cfm) == true);
	break;
      case Result::RSE_CAL:
	cfm = resl.getoval (i);
	d_state.call (cfm);
	break;
      case Result::RSE_RRB:
	cfm = resl.getoval (i);
	d_state.setcfm (cfm);
	break;
      case Result::RSE_RET:
	cfm = resl.getoval (i);
	d_state.retn (cfm);
	d_state.fill ();
	break;
      case Result::RSE_LOP:
	cfm = resl.getoval (i);
	d_state.loop (cfm);
	break;
      default:
	break;
      }
    }
  }
}
