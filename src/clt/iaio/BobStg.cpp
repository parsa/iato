// ---------------------------------------------------------------------------
// - BobStg.cpp                                                              -
// - iato:iaio client - bypass operand branch stage class implementation     -
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
#include "BobStg.hpp"
#include "Exception.hpp"

namespace iato {
  using namespace std;

  // create a new bypass stage by context and unit

  BobStg::BobStg (Stx* stx, t_unit unit) : DlyStg (stx, unit, RESOURCE_BOB) {
    p_lbn = 0;
    p_iib = 0;
    reset ();
  }

  // create a new bypass stage by context, unit and index

  BobStg::BobStg (Stx* stx, t_unit unit, const long sidx) : 
          DlyStg (stx, unit, sidx, RESOURCE_BOB) {
    p_lbn = 0;
    p_iib = 0;
  }

  // create a new bypass stage by context, unit and name

  BobStg::BobStg (Stx* stx, t_unit unit, const long sidx, const string& name) :
          DlyStg (stx, unit, sidx, name) {
    p_lbn = 0;
    p_iib = 0;
  }

  // reset this bypass stage

  void BobStg::reset (void) {
    DlyStg::reset ();
  }

  // flush this bypass stage

  void BobStg::flush (void) {
    DlyStg::flush ();
  }

  // activate this bypass stage

  void BobStg::activate (void) {
    // get the previous stage and map it to dly
    DlyStg* dly = dynamic_cast <DlyStg*> (p_pstg);
    assert (dly);
    // get the evaluated instruction and operand
    d_inst = dly->getinst ();
    d_oprd = dly->getoprd ();
    dly->clean ();
    // check for valid instruction and clean if invalid
    if (d_inst.isvalid () == false) {
      clean ();
      return;
    }
    // get the bypass operand
    try {
      // update the predicate from the late bypass network
      p_lbn->predup (d_inst, d_oprd);
      // update operand from the late bypass
      p_lbn->update (d_oprd);
    } catch (Interrupt& vi) {
      // set the interrupt info
      d_inst.setintr (true);
      vi.setinst (d_inst);
      // set the interrupt buffer
      long iidx = d_inst.getiib ();
      p_iib->setintr (iidx, vi);
      // reset the operand
      d_oprd.reset ();
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

  void BobStg::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: bypass operand branch stage" << endl;
    cout << "\tdelayable latency  \t: " << getdlat () << endl;
  }

  // bind this stage from the environment

  void BobStg::bind (Env* env, Stage* pstg, Stage* nstg) {
    // bind the stage locally
    Stage::bind (env, pstg, nstg);
    // bind the late bypass network
    p_lbn = dynamic_cast <Bpn*> (env->get (RESOURCE_LBN));
    if (!p_lbn) {
      string msg = "cannot bind late bypass network within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the iib
    p_iib = dynamic_cast <Iib*> (env->get (RESOURCE_IIB));
    if (!p_iib) {
      string msg = "cannot bind interrupt buffer within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
  }
}
