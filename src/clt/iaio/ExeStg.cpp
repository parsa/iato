// ---------------------------------------------------------------------------
// - ExeStg.cpp                                                              -
// - iato:iaio client - execute stage class implementation                   -
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
#include "ExeStg.hpp"
#include "Mexecute.hpp"
#include "Iexecute.hpp"
#include "Fexecute.hpp"
#include "Bexecute.hpp"
#include "Exception.hpp"

namespace iato {
  using namespace std;

  // create a new execute stage by context and unit

  ExeStg::ExeStg (Stx* stx, t_unit unit) : ResStg (stx, unit, RESOURCE_EXE) {
    p_ebn = 0;
    p_lbn = 0;
    p_bpe = new Bpe (stx);
    p_emb = 0;
    p_lmb = 0;
    p_mbe = 0;
    p_iib = 0;
    p_msi = 0;
    p_exe = 0;
    p_psb = 0;
    // set the execution unit
    if (unit == MUNIT) p_exe = new Mexecute;
    if (unit == IUNIT) p_exe = new Iexecute;
    if (unit == FUNIT) p_exe = new Fexecute;
    if (unit == BUNIT) assert (false);
    assert (p_exe);
    // set the M unit resources
    if (unit == MUNIT) {
      p_msi = new Msi (stx);
      p_mbe = new Mbe (d_sidx, true);
    }
    reset ();
  }

  // create a new execute stage by context, unit and index

  ExeStg::ExeStg (Stx* stx, t_unit unit, const long sidx) : 
          ResStg (stx, unit, sidx, RESOURCE_EXE) {
    p_ebn = 0;
    p_lbn = 0;
    p_bpe = new Bpe (stx);
    p_emb = 0;
    p_lmb = 0;
    p_mbe = 0;
    p_iib = 0;
    p_msi = 0;
    p_exe = 0;
    p_psb = 0;
    // set the execution unit
    if (unit == MUNIT) p_exe = new Mexecute;
    if (unit == IUNIT) p_exe = new Iexecute;
    if (unit == FUNIT) p_exe = new Fexecute;
    if (unit == BUNIT) assert (false);
    assert (p_exe);
    // set the M unit resources
    if (unit == MUNIT) {
      p_msi = new Msi (stx);
      p_mbe = new Mbe (d_sidx, true);
    }
    reset ();
  }

  // create a new execute stage by context, unit and name

  ExeStg::ExeStg (Stx* stx, t_unit unit, const long sidx, const string& name) :
          ResStg (stx, unit, sidx, name) {
    p_ebn = 0;
    p_lbn = 0;
    p_bpe = new Bpe (stx);
    p_emb = 0;
    p_lmb = 0;
    p_mbe = 0;
    p_iib = 0;
    p_msi = 0;
    p_exe = 0;
    p_psb = 0;
    // set the execution unit
    if (unit == MUNIT) p_exe = new Mexecute;
    if (unit == IUNIT) p_exe = new Iexecute;
    if (unit == FUNIT) p_exe = new Fexecute;
    if (unit == BUNIT) assert (false);
    assert (p_exe);
    // set the M unit resources
    if (unit == MUNIT) {
      p_msi = new Msi (stx);
      p_mbe = new Mbe (d_sidx, true);
    }
    reset ();
  }

  // destroy this execute stage

  ExeStg::~ExeStg (void) {
    delete p_bpe;
    delete p_mbe;
    delete p_msi;
    delete p_exe;
  }

  // reset this execute stage

  void ExeStg::reset (void) {
    ResStg::reset ();
    p_bpe->reset  ();
    if (p_mbe) p_mbe->reset  ();
  }

  // flush this execute stage

  void ExeStg::flush (void) {
    ResStg::flush ();
    p_bpe->reset  ();
    if (p_mbe) p_mbe->reset  ();
  }

  // activate this execute stage

