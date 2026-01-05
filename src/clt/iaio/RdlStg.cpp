// ---------------------------------------------------------------------------
// - RdlStg.cpp                                                              -
// - iato:iaio client - result delay stage class implementation               -
// ---------------------------------------------------------------------------
// - (c) inria 2002-2004                                                     -
// ---------------------------------------------------------------------------

#include "RdlStg.hpp"

namespace iato {

  // create a new result delay stage by context, unit and index
  RdlStg::RdlStg (Stx* stx, t_unit unit, const long sidx)
    : ResStg (stx, unit, sidx, "RDL") {
    reset ();
  }

  // reset this stage
  void RdlStg::reset (void) {
    ResStg::reset ();
  }

  // flush this stage
  void RdlStg::flush (void) {
    ResStg::flush ();
  }

  // activate this stage
  void RdlStg::activate (void) {
    // previous stage must be a result stage
    ResStg* res = dynamic_cast<ResStg*> (p_pstg);
    assert (res);
    // pull and clean previous stage
    d_inst = res->getinst ();
    d_resl = res->getresl ();
    res->clean ();
    // clean if invalid
    if (d_inst.isvalid () == false) {
      clean ();
      return;
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
}


