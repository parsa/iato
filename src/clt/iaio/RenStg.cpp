// ---------------------------------------------------------------------------
// - RenStg.cpp                                                              -
// - iato:iato client - renaming stage class implementation                  -
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
#include "RenStg.hpp"

namespace iato {
  using namespace std;

  // create a new renaming stage by context and unit

  RenStg::RenStg (Stx* stx, t_unit unit) : Stage (stx, RESOURCE_REN) {
    d_unit = unit;
    p_isl  = new Slot (unit);
    p_osl  = new Slot (unit);
    p_rse  = 0;
    p_ipb  = 0;
    p_opb  = 0;
    p_iib  = 0;
    reset ();
  }

  // create a new renaming stage by context, index and unit

  RenStg::RenStg (Stx* stx, t_unit unit, const long sidx) : 
           Stage (stx, sidx, RESOURCE_REN) {
    d_unit = unit;
    p_isl  = new Slot (unit);
    p_osl  = new Slot (unit);
    p_rse  = 0;
    p_ipb  = 0;
    p_opb  = 0;
    p_iib  = 0;
    reset ();
  }
  // create a new logical rename stage by context and name

  RenStg::RenStg (Stx* stx, t_unit unit, const long sidx, const string& name) :
           Stage (stx, sidx, name) {
    d_unit = unit;
    p_isl  = new Slot (unit);
    p_osl  = new Slot (unit);
    p_rse  = 0;
    p_ipb  = 0;
    p_opb  = 0;
    p_iib  = 0;
    reset ();
  }

  // destroy this logical rename stage

  RenStg::~RenStg (void) {
    delete p_isl;
    delete p_osl;
  }

  // reset this logical rename stage

  void RenStg::reset (void) {
    Stage::reset ();
    p_isl->reset ();
    p_osl->reset ();
  }
    
  // flush this logical rename stage

  void RenStg::flush (void) {
    Stage::flush  ();
    p_isl->reset ();
    p_osl->reset ();
  }

  // return true if the stage is holding

  bool RenStg::isholding (void) const {
    return !p_osl->isfree ();
  }

  // rename the instruction with the help of the rse and set
  // the speculative cfm for the instruction

  void RenStg::activate (void) {
    // make sure the output slot is free
    assert (p_osl->isfree () == true);
    // grab the instruction from the slot
    Ssi ssi = p_isl->grabinst ();
    // check for valid instruction and rename
    if (ssi.isvalid () == true) {
      try {
	// save the cfm in the instruction
	Rse::State irs = p_rse->getsst ();
	ssi.setiste (irs);
	// preset the rse with the instruction
	p_rse->preset (ssi);
	// rename the instruction by rse
	p_rse->rename (ssi);
	// after set the rse with the instruction
	p_rse->aftset (ssi);
	// set the speculative cfm in the instruction
	Rse::State srs = p_rse->getsst ();
	ssi.setsste (srs);
      } catch (Interrupt& vi) {
	// set the interrupt info
	ssi.setintr (true);
	vi.setinst (ssi);
	// set the interrupt buffer
	long iidx = ssi.getiib ();
	p_iib->setintr (iidx, vi);
      }
    }
    // set the instruction
    p_osl->setinst (ssi);
    // check if the previous stage is halted
    if (p_pstg && (p_pstg->ishalted () == true)) {
      d_halt = true;
    }
    // update the tracer
    if (p_tracer) {
      if (ssi.isvalid () == true) {
	Record rcd (d_name, ssi);
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

  void RenStg::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: instruction renaming" << endl;
    cout << "\tdelayable latency  \t: " << getdlat () << endl;
  }

  // bind this stage from the environment

  void RenStg::bind (Env* env, Stage* pstg, Stage* nstg) {
    // bind the stage locally
    Stage::bind (env, pstg, nstg);
    // bind the rse
    p_rse = dynamic_cast <Rse*> (env->get (RESOURCE_RSE));
    if (!p_rse) {
      string msg = "cannot bind rse within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the ipb
    p_ipb = dynamic_cast <Spb*> (env->get (RESOURCE_IPB));
    if (!p_ipb) {
      string msg = "cannot bind input slot buffer within stage ";
      throw Exception ("bind-error", msg + d_name);
    } else {
      p_ipb->add (p_isl);
    }
    // bind the opb
    p_opb = dynamic_cast <Spb*> (env->get (RESOURCE_OPB));
    if (!p_opb) {
      string msg = "cannot bind output slot buffer within stage ";
      throw Exception ("bind-error", msg + d_name);
    } else {
      p_opb->add (p_osl);
    }
    // bind the iib
    p_iib = dynamic_cast <Iib*> (env->get (RESOURCE_IIB));
    if (!p_iib) {
      string msg = "cannot bind iib within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
  }

  // return the renamed instruction

  Ssi RenStg::getinst (void) const {
    return p_osl->getinst ();
  }

  // return the renamed instruction and clean the stage

  Ssi RenStg::grabinst (void) {
    return p_osl->grabinst ();
  }
}
