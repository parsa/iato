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
      if (Dsi::isprnm (pred) == true) {
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
      if (Dsi::isprnm (sreg)   == true) {
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
      if (Dsi::isprnm (dreg)   == false) continue;
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
    p_urf  = 0;
    p_iib  = 0;
    reset ();
  }

  // create a new physical rename stage by context and name

  PrnStg::PrnStg (Stx* stx, const string& name) : Stage (stx, name) {
    d_swsz = stx->getiwsz () * BN_SLSZ; assert (d_swsz > 0);
    p_urf  = 0;
    p_iib  = 0;
    reset ();
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
    // return true if one queue has reached the threshold
    bool qthr = d_mbuf.isthr () || d_ibuf.isthr () ||
                d_fbuf.isthr () || d_bbuf.isthr ();
    return qthr;
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
      Dsi dsi = lrn->getinst (i);
      if (dsi.isvalid () == false) continue;
      try {
	// rename it by the urf (rat and trb)
	assert (prninst (dsi, p_urf) == true);
	// expand the instruction
	switch (dsi.getsunit ()) {
	case MUNIT:
	  d_mbuf.push (dsi);
	  break;
	case IUNIT:
	  d_ibuf.push (dsi);
	  break;
	case FUNIT:
	  d_fbuf.push (dsi);
	  break;
	case BUNIT:
	  d_bbuf.push (dsi);
	  break;	
	default:
	  assert (false);
	  break;
	}
	// update the tracer
	if (p_tracer) {
	  Record rcd (d_name, dsi);
	  p_tracer->add (rcd);
	}
      } catch (Interrupt& vi) {
	vi.setinst (dsi);
	long iiib = dsi.getiib ();
	p_iib->setintr (iiib, vi);
      }
    }
    // check if the previous stage is halted
    if (p_pstg && (p_pstg->ishalted () == true)) {
      d_halt = true;
    }
    // update the tracer
    if (p_tracer) {
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

  // clear the instruction array
  
  void PrnStg::clear (void) {
    d_mbuf.reset ();
    d_ibuf.reset ();
    d_fbuf.reset ();
    d_bbuf.reset ();
  }

  // return true if all queues are empty

  bool PrnStg::isempty (void) const {
    bool flag = d_mbuf.isempty () && d_ibuf.isempty () && 
                d_fbuf.isempty () && d_bbuf.isempty ();
    return flag;
  }

  // return true if a queue is empty by unit

  bool PrnStg::isempty (const t_unit unit) const {
    bool result = false;
    switch (unit) {
    case MUNIT:
      result = d_mbuf.isempty ();
      break;
    case IUNIT:
      result = d_ibuf.isempty ();
      break;
    case FUNIT:
      result = d_fbuf.isempty ();
      break;
    case BUNIT:
      result = d_bbuf.isempty ();
      break;
    default:
      break;
    }
    return result;
  }

  // return true if all queues are full

  bool PrnStg::isfull (void) const {
    bool flag = d_mbuf.isfull () || d_ibuf.isfull () ||
                d_fbuf.isfull () || d_bbuf.isfull ();
    return flag;
  }

  // return true if a queue is full by unit

  bool PrnStg::isfull (const t_unit unit) const {
    bool result = false;
    switch (unit) {
    case MUNIT:
      result = d_mbuf.isfull ();
      break;
    case IUNIT:
      result = d_ibuf.isfull ();
      break;
    case FUNIT:
      result = d_fbuf.isfull ();
      break;
    case BUNIT:
      result = d_bbuf.isfull ();
      break;
    default:
      break;
    }
    return result;
  }

  // pop an instruction by unit

  Dsi PrnStg::pop (const t_unit unit) {
    Dsi dsi;
    switch (unit) {
    case MUNIT:
      dsi = d_mbuf.pop ();
      break;
    case IUNIT:
      dsi = d_ibuf.pop ();
      break;
    case FUNIT:
      dsi = d_fbuf.pop ();
      break;
    case BUNIT:
      dsi = d_bbuf.pop ();
      break;
    default:
      assert (false);
      break;
    }
    return dsi;
  }
}
