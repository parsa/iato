// ---------------------------------------------------------------------------
// - Main.cpp                                                                -
// - iato:iaka client - main program implementation                          -
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
#include "Emulator.hpp"
#include "Exception.hpp"

int main (int argc, char** argv) {
  using namespace iato;

  try {
    // process the arguments
    Option opt (argc, argv);
    // get the option values
    bool   cdmp = opt.getcdmp ();
    bool   tflg = opt.gettflg ();
    bool   tbfl = opt.gettbfl ();
    bool   tbof = opt.gettbof ();
    bool   tpof = opt.gettpof ();
    bool   tpgf = opt.gettpgf ();
    bool   dflg = opt.getdflg ();
    bool   tvfl = opt.gettvfl ();
    bool   cflg = opt.getcflg ();
    bool   sflg = opt.getsflg ();
    long   mbnd = opt.getmbnd ();
    long   iwsz = opt.getiwsz ();
    long   gnum = opt.getgnum ();
    long   pfil = opt.getpfil ();
    long   tthr = opt.gettthr ();
    t_long btrc = opt.getbtrc ();
    t_long etrc = opt.getetrc ();
    string tnam = opt.gettnam ();
    string rcty = opt.getrcty ();
    string rsrc = opt.getrsrc ();
    string name = opt.getname ();
    string bprd = opt.getbprd ();
    
    // create a new context
    Atx* atx = new Atx;
    atx->setbool  ("TRACER-FLAG",              tflg);
    atx->setbool  ("TRACER-BRANCH-FLAG",       tbfl);
    atx->setbool  ("TRACER-BRANCH-ONLY",       tbof);
    atx->setbool  ("TRACER-PREDICATE-ONLY",    tpof);
    atx->setbool  ("TRACER-PREDICATE-TARGETS", tpgf);
    atx->setbool  ("TRACER-VERBOSE-FLAG",      tvfl);
    atx->setstr   ("TRACER-FILE-NAME",         tnam);
    atx->setstr   ("TRACER-RECORD-TYPE",       rcty);
    atx->setstr   ("TRACER-RECORD-NAME",       rsrc);
    atx->setlong  ("TRACER-THRESHOLD",         tthr);
    atx->setllong ("TRACER-BEGIN-INDEX",       btrc);
    atx->setllong ("TRACER-END-INDEX",         etrc);
    atx->setstr   ("TRACER-PROGRAM-NAME",      name);
    atx->setbool  ("DISPERSE-FLAG",            dflg);
    atx->setbool  ("CHECKER-FLAG",             cflg);
    atx->setbool  ("STAT-FLAG",                sflg);
    atx->setlong  ("MAXIMUM-CYCLE-COUNT",      mbnd);
    atx->setlong  ("LR-GR-SIZE",               gnum);
    
    // set special data for disperse mode
    if (dflg == true) {
      atx->setlong  ("ISSUE-WIDT",             iwsz);
      atx->setlong  ("BRANCH-PIPELINE-REFILL", pfil);
      atx->setlong  ("TRACER-MAX-RECORD",      atx->getswsz ());      
      atx->setstr   ("BRANCH-PREDICTOR-TYPE",  bprd);
    }
    
    // update the context with a vector of parameters
    atx->parse (opt.getcprm ());
    // check mif we dump the context
    if (cdmp == true) {
      atx->dump ();
      delete atx;
      return 0;
    }
    
    // create a new emulator
    Emulator* emu = new Emulator (atx, name, opt.getargv ());
    // reset bind and run the emulator
    emu->reset ();
    emu->bind  ();
    // run and eventually check
    int status = emu->run ();
    // print some emulator stats
    emu->pstat ();
    // clean everything
    delete emu;
    delete atx;
    return status;
  } catch (const Exception& e) {
    e.abort ();
  } catch (...) {
    cerr << "fatal: unknown exception caught, aborting" << endl;
    exit (1);
  }
}
