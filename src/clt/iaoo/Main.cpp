// ---------------------------------------------------------------------------
// - Main.cpp                                                                -
// - iato:iaoo client - main program implementation                          -
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

#include "Option.hpp"
#include "Simulator.hpp"
#include "Exception.hpp"

int main (int argc, char** argv) {
  using namespace iato;

  // main try/catch block
  try {
    // process the arguments
    Option opt (argc, argv);

    // get the option values
    bool   cdump = opt.getcdump ();
    bool   tflag = opt.gettflag ();
    bool   tbflg = opt.gettbflg ();
    bool   thflg = opt.getthflg ();
    bool   vflag = opt.getvflag ();
    bool   rflag = opt.getrflag ();
    bool   cflag = opt.getcflag ();
    bool   sflag = opt.getsflag ();
    bool   inops = opt.getinops ();
    long   winsz = opt.getwinsz ();
    long   munit = opt.getmunit ();
    long   iunit = opt.getiunit ();
    long   funit = opt.getfunit ();
    long   bunit = opt.getbunit ();
    long   grnum = opt.getgrnum ();
    long   tthrs = opt.gettthrs ();
    long   sccnt = opt.getsccnt ();
    string pname = opt.getpname ();
    string tname = opt.gettname ();
    string trcty = opt.gettrcty ();
    string trsrc = opt.gettrsrc ();
    string abprd = opt.getabprd ();
    string apprd = opt.getapprd ();
    t_long maxcc = opt.getmaxcc ();
    t_long btrcc = opt.getbtrcc ();
    t_long etrcc = opt.getetrcc ();
    
    // create a new context
    Stx* stx = new Stx;
    stx->setbool  ("TRACER-FLAG",              tflag);
    stx->setbool  ("TRACER-BRANCH-FLAG",       tbflg);
    stx->setbool  ("TRACER-RESCHEDULE-FLAG",   thflg);
    stx->setbool  ("TRACER-VERBOSE-FLAG",      vflag);
    stx->setstr   ("TRACER-FILE-NAME",         tname);  
    stx->setstr   ("TRACER-RECORD-TYPE",       trcty);
    stx->setstr   ("TRACER-RECORD-NAME",       trsrc);
    stx->setstr   ("TRACER-PROGRAM-NAME",      pname);
    stx->setlong  ("TRACER-THRESHOLD",         tthrs);
    stx->setllong ("TRACER-BEGIN-INDEX",       btrcc);
    stx->setllong ("TRACER-END-INDEX",         etrcc);
    stx->setbool  ("CHECKER-FLAG",             cflag);
    stx->setbool  ("STAT-FLAG",                sflag);
    stx->setlong  ("STAT-CYCLE-COUNT",         sccnt);
    stx->setbool  ("IGNORE-NOP",               inops);
    stx->setlong  ("MAXIMUM-CYCLE-COUNT",      maxcc);
    stx->setlong  ("ISSUE-WIDTH",              winsz);
    stx->setlong  ("NUMBER-M-UNITS",           munit);
    stx->setlong  ("NUMBER-I-UNITS",           iunit);
    stx->setlong  ("NUMBER-F-UNITS",           funit);
    stx->setlong  ("NUMBER-B-UNITS",           bunit);
    stx->setlong  ("LR-GR-SIZE",               grnum);
    stx->setstr   ("BRANCH-PREDICTOR-TYPE",    abprd);
    stx->setstr   ("PREDICATE-PREDICTOR-TYPE", apprd);
    
    // update the context with a vector of parameters
    stx->parse (opt.getcprm ());
    // check mif we dump the context
    if (cdump == true) {
      stx->dump ();
      delete stx;
      return 0;
    }
    
    // create a new simulator by name and arguments
    Simulator* sim = new Simulator (stx, pname, opt.getargv ());
    // bind and reset the simulator
    sim->bind  ();
    sim->reset ();
    // run the simulator
    int status = sim->run ();
    // check for report
    if (rflag == true) sim->report ();
    // eventually do stats
    if (sflag == true) sim->pstat  ();
    delete sim;
    delete stx;
    return status;
  } catch (const Exception& e) {
    e.abort ();
  } catch (...) {
    cerr << "fatal: unknown exception caught, aborting" << endl;
    exit (1);
  }
}
