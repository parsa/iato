// ---------------------------------------------------------------------------
// - Fp3Stg.cpp                                                              -
// - iato:iaio client - floating-point stage 3 class implementation          -
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
#include "Fp3Stg.hpp"
#include "Exception.hpp"

namespace iato {
  using namespace std;

  // create a new floating-point stage 3 by context and unit

  Fp3Stg::Fp3Stg (Stx* stx, t_unit unit) : DlyStg (stx, unit, RESOURCE_FP3) {
    assert (unit == FUNIT);
    p_pfr  = 0;
    reset ();
  }

  // create a new floating-point stage 3 by context, unit and index

  Fp3Stg::Fp3Stg (Stx* stx, t_unit unit, const long sidx) : 
          DlyStg (stx, unit, sidx, RESOURCE_FP3) {
    assert (unit == FUNIT);
    p_pfr  = 0;
    reset ();
  }

  // create a new floating-point stage 3 by context, unit and name

  Fp3Stg::Fp3Stg (Stx* stx, t_unit unit, const long sidx, const string& name) :
          DlyStg (stx, unit, sidx, name) {
    assert (unit == FUNIT);
    p_pfr  = 0;
    reset ();
  }

  // flush this floating-point stage

  void Fp3Stg::flush (void) {
    assert (true);
  }

  // activate this floating-point stage 3

  void Fp3Stg::activate (void) {
    // get the previous stage and map it to delay stage
    DlyStg* dly = dynamic_cast <DlyStg*> (p_pstg);
    assert (dly);
    // get the previous stage data and clean.
    d_inst = dly->getinst ();
    d_oprd = dly->getoprd ();
    dly->clean ();
    // check for valid instruction and clean if invalid
    if (d_inst.isvalid () == false) {
      clean ();
      return;
    }
    // instruction nullified flag
    bool nnlf = p_pfr->chkrii (d_inst.getrix ());
    // instruction concellation flag
    bool cnlf = d_inst.getcnlf () || nnlf;
    // instruction interrupt flag
    bool intr = d_inst.getintr ();
    // if the instruction is not interrupted, update the cancel flag
    if (intr == false) {
      d_inst.setcnlf (cnlf);
    }
    // check if the previous stage is halted
    if (p_pstg) d_halt = p_pstg->ishalted ();
    // update the tracer
    if (p_tracer) {
      Record rcd (d_name, d_inst, !d_inst.getcnlf ());
      p_tracer->add (rcd);
      if (d_halt == true) {
	Record rcd (d_name);
	rcd.settype (Record::HALTED);
	p_tracer->add (rcd);
      }
    }
  }

  // report some stage information

  void Fp3Stg::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: floating-point stage 3" << endl;
    cout << "\tdelayable latency  \t: " << getdlat () << endl;
  }

  // bind this stage from the environment

  void Fp3Stg::bind (Env* env, Stage* pstg, Stage* nstg) {
    // bind the stage locally
    Stage::bind (env, pstg, nstg);
    // bind the restart engine
    p_pfr = dynamic_cast <Resteer*> (env->get (RESOURCE_PFR));
    if (!p_pfr) {
      string msg = "cannot bind restart engine within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
  }
}
