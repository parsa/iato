// ---------------------------------------------------------------------------
// - Simulator.cpp                                                           -
// - iato:iaoo client - simulator class implementation                       -
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

#include "KrnExit.hpp"
#include "Simulator.hpp"

namespace iato {

  // create a new simulator with a context and a program name

  Simulator::Simulator (Stx* stx, const string& name) {
    // save the context parameters
    d_name  = name;
    d_tflag = stx->getbool ("TRACER-FLAG");
    d_vflag = stx->getbool ("TRACER-VERBOSE-FLAG");
    d_maxcc = stx->getlong ("MAXIMUM-CYCLE-COUNT");
    d_maxic = stx->getlong ("MAXIMUM-INSTRUCTION-COUNT");
    d_cflag = stx->getbool ("CHECKER-FLAG");
    d_sflag = stx->getbool ("STAT-FLAG");
    d_sccnt = stx->getlong ("STAT-CYCLE-COUNT");
    // the system interface
    p_psys = new System (stx, name);
    // the processor core
    p_proc = new Processor (stx);
    // install a checker if needed
    p_pchk = d_cflag ? p_psys->getchecker () : 0;
    // install a tracer if needed
    p_ptrc = d_tflag ? new Tracer (stx) : 0;
    // install a stat collection if needed
    string sname = stx->getstr  ("STAT-FILE-NAME");
    p_pstc = d_sflag ? new Stat (sname) : 0;
  }

  // create a new simulator with a context, a program name and arguments

  Simulator::Simulator (Stx* stx, const string& name, 
			const vector<string>& argv) {
    // save the context parameters
    d_name  = name;
    d_tflag = stx->getbool ("TRACER-FLAG");
    d_vflag = stx->getbool ("TRACER-VERBOSE-FLAG");
    d_maxcc = stx->getlong ("MAXIMUM-CYCLE-COUNT");
    d_maxic = stx->getlong ("MAXIMUM-INSTRUCTION-COUNT");
    d_cflag = stx->getbool ("CHECKER-FLAG");
    d_sflag = stx->getbool ("STAT-FLAG");
    d_sccnt = stx->getlong ("STAT-CYCLE-COUNT");
    // the system interface
    p_psys = new System (stx, name, argv);
    // the processor core
    p_proc = new Processor (stx);
    // install a tracer if needed
    p_ptrc = d_tflag ? new Tracer (stx) : 0;
    // install a checker if needed
    p_pchk = d_cflag ? p_psys->getchecker () : 0;
    // install a stat collection if needed
    string sname = stx->getstr  ("STAT-FILE-NAME");
    p_pstc = d_sflag ? new Stat (sname) : 0;
  }

  // destroy this simulator

  Simulator::~Simulator (void) {
    delete p_psys;
    delete p_proc;
    delete p_ptrc;
    delete p_pchk;
    delete p_pstc;
  }

  // reset the architecture

  void Simulator::reset (void) {
    d_cycle = 0;
    p_psys->reset ();
    p_proc->reset ();
    if (p_ptrc) p_ptrc->reset ();
    if (p_pstc) p_pstc->reset ();
  }

  // bind the various component

  void Simulator::bind (void) {
    // bind the processor
    p_proc->bind (p_psys);
    // set globally the stat collector
    if (p_pstc) {
      p_psys->setstc (p_pstc);
      p_proc->setstc (p_pstc);
    }
    // set globally the tracer
    if (p_ptrc) {
      p_psys->settrc (p_ptrc);
      p_proc->settrc (p_ptrc);
    }
  }

  // run the simulator

  int Simulator::run (void) {
    int status = 0;
    try {
      loop ();
    } catch (const KrnExit& xit) {
      // eventually print last trace
      if (p_ptrc) p_ptrc->print ();
      // extract status
      status = xit.getstatus ();
    }
    // mark the the stat collection if needed
    if (p_pstc) p_pstc->markes ();
    // now run the checker if requested
    if (p_pchk) {
      status = p_pchk->check (p_proc->getrbk ()) ? 0 : 1;
    }
    // save status
    return status;
  }

  // loop in the simulator

  void Simulator::loop (void) {
    while (p_proc->ishalted () == false) {
      // check for next cycle
      d_cycle++;
      // check for max cycle
      if ((p_pstc) && (p_pstc->ismaxcc (d_maxcc) == true)) break;
      // check for max instrctions
      if ((p_pstc) && (p_pstc->ismaxic (d_maxic) == true)) break;
      // initiate a new stat cycle
      if (p_pstc) p_pstc->marksc ();
      // initiate new tracer cycle
      if (p_ptrc) p_ptrc->newtrace ();
      // execute one cycle - if an interrupt occurs in the system
      // the processor external interrupt is set
      p_proc->run ();
      try {
	p_psys->run ();
      } catch (const Interrupt& vi) {
	p_proc->extint (vi);
      }
      // eventually print tracer
      if (p_ptrc) p_ptrc->print ();
      // eventually print some stat
      if ((d_sccnt != 0) && (d_cycle > 0) && ((d_cycle % d_sccnt) == 0)) {
	if (p_pstc) p_pstc->dump ();
      }
    }
  }

  // report some simulator information

  void Simulator::report (void) const {
    if (p_psys) p_psys->report ();
    if (p_proc) p_proc->report ();
  }

  // print some simulator stats

  void Simulator::pstat (void) {
    if (!p_pstc) return;
    cout << endl;
    cout << "program name                   : " << d_name << endl;
    p_pstc->print ();
  }

  // execute a complete simulation

  void Simulator::exec (void) {
    bind  ();
    reset ();
    run   ();
  }
}
