// ---------------------------------------------------------------------------
// - WrbStg.cpp                                                              -
// - iato:iaio client - write-back stage class implementation                -
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
#include "WrbStg.hpp"

namespace iato {
  using namespace std;

  // create a new write-back stage by context and unit

  WrbStg::WrbStg (Stx* stx, t_unit unit): ResStg (stx, unit, RESOURCE_WRB) {
    d_ref = false;
    p_rse = 0;
    p_iib = 0;
    p_msi = 0;
    p_bpr = 0;
    p_pfr = 0;
    p_rbk = 0;
    p_wdg = 0;
    p_psb = 0;
    // set the msi for M units
    if (unit == MUNIT) p_msi = new Msi (stx);
    reset ();
  }

  // create a new write-back stage by context, unit and index

  WrbStg::WrbStg (Stx* stx, t_unit unit, const long sidx) : 
          ResStg (stx, unit, sidx, RESOURCE_WRB) {
    d_ref = false;
    p_rse = 0;
    p_iib = 0;
    p_msi = 0;
    p_bpr = 0;
    p_pfr = 0;
    p_rbk = 0;
    p_wdg = 0;
    p_psb = 0;
    // set the msi for M units
    if (unit == MUNIT) p_msi = new Msi (stx);
    // set the B unit flag (last unit)
    if (unit == BUNIT) {
      long bnum = stx->getlong ("NUMBER-B-UNITS"); assert (bnum > 0);
      if (sidx == bnum - 1) d_ref = true;
    }
    reset ();
  }

  // create a new write-back stage by context, unit, index and name

  WrbStg::WrbStg (Stx* stx, t_unit unit, const long sidx, const string& name) :
          ResStg (stx, unit, sidx, name) {
    d_ref = false;
    p_rse = 0;
    p_iib = 0;
    p_msi = 0;
    p_bpr = 0;
    p_pfr = 0;
    p_rbk = 0;
    p_wdg = 0;
    p_psb = 0;
    // set the msi for M units
    if (unit == MUNIT) p_msi = new Msi (stx);
    // set the B unit flag (last unit)
    if (unit == BUNIT) {
      long bnum = stx->getlong ("NUMBER-B-UNITS"); assert (bnum > 0);
      if (sidx == bnum - 1) d_ref = true;
    }
    reset ();
  }

  // destroy this state

  WrbStg::~WrbStg (void) {
    delete p_msi;
  }

  // reset this write-back stage

  void WrbStg::reset (void) {
    Stage::reset ();
    clean ();
    if (p_wdg) p_wdg->reset ();
  }

  // flush this write-back stage

  void WrbStg::flush (void) {
    Stage::flush ();
    clean ();
    if (p_wdg) p_wdg->reset ();
  }

  // activate this write-back stage

