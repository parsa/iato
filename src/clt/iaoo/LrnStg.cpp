// ---------------------------------------------------------------------------
// - LrnStg.cpp                                                              -
// - iato:iaoo client - logical renaming stage class implementation          -
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
#include "DecStg.hpp"
#include "LrnStg.hpp"
#include "Exception.hpp"

namespace iato {
  using namespace std;

  // create a new logical rename stage by context and name

  LrnStg::LrnStg (Stx* stx) : Stage (stx, RESOURCE_LRN) {
    d_swsz = stx->getswsz (); assert (d_swsz > 0);
    p_inst = new Dsi[d_swsz];
    p_rse  = 0;
    p_iib  = 0;
    p_pps  = 0;
    reset ();
  }

  // create a new logical rename stage by context and name

  LrnStg::LrnStg (Stx* stx, const string& name) : Stage (stx, name) {
    d_swsz = stx->getswsz (); assert (d_swsz > 0);
    p_inst = new Dsi[d_swsz];
    p_rse  = 0;
    p_iib  = 0;
    p_pps  = 0;
    reset ();
  }

  // destroy this logical rename stage

  LrnStg::~LrnStg (void) {
    delete [] p_inst;
  }

  // reset this logical rename stage

  void LrnStg::reset (void) {
    Stage::reset ();
    clear ();
  }
    
  // flush this logical rename stage

  void LrnStg::flush (void) {
    Stage::flush ();
    clear ();
  }

  // return true if the stage is holding

  bool LrnStg::isholding (void) const {
    if (p_nstg) return p_nstg->isholding ();
    return false;
  }

  // activate this logical rename stage - the instructions are queried from
  // the decode stage, the rse is preset speculatively and logical renaming
  // take place. Note that the rse state is as a speculative cfm in the 
  // instruction.

  void LrnStg::activate (void) {
    // get the previous stage and map it to decode
    DecStg* dec = dynamic_cast <DecStg*> (p_pstg);
    assert (dec);
    // loop in the decode instruction buffer
    for (long i = 0; i < d_swsz; i++) {
      p_inst[i] = dec->getinst (i);
      if (p_inst[i].isvalid () == true) {
	try {
	  // save the instruction state - this permits to get
	  // the cfm used to compute the alloc
	  Rse::State irs = p_rse->getsst ();
	  p_inst[i].setiste (irs);
	  // preset the rse with the instruction
	  p_rse->preset (p_inst[i]);
	  // rename the instruction by rse
	  p_rse->rename (p_inst[i]);
	  // after set the rse with the instruction
	  p_rse->aftset (p_inst[i]);
	  // set the speculative state in the instruction
	  Rse::State srs = p_rse->getsst ();
	  p_inst[i].setsste (srs);
	  // predict the predicate
	  if (p_pps->ispredict (p_inst[i]) == true) {
	    bool pval = p_pps->predict (p_inst[i]);
	    p_inst[i].setppvl (pval);
	  }
	} catch (Interrupt& vi) {
	  vi.setinst (p_inst[i]);
	  long iiib = p_inst[i].getiib (); 
	  p_iib->setintr (iiib, vi);
	  p_inst[i].reset ();
	}
      }
    }
    // check if the previous stage is halted
    if (p_pstg && (p_pstg->ishalted () == true)) {
      d_halt = true;
    }
    // update the tracer
    if (p_tracer) {
      for (long i = 0; i < d_swsz; i++) {
	if (p_inst[i].isvalid () == true) {
	  Record rcd (d_name, p_inst[i]);
	  p_tracer->add (rcd);
	}
      }
      if (d_halt == true) {
	Record rcd (d_name);
	rcd.settype (Record::HALTED);
	p_tracer->add (rcd);
      }
    }
  }

  // report some stage information

  void LrnStg::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: instruction logical renaming" << endl;
    cout << "\tinstruction buffer \t: " << d_swsz     << endl;
    cout << "\tdelayable latency  \t: " << getdlat () << endl;
  }

  // clear the instruction array
  
  void LrnStg::clear (void) {
    for (long i = 0; i < d_swsz; i++) p_inst[i].reset ();
  }

  // bind this stage from the environment

  void LrnStg::bind (Env* env, Stage* pstg, Stage* nstg) {
    // bind the stage locally
    Stage::bind (env, pstg, nstg);
    // bind the rse
    p_rse = dynamic_cast <Rse*> (env->get (RESOURCE_RSE));
    if (!p_rse) {
      string msg = "cannot bind rse within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the iib
    p_iib = dynamic_cast <Iib*> (env->get (RESOURCE_IIB));
    if (!p_iib) {
      string msg = "cannot bind iib within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the predicate predictor 
    p_pps = dynamic_cast <Predicate*> (env->get (RESOURCE_PPS));
    if (!p_pps) {
      string msg = "cannot bind predicate predictor within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
  }

  // return an instruction by index

  Dsi LrnStg::getinst (const long index) const {
    assert ((index >= 0) && (index < d_swsz));
    Dsi dsi = p_inst[index];
    p_inst[index].reset ();
    return dsi;
  }
}
