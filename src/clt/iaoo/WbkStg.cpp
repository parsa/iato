// ---------------------------------------------------------------------------
// - WbkStg.cpp                                                              -
// - iato:iaoo client - write-back stage class implementation                -
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
#include "ResStg.hpp"
#include "WbkStg.hpp"

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

  // this procedure allocate a new irb entry and update the rob with
  // the irb index
  static void update_rob (const Dsi& dsi, const Result& resl, 
			  const Interrupt& intr, Irb* irb, 
			  Rob* rob, Detect* det) {
    // check for valid instruction
    if (dsi.isvalid () == false) return;
    // allocate a new irb entry and get rob index
    long index = irb->alloc  (dsi, resl); assert (index != -1);
    long ridx  = dsi.getrix  ();
    bool cnlf  = dsi.getcnlf ();
    // update the rob with execution info
    rob->setirb  (ridx, index);
    rob->setexe  (ridx, true);
    rob->setcnlf (ridx, cnlf);
    rob->setintr (ridx, intr);
    // call the detection logic to validate speculation
    rob->setbsip (ridx, det->chksip (dsi, resl));
  }

  // create a new write-back stage by context and unit

  WbkStg::WbkStg (Stx* stx, t_unit unit): Stage (stx, RESOURCE_WBK) {
    d_unit = unit;
    p_irb  = 0;
    p_rob  = 0;
    p_urf  = 0;
    p_gcs  = 0;
    p_det  = 0;
    reset ();
  }

  // create a new write-back stage by context, unit and name

  WbkStg::WbkStg (Stx* stx, t_unit unit, 
		  const string& name) : Stage (stx, name) {
    d_unit = unit;
    p_irb  = 0;
    p_rob  = 0;
    p_urf  = 0;
    p_gcs  = 0;
    p_det  = 0;
    reset ();
  }

  // activate this update stage - this is done by grabbing the
  // instruction and associated result. The rob is updated with
  // the irb index and the instruction is marked executed. At the same time
  // the urb is updated with the result

  void WbkStg::activate (void) {
    // get the previous stage and map it to a result stage
    ResStg* res = dynamic_cast <ResStg*> (p_pstg);
    assert (res);
    // get the execute instruction, result and interrupt
    Dsi       inst = res->getinst ();
    Result    resl = res->getresl ();
    Interrupt intr = res->getintr ();
    // update latency or broadcast tags for those instructions
    // that have latency or are cancelled
    inst = p_gcs->setrdy (inst);
    // if the instruction is valid, some update occurs, depending on
    // the cancellation status
    if (inst.isvalid () == true) {
      assert (check_pred_cnlf (inst) == true);
      // update the urf depending on whether the instruction is cancelled
      if (inst.getcnlf () == false) {
	p_urf->update (resl);
      } else {
	p_urf->reroute (resl);
      }
      // update the rob
      update_rob (inst, resl, intr, p_irb, p_rob, p_det);
    }
    // broadcast tag for instruction cancellation
    p_gcs->setcnl (resl);
    // clear station entry
    p_gcs->clear (inst);
    // clear bypass network entry
    p_bpn->clear (resl);
    // check if the previous stage is halted
    if (p_pstg) d_halt = p_pstg->ishalted ();
    // update the tracer
    if (p_tracer) {
      if (inst.isvalid () == true) {
	Record rcd (d_name, inst);
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

  void WbkStg::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: update stage" << endl;
    cout << "\tdelayable latency  \t: " << getdlat () << endl;
  }

  // bind this stage from the environment

  void WbkStg::bind (Env* env, Stage* pstg, Stage* nstg) {
    // bind the stage locally
    Stage::bind (env, pstg, nstg);
    // bind the irb
    p_irb = dynamic_cast <Irb*> (env->get (RESOURCE_IRB));
    if (!p_irb) {
      string msg = "cannot bind instruction result buffer within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the rob
    p_rob = dynamic_cast <Rob*> (env->get (RESOURCE_ROB));
    if (!p_rob) {
      string msg = "cannot bind reorder buffer within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the urf
    p_urf = dynamic_cast <Urf*> (env->get (RESOURCE_URF));
    if (!p_urf) {
      string msg = "cannot bind universal register file within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the gcs
    p_gcs = dynamic_cast <Gcs*> (env->get (RESOURCE_GCS));
    if (!p_gcs) {
      string msg = "cannot bind central station within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the bpn
    p_bpn = dynamic_cast <Bpn*> (env->get (RESOURCE_BPN));
    if (!p_bpn) {
      string msg = "cannot bind bypass network within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the detection logic
    p_det = dynamic_cast <Detect*> (env->get (RESOURCE_DTL));
    if (!p_det) {
      string msg = "cannot bind detection logic within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
  }
}
