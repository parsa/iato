// ---------------------------------------------------------------------------
// - ExeStg.cpp                                                              -
// - iato:iaoo client - execute stage class implementation                   -
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
#include "DlyStg.hpp"
#include "ExeStg.hpp"
#include "Mexecute.hpp"
#include "Iexecute.hpp"
#include "Fexecute.hpp"
#include "Bexecute.hpp"
#include "Exception.hpp"

namespace iato {
  using namespace std;

  // create a new execute stage by context and unit

  ExeStg::ExeStg (Stx* stx, t_unit unit): ResStg (stx, unit, RESOURCE_EXE) {
    p_exec = 0;
    p_bpn  = 0;
    p_bpe  = new Bpe (stx);
    p_gcs  = 0;
    p_mli  = 0;
    // set the execution units
    if (d_unit == MUNIT) p_exec = new Mexecute;
    if (d_unit == IUNIT) p_exec = new Iexecute;
    if (d_unit == FUNIT) p_exec = new Fexecute;
    if (d_unit == BUNIT) p_exec = new Bexecute;
    assert (p_exec);
    // set the mli for M units
    if (d_unit == MUNIT) p_mli = new Mli (stx);
    reset ();
  }

  // create a new execute stage by context, unit and name

  ExeStg::ExeStg (Stx* stx, t_unit unit, 
		  const string& name) : ResStg (stx, unit, name) {
    p_exec = 0;
    p_bpn  = 0;
    p_bpe  = new Bpe (stx);
    p_gcs  = 0;
    p_mli  = 0;
    // set the execution units
    if (d_unit == MUNIT) p_exec = new Mexecute;
    if (d_unit == IUNIT) p_exec = new Iexecute;
    if (d_unit == FUNIT) p_exec = new Fexecute;
    if (d_unit == BUNIT) p_exec = new Bexecute;
    assert (p_exec);
    // set the mli for M units
    if (d_unit == MUNIT) p_mli = new Mli (stx);
    reset ();
  }

  // destroy this execute stage

  ExeStg::~ExeStg (void) {
    delete p_bpe;
    delete p_mli;
    delete p_exec;
  }

  // reset this execute stage

  void ExeStg::reset (void) {
    ResStg::reset ();
    p_bpe->reset  ();
    if (p_mli) p_mli->reset  ();
  }

  // flush this execute stage

  void ExeStg::flush (void) {
    ResStg::flush ();
    p_bpe->reset  ();
    if (p_mli) p_mli->reset  ();
  }

  // activate this execute stage - the instructions are coming from the
  // previous stage and are assumed to be physically renamed.

  void ExeStg::activate (void) {
    // get the previous stage and map it to dly
    DlyStg* dly = dynamic_cast <DlyStg*> (p_pstg);
    assert (dly);
    // get the evaluated instruction and operand
    d_inst       = dly->getinst ();
    Operand oprd = dly->getoprd ();
    // update latency or broadcast tags
    d_inst = p_gcs->setrdy (d_inst);
    // check the instruction predicate - with branch, the predicate might
    // be forwarded by the other units
    if ((d_unit == BUNIT) && (d_inst.isvalid () == true)) {
      // check if we need to evaluate the predicate
      if (d_inst.getpnrd () == true) {
	// get the predicate and evaluate it
	Rid pred = d_inst.getpnum ();
	Uvr uvr = p_bpn->eval (pred);
	if (uvr.isvalid () == true) {
	  // mark the instruction
	  bool bval = uvr.getbval ();
	  d_inst.setpnrd (false);
	  d_inst.setcnlf (!bval);
	} else {
	  // reschedule the instruction
	  p_gcs->resched (d_inst);
	  d_inst.reset ();
	  d_resl.reset ();
	}
      }
      // evaluate the operands in the bypass
      p_bpn->eval (oprd);
      // check if we reschedule
      if (oprd.isvalid () == false) {
	// reschedule the instruction
	p_gcs->resched (d_inst);
	d_inst.reset ();
	d_resl.reset ();
      }
    }
    // check the instruction for validity. If the instruction is not valid
    // no result is produced
    if (d_inst.isvalid () == true) {
      // if the instruction has been cancelled, a default result is produced
      // without data. It is used later to update the trb
      if (d_inst.getcnlf () == false) {
	assert (d_inst.getelat () == 0);
	try {
	  // compute the result
	  assert (oprd.isvalid () == true);
	  d_resl = p_exec->exec (d_inst, oprd);
	  // preset the result with the port request if needed
	  if (p_mli) p_mli->preset (d_inst, d_resl);
	  // update the bpe (for bypass)
	  p_bpe->update (d_resl);
	  // fix interrupt
	  d_intr.reset ();
	} catch (const Interrupt& vi) {
	  d_resl.reset ();
	  d_intr = vi;
	  d_intr.setinst (d_inst);
	}
      } else {
	d_resl = d_inst.getresl ();
	d_intr.reset ();
      }
    } else {
      d_resl.reset ();
      d_intr.reset ();
    }
    // check if the previous stage is halted
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

  void ExeStg::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: execute stage" << endl;
    cout << "\tdelayable latency  \t: " << getdlat () << endl;
  }

  // bind this stage from the environment

  void ExeStg::bind (Env* env, Stage* pstg, Stage* nstg) {
    // bind the stage locally
    Stage::bind (env, pstg, nstg);
    // bind the bpn
    p_bpn = dynamic_cast <Bpn*> (env->get (RESOURCE_BPN));
    if (!p_bpn) {
      string msg = "cannot bind bypass network within stage ";
      throw Exception ("bind-error", msg + d_name);
    } else {
      p_bpn->add (p_bpe);
    }
    // bind the gcs
    p_gcs = dynamic_cast <Gcs*> (env->get (RESOURCE_GCS));
    if (!p_gcs) {
      string msg = "cannot bind central station within stage ";
      throw Exception ("bind-error", msg + d_name);
    }    
    // bind the mta and the mob
    Mta* mta = dynamic_cast <Mta*> (env->get (RESOURCE_MTA));
    Mob* mob = dynamic_cast <Mob*> (env->get (RESOURCE_MOB));
    if (!mta) {
      string msg = "cannot bind mta within port request ";
      throw Exception ("bind-error", msg + d_name);
    } 
    if (!mob) {
      string msg = "cannot bind mob within port request ";
      throw Exception ("bind-error", msg + d_name);
    } 
    if (p_mli) p_mli->bind (mta, mob);

  }
}
