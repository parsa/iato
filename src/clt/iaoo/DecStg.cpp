// ---------------------------------------------------------------------------
// - DecStg.cpp                                                              -
// - iato:iaoo client - instruction decode stage class implementation        -
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
#include "IpgStg.hpp"
#include "DecStg.hpp"
#include "Exception.hpp"

namespace iato {

  // create a new decode stage by context and name

  DecStg::DecStg (Stx* stx) : Stage (stx, RESOURCE_DEC) {
    d_iwsz = stx->getiwsz (); assert (d_iwsz > 0);
    d_swsz = stx->getswsz (); assert (d_swsz > 0);
    d_inop = stx->getbool ("IGNORE-NOP");
    p_inst = new Dsi[d_swsz];
    p_rob  = 0;
    p_mob  = 0;
    p_iib  = 0;
    p_pfr  = 0;
    reset ();
  }

  // create a new dec stage by context and name

  DecStg::DecStg (Stx* stx, const string& name) : Stage (stx, name) {
    d_iwsz = stx->getiwsz (); assert (d_iwsz > 0);
    d_swsz = stx->getswsz (); assert (d_swsz > 0);
    d_inop = stx->getbool ("IGNORE-NOP");
    p_inst = new Dsi[d_swsz];
    p_rob  = 0;
    p_mob  = 0;
    p_iib  = 0;
    p_pfr  = 0;
    reset ();
  }

  // destroy this dec stage

  DecStg::~DecStg (void) {
    delete [] p_inst;
  }

  // reset this dec stage

  void DecStg::reset (void) {
    Stage::reset ();
    clear ();
  }
    
  // flush this dec stage

  void DecStg::flush (void) {
    Stage::flush ();
    clear ();
  }

  // return true if the stage is holding

  bool DecStg::isholding (void) const {
    // cehck for the next stage
    if ((p_nstg) && (p_nstg->isholding () == true)) return true;
    // check for local resources
    if ((p_iib) && (p_iib->isthr () == true)) return true;
    if ((p_mob) && (p_mob->isthr () == true)) return true;    
    return false;
  }

  // activate this decode stage - the bundle are queried from the
  // previous stage and decoded on the fly.

  void DecStg::activate (void) {
    // get the previous stage and map it to ipg
    IpgStg* ipg = dynamic_cast <IpgStg*> (p_pstg);
    assert (ipg);
    // get the bundle and decode them
    long ipos = 0;
    for (long i = 0; i < d_iwsz; i++) {
      Bundle bndl = ipg->getbndl (i);
      if (bndl.isvalid () == true) {
	// decode instruction, set rob and mark crb
	for (long j = 0; j < BN_SLSZ; j++) {
	  try {
	    p_inst[ipos] = bndl.getinstr (j);
	    if (p_inst[ipos].isvalid () == true) {
	      // grab instruction ip and slot
	      t_octa ip   = p_inst[ipos].getiip  ();
	      long   slot = p_inst[ipos].getslot ();
	      // grab the serialization restart flag
	      bool srlz = p_pfr->getsrlz ();
	      // check for restart
	      if (p_pfr->check (ip, slot) == true) {
		// eventually allocate memory ordering buffer entry
		bool ildb = p_inst[ipos].getldb ();
		bool istb = p_inst[ipos].getstb ();
		long imob = -1;
		if ((ildb == true) || (istb == true)) {
		  imob = p_mob->alloc (ildb, istb);
		  assert (imob != -1);
		  p_inst[ipos].setmob (imob);
		}
		// check for nop execution
		if ((d_inop == true) && (p_inst[ipos].isnop () == true)) {
		  t_unit unit = p_inst[ipos].getbunit ();
		  p_rob->alloc (ip, slot, unit, false, true);
		  p_inst[ipos].reset ();
		} else {
		  // allocate instruction interrupt
		  long iidx = p_iib->alloc ();
		  p_inst[ipos].setiib (iidx);
		  // check for pre-serialization
		  if ((p_inst[ipos].ispresr () == true) && (srlz == false)) {
		    t_unit unit = p_inst[ipos].getbunit ();
		    p_rob->alloc (ip, slot, unit, true, false);
		  }
		  // allocate the rob index
		  long ridx = p_rob->alloc (p_inst[ipos], imob, iidx);
		  p_inst[ipos].setrix (ridx);
		}
	      } else {
		// reset instruction
		p_inst[ipos].reset ();
	      }
	    }
	    ipos++;
	  } catch (const Interrupt& vi) {
	    // clean eventually allocated resources
	    long imob = p_inst[ipos].getmob ();
	    if (imob != -1) p_mob->clear (imob);
	    long iiib = p_inst[ipos].getiib ();
	    if (iiib == -1) iiib = p_iib->alloc ();
	    // clean resource and mark interrupt
	    p_inst[ipos].reset ();
	    // set interrupt and mark rob
	    p_iib->setintr (iiib, vi);
	    p_rob->alloc (iiib);
	  }
	}
      } else {
	ipos += BN_SLSZ;
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

  void DecStg::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: instruction decode" << endl;
    cout << "\tbundle window size \t: " << d_iwsz     << endl;
    cout << "\tinstruction buffer \t: " << d_swsz     << endl;
    cout << "\tdelayable latency  \t: " << getdlat () << endl;
  }

  // clear the instruction array
  
  void DecStg::clear (void) {
    for (long i = 0; i < d_swsz; i++) p_inst[i].reset ();
  }

  // bind this stage from the environment

  void DecStg::bind (Env* env, Stage* pstg, Stage* nstg) {
    // bind the stage locally
    Stage::bind (env, pstg, nstg);
    // bind the rob
    p_rob = dynamic_cast <Rob*> (env->get (RESOURCE_ROB));
    if (!p_rob) {
      string msg = "cannot bind reorder buffer within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the memory ordering buffer
    p_mob = dynamic_cast <Mob*> (env->get (RESOURCE_MOB));
    if (!p_mob) {
      string msg = "cannot bind memory ordering buffer within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the instruction interrupt buffer
    p_iib = dynamic_cast <Iib*> (env->get (RESOURCE_IIB));
    if (!p_iib) {
      string msg = "cannot bind instruction interrupt buffer within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the restart resource
    p_pfr = dynamic_cast <Restart*> (env->get (RESOURCE_PFR));
    if (!p_pfr) {
      string msg = "cannot bind restart engine within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
  }

  // return an instruction by index

  Dsi DecStg::getinst (const long index) const {
    assert ((index >= 0) && (index < d_swsz));
    Dsi dsi = p_inst[index];
    p_inst[index].reset ();
    return dsi;
  }
}
