// ---------------------------------------------------------------------------
// - Main.cpp                                                                -
// - iato:iata client - main program implementation                          -
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
#include "Tracer.hpp"
#include "Prstat.hpp"
#include "Generate.hpp"
#include "Printing.hpp"
#include "Exception.hpp"
#include "Statistic.hpp"

namespace iato {
  using namespace std;
  
  // this procedure process the trace header option
  static void process_trace_header (const Option& opt) {
    // get the options
    bool hflg = opt.gethflg ();
    // check for header reporting flag
    if (hflg == false) return;
    // create a new reader and reports the header
    Tracer::Reader* p_reader = new Tracer::Reader (opt.getname ());
    p_reader->hdinfo ();
    delete p_reader;
  }

  // this procedure process the print options
  static void process_printing (const Option& opt) {
    // get the options
    string rcty = opt.gettftr  ();
    string rsrc = opt.getnftr  ();
    string pair = opt.getpair  ();
    string grup = opt.getgroup ();
    bool   dflg = opt.getdflg  ();
    // check for the printing flag
    if (dflg == false) return;
    // create context
    Atx* atx = new Atx;
    atx->setstr  ("TRACER-RECORD-TYPE",  rcty);
    atx->setstr  ("TRACER-RECORD-NAME",  rsrc);
    atx->setstr  ("TRACER-RECORD-PAIR",  pair);
    atx->setstr  ("TRACER-RECORD-GROUP", grup);
    // update the context with a vector of parameters
    atx->parse (opt.getcprm ());
    // create printing object
    Printing po (opt.getname (), atx);
    po.print ();
    delete atx;
  }

  // this procedure process the statistic according to options
  static void process_statistic (const Option& opt) {
    // get the trace name
    string name = opt.getname ();
    // get statistic options
    bool   sflg = opt.getsflg  ();
    bool   bflg = opt.getbflg  ();
    bool   iflg = opt.getiflg  ();
    bool   nflg = opt.getnflg  ();
    long   neti = opt.getneti  ();
    t_octa tbgn = opt.gettbgn  ();
    t_octa tend = opt.gettend  ();
    string rcty = opt.gettftr  ();
    string rsrc = opt.getnftr  ();
    string pair = opt.getpair  ();
    string grup = opt.getgroup ();
    // check for the statistic flag
    if (sflg == false) return;
    // create the context
    Atx* atx = new Atx;
    atx->setstr  ("TRACER-RECORD-TYPE",  rcty);
    atx->setstr  ("TRACER-RECORD-NAME",  rsrc);
    atx->setstr  ("TRACER-RECORD-PAIR",  pair);
    atx->setstr  ("TRACER-RECORD-GROUP", grup);
    // update the context with a vector of parameters
    atx->parse (opt.getcprm ());
    // create statistic object
    try {
      Statistic stat (name, atx);
      stat.setflg    (bflg, iflg, nflg);
      stat.setneti   (neti);
      stat.settbgn   (tbgn);
      stat.settend   (tend);
      stat.compute   ();
      stat.print     ();
      delete atx;
    } catch (...) {
      delete atx;
      throw;
    }
  }

  // this procedure process the trace generation
  static void process_generate (const Option& opt) {
    // get the options
    bool   gflg  = opt.getgflg  ();
    string iname = opt.getname  ();
    string oname = opt.getgname ();
    long     win = opt.getwin   ();
    long     brp = opt.getbrp   ();
    long     flt = opt.getflat  ();
    // check for the generate flag
    if (gflg == false) return;
    // create generate object
    Generate gen (iname, oname);
    gen.setparam (win, brp, flt);
    gen.compute  ();
  }

  // this procedure process the predicate analysis
  static void process_prstat (const Option& opt) {
    // get the options
    bool   mdat = opt.getmdat  ();
    bool   pflg = opt.getpflg  ();
    string name = opt.getname  ();
    string pred = opt.getpnam  ();
    string rcty = opt.gettftr  ();
    string rsrc = opt.getnftr  ();
    string pair = opt.getpair  ();
    string grup = opt.getgroup ();
    // check for the predicate flag
    if ((mdat == false) && (pflg == false)) return;
    // create the context
    Atx* atx = new Atx;
    atx->setstr  ("PREDICATE-PREDICTOR-TYPE", pred);
    atx->setstr  ("TRACER-RECORD-TYPE",       rcty);
    atx->setstr  ("TRACER-RECORD-NAME",       rsrc);
    atx->setstr  ("TRACER-RECORD-PAIR",       pair);
    atx->setstr  ("TRACER-RECORD-GROUP",      grup);
    // update the context with a vector of parameters
    atx->parse (opt.getcprm ());
    // create analyzer object
    try {
      Prstat prstat (name, atx);
      if (mdat == true) {
	prstat.mkdata ();
      }
      if (pflg == true) {
	prstat.compute   ();
	prstat.print     ();
      }
      delete atx;
    } catch (...) {
      delete atx;
      throw;
    }
  }


}

// and everything start here

int main (int argc, char** argv) {
  using namespace iato;
  try {
    // process the arguments
    Option opt (argc, argv);
    // process trace header
    process_trace_header (opt);
    // process printing
    process_printing (opt);
    // process statistic
    process_statistic (opt);
    // process generate trace
    process_generate (opt);
    // process predicate analysis
    process_prstat (opt);
  } catch (const Exception& e) {
    e.abort ();
  }
  // done
  return 0;
}
