// ---------------------------------------------------------------------------
// - BedStg.cpp                                                              -
// - iato:iaio client - branch execute and detect stage class implementation -
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
#include "BedStg.hpp"
#include "Bexecute.hpp"
#include "Exception.hpp"

namespace iato {
  using namespace std;

  // create a new execute stage by context and unit

  BedStg::BedStg (Stx* stx, t_unit unit) : ResStg (stx, unit, RESOURCE_BED) {
    p_exe = 0;
    p_ebn = 0;
    p_lbn = 0;
    p_bpe = new Bpe (stx);
    p_iib = 0;
    p_det = 0;
    p_bpr = 0;
    p_pfr = 0;
    p_psb = 0;
    // set the execution unit
    if (unit == MUNIT) assert (false);
    if (unit == IUNIT) assert (false);
    if (unit == FUNIT) assert (false);
    if (unit == BUNIT) p_exe = new Bexecute;
    assert (p_exe);
    reset ();
  }

  // create a new execute stage by context, unit and index

  BedStg::BedStg (Stx* stx, t_unit unit, const long sidx) : 
          ResStg (stx, unit, sidx, RESOURCE_BED) {
    p_exe = 0;
    p_ebn = 0;
    p_lbn = 0;
    p_bpe = new Bpe (stx);
    p_iib = 0;
    p_det = 0;
    p_bpr = 0;
    p_pfr = 0;
    p_psb = 0;
    // set the execution unit
    if (unit == MUNIT) assert (false);
    if (unit == IUNIT) assert (false);
    if (unit == FUNIT) assert (false);
    if (unit == BUNIT) p_exe = new Bexecute;
    assert (p_exe);
    // set the B unit flag (last unit)
    if (unit == BUNIT) {
      long bnum = stx->getlong ("NUMBER-B-UNITS"); assert (bnum > 0);
      if (sidx == bnum - 1) d_ecf = true;
    }
    reset ();
  }

  // create a new execute stage by context, unit and name

  BedStg::BedStg (Stx* stx, t_unit unit, const long sidx, const string& name) :
          ResStg (stx, unit, sidx, name) {
    p_exe = 0;
    p_ebn = 0;
    p_lbn = 0;
    p_bpe = new Bpe (stx);
    p_iib = 0;
    p_det = 0;
    p_bpr = 0;
    p_pfr = 0;
    p_psb = 0;
    // set the execution unit
    if (unit == MUNIT) assert (false);
    if (unit == IUNIT) assert (false);
    if (unit == FUNIT) assert (false);
    if (unit == BUNIT) p_exe = new Bexecute;
    assert (p_exe);
    // set the B unit flag (last unit)
    if (unit == BUNIT) {
      long bnum = stx->getlong ("NUMBER-B-UNITS"); assert (bnum > 0);
      if (sidx == bnum - 1) d_ecf = true;
    }
    reset ();
  }

  // destroy this execute stage

  BedStg::~BedStg (void) {
    delete p_bpe;
    delete p_exe;
  }

  // reset this execute stage

  void BedStg::reset (void) {
    ResStg::reset ();
    p_bpe->reset  ();
    if (p_det) p_det->reset ();
  }

  // flush this execute stage

  void BedStg::flush (void) {
    ResStg::flush ();
    p_bpe->reset  ();
    if (p_det) p_det->reset ();
  }

  // activate this branch execute and detect stage

  void BedStg::activate (void) {
    // get the previous stage and map it to dly
    DlyStg* dly = dynamic_cast <DlyStg*> (p_pstg);
    assert (dly);
    // get the evaluated instruction and operand
    d_inst       = dly->getinst ();
    Operand oprd = dly->getoprd ();
    dly->clean ();
    // check for valid instruction and clean if invalid
    if (d_inst.isvalid () == false) {
      if (d_ecf == true) p_ebn->reset ();
      clean ();
      return;
    }
    // compute the instruction result
    try {
      // update the predicate from the early bypass network
      p_ebn->predup (d_inst, oprd);
      // update the operand from the early bypass as well
      p_ebn->update (oprd);
      assert (oprd.isvalid () == true);
      // if the instruction has been interrupted, update the restart engine
      if ((d_inst.getcnlf () == false) && (d_inst.getintr () == true)) {
	// update the restart index and clean result
	p_pfr->setrii (d_inst.getrix ());
	d_resl.reset ();
	// clean the bypass at last element
	if (d_ecf == true) p_ebn->reset ();
	return;
      }
      // compute the result
      d_resl = p_exe->exec (d_inst, oprd);
      // check for valid speculation
      d_inst.setvspf (p_det->chksip (d_inst, d_resl));
      // update the branch prediction system
      p_bpr->markbr (d_inst, d_resl, !d_inst.getcnlf ());
      // update the bpe (for late bypass) if not cancelled
      if (d_inst.getcnlf () == false) {
	p_bpe->update (d_resl);
      } else {
	p_bpe->reset ();
	d_resl.reset ();
      }
    } catch (Interrupt& vi) {
      // set the interrupt info
      d_inst.setintr (true);
      vi.setinst (d_inst);
      // set the interrupt buffer
      long iidx = d_inst.getiib ();
      p_iib->setintr (iidx, vi);
      // update the restart engine
      if (d_inst.getcnlf () == false) p_pfr->setrii (d_inst.getrix ());
      // reset the result
      d_resl.reset ();
    }
    // unlock the scoreboard
    p_psb->unlock (d_inst);
    // eventually, clean the early bypass (this is the last stage)
    if (d_ecf == true) p_ebn->reset ();
    // if the instruction is valid, and the result updates the ip
    // the restart engine is set as well. If the branch speculation is
    // is valid, the index is not updated as the instruction are cleared
    // in the fetch engine
    if ((d_inst.isvalid ()     == true) && (d_inst.getcnlf () == false) &&
	(d_resl.isreg   (IPRG) == true) && (d_inst.getvspf () == false)) {
      p_pfr->setrii (d_inst.getrix ());
    }
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

  void BedStg::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: branch execute and detect stage" << endl;
    cout << "\tdelayable latency  \t: " << getdlat () << endl;
  }

  // bind this stage from the environment

  void BedStg::bind (Env* env, Stage* pstg, Stage* nstg) {
    // bind the stage locally
    Stage::bind (env, pstg, nstg);
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
    } else {
      p_lbn->add (p_bpe);
    }
    // bind the iib
    p_iib = dynamic_cast <Iib*> (env->get (RESOURCE_IIB));
    if (!p_iib) {
      string msg = "cannot bind interrupt buffer within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the detection logic
    p_det = dynamic_cast <Detect*> (env->get (RESOURCE_DTL));
    if (!p_det) {
      string msg = "cannot bind detection logic within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the branch predictor
    p_bpr = dynamic_cast <Branch*> (env->get (RESOURCE_BPS));
    if (!p_bpr) {
      string msg = "cannot bind branch predictor within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the restart engine
    p_pfr = dynamic_cast <Resteer*> (env->get (RESOURCE_PFR));
    if (!p_pfr) {
      string msg = "cannot bind restart engine within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the scoreboard
    p_psb = dynamic_cast <Scoreboard*> (env->get (RESOURCE_PSB));
    if (!p_psb) {
      string msg = "cannot bind scoreboard within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
  }

  // clean the branch execute and detect stage resources

  void BedStg::clean (void) {
    ResStg::clean ();
    if (p_bpe) p_bpe->reset ();
  }
}
