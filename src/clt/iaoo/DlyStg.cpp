// ---------------------------------------------------------------------------
// - DlyStg.cpp                                                              -
// - iato:iaoo client - execute delay stage class implementation             -
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
#include "Exception.hpp"

namespace iato {
  using namespace std;

  // create a new execute stage by context and unit

  DlyStg::DlyStg (Stx* stx, t_unit unit): Stage (stx, RESOURCE_DLY) {
    d_flsh = stx->getbool ("PARTIAL-FLUSH-MODE");
    d_unit = unit;
    p_gcs  = 0;
    reset ();
  }

  // create a new execute stage by context, unit and name

  DlyStg::DlyStg (Stx* stx, t_unit unit, 
		  const string& name) : Stage (stx, name) {
    d_flsh = stx->getbool ("PARTIAL-FLUSH-MODE");
    d_unit = unit;
    p_gcs  = 0;
    reset ();
  }

  // reset this execute stage

  void DlyStg::reset (void) {
    Stage::reset ();
    d_inst.reset ();
    d_oprd.reset ();
  }
    
  // flush this execute stage

  void DlyStg::flush (void) {
    Stage::flush ();
    d_inst.reset ();
    d_oprd.reset ();
  }


  // activate this execute stage - the instructions are coming from the
  // previous stage and are assumed to be physically renamed.

  void DlyStg::activate (void) {
    // get the previous stage and map it to dly
    DlyStg* dly = dynamic_cast <DlyStg*> (p_pstg);
    assert (dly);
    // get the delayed instruction
    Dsi inst = dly->getinst ();
    // update latency or broadcast result
    d_inst = p_gcs->setrdy (inst);
    // grab the operand
    d_oprd = dly->getoprd ();
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

  void DlyStg::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: delay stage" << endl;
    cout << "\tdelayable latency  \t: " << getdlat () << endl;
  }

  // bind this stage from the environment

  void DlyStg::bind (Env* env, Stage* pstg, Stage* nstg) {
    // bind the stage locally
    Stage::bind (env, pstg, nstg);
    // bind the gcs
    p_gcs = dynamic_cast <Gcs*> (env->get (RESOURCE_GCS));
    if (!p_gcs) {
      string msg = "cannot bind central station within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
  }

  // return the executed instruction

  Dsi DlyStg::getinst (void) const {
    return d_inst;
  }

  // return the evaluated operand

  Operand DlyStg::getoprd (void) const {
    return d_oprd;
  }
}
