// ---------------------------------------------------------------------------
// - Emulator.cpp                                                            -
// - iato:iaka client - emulator class implementation                        -
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

#include "Eio.hpp"
#include "Bits.hpp"
#include "KrnExit.hpp"
#include "Syscall.hpp"
#include "Emulator.hpp"
#include "Mexecute.hpp"
#include "Iexecute.hpp"
#include "Fexecute.hpp"
#include "Bexecute.hpp"

namespace iato {

  // create a new emulator with a context and a program name

  Emulator::Emulator (Atx* atx, const string& name) {
    // save the context parameters
    d_name  = name;
    d_tflag = atx->getbool ("TRACER-FLAG");
    d_vflag = atx->getbool ("TRACER-VERBOSE-FLAG");
    d_bflag = atx->getbool ("TRACER-BRANCH-FLAG");
    d_boflg = atx->getbool ("TRACER-BRANCH-ONLY");
    d_poflg = atx->getbool ("TRACER-PREDICATE-ONLY");
    d_pgflg = atx->getbool ("TRACER-PREDICATE-TARGETS");
    d_cflag = atx->getbool ("CHECKER-FLAG");
    d_sflag = atx->getbool ("STAT-FLAG");
    d_dflag = atx->getbool ("FETCHER-DISPERSE-FLAG");
    d_maxcc = atx->getlong ("MAXIMUM-CYCLE-COUNT");
    d_bpfil = atx->getlong ("BRANCH-PIPELINE-REFILL");
    // the emulator resources
    p_irt   = new Irt      (atx);
    p_rse   = new Renamer  (atx);
    p_rbk   = new Register (atx);
    p_mmap  = new Mapper   (atx);
    p_ftch  = new Fetcher  (atx);
    p_bprd  = Branch::mkbr (atx);
    // the emulator units
    p_munit = new Mexecute;
    p_iunit = new Iexecute;
    p_funit = new Fexecute;
    p_bunit = new Bexecute;
    // create an elf image
    p_elf   = new ElfImage (atx, d_name);
    p_mem   = p_elf->getimage ();
    // install a tracer if needed
    p_tracer = d_tflag ? new Tracer (atx) : 0;
    // install a checker if needed
    p_checker = d_cflag ? p_elf->getchecker () : 0;
    // install a stat collection if needed
    p_stat = d_sflag ? new Stat : 0;
    // reset everything
    reset ();
  }

  // create a new emulator with a context, a program name and arguments

  Emulator::Emulator (Atx* atx, const string& name, 
		      const vector<string>& argv) {
    // save the context parameters
    d_name  = name;
    d_tflag = atx->getbool ("TRACER-FLAG");
    d_vflag = atx->getbool ("TRACER-VERBOSE-FLAG");
    d_bflag = atx->getbool ("TRACER-BRANCH-FLAG");
    d_boflg = atx->getbool ("TRACER-BRANCH-ONLY");
    d_poflg = atx->getbool ("TRACER-PREDICATE-ONLY");
    d_pgflg = atx->getbool ("TRACER-PREDICATE-TARGETS");
    d_cflag = atx->getbool ("CHECKER-FLAG");
    d_sflag = atx->getbool ("STAT-FLAG");
    d_dflag = atx->getbool ("FETCHER-DISPERSE-FLAG");
    d_maxcc = atx->getlong ("MAXIMUM-CYCLE-COUNT");
    d_bpfil = atx->getlong ("BRANCH-PIPELINE-REFILL");
    // the emulator resources
    p_irt   = new Irt      (atx);
    p_rse   = new Renamer  (atx);
    p_rbk   = new Register (atx);
    p_mmap  = new Mapper   (atx);
    p_ftch  = new Fetcher  (atx);
    p_bprd  = Branch::mkbr (atx);
    // the emulator units
    p_munit = new Mexecute;
    p_iunit = new Iexecute;
    p_funit = new Fexecute;
    p_bunit = new Bexecute;
    // create an elf image
    p_elf   = new ElfImage (atx, d_name, argv);
    p_mem   = p_elf->getimage ();
    // install a tracer if needed
    p_tracer = d_tflag ? new Tracer (atx) : 0;
    // install a checker if needed
    p_checker = d_cflag ? p_elf->getchecker () : 0;
    // install a stat collection if needed
    p_stat = d_sflag ? new Stat : 0;
    // reset everything
    reset ();
  }

