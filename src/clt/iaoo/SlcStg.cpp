// ---------------------------------------------------------------------------
// - SlcStg.cpp                                                              -
// - iato:iaoo client - select stage class implementation                    -
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
#include "ExpStg.hpp"
#include "SlcStg.hpp"
#include "Exception.hpp"

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

  // this function returns the static instruction latency by unit type
  static long get_unit_latency (Stx* stx, t_unit unit) {
    if (!stx) return 0;
    long result = 0;
    switch (unit) {
    case MUNIT:
      result = stx->getlong ("LATENCY-M-UNIT");
      break;
    case IUNIT:
      result = stx->getlong ("LATENCY-I-UNIT");
      break;
    case FUNIT:
      result = stx->getlong ("LATENCY-F-UNIT");
      break;
    case BUNIT:
      result = stx->getlong ("LATENCY-B-UNIT");
      break;
    default:
      break;
    }
    return result;
  }

  // this procedure marks an instruction as canceled if the predicate can be 
  // evaluated as ready and true, else return false
  static bool check_cancel (const Dsi& dsi, Urf* urf, Register* rbk) {
    // get the predicate
    if (dsi.isvalid () == false) return false;
    Rid pred = dsi.getpnum ();
    // check if the predicate is ready
    if (urf->isready (pred) == false) return false;
    // here the predicate is ready, so try to evaluate it
    Uvr uvr = urf->eval (pred);
    if (uvr.isvalid () == true) {
      assert (uvr.gettype () == Uvr::BBV);
      return !uvr.getbval ();
    }
    // evaluate in the register bank
    long pnum = pred.getpnum ();
    return !rbk->getbval (PREG, pnum);
  }

  // this procedure set the rid ready bit if the trb is marked ready
  static void mark_ready (Rid& rid, Urf* urf) {
    // check if the rid is valid
    if (rid.isvalid () == false) return;
    // check the urf with the rid 
    if (urf->isready (rid) == true) rid.seterdy (true);
  }

  // this procedure marks the instruction rid ready bits
  static void mark_ready (Dsi& dsi, Urf* urf) {
    // check the instruction
    if (dsi.isvalid () == false) return;
    // mark the predicate
    Rid pred = dsi.getpnum ();
    mark_ready (pred, urf);
    dsi.setpnum (pred);
    // mark the source operands
    for (long i = 0; i < IA_MSRC; i++) {
      Rid rid = dsi.getsnum (i);
      mark_ready  (rid, urf);
      dsi.setsnum (i,rid);
    }
  }

  // create a new select stage by context and unit

  SlcStg::SlcStg (Stx* stx, t_unit unit): Stage (stx, RESOURCE_SLC) {
    d_flsh = stx->getbool ("PARTIAL-FLUSH-MODE");
    d_unit = unit;
    d_ulat = get_unit_latency (stx, unit);
    p_tsta = new Station (stx, unit);
    p_urf  = 0;
    p_rbk  = 0;
    p_gcs  = 0;
    reset ();
  }

  // create a new select stage by context, unit and name

  SlcStg::SlcStg (Stx* stx,t_unit unit,const string& name) : Stage (stx,name) {
    d_flsh = stx->getbool ("PARTIAL-FLUSH-MODE");
    d_unit = unit;
    d_ulat = get_unit_latency (stx, unit);
    p_tsta = new Station (stx, unit);
    p_urf  = 0;
    p_rbk  = 0;
    p_gcs  = 0;
    reset ();
  }

  // destroy this select stage

  SlcStg::~SlcStg (void) {
    delete p_tsta;
  }

  // reset this select stage

  void SlcStg::reset (void) {
    Stage::reset  ();
    p_tsta->reset ();
    d_inst.reset  ();
  }
    
  // flush this select stage

  void SlcStg::flush (void) {
    Stage::flush  ();
    p_tsta->reset ();
    d_inst.reset  ();
  }

  // partial flush this stage

  void SlcStg::pflsh (void) {
    if (d_flsh == false) return;
    p_tsta->pflsh ();
    d_inst.reset  ();
  }

  // activate this select stage - the instructions are coming from the
  // previous stage and are assumed to be physically renamed.

  void SlcStg::activate (void) {
    // get the previous stage and map it to exp
    ExpStg* exp = dynamic_cast <ExpStg*> (p_pstg);
    assert (exp);
    // select first the ready instruction
    d_inst = p_tsta->getrdy ();
    // check if the station is full
    if (p_tsta->isfull () == false) {
      // grab the instruction from the unit queue
      Dsi dsi = exp->pop (d_unit);
      if (dsi.isvalid () == true) {
	// check if the instruction is canceled or not
	if (check_cancel (dsi, p_urf, p_rbk) == true) {
	  // mark the instruction as canceled
	  dsi.setcnlf (true);
	} else {
	  // mark the ready operands
	  mark_ready (dsi, p_urf);
	}
	// consistency check
	assert (check_pred_cnlf (dsi) == true);
	// set latency
	dsi.setelat   (d_ulat);
	// allocate station entry
	p_tsta->alloc (dsi);
      }
    }
    // check if the previous stage is halted - if it is halted, we check
    // that the station is empty before going into halt mode
    if (p_pstg && (p_pstg->ishalted () == true)) {
      d_halt = p_tsta->isempty ();
    }
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

  void SlcStg::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: select stage" << endl;
    cout << "\tdelayable latency  \t: " << getdlat () << endl;
  }

  // bind this stage from the environment

  void SlcStg::bind (Env* env, Stage* pstg, Stage* nstg) {
    // bind the stage locally
    Stage::bind (env, pstg, nstg);
    // bind the urf
    p_urf = dynamic_cast <Urf*> (env->get (RESOURCE_URF));
    if (!p_urf) {
      string msg = "cannot bind universal register file within stage ";
      throw Exception ("bind-error", msg + d_name);
    }    
    // bind the register bank
    p_rbk = dynamic_cast <Register*> (env->get (RESOURCE_RBK));
    if (!p_rbk) {
      string msg = "cannot bind register bank within stage ";
      throw Exception ("bind-error", msg + d_name);
    }

    // bind the gcs and add the station
    p_gcs = dynamic_cast <Gcs*> (env->get (RESOURCE_GCS));
    if (!p_gcs) {
      string msg = "cannot bind central station within stage ";
      throw Exception ("bind-error", msg + d_name);
    } else {
      long igcs = p_gcs->add (p_tsta);
      p_tsta->setgcs (igcs);
    }
  }

  // return the selected instruction

  Dsi SlcStg::getinst (void) const {
    return d_inst;
  }
}
