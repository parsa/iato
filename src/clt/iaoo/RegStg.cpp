// ---------------------------------------------------------------------------
// - RegStg.cpp                                                              -
// - iato:iaoo client - operand acquisition stage class implementation       -
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
#include "SlcStg.hpp"
#include "RegStg.hpp"
#include "Exception.hpp"

namespace iato {
  using namespace std;

  // this procedure checks that an instruction is not cancelled with
  // a always true predicate (security only)
  static bool check_pred_cnlf (const Dsi& dsi) {
    // nothing wrong with invalid instruction
    if (dsi.isvalid () == false) return true;
    // get the predicate and do not check for predicate
    Rid rid = dsi.getpnum ();
    if (rid.getlnum () != 0) return true;
    // here the cancel flag must be false
    if (dsi.getcnlf () == false) return true;
    return false;
  }

  // this procedure evaluates an operand with static value associated with
  // the instruction. These values are normally those cosrresponding to
  // the registers that are not renamed (aka speculative).
  static void eval_oprd_inst (const Dsi& dsi, Operand& oprd) {
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
	oprd.setoval (i, dsi.getiip ());
	break;
      case CFMR:
	oprd.setoval (i, dsi.geticfm().getcfm ());
	break;
      default:
	break;
      }
    }
  }

  // this procedure checks that an instruction is predicated and that the
  // predicate is available in the in-flight resources only
  static bool check_pred_inf (const Instr& inst, Bpn* bpn, Urf* urf) {
    // check for valid
    if (inst.isvalid () == false) return false;
    if (inst.ispred  () == false) return false;
    // get the predicate
    Rid  prid = inst.getpnum ();
    // check in the bypass
    Uvr uvr = bpn->eval (prid);
    if (uvr.isvalid () == true) return true;
    // check in the urf
    uvr = urf->eval (prid);
    if (uvr.isvalid () == true) return true;
    // only in the rbk
    return false;
  }

  // this procedure evaluate the instruction predicate with the urf and rbk
  static bool eval_pred (const Rid& pred, Urf* urf, Register* rbk) {
    assert (pred.isvalid () == true);
    // evaluate in the urf
    Uvr uvr = urf->eval (pred);
    if (uvr.isvalid () == true) {
      assert (uvr.gettype () == Uvr::BBV);
      return uvr.getbval ();
    }
    // evaluate in the register bank
    long pnum = pred.getpnum ();
    return rbk->getbval (PREG, pnum);
  }

  // create a new evaluation stage by context

  RegStg::RegStg (Stx* stx, t_unit unit) : DlyStg (stx, unit, RESOURCE_REG) {
    d_gpp  = stx->getbool ("GLOBAL-PREDICATE-PREDICTION");
    p_urf  = 0;
    p_bpn  = 0;
    p_rbk  = 0;
    p_pps  = 0;
    reset  ();
  }

  // create a new evaluation stage by context and name

  RegStg::RegStg (Stx* stx, t_unit unit, 
		  const string& name) : DlyStg (stx, unit, name) {
    d_gpp  = stx->getbool ("GLOBAL-PREDICATE-PREDICTION");
    p_urf  = 0;
    p_bpn  = 0;
    p_rbk  = 0;
    p_pps  = 0;
    reset  ();
  }

  // activate this evaluation stage. The order of operand evaluation
  // is not important since it is done in parallel.

  void RegStg::activate (void) {
    // get the previous stage and map it to select
    SlcStg* slc = dynamic_cast <SlcStg*> (p_pstg);
    assert (slc);
    // get the selected instruction
    Dsi inst = slc->getinst ();
    // update latency or broadcast result
    d_inst = p_gcs->setrdy (inst);
    // evaluate the operands from a valid instruction
    if (d_inst.isvalid () == true) {
      // get predicate info
      Rid  pred = d_inst.getpnum ();
      bool prdy = true;
      // evaluate first in the bypass network
      Uvr uvr = p_bpn->eval (pred);
      if (uvr.isvalid () == true) {
	assert (uvr.gettype () == Uvr::BBV);
	bool bval = uvr.getbval ();
	if (bval == false) d_inst.setcnlf (true);
      } else {
	// check if we can evaluate the predicate in the urf or rbk
	if (p_urf->isready (pred) == true) {
	  bool bval = eval_pred (pred, p_urf, p_rbk);
	  if (bval == false) d_inst.setcnlf (true);
	} else {
	  prdy = false;
	}
      }
      // check for consistency
      assert (check_pred_cnlf (d_inst) == true);      
      // if the global predicate prediction is active (for analysis purpose)
      // the prediction is forced here and the predicate ready flag is set.
      // this rule applies only to non branch instructions.
      // carefull here since a deadlock can occur if the misprediction persit
      // for this reason, we force the prediction with only in-flight
      // predicate computation
      if ((d_gpp == true) && (check_pred_inf (inst, p_bpn, p_urf) == true) &&
	  (d_inst.isbr () == false) && (p_pps->ispredict (d_inst) == true)) {
	bool pval = p_pps->predict (d_inst);
	d_inst.setcnlf (!pval);
	d_inst.setppfl (true);
	prdy = true;
      }
      // here the predicate has been evaluated or the predicate was not ready
      // if the predicate was evaluated, the instruction is marked
      // cancelled or not. If not ready, the instruction must be rescheduled
      // unless the instruction is a branch. In that case, the operation
      // continues but the predicate is marked not ready. Actually, if a 
      // predicate prediction system is available, an attempt to predict the
      // predicate is made at this stage. If the predicate cannot be predicted,
      // the instruction is rescheduled. If not, the predicate is predicted and
      // the predicate predict flag along its value is set in the instruction.
      // If the instruction is not cancelled, the evaluation is done across the
      // bypass network, the urf and rbk. 
      if (prdy == false) {
	// check for a branch
	if (d_inst.isbr () == true) {
	  prdy = true;
	  d_inst.setpnrd (true);
	} else {
	  // try to predict the predicate
	  if (p_pps->ispredict (d_inst) == true) {
	    bool pval = p_pps->predict (d_inst);
	    d_inst.setcnlf (!pval);
	    d_inst.setppfl (true);
	    prdy = true;
	  } else {
	    // the predicate cannot be predicted, so reschedule
	    p_gcs->resched (d_inst);
	    d_inst.reset ();
	    d_oprd.reset ();
	  }
	}
      }
      if (prdy == true) {
	if (d_inst.getcnlf () == false) {
	  d_oprd = d_inst.getoper ();
	  // evaluate locally (a.k.a speculative cfm in the instruction)
	  eval_oprd_inst (d_inst, d_oprd);
	  // evaluate within the bypass network
	  p_bpn->eval (d_oprd);
	  // evaluate within the urb or reschedule
	  if (p_urf->isready (d_oprd) == true) {
	    p_urf->eval (d_oprd);
	    // evaluate within the register bank
	    p_rbk->eval (d_oprd);
	    // check that it is all done
	    assert (d_oprd.isvalid () == true);
	  } else {
	    p_gcs->resched (d_inst);
	    d_inst.reset ();
	    d_oprd.reset ();
	  }
	} else {
	  // here, the instruction has been cancelled
	  d_oprd.reset ();
	}
      }
    } else {
      // the instruction was not valid
      d_oprd.reset ();
    }
    // check if the previous stage is halted - if it is halted, we check
    // that the station is empty before going into halt mode
    if (p_pstg) d_halt = p_pstg->ishalted ();
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
    cout << "\tresource type\t\t: evaluation stage" << endl;
    cout << "\tdelayable latency  \t: " << getdlat () << endl;
  }

  // bind this stage from the environment

  void RegStg::bind (Env* env, Stage* pstg, Stage* nstg) {
    // bind the stage locally
    DlyStg::bind (env, pstg, nstg);
    // bind the trb
    p_urf = dynamic_cast <Urf*> (env->get (RESOURCE_URF));
    if (!p_urf) {
      string msg = "cannot bind universal register file within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the bpn
    p_bpn = dynamic_cast <Bpn*> (env->get (RESOURCE_BPN));
    if (!p_bpn) {
      string msg = "cannot bind bypass network within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the register bank
    p_rbk = dynamic_cast <Register*> (env->get (RESOURCE_RBK));
    if (!p_rbk) {
      string msg = "cannot bind register bank within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the predicate predictor 
    p_pps = dynamic_cast <Predicate*> (env->get (RESOURCE_PPS));
    if (!p_pps) {
      string msg = "cannot bind predicate predictor within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
  }
}