  // destroy this emulator

  Emulator::~Emulator (void) {
    delete p_elf;
    delete p_mem;
    delete p_rse;
    delete p_rbk;
    delete p_stat;
    delete p_ftch;
    delete p_mmap;
    delete p_bprd;
    delete p_munit;
    delete p_iunit;
    delete p_funit;
    delete p_bunit;
    delete p_tracer;
    delete p_checker;
  }

  // reset the emulator

  void Emulator::reset (void) {
    // reset local info
    d_cycle = 0;
    // reset resources    
    p_rse->reset  ();
    p_rbk->reset  ();
    // reset memory resources
    p_ftch->reset ();
    p_mmap->reset ();
    // reset optional resources
    if (p_bprd)   p_bprd->reset   ();
    if (p_tracer) p_tracer->reset ();
    if (p_stat)   p_stat->reset   ();
    // set the fpsr, unat and rnat by following the party line
    p_rbk->write (AREG, AR_FPSR, DEF_FPSR);
    p_rbk->write (AREG, AR_RNAT, DEF_RNAT);
    p_rbk->write (AREG, AR_UNAT, DEF_UNAT);
    // set the ip with the entry point
    p_rbk->write (IPRG, 0, p_elf->getentry ());
    // set the stack pointer with its default
    ElfStack* stk = p_mem->getstkm ();
    p_rbk->write (GREG, ABI_SP, stk->getstkva ());
    // set the backing store pointer
    ElfBsa* bsa = p_mem->getbsam ();
    assert ((bsa->getbase () & 0x00000000000001FFULL) == OCTA_0);
    p_rbk->write (AREG, AR_BSP,  bsa->getbase ());
    p_rbk->write (AREG, AR_BSPS, bsa->getbase ());
  }

  // print some emulator stats

  void Emulator::pstat (void) {
    if (!p_stat) return;
    cout << endl;
    cout << "program name                   : " << d_name << endl;
    p_stat->print ();
  }

  // bind the emulator

  void Emulator::bind (void) {
    // bind the rse with the memory and register bank
    p_rse->bind (p_mem, p_rbk);
    // bind the fetch buffer
    p_ftch->bind (p_mem);
    // bind the memory mapper
    p_mmap->bind (p_mem);
    // bind the tracer
    p_rse->settrc  (p_tracer);
    p_irt->settrc  (p_tracer);
    p_rbk->settrc  (p_tracer);
    p_ftch->settrc (p_tracer);
    p_mmap->settrc (p_tracer);
    // bind the irt with a syscall plugin
    p_irt->bind (FAULT_IT_INST_BREAK, new Syscall (p_rse, p_rbk, p_mem));
  }

  // run the emulator

  int Emulator::run (void) {
    int status = 0;
    try {
      loop ();
    } catch (const KrnExit& xit) {
      status = xit.getstatus ();
    } catch (const Exception& ve) {
      Exception ce = ve;
      ce.setcycle (d_cycle);
      throw ce;
    }
    // eventually, print the tracer
    if (p_tracer) p_tracer->print ();
    // mark the the stat collection if needed
    if (p_stat) p_stat->markes ();
    // now run the checker if requested
    if (p_checker) {
      status = p_checker->check (p_rbk) ? 0 : 1;
    }
    return status;
  }

  // loop in the emulator

