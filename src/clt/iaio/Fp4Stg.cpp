// ---------------------------------------------------------------------------
// - Fp4Stg.cpp                                                              -
// - iato:iaio client - floating-point execute stage class implementation    -
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

#include "Srn.hpp"
#include "DlyStg.hpp"
#include "Fp4Stg.hpp"
#include "Fexecute.hpp"

namespace iato {
  using namespace std;

  // create a new execute stage by context and unit

  Fp4Stg::Fp4Stg (Stx* stx, t_unit unit) : ResStg (stx, unit, RESOURCE_FP4) {
    assert (unit == FUNIT);
    p_lbn = 0;
    p_bpe = new Bpe (stx);
    p_exe = new Fexecute;
    p_psb = 0;
    reset ();
  }

  // create a new execute stage by context, unit and index

  Fp4Stg::Fp4Stg (Stx* stx, t_unit unit, const long sidx) : 
          ResStg (stx, unit, sidx, RESOURCE_FP4) {
    assert (unit == FUNIT);
    p_lbn = 0;
    p_bpe = new Bpe (stx);
    p_exe = new Fexecute;
    p_psb = 0;
    reset ();
  }

  // create a new execute stage by context, unit and name

  Fp4Stg::Fp4Stg (Stx* stx, t_unit unit, const long sidx, const string& name) :
          ResStg (stx, unit, sidx, name) {
    assert (unit == FUNIT);
    p_lbn = 0;
    p_bpe = new Bpe (stx);
    p_exe = new Fexecute;
    p_psb = 0;
    reset ();
  }

  // destroy this execute stage

  Fp4Stg::~Fp4Stg (void) {
    delete p_bpe;
    delete p_exe;
  }

  // reset this execute stage

  void Fp4Stg::reset (void) {
    ResStg::reset ();
    p_bpe->reset  ();
  }

  // flush this floating-point stage stage

  void Fp4Stg::flush (void) {
    assert (true);
  }

  // activate this execute stage

  void Fp4Stg::activate (void) {
    // get the previous stage and map it to dly
    DlyStg* dly = dynamic_cast <DlyStg*> (p_pstg);
    assert (dly);
    // clean the bpe
    p_bpe->reset ();
    // get the evaluated instruction and operand
    d_inst       = dly->getinst ();
    Operand oprd = dly->getoprd ();
    dly->clean ();
    // check for valid instruction and clean if needed
    if (d_inst.isvalid () == false) {
      clean ();
      return;
    }
    // if the instruction has been interrupted, do nothing
    if (d_inst.getintr () == true) {
      // unlock the resources
      p_psb->unlock (d_inst);
      // clean result
      d_resl.reset ();      
      // update tracer and return
      if (p_tracer && (d_inst.isvalid () == true)) {
	Record rcd (d_name, d_inst);
	p_tracer->add (rcd);
      }
      return;
    }
    // compute the instruction result
    d_resl = p_exe->exec (d_inst, oprd);
    // update the bpe (for late bypass) if not cancel
    if (d_inst.getcnlf () == false) {
      // bypass update
      p_bpe->update (d_resl);
    } else {
      p_bpe->reset ();
    }
    // unlock the resources
    p_psb->unlock (d_inst);
    // check if the previous stage is halted
    if (p_pstg) d_halt = p_pstg->ishalted ();
    // update the tracer
    if (p_tracer) {
      Record rcd (d_name, d_inst);
      p_tracer->add (rcd);
      if (d_halt == true) {
	Record rcd (d_name);
	rcd.settype (Record::HALTED);
	p_tracer->add (rcd);
      }
    }
  }

  // report some stage information

  void Fp4Stg::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: floating-point stage 4" << endl;
    cout << "\tdelayable latency  \t: " << getdlat () << endl;
  }

  // bind this stage from the environment

  void Fp4Stg::bind (Env* env, Stage* pstg, Stage* nstg) {
    // bind the stage locally
    Stage::bind (env, pstg, nstg);
    // bind the late bypass network
    p_lbn = dynamic_cast <Bpn*> (env->get (RESOURCE_LBN));
    if (!p_lbn) {
      string msg = "cannot bind late bypass network within stage ";
      throw Exception ("bind-error", msg + d_name);
    } else {
      p_lbn->add (p_bpe);
    }
    // bind the scoreboard
    p_psb = dynamic_cast <Scoreboard*> (env->get (RESOURCE_PSB));
    if (!p_psb) {
      string msg = "cannot bind scoreboard within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
  }
}
