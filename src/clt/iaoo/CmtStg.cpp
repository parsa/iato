// ---------------------------------------------------------------------------
// - CmtStg.cpp                                                              -
// - iato:iaoo client - commit/retire stage class implementation             -
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
#include "CmtStg.hpp"
#include "Exception.hpp"

namespace iato {


  // create a new commit stage by context

  CmtStg::CmtStg (Stx* stx) : Stage (stx, RESOURCE_CMT) {
    d_iwsz = stx->getiwsz (); assert (d_iwsz > 0);
    d_swsz = d_iwsz * BN_SLSZ;
    p_rse  = 0;
    p_rob  = 0;
    p_irb  = 0;
    p_irt  = 0;
    p_urf  = 0;
    p_iib  = 0;
    p_gcs  = 0;
    p_mob  = 0;
    p_mli  = new Mli (stx);
    p_dtl  = 0;
    p_pfr  = 0;
    p_rbk  = 0;
    p_bpr  = 0;
    p_ppr  = 0;
    reset ();
  }
  
  // create a new commit stage by context and name

  CmtStg::CmtStg (Stx* stx, const string& name) : Stage (stx,name) {
    d_iwsz = stx->getiwsz (); assert (d_iwsz > 0);
    d_swsz = d_iwsz * BN_SLSZ;
    p_rse  = 0;
    p_rob  = 0;
    p_irb  = 0;
    p_irt  = 0;
    p_urf  = 0;
    p_iib  = 0;
    p_gcs  = 0;
    p_mob  = 0;
    p_mli  = new Mli (stx);
    p_dtl  = 0;
    p_pfr  = 0;
    p_rbk  = 0;
    p_bpr  = 0;
    p_ppr  = 0;
    reset ();
  }

  // destroy this commit stage

  CmtStg::~CmtStg (void) {
    delete p_mli;
  }

  // reset this stage

  void CmtStg::reset (void) {
    Stage::reset ();
    p_mli->reset ();
  }

  // activate this commit stage. The instruction are selected from the 
  // rob in order and their results grabbed from the irb.

