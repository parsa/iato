// ---------------------------------------------------------------------------
// - PrnStg.cpp                                                              -
// - iato:iaoo client - physical renaming stage class implementation         -
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
#include "LrnStg.hpp"
#include "PrnStg.hpp"
#include "Interrupt.hpp"

namespace iato {
  using namespace std;

  // this procedure returns true if the rid must be physically renamed
  static bool isprnm (const Rid& rid) {
    // if the rid is not valid, it cannot be renamed
    if (rid.isvalid () == false) return false;
    // check for logical renaming
    if (rid.islrnm () == false) return false;
    // check special registers
    if (rid.gettype () == IPRG) return false;
    if (rid.gettype () == CFMR) return false;
    if (rid.gettype () == PRRG) return false;
    if (rid.gettype () == PROT) return false;
    if (rid.gettype () == UMRG) return false;
    if (rid.gettype () == PSRG) return false;
    // all others can be renamed
    return true;
  }

  // this procedure renames a logical instruction with the urf (rat+trb)
  static bool prninst (Dsi& dsi, Urf* urf) {
    // check first the instruction
    if (dsi.isvalid () == false) return true;
    // get the rat and trb
    Rat* rat = urf->getrat ();
    Trb* trb = urf->gettrb ();
    // rename the predicate - if the predicate is not physically
    // renamed, it is marked ready
    Rid  pred = dsi.getpnum ();
    if (pred.isvalid () == true) {
      if (isprnm (pred) == true) {
	long vnum = rat->getmap (pred);
	assert (vnum != -1);
	pred.setvnum (vnum);
      } else {
	pred.seterdy (true);
      }
      dsi.setpnum  (pred);
    }
    // rename the source registers - if the register is not physically
    // renamed it is marked ready
    for (long i = 0; i < IA_MSRC; i++) {
      Rid sreg = dsi.getsnum (i);
      if (sreg.isvalid () == false) continue;
      if (isprnm (sreg)   == true) {
	long vnum = rat->getmap (sreg);
	assert (vnum != -1);
	sreg.setvnum (vnum);
      } else {
	sreg.seterdy (true);
      }
      dsi.setsnum  (i,sreg);
    }
    // rename the destination registers
    for (long i = 0; i < IA_MDST; i++) {
      Rid dreg = dsi.getdnum (i);
      if (dreg.isvalid () == false) continue;
      if (isprnm (dreg)   == false) continue;
      // allocate a new trb entry and update the rat
      long vnum = trb->alloc  (); assert (vnum != -1);
      long onum = rat->setmap (dreg, vnum);
      // update the rid
      dreg.setvnum (vnum, onum);
      dsi.setdnum  (i, dreg);
    }
    return true;
  }

  // create a new physical rename stage by context and name

  PrnStg::PrnStg (Stx* stx) : Stage (stx, RESOURCE_PRN) {
    d_swsz = stx->getiwsz () * BN_SLSZ; assert (d_swsz > 0);
    p_inst = new Dsi[d_swsz];
    p_urf  = 0;
    p_iib  = 0;
    reset ();
  }

  // create a new physical rename stage by context and name

  PrnStg::PrnStg (Stx* stx, const string& name) : Stage (stx, name) {
    d_swsz = stx->getiwsz () * BN_SLSZ; assert (d_swsz > 0);
    p_inst = new Dsi[d_swsz];
    p_urf  = 0;
    p_iib  = 0;
    reset ();
  }

  // destroy this physical rename stage

  PrnStg::~PrnStg (void) {
    delete [] p_inst;
  }

  // reset this physical rename stage

  void PrnStg::reset (void) {
    Stage::reset ();
    clear ();
  }
    
  // flush this physical rename stage

  void PrnStg::flush (void) {
    Stage::flush ();
    p_urf->flush ();
    clear ();
  }

  // return true if the stage is holding

  bool PrnStg::isholding (void) const {
    if (p_nstg) return p_nstg->isholding ();
    return false;
  }

  // activate this physical rename stage - the bundle are queried from the
  // previous stage and physical renamed on the fly. Initiliy the rob and crb
  // allocation is done in this stage for each valid bundle.

  void PrnStg::activate (void) {
    // get the previous stage and map it to lrn
    LrnStg* lrn = dynamic_cast <LrnStg*> (p_pstg);
    assert (lrn);
    // loop in the logical renamed instruction buffer
    for (long i = 0; i < d_swsz; i++) {
      // get logical renamed instruction
      p_inst[i] = lrn->getinst (i);
      try {
	// rename it by the urf (rat and trb)
	assert (prninst (p_inst[i], p_urf) == true);
      } catch (Interrupt& vi) {
	vi.setinst (p_inst[i]);
	long iiib = p_inst[i].getiib ();
	p_iib->setintr (iiib, vi);
	p_inst[i].reset ();
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

  void PrnStg::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: instruction physical renaming" << endl;
    cout << "\tinstruction buffer \t: " << d_swsz     << endl;
    cout << "\tdelayable latency  \t: " << getdlat () << endl;
  }

  // clear the instruction array
  
  void PrnStg::clear (void) {
    for (long i = 0; i < d_swsz; i++) p_inst[i].reset ();
  }

  // bind this stage from the environment

  void PrnStg::bind (Env* env, Stage* pstg, Stage* nstg) {
    // bind the stage locally
    Stage::bind (env, pstg, nstg);
    // bind the universal file
    p_urf = dynamic_cast <Urf*> (env->get (RESOURCE_URF));
    if (!p_urf) {
      string msg = "cannot bind universal register file within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the iib
    p_iib = dynamic_cast <Iib*> (env->get (RESOURCE_IIB));
    if (!p_iib) {
      string msg = "cannot bind iib within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
  }

  // return an instruction by index

  Dsi PrnStg::getinst (const long index) const {
    assert ((index >= 0) && (index < d_swsz));
    Dsi dsi = p_inst[index];
    p_inst[index].reset ();
    return dsi;
  }
}
