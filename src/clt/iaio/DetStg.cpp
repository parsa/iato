// ---------------------------------------------------------------------------
// - DetStg.cpp                                                              -
// - iato:iaio client - detect stage class implementation                    -
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
#include "ExeStg.hpp"
#include "DetStg.hpp"

namespace iato {
  using namespace std;

  // create a new detect stage by context and unit

  DetStg::DetStg (Stx* stx, t_unit unit) : ResStg (stx, unit, RESOURCE_DET) {
    p_iib  = 0;
    p_ebn  = 0;
    p_lbn  = 0;
    p_bpe  = new Bpe (stx);
    p_lmb  = 0;
    p_mbe  = 0;
    p_det  = 0;
    p_pfr  = 0;
    // set the unit resources
    if (unit == MUNIT) p_mbe = new Mbe;
    if (unit == BUNIT) assert (false);
    reset ();
  }

  // create a new detect stage by context, unit and index

  DetStg::DetStg (Stx* stx, t_unit unit, const long sidx) : 
          ResStg (stx, unit, sidx, RESOURCE_DET) {
    p_iib  = 0;
    p_ebn  = 0;
    p_lbn  = 0;
    p_bpe  = new Bpe (stx);
    p_lmb  = 0;
    p_mbe  = 0;
    p_det  = 0;
    p_pfr  = 0;
    // set the unit resources
    if (unit == MUNIT) p_mbe = new Mbe;
    if (unit == BUNIT) assert (false);
    reset ();
  }

  // create a new detect stage by context, unit and name

  DetStg::DetStg (Stx* stx, t_unit unit, const long sidx, const string& name) :
          ResStg (stx, unit, sidx, name) {
    p_iib  = 0;
    p_ebn  = 0;
    p_lbn  = 0;
    p_bpe  = new Bpe (stx);
    p_lmb  = 0;
    p_mbe  = 0;
    p_det  = 0;
    p_pfr  = 0;
    // set the  unit resources
    if (unit == MUNIT) p_mbe = new Mbe;
    if (unit == BUNIT) assert (false);
    reset ();
  }

  // destroy this detect stage

  DetStg::~DetStg (void) {
    delete p_bpe;
    delete p_mbe;
  }

  // reset this detect stage

  void DetStg::reset (void) {
    ResStg::reset ();
    p_bpe->reset  ();
    if (p_mbe) p_mbe->reset ();
    if (p_det) p_det->reset ();
  }

  // flush this detect stage

  void DetStg::flush (void) {
    ResStg::flush ();
    p_bpe->reset  ();
    if (p_mbe) p_mbe->reset ();
    if (p_det) p_det->reset ();
  }

  // activate this detect stage by checking if some exception have occured

  void DetStg::activate (void) {
    // get the previous stage and map it to execute
    ExeStg* exe = dynamic_cast <ExeStg*> (p_pstg);
    assert (exe);
    // get the previous stage data and clean
    d_inst  = exe->getinst ();
    d_resl  = exe->getresl ();
    Mrt mrt = exe->getmrt  ();
    exe->clean ();
    // set the memory bypass with the previous mrt
    if (p_mbe) p_mbe->setmrt (mrt);
    // check for valid instruction and clean if invalid
    if (d_inst.isvalid () == false) {
      clean ();
      return;
    }
    // try to detect interrupted instructions
    try {
      // if the instruction has been interrupted, update the restart engine
      if ((d_inst.getcnlf () == false) && (d_inst.getintr () == true)) {
	// set the restart index and clean result
	p_pfr->setrii (d_inst.getrix ());
	d_resl.reset ();
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
	return;
      }
      // eventually fix some of the raw and waw legal violations with the
      // early bypass network
      p_ebn->update (d_inst, d_resl);
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
    // if the instruction is valid, and the result updates the ip
    // the restart engine is set as well
    if ((d_inst.isvalid () == true) && (d_inst.getcnlf () == false) &&
	(d_resl.isreg (IPRG) == true)) {
      p_pfr->setrii (d_inst.getrix ());
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

  void DetStg::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: detect stage" << endl;
    cout << "\tdelayable latency  \t: " << getdlat () << endl;
  }

  // bind this stage from the environment

  void DetStg::bind (Env* env, Stage* pstg, Stage* nstg) {
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
    // bind the late memory bypass network for the M unit only
    if (p_mbe) {
      // bind the late memory bypass network
      p_lmb = dynamic_cast <Mbn*> (env->get (RESOURCE_LMB));
      if (!p_lmb) {
	string msg = "cannot bind late memory bypass network within stage ";
	throw Exception ("bind-error", msg + d_name);
      } else {
	p_lmb->add (p_mbe);
      }
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
    // bind the restart engine
    p_pfr = dynamic_cast <Resteer*> (env->get (RESOURCE_PFR));
    if (!p_pfr) {
      string msg = "cannot bind restart engine within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
  }

  // clean the detect stage resources

  void DetStg::clean (void) {
    ResStg::clean ();
    if (p_bpe) p_bpe->reset ();
    if (p_mbe) p_mbe->reset ();
  }
}
