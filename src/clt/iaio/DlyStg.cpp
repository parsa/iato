// ---------------------------------------------------------------------------
// - DlyStg.cpp                                                              -
// - iato:iaio client - execute delay stage class implementation             -
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

  // create a new execute stage by context, unit and index

  DlyStg::DlyStg (Stx* stx, t_unit unit, const string& name): 
           Stage (stx, name) {
    d_unit = unit;
    reset ();
  }

  // create a new execute stage by context, unit, index and name

  DlyStg::DlyStg (Stx* stx, t_unit unit, const long sidx, const string& name) :
           Stage (stx, sidx, name) {
    d_unit = unit;
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

  // return true if the stage is holding

  bool DlyStg::isholding (void) const {
    if (p_nstg) return p_nstg->isholding ();
    return false;
  }

  // activate this execute stage - the instructions are coming from the
  // previous stage and are assumed to be physically renamed.

  void DlyStg::activate (void) {
    // get the previous stage and map it to dly
    DlyStg* dly = dynamic_cast <DlyStg*> (p_pstg);
    assert (dly);
    // get the delayed instruction and operand and clean
    d_inst = dly->getinst ();
    d_oprd = dly->getoprd ();
    dly->clean ();
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

  // clean this stage data

  void DlyStg::clean (void) {
    d_inst.reset ();
    d_oprd.reset ();
  }

  // return the executed instruction

  Ssi DlyStg::getinst (void) const {
    return d_inst;
  }

  // return the evaluated operand

  Operand DlyStg::getoprd (void) const {
    return d_oprd;
  }
}