  void WrbStg::activate (void) {
    // get the previous stage and map it to result
    ResStg* res = dynamic_cast <ResStg*> (p_pstg);
    assert (res);
    // get the detected instruction and result
    d_inst = res->getinst ();
    d_resl = res->getresl ();
    res->clean ();
    // check for valid instruction
    if (d_inst.isvalid () == false) {
      clean ();
      if (d_ref == true) p_pfr->process ();
      return;
    }
    // unset the serialize condition
    p_psb->unsetsr (d_inst);
    // the pending restart flag
    bool pend = p_pfr->ispending () && (d_unit == BUNIT);
    // instruction nullified flag
    bool nnlf = p_pfr->chkrii (d_inst.getrix ());
    // instruction concellation flag
    bool cnlf = d_inst.getcnlf () || nnlf;
    // instruction interrupt flag
    bool intr = d_inst.getintr ();
    // invalid speculation flag
    bool ispf = !d_inst.getvspf ();
    // pipe flush branch taken
    bool pfbt = (d_inst.getsfl () == false) && (d_resl.isreg (IPRG) == true);
    // global pipe flush flags
    bool pfbv = intr || ispf || pfbt;
    // make sure we can write the results
    // results and rse updates can only be made when the instruction is not
    // cancelled, nor interupted, nor pending in B unit
    if ((cnlf == false) && (intr == false) && (pend == false)) {
      // update the rse and register banks
      p_rse->update  (d_resl);
      p_rbk->write   (d_resl);
      // store updates
      if (p_msi) p_msi->update (d_inst, d_resl);
    }
    // clean the iib and terminate
    if (pfbv == false) {
      // clean the iib
      long iidx = d_inst.getiib ();
      p_iib->clear (iidx);
      // update the stat collection
      if (p_stat && !nnlf) p_stat->addinst (d_inst, cnlf);
      // notify the watchdog
      if (p_wdg) p_wdg->reset ();
      // update the tracer
      if (p_tracer) {
	Record rcd (d_name, d_inst, !cnlf);
	p_tracer->add (rcd);
      }
      // check for valid rse state if not pending - if the rse is invalid
      // a pipeline flush is requested assuming the machine is in a good state
      if ((pend == false) && (p_rse->validate (d_inst.getsste ()) == false)) {
	// restore the branch predictor history
	p_bpr->sethist (d_inst.gethist ());
	// select restart condition
	if ((cnlf == false) && (d_resl.isreg (IPRG) == true)) {
	  p_pfr->pflcl ();
	} else {
	  t_octa iip  = d_inst.getiip  ();
	  long   slot = d_inst.getslot ();
	  p_pfr->pfnxt (iip, slot);
	}
      }
      // clean and process pending flush
      clean ();
      if (d_ref == true) p_pfr->process ();
      return;
    }
    // check for interrupted instruction
    if ((cnlf == false) && (intr == true)) {
      // check if we have the valid interrupted instruction
      long iidx = d_inst.getiib ();
      if (p_pfr->getrii () == d_inst.getrix ()) {
	Interrupt vi = p_iib->getintr (iidx);
	p_pfr->setintr (vi);
      }
      // clean iib
      p_iib->clear (iidx);
      // update the stat collection
      if (p_stat && !nnlf) p_stat->addinst (d_inst);
      if (p_stat && !nnlf) p_stat->markpf  (false);
      // notify the watchdog
      if (p_wdg) p_wdg->reset ();
      // update the tracer
      if (p_tracer) {
	Record rcd (d_name, d_inst, !cnlf);
	p_tracer->add (rcd);
      }
      // clean and process pending flush
      clean ();
      if (d_ref == true) p_pfr->process ();
      return;
    }
    // check for cancelled interrupted instruction
    if ((cnlf == true) && (intr == true)) {
      // clean iib
      long iidx = d_inst.getiib ();
      p_iib->clear (iidx);
      // update the stat collection
      if (p_stat && !nnlf) p_stat->addinst (d_inst, true);
      // notify the watchdog
      if (p_wdg) p_wdg->reset ();
      // update the tracer
      if (p_tracer) {
	Record rcd (d_name, d_inst, !cnlf);
	p_tracer->add (rcd);
      }
      // clean and process pending flush
      clean ();
      if (d_ref == true) p_pfr->process ();
      return;
    }
    // check for pipe flush by speculation
    if (ispf == true) {
      // restore the branch predictor history
      p_bpr->sethist (d_inst.gethist ());
      // select restart condition
      if (cnlf == false) {
	// update the restart engine
	if (d_resl.isreg (IPRG) == true) {
	  p_pfr->pflcl ();
	} else {
	  t_octa iip  = d_inst.getiip  ();
	  long   slot = d_inst.getslot ();
	  p_pfr->pfnxt (iip, slot);
	}
      } else {
	t_octa iip  = d_inst.getiip  ();
	long   slot = d_inst.getslot ();
	p_pfr->pfnxt (iip, slot);
      }
      // update the stat collection
      if (p_stat && !nnlf) p_stat->addinst (d_inst, cnlf);
      if (p_stat && !nnlf) p_stat->markpf  (d_inst.isbr ());
      // notify the watchdog
      if (p_wdg) p_wdg->reset ();
      // update the tracer
      if (p_tracer) {
	Record rcd (d_name, d_inst, !cnlf);
	p_tracer->add (rcd);
      }
      // clean and process pending flush
      clean ();
      if (d_ref == true) p_pfr->process ();
      return;
    }
    // check for pipe flush by taken branch
    if ((cnlf == false) && (intr == false) && (pfbt == true)) {
      // restore the branch predictor history
      p_bpr->sethist (d_inst.gethist ());
      // call the restart engine and request a pipeline flush
      // the next ip, assuming slot 0 is already set and the rse
      // is in a valid state
      p_pfr->pfdef ();
      // update the stat collection
      if (p_stat && !nnlf) p_stat->addinst (d_inst, cnlf);
      if (p_stat && !nnlf) p_stat->markpf  (d_inst.isbr ());
      // notify the watchdog
      if (p_wdg) p_wdg->reset ();
      // update the tracer
      if (p_tracer) {
	Record rcd (d_name, d_inst, !cnlf);
	p_tracer->add (rcd);
      }
      // clean and process pending flush
      clean ();
      if (d_ref == true) p_pfr->process ();
      return;
    }
    // if we are here, we simply clean the stage and eventually process
    clean ();
    if (d_ref == true) p_pfr->process ();
  }

  // report some stage information

  void WrbStg::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: write-back stage" << endl;
    cout << "\tdelayable latency  \t: " << getdlat () << endl;
  }

  // bind this stage from the environment

  void WrbStg::bind (Env* env, Stage* pstg, Stage* nstg) {
    // bind the stage locally
    Stage::bind (env, pstg, nstg);
    // bind the rse
    p_rse = dynamic_cast <Rse*> (env->get (RESOURCE_RSE));
    if (!p_rse) {
      string msg = "cannot bindrse stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the iib
    p_iib = dynamic_cast <Iib*> (env->get (RESOURCE_IIB));
    if (!p_iib) {
      string msg = "cannot bind interrupt buffer within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the mta and the msi
    Mta* mta = dynamic_cast <Mta*> (env->get (RESOURCE_MTA));
    if (!mta) {
      string msg = "cannot bind mta within port request ";
      throw Exception ("bind-error", msg + d_name);
    } 
    if (p_msi) p_msi->bind (mta);
    // bind the branch predictor
    p_bpr = dynamic_cast <Branch*> (env->get (RESOURCE_BPS));
    if (!p_bpr) {
      string msg = "cannot bind branch predictor within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the restart resource
    p_pfr = dynamic_cast <Resteer*> (env->get (RESOURCE_PFR));
    if (!p_pfr) {
      string msg = "cannot bind restart engine within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the register bank
    p_rbk = dynamic_cast <Register*> (env->get (RESOURCE_RBK));
    if (!p_rbk) {
      string msg = "cannot bind register bank within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the watchdog system
    p_wdg = dynamic_cast <Watchdog*> (env->get (RESOURCE_WDG));
    if (!p_wdg) {
      string msg = "cannot bind watchdog within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the scoreboard
    p_psb = dynamic_cast <Scoreboard*> (env->get (RESOURCE_PSB));
    if (!p_psb) {
      string msg = "cannot bind scoreboard within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
  }
}