  void Emulator::loop (void) {
    // check for dispersal and initialize tracing
    if ((p_stat) && (d_dflag == true)) p_stat->marksc (d_bpfil+1);
    if ((p_tracer) && (d_dflag == true)) {
      p_tracer->newtraces (d_bpfil+1);
      d_cycle += d_bpfil+1;
    }
    // loop within the bundle
    while (true) {
      // check for next cycle
      if ((d_maxcc != 0) && (d_cycle >= d_maxcc)) break;
      // check for empty fetcher
      if (p_ftch->isempty () == true) {
	// get the current ip
	t_octa ip = p_rbk->getoval (IPRG, 0);
	p_ftch->fill (ip);
	// initiate new cycle
	if ((p_stat) && (d_dflag == true)) p_stat->marksc ();
	if (p_tracer) p_tracer->newtrace ();
	d_cycle++;
      }
      // get the bundle to process
      Bundle bndl = p_ftch->getbndl ();
      // execute the bundle
      exec (bndl);
      // eventually, print the tracer
      if ((p_tracer) && (p_ftch->isempty () == true)) p_tracer->print ();
    }
    // check for dispersal and print
    if ((p_tracer) && (d_dflag == true)) p_tracer->print ();
  }

  // execute a bundle

  void Emulator::exec (const Bundle& bnd) {
    // put bundle in stat collection
    if (p_stat) p_stat->addbndl (bnd);
    // eventually use the tracer to store bundle
    if ((p_tracer) && (d_dflag == false) && 
	(d_boflg == false) && (d_poflg == false) && (d_pgflg == false)) {
      Record rcd ("EMU");
      rcd.setbndl (bnd);
      p_tracer->add (rcd);
    }
    // get instructions
    Instr i0 = bnd.getinstr (0);
    Instr i1 = bnd.getinstr (1);
    Instr i2 = bnd.getinstr (2);
    // execute instruction slot 0
    try {
      // execute instruction
      if (exec (i0) == true) return;
    } catch (Interrupt& vi) {
      vi.setinst (i0);
      throw vi;
    }
    // execute instruction slot 1
    try {
      // execute instruction
      if (exec (i1) == true) return;
    } catch (Interrupt& vi) {
      vi.setinst (i1);
      throw vi;
    }
    // execute instruction slot 2
    try {
      // execute instruction
      if (exec (i2) == true) return;
    } catch (Interrupt& vi) {
      vi.setinst (i2);
      throw vi;
    }
    // update the ip
    t_octa ip = bnd.getbip () + BN_BYSZ;
    p_rbk->write (IPRG, 0, ip);
  }

  // execute an instruction