  void CmtStg::activate (void) {
    // look in the rob for the next available instruction
    // the number of instruction to process is equivalent to
    // the instruction window size
    for (long  i = 0; i < d_swsz; i++) {
      if (p_rob->ispop () == false) break;
      // check for interrupt or normal instruction
      if (p_rob->isintr () == true) {
	// get the interrupt and check or route
	bool sbit = p_rob->getsbit ();
	long iiib = p_rob->ipop    ();
	if (iiib == -1) {
	  string msg = "unallocated interrupt in rob";
	  throw Exception ("rob-error", msg);
	}
	// get the interrupt and clean the resource
	Interrupt vi = p_iib->getintr (iiib);
	p_iib->clear (iiib);
	// check for speculation
	if (sbit == true) {
	  Instr inst = vi.getinst ();
	  assert (inst.isvalid () == true);
	  // mark the stat engine
	  if (p_stat) p_stat->markpf (inst.isbr ());
	  // update the restart engine
	  p_pfr->pfstd (vi.getip (), vi.getslot ());
	  return;
	}
	// get the instruction
	Instr inst = vi.getinst ();
	// update the stat collection
	if (p_stat) p_stat->addinst (inst);
	if (p_stat) p_stat->markpf  (false);
	// update the tracer with commit info
	if ((p_tracer) && (inst.isvalid () == true)) {
	  Record rcd (d_name, inst);
	  p_tracer->add (rcd);
	}
	// here we route the interrupt
	p_irt->route (vi);
	// update the ip
	updip (inst);
	// update the restart engine
	p_pfr->pfnxt (vi.getip (), vi.getslot ());
      } else {
	// check if the current rob entry is a serialization
	if (p_rob->issrlz () == true) {
	  // pop this entry
	  p_rob->spop ();
	  // get the instruction ip and slot
	  t_octa ip   = p_rob->getiip  ();
	  long   slot = p_rob->getslot ();
	  // set the restart engine
	  p_pfr->pfsrl (ip, slot);
	  return;
	}
	// check if the current rob entry is a nop
	if (p_rob->isnop () == true) {
	  // get the instruction ip and slot
	  t_octa ip   = p_rob->getiip  ();
	  long   slot = p_rob->getslot ();
	  t_unit unit = p_rob->getunit ();
	  // update the stat collection
	  if (p_stat) p_stat->addnop (unit);
	  // pop this entry
	  p_rob->npop ();
	  // update the ip
	  updip (ip, slot);
	  continue;
	}
	// get the mob index
	long imob = p_rob->getimob ();
	// get the iib index
	long iiib = p_rob->getiiib ();
	// get the instruction ip and slot
	t_octa ip   = p_rob->getiip  ();
	long   slot = p_rob->getslot ();
	// check that the memory load has not been cancelled
	if (imob != -1) {
	  if (p_mob->iscancel (imob) == true) {
	    p_pfr->pfstd (ip, slot);
	    return;
	  }
	}
	// get the branch speculative status
	bool bbss = p_rob->getbbss ();
	// get the cancel flag
	bool cnlf = p_rob->iscancel ();
	// grab the latest irb index
	long iidx = p_rob->pop ();
	if (iidx != -1) {
	  // grab the instruction
	  Dsi dsi =  p_irb->getinst (iidx);
	  // grab the result
	  Result resl = p_irb->getresl (iidx);
	  // check for valid predicate prediction
	  if (dsi.getppfl () == true) {
	    if (p_dtl->chkspp (dsi) == true) {
	      // update statistics
	      if (p_stat) p_stat->markpp (true);
	      // update the predicate predictor
	      p_ppr->markpp (dsi, dsi.getppvl ());
	    } else {
	      // update statistics
	      if (p_stat) p_stat->markpp (false);
	      if (p_stat) p_stat->markpf (dsi.isbr ());
	      // update the predicate predictor
	      p_ppr->markpp (dsi, !dsi.getppvl ());
	      // update restart engine
	      p_pfr->pfstd (ip, slot);
	      return;
	    }
	  } else {
	    // update the predicate predictor
	    p_ppr->markpp (dsi, !cnlf);
	  }
	  // check for cancellation status
	  if (cnlf == false) {
	    try {
	      // update the rse
	      p_rse->update (resl);
	      // update the register bank
	      p_rbk->write  (resl);
	      // update the memory
	      p_mli->update (dsi, resl);
	      // clear the urf
	      p_urf->clean  (resl);
	    } catch (Interrupt& vi) {
	      vi.setinst (dsi);
	      throw vi;
	    }
	  } else {
	    // cancel in the urf
	    p_urf->cancel (resl);
	    // broadcast to force operands ready
	    p_gcs->setrdy (resl);
	  }
	  // update the architectural ip
	  updip (dsi, resl, cnlf);
	  // update the branch prediction system
	  p_bpr->markbr (dsi, resl, !cnlf);
	  // clear the station entry
	  p_gcs->clear (dsi);
	  // clean the irb
	  p_irb->clear (iidx);
	  // clear the mob
	  if (imob != -1) p_mob->clear (imob);
	  // clear the iib
	  if (iiib != -1) p_iib->clear (iiib);
	  // update the stat machinery with branch prediction
	  // branches are predicted if the instruction is a branch
	  // with the speculative flag set. The prediction failed
	  // if the bbss flag is false
	  if ((dsi.isbr () == true) && (dsi.getsfl () == true)) {
	    if (p_stat) p_stat->markbp (bbss);
	  }
	  // check for valid ip speculation
	  if (bbss == false) {
	    // restore the branch predictor history
	    p_bpr->sethist (dsi.gethist ());
	    // if the ip condition is defined by the result
	    // the machine is in a good state and can be restarted
	    // right away with local condition, else we use the next
	    // instruction
	    if (resl.isreg (IPRG) == true) {
	      p_pfr->pflcl ();
	    } else {
	      p_pfr->pfnxt (ip,slot);
	    }
	    // update the stat collection
	    if (p_stat) p_stat->addinst (dsi, cnlf, dsi.getxflg ());
	    if (p_stat) p_stat->markpf  (dsi.isbr ());
	    // update the tracer with commit info
	    if ((p_tracer) && (dsi.isvalid () == true)) {
	      Record rcd (d_name, dsi, !cnlf);
	      p_tracer->add (rcd);
	    }
	    return;
	  }
	  // check for taken branch - in that case, the remaining instruction
	  // are not commited and the pipe is flushed. Normally, we should
	  // invalidate the remaining instructions in the bundle since
	  // this case the BBB bundle where the taken branch is the next bundle
	  if ((dsi.getsfl () == false) && (resl.isreg (IPRG) == true)) {
	    // restore the branch predictor history
	    p_bpr->sethist (dsi.gethist ());
	    // call the restart engine and request a pipeline flush
	    // the next ip, assuming slot 0 is already set and the rse
	    // is in a valid state
	    p_pfr->pfdef ();
	    // update the stat collection
	    if (p_stat) p_stat->addinst (dsi, cnlf, dsi.getxflg ());
	    if (p_stat) p_stat->markpf  (dsi.isbr ());
	    // update the tracer with commit info
	    if ((p_tracer) && (dsi.isvalid () == true)) {
	      Record rcd (d_name, dsi, !cnlf);
	      p_tracer->add (rcd);
	    }
	    return;
	  }
	  // check for post serialization with non-renamed register.
	  // in that case, the instrution commits but the pipeline is
	  // flushed and restarted at the next instruction
	  if (resl.isreg (PRRG) || resl.isreg (PROT) ||
	      resl.isreg (UMRG) || resl.isreg (PSRG)) {
	    // restore the branch predictor history
	    p_bpr->sethist (dsi.gethist ());
	    // flush and restart at next instruction
	    p_pfr->pfnxt (ip, slot);
	    // update the stat collection
	    if (p_stat) p_stat->addinst (dsi, cnlf, dsi.getxflg ());
	    if (p_stat) p_stat->markpf  (dsi.isbr ());
	    // update the tracer with commit info
	    if ((p_tracer) && (dsi.isvalid () == true)) {
	      Record rcd (d_name, dsi, !cnlf);
	      p_tracer->add (rcd);
	    }
	    return;
	  }
	  // check for valid rse speculation
	  if (p_rse->validate (dsi.getsste ()) == false) {
	    // restore the branch predictor history
	    p_bpr->sethist (dsi.gethist ());
	    // flush and restart at current condition
	    // since the instruction has update the ip
	    p_pfr->pflcl ();
	    // update the stat collection
	    if (p_stat) p_stat->addinst (dsi, cnlf, dsi.getxflg ());
	    if (p_stat) p_stat->markpf  (dsi.isbr ());
	    // update the tracer with commit info
	    if ((p_tracer) && (dsi.isvalid () == true)) {
	      Record rcd (d_name, dsi, !cnlf);
	      p_tracer->add (rcd);
	    }
	    return;
	  }
	  // update the stat collection
	  if (p_stat) p_stat->addinst (dsi, cnlf, dsi.getxflg ());
	  // update the tracer with commit info
	  if ((p_tracer) && (dsi.isvalid () == true)) {
	    Record rcd (d_name, dsi, !cnlf);
	    p_tracer->add (rcd);
	  }
	} else {
	  assert (false);
	}
      }
    }
    // check if the previous stage is halted - if it is halted, we check
    // that the rob is empty before halting everything
    if (p_pstg) {
      if ((p_pstg->ishalted () == true) && (p_rob->isempty () == true)) {
	d_halt = true;
      }
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

  void CmtStg::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: commit stage" << endl;
    cout << "\tdelayable latency  \t: " << getdlat () << endl;
  }

  // bind this stage from the environment

  void CmtStg::bind (Env* env, Stage* pstg, Stage* nstg) {
    // bind the stage locally
    Stage::bind (env, pstg, nstg);
    // bind the rob
    p_rob = dynamic_cast <Rob*> (env->get (RESOURCE_ROB));
    if (!p_rob) {
      string msg = "cannot bind reorder buffer within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the irb
    p_irb = dynamic_cast <Irb*> (env->get (RESOURCE_IRB));
    if (!p_irb) {
      string msg = "cannot bind instruction result buffer within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the irt
    p_irt = dynamic_cast <Irt*> (env->get (RESOURCE_IRT));
    if (!p_irt) {
      string msg = "cannot bind interrupt router within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the rse
    p_rse = dynamic_cast <Rse*> (env->get (RESOURCE_RSE));
    if (!p_rse) {
      string msg = "cannot bind rse within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the urf
    p_urf = dynamic_cast <Urf*> (env->get (RESOURCE_URF));
    if (!p_urf) {
      string msg = "cannot bind universal register file within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the detection logic
    p_dtl = dynamic_cast <Detect*> (env->get (RESOURCE_DTL));
    if (!p_dtl) {
      string msg = "cannot bind detection logic within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the restart resource
    p_pfr = dynamic_cast <Restart*> (env->get (RESOURCE_PFR));
    if (!p_pfr) {
      string msg = "cannot bind restart resource within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the register bank
    p_rbk = dynamic_cast <Register*> (env->get (RESOURCE_RBK));
    if (!p_rbk) {
      string msg = "cannot bind register bank within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the iib
    p_iib = dynamic_cast <Iib*> (env->get (RESOURCE_IIB));
    if (!p_iib) {
      string msg = "cannot bind iib within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the central station
    p_gcs = dynamic_cast <Gcs*> (env->get (RESOURCE_GCS));
    if (!p_gcs) {
      string msg = "cannot bind central station within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the memory ordering buffer
    p_mob = dynamic_cast <Mob*> (env->get (RESOURCE_MOB));
    if (!p_mob) {
      string msg = "cannot bind memory ordering buffer within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the branch predictor
    p_bpr = dynamic_cast <Branch*> (env->get (RESOURCE_BPS));
    if (!p_bpr) {
      string msg = "cannot bind branch predictor within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the predicate predictor
    p_ppr = dynamic_cast <Predicate*> (env->get (RESOURCE_PPS));
    if (!p_ppr) {
      string msg = "cannot bind predicate predictor within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the mta and memory logic
    Mta* mta = dynamic_cast <Mta*> (env->get (RESOURCE_MTA));
    if (!mta) {
      string msg = "cannot bind ta within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    p_mli->bind (mta, p_mob);
  }
  
  // update the architectural ip with an ip and slot

  void CmtStg::updip (const t_octa iip, const long slot) {
    // get the current ip and compute next one
    t_octa cip = p_rbk->getoval (IPRG, 0);
    t_octa nip = cip + BN_BYSZ;
    // check instruction ip
    assert (iip == cip);
    // do thing if we are not at slot 2
    if (slot != 2) return;
    // update the ip
    p_rbk->write (IPRG, 0, nip);
  }
  
  // update the architectural ip for an instruction

  void CmtStg::updip (const Dsi& dsi) {
    // do nothing with nothing
    if (dsi.isvalid () == false) return;
    // get the current ip and compute next one
    t_octa cip = p_rbk->getoval (IPRG, 0);
    t_octa nip = cip + BN_BYSZ;
    t_octa iip = dsi.getiip ();
    // check instruction ip
    assert (iip == cip);
    // do thing if we are not at slot 2
    if (dsi.getslot () != 2) return;
    // update the ip
    p_rbk->write (IPRG, 0, nip);
  }

  // update the architectural ip and check for commited instructions

  void CmtStg::updip (const Dsi& dsi, const Result& resl, const bool cnlf) {
    // do nothing with nothing
    if (dsi.isvalid () == false) return;
    // do nothing if the instruction is not cancelled and the ip is updated
    if ((cnlf == false) && (resl.isreg (IPRG) == true)) return;
    // update the ip
    updip (dsi);
  }
}
