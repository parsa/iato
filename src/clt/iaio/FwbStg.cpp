// ---------------------------------------------------------------------------
// - FwbStg.cpp                                                              -
// - iato:iaio client - floating write-back stage class implementation       -
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
#include "ResStg.hpp"
#include "FwbStg.hpp"

namespace iato {
  using namespace std;

  // create a new write-back stage by context and unit

  FwbStg::FwbStg (Stx* stx, t_unit unit): ResStg (stx, unit, RESOURCE_FWB) {
    // check the F latency
    long flat = stx->getlong ("LATENCY-F-UNIT");
    assert ((flat == 1) || (flat == 4));
    d_lflg = flat == 1 ? false : true;
    // initialize resource pointers
    p_iib = 0;
    p_pfr = 0;
    p_rbk = 0;
    // reset eveything
    reset ();
  }

  // create a new write-back stage by context, unit and index

  FwbStg::FwbStg (Stx* stx, t_unit unit, const long sidx): 
          ResStg (stx, unit, sidx, RESOURCE_FWB) {
    // check the F latency
    long flat = stx->getlong ("LATENCY-F-UNIT");
    assert ((flat == 1) || (flat == 4));
    d_lflg = flat == 1 ? false : true;
    // initialize resource pointers
    p_iib = 0;
    p_pfr = 0;
    p_rbk = 0;
    // reset eveything
    reset ();
  }

  // create a new write-back stage by context, unit, index and name

  FwbStg::FwbStg (Stx* stx, t_unit unit, const long sidx, const string& name) :
          ResStg (stx, unit, sidx, name) {
    // check the F latency
    long flat = stx->getlong ("LATENCY-F-UNIT");
    assert ((flat == 1) || (flat == 4));
    d_lflg = flat == 1 ? false : true;
    // initialize resource pointers
    p_iib = 0;
    p_pfr = 0;
    p_rbk = 0;
    // reset eveything
    reset ();
  }

  // reset this write-back stage

  void FwbStg::reset (void) {
    Stage::reset ();
    clean ();
  }

  // flush this write-back stage

  void FwbStg::flush (void) {
    if (d_lflg == false) {
      Stage::flush ();
      clean ();
    }
  }

  // activate this write-back stage

  void FwbStg::activate (void) {
    // get the previous stage and map it to result stage
    ResStg* res = dynamic_cast <ResStg*> (p_pstg);
    assert (res);
    // get the executed instruction and result
    d_inst = res->getinst ();
    d_resl = res->getresl ();
    res->clean ();
    // check for valid instruction
    if (d_inst.isvalid () == false) {
      clean ();
      return;
    }
    // instruction nullified flag
    bool nnlf = d_lflg ? false : p_pfr->chkrii (d_inst.getrix ());
    // instruction concellation flag
    bool cnlf = d_inst.getcnlf () || nnlf;
    // instruction interrupt flag
    bool intr = d_inst.getintr ();
    // make sure we can write the results
    if ((cnlf == false) && (intr == false)) {
      // register updates
      p_rbk->write   (d_resl);
    }
    // clean the iib and terminate
    if (intr == false) {
      // clean the iib
      long iidx = d_inst.getiib ();
      p_iib->clear (iidx);
      // update the stat collection
      if (p_stat && !nnlf) p_stat->addinst (d_inst, cnlf);
      // update the tracer
      if (p_tracer) {
	Record rcd (d_name, d_inst, !cnlf);
	p_tracer->add (rcd);
      }
      // clean and return
      clean ();
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
      // update the tracer
      if (p_tracer) {
	Record rcd (d_name, d_inst, !cnlf);
	p_tracer->add (rcd);
      }
      // clean and return
      clean ();
      return;
    }
    // check for cancelled interrupted instruction
    if ((cnlf == true) && (intr == true)) {
      // clean iib
      long iidx = d_inst.getiib ();
      p_iib->clear (iidx);
      // update the stat collection
      if (p_stat && !nnlf) p_stat->addinst (d_inst);
      // update the tracer
      if (p_tracer) {
	Record rcd (d_name, d_inst, !cnlf);
	p_tracer->add (rcd);
      }
      // clean and return
      clean ();
      return;
    }
    // if we are here, we simply clean the stage
    clean ();
  }

  // report some stage information

  void FwbStg::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: floating write-back stage" << endl;
    cout << "\tdelayable latency  \t: " << getdlat () << endl;
  }

  // bind this stage from the environment

  void FwbStg::bind (Env* env, Stage* pstg, Stage* nstg) {
    // bind the stage locally
    Stage::bind (env, pstg, nstg);
    // bind the iib
    p_iib = dynamic_cast <Iib*> (env->get (RESOURCE_IIB));
    if (!p_iib) {
      string msg = "cannot bind interrupt buffer within stage ";
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
  }
}