  bool Emulator::exec (Instr& inst) {
    // check for valid instruction
    if (inst.isvalid () == false) return false;
    // update the instruction speculative ip
    if (p_bprd && (d_dflag == true) && (inst.isbr () == true)) {
      t_octa iip  = inst.getiip  ();
      // ignore non predicted branch
      if (p_bprd->ispredict (iip) == true) {
	t_octa sip = p_bprd->predict (iip);
	inst.setsip (sip);
      }
    }
    // try to execute the instruction
    bool pred = true;
    try {
      // preset the rse with the instruction
      p_rse->preset (inst);
      // rename the instruction with the rse
      p_rse->rename (inst);
      // after set the rse with the instruction
      p_rse->aftset (inst);
      // evaluate the predicate
      Rid prid = inst.getpnum   ();
      pred     = p_rbk->getbval (prid);
      // evaluate the operand
      Operand oprd = inst.getoper ();
      p_rbk->eval (oprd);
      // execute with unit
      Result result;
      switch (inst.getsunit ()) {
      case MUNIT:
	result = p_munit->exec (inst, oprd);
	break;
      case IUNIT:
	result = p_iunit->exec (inst, oprd);
	break;
      case FUNIT:
	result = p_funit->exec (inst, oprd);
	break;
      case BUNIT:
	result = p_bunit->exec (inst, oprd);
	break;
      default:
	assert (false);
	break;
      }
      // eventually use the tracer to store instruction
      if (p_tracer) {
	if ((d_boflg ==  true) || (d_poflg ==  true) || (d_pgflg == true)) {
	  if ((d_boflg == true) && (inst.isbr () == true)) {
	    if ((d_bflag == true) && (result.isreg (IPRG) == true)) {
	      t_octa rip = result.getrip ();
	      Record rcd ("EBO", inst, !pred, rip);
	      p_tracer->add (rcd);
	    } else {
	      Record rcd ("EBO", inst, pred);
	      p_tracer->add (rcd);
	    }
	  }
	  if ((d_poflg == true) && (inst.ispred () == true)) {
	    if ((d_bflag == true) && (result.isreg (IPRG) == true)) {
	      t_octa rip = result.getrip ();
	      Record rcd ("EPO", inst, !pred, rip);
	      p_tracer->add (rcd);
	    } else {
	      Record rcd ("EPO", inst, pred);
	      p_tracer->add (rcd);
	    }
	  }
	  if ((d_pgflg == true) && (inst.ispgen () == true)) {
	    Record rcd ("EPG", inst, pred);
	    p_tracer->add (rcd);
	  }
	} else {
	  // mark the branch value if requested
	  if ((d_bflag == true) && (result.isreg (IPRG) == true)) {
	    t_octa rip = result.getrip ();
	    Record rcd ("EMU", inst, !pred, rip);
	    p_tracer->add (rcd);
	  } else {
	    Record rcd ("EMU", inst, pred);
	    p_tracer->add (rcd);
	  }
	}
      }
      // eventually add the instruction in the stat collection
      if (p_stat) p_stat->addinst (inst, !pred);
      // update memory, rse and bank
      if (pred == true) update (result);
      // check for pipeline refill if branch in disperse mode
      if ((pred == true) && (d_dflag == true) && (inst.isbr () == true)) {
	if (p_tracer) p_tracer->print ();
	// check for branch predictor
	if (p_bprd) {
	  // update the branch predictor
	  p_bprd->markbr (inst, result, pred);
	  // update pipe filling
	  t_octa sip = inst.getsip   ();
	  t_octa rip = result.getrip ();
	  if ((sip != rip) || (inst.isbr () == true)) {
	    if (p_stat)   p_stat->marksc      (d_bpfil);
	    if (p_tracer) p_tracer->newtraces (d_bpfil);
	    d_cycle += d_bpfil;
	  }
	} else {
	  if (p_stat)   p_stat->marksc      (d_bpfil);
	  if (p_tracer) p_tracer->newtraces (d_bpfil);
	  d_cycle += d_bpfil;
	}
      } else {
	// check for stop bit in disperse mode, not at slot 2
	// check special case with chk instructions that are not branches
	if ((d_dflag == true) && (result.isreg (IPRG) == false) &&
	    (inst.getstop () == true) && (inst.getslot () != 2)) {
	  // update stat and tracer if needed
	  if (p_stat)   p_stat->marksc     ();
	  if (p_tracer) p_tracer->print    ();
	  if (p_tracer) p_tracer->newtrace ();
	  d_cycle++;
	  return true;
	}
      }
      // check for flushing
      bool status = pred && result.isreg (IPRG);
      // check for ip change
      if ((status == true) && (d_dflag == true)) p_ftch->reset ();
      return status;
    } catch (const Interrupt& vi) {
      // eventually add the instruction in the stat collection
      if (p_stat) p_stat->addinst (inst, !pred);
      // update record tracer
      if (p_tracer) {
	if ((d_poflg == true) && (inst.ispred () == true)) {
	  Record rcd ("EPO", inst, pred);
	  p_tracer->add (rcd);
	} else {
	  Record rcd ("EMU", inst, pred);
	  p_tracer->add (rcd);
	}
      }
      // route interrupt
      if (pred == true) p_irt->route (vi);
      // force pipe refill in disperse mode
      if (d_dflag == true) {
	if (p_stat)   p_stat->marksc      (d_bpfil+1);
	if (p_tracer) p_tracer->print     ();
	if (p_tracer) p_tracer->newtraces (d_bpfil+1);
	d_cycle += d_bpfil + 1;
	p_ftch->refill (inst.getiip (), inst.getslot ());
	return true;
      }
    }
    // continue execution
    return false;
  }

  // write memory/register resources and update alat/rse

  void Emulator::update (Result& resl) {
    assert (resl.isvalid () == true);
    // check for memory update
    p_mmap->update (resl);
    // check for rse update
    p_rse->update (resl);
    // check for register update
    p_rbk->write (resl);
  }
}
