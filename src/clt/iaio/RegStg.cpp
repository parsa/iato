// ---------------------------------------------------------------------------
// - RegStg.cpp                                                              -
// - iato:iato client - register read stage class implementation             -
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

#include "Srn.hpp"
#include "RenStg.hpp"
#include "RegStg.hpp"

namespace iato {
  using namespace std;

  // this procedure evaluates an operand with static value associated with
  // the instruction. These values are normally those cosrresponding to
  // the registers that are not renamed (aka speculative).
  static void eval_oprd (const Ssi& ssi, Operand& oprd) {
    // iterate through the rid
    for (long i = 0; i < IA_MSRC; i++) {
      // discard already valid value
      if (oprd.isvalid (i) == true) continue;
      // get the rid and check validity
      Rid rid = oprd.getrid (i);
      if (rid.isvalid () == false) continue;
      // evaluates with the rid type
      switch (rid.gettype ()) {
      case IPRG:
	oprd.setoval (i, ssi.getiip ());
	break;
      case CFMR:
	oprd.setoval (i, ssi.geticfm().getcfm ());
	break;
      default:
	break;
      }
    }
  }

  // create a new register read stage by context and unit

  RegStg::RegStg (Stx* stx, t_unit unit) : DlyStg (stx, unit, RESOURCE_REG) {
    p_iib = 0;
    p_ebn = 0;
    p_lbn = 0;
    p_hdl = 0;
    p_rbk = 0;
    p_psb = 0;
    reset ();
  }

  // create a new register read stage by context, index and unit

  RegStg::RegStg (Stx* stx, t_unit unit, const long sidx) : 
          DlyStg (stx, unit, sidx, RESOURCE_REG) {
    p_iib = 0;
    p_ebn = 0;
    p_lbn = 0;
    p_hdl = 0;
    p_rbk = 0;
    p_psb = 0;
    reset ();
  }

  // create a new register read stage by context and name

  RegStg::RegStg (Stx* stx, t_unit unit, const long sidx, const string& name) :
          DlyStg (stx, unit, sidx, name) {
    p_iib = 0;
    p_ebn = 0;
    p_lbn = 0;
    p_hdl = 0;
    p_rbk = 0;
    p_psb = 0;
    reset ();
  }

  // reset this register read stage

  void RegStg::reset (void) {
    DlyStg::reset  ();
    if (p_hdl) p_hdl->reset ();
    clean ();
  }
    
  // flush this register read  stage

  void RegStg::flush (void) {
    DlyStg::flush  ();
    clean ();
  }

  // return true if the stage is holding

  bool RegStg::isholding (void) const {
    return p_hdl->ishazard ();
  }

  // proceed the register read instruction. the whole problem
  // is to grab the instruction, check with all of them that the
  // register reqd can proceed by looking at dependencies with
  // the help of the scoreboard

  void RegStg::activate (void) {
    // get the previous stage
    RenStg* ren = dynamic_cast <RenStg*> (p_pstg);
    // get the instruction
    d_inst = ren->grabinst ();
    // check for valid instruction - reset stage data if invalid
    if (d_inst.isvalid () == false) {
      clean ();
      return;
    }
    // if the instruction has been interrupted, do nothing
    if (d_inst.getintr () == true) {
      if (p_tracer && (d_inst.isvalid () == true)) {
	Record rcd (d_name, d_inst);
	p_tracer->add (rcd);
      }
      d_oprd.reset ();
      return;
    }
    // try to evaluate the operands
    try {
      d_oprd = d_inst.getoper ();
      // evaluate the predicate across the register bank
      Rid  pred = d_inst.getpnum ();
      bool cnlf = !p_rbk->getbval (pred);
      d_inst.setcnlf (cnlf);
      // update the predicate with the late bypass network
      p_lbn->predup (d_inst);
      // update the predicate with the early bypass network
      p_ebn->predup (d_inst);
      // update static operands
      eval_oprd (d_inst, d_oprd);
      // update the operand from the register bank
      p_rbk->eval (d_oprd);
      // update them from the late bypass network
      p_lbn->update (d_oprd);
      // update them from the early bypass network
      p_ebn->update (d_oprd);
      // mark the scoreboard
      p_psb->lock (d_inst);
    } catch (Interrupt& vi) {
      // set the interrupt info
      d_inst.setintr (true);
      vi.setinst (d_inst);
      // set the interrupt buffer
      long iidx = d_inst.getiib ();
      p_iib->setintr (iidx, vi);
      // reset the operand
      d_oprd.reset ();
    }
    // check if the previous stage is halted
    if (p_pstg && (p_pstg->ishalted () == true)) {
      d_halt = true;
    }
    // update the tracer
    if (p_tracer) {
      if (d_inst.isvalid () == true) {
	Record rcd (d_name, d_inst);
	p_tracer->add (rcd);
      }
      if (d_halt == true) {
	Record rcd (d_name);
	rcd.settype (Record::HALTED);
	p_tracer->add (rcd);
      }
    }
  }
  
  // report some stage information

  void RegStg::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: instruction renaming" << endl;
    cout << "\tdelayable latency  \t: " << getdlat () << endl;
  }

  // bind this stage from the environment

  void RegStg::bind (Env* env, Stage* pstg, Stage* nstg) {
    // bind the stage locally
    Stage::bind (env, pstg, nstg);
    // bind the iib
    p_iib = dynamic_cast <Iib*> (env->get (RESOURCE_IIB));
    if (!p_iib) {
      string msg = "cannot bind iib within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the early bypass network
    p_ebn = dynamic_cast <Bpn*> (env->get (RESOURCE_EBN));
    if (!p_ebn) {
      string msg = "cannot bind early bypass network within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the late bypass network
    p_lbn = dynamic_cast <Bpn*> (env->get (RESOURCE_LBN));
    if (!p_lbn) {
      string msg = "cannot bind late bypass network within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the register bank
    p_rbk = dynamic_cast <Register*> (env->get (RESOURCE_RBK));
    if (!p_rbk) {
      string msg = "cannot bind register bank within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the scoreboard
    p_psb = dynamic_cast <Scoreboard*> (env->get (RESOURCE_PSB));
    if (!p_psb) {
      string msg = "cannot bind scoreboard within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the hazard detection logic
    p_hdl = dynamic_cast <Hazard*> (env->get (RESOURCE_HDL));
    if (!p_hdl) {
      string msg = "cannot bind scoreboard within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
  }
}