  void ExeStg::activate (void) {
    // get the previous stage and map it to dly
    DlyStg* dly = dynamic_cast <DlyStg*> (p_pstg);
    assert (dly);
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
    try {
      // update the predicate from the late bypass network
      p_lbn->predup (d_inst, oprd);
      // update operand from the late bypass
      p_lbn->update (oprd);
      assert (oprd.isvalid () == true);
      // compute the result
      d_resl = p_exe->exec (d_inst, oprd);
      // eventually update a load 
      if ((d_inst.getcnlf () == false) && (d_inst.getldb () == true)) {
	// get the load mrt
	Mrt mrt = d_resl.getmrt ();
	// try to update it with the early bypass

	if ((mrt.isvalid () == true) && (p_emb)) {
	  Mrt emrt = p_emb->update (mrt, d_sidx);
	  if (emrt.isvalid () == true)  d_resl.update (emrt);
	}
	// try to update it with the early bypass
	mrt = d_resl.getmrt ();
	if ((mrt.isvalid () == true) && (p_lmb)) {
	  Mrt lmrt = p_lmb->update (mrt, d_sidx);
	  if (lmrt.isvalid () == true)  d_resl.update (lmrt);
	}
      }
      // preset the memory port request
      if (p_msi) p_msi->preset (d_inst, d_resl);
      // update the bpe (for early bypass) if not cancel
      if (d_inst.getcnlf () == false) {
	// bypass update
	p_bpe->update (d_resl);
      } else {
	p_bpe->reset ();
      }
      // update the early memory bypass with a store
      if ((d_inst.getcnlf () == false) && (d_inst.getstb () == true)) {
	Mrt mrt = d_resl.getmrt ();
	if (p_mbe) p_mbe->setmrt (mrt);
      } else {
	if (p_mbe) p_mbe->reset ();
      }
    } catch (Interrupt& vi) {
      // set the interrupt info
      d_inst.setintr (true);
      vi.setinst (d_inst);
      // set the interrupt buffer
      long iidx = d_inst.getiib ();
      p_iib->setintr (iidx, vi);
      // reset the result
      d_resl.reset ();
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
    // bind the early bypass network
    p_ebn = dynamic_cast <Bpn*> (env->get (RESOURCE_EBN));
    if (!p_ebn) {
      string msg = "cannot bind early bypass network within stage ";
      throw Exception ("bind-error", msg + d_name);
    } else {
      p_ebn->add (p_bpe);
    }
    // bind the late bypass network
    p_lbn = dynamic_cast <Bpn*> (env->get (RESOURCE_LBN));
    if (!p_lbn) {
      string msg = "cannot bind late bypass network within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the early memory bypass network for the M unit only
    if (p_mbe) {
      p_emb = dynamic_cast <Mbn*> (env->get (RESOURCE_EMB));
      if (!p_emb) {
	string msg = "cannot bind early memory bypass network within stage ";
	throw Exception ("bind-error", msg + d_name);
      } else {
	p_emb->add (p_mbe);
      }
      // bind the late memory bypass network
      p_lmb = dynamic_cast <Mbn*> (env->get (RESOURCE_LMB));
      if (!p_lmb) {
	string msg = "cannot bind late memory bypass network within stage ";
	throw Exception ("bind-error", msg + d_name);
      }
    }
    // bind the iib
    p_iib = dynamic_cast <Iib*> (env->get (RESOURCE_IIB));
    if (!p_iib) {
      string msg = "cannot bind interrupt buffer within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the mta within the msi
    Mta* mta = dynamic_cast <Mta*> (env->get (RESOURCE_MTA));
    if (!mta) {
      string msg = "cannot bind mta within port request ";
      throw Exception ("bind-error", msg + d_name);
    } 
    if (p_msi) p_msi->bind (mta);
    // bind the scoreboard
    p_psb = dynamic_cast <Scoreboard*> (env->get (RESOURCE_PSB));
    if (!p_psb) {
      string msg = "cannot bind scoreboard within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
  }

  // clean the execute stage resources

  void ExeStg::clean (void) {
    ResStg::clean ();
    if (p_mbe) p_mbe->reset ();
  }

  // return the stage bypass mrt

  Mrt ExeStg::getmrt (void) const {
    Mrt mrt;
    if (p_mbe) mrt = p_mbe->getmrt ();
    return mrt;
  }
}
