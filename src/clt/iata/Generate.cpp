// ---------------------------------------------------------------------------
// - Generate.cpp                                                            -
// - iato:iata client - generate perfect trace class implementation          -
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

#include "Generate.hpp"

namespace iato {
  using namespace std;

  // create a new Generate objects

  Generate::Generate (const string input, const string output) {
    p_reader = new Tracer::Reader (input);
    p_tracer = new Tracer ();
    p_cvrcd  = 0;
    p_tracer->setname (output);
    p_tracer->addtype (Record::RINSTR);
    d_iwmax = DEF_WINSIZE;
    d_brpty = DEF_BRPTY;
    d_flat  = DEF_FULAT;
    d_maxl  = 1;
  }

  // destroy this Generate object

  Generate::~Generate (void) {
    delete p_reader;
    delete p_tracer;
    delete p_cvrcd;
  }
  
  // set parameter

  void Generate::setparam (const long winsz, const long brpty, 
			   const long flat) {
    if (winsz != 0) d_iwmax = winsz;
    d_brpty = brpty;
    if (flat != 0) d_flat = flat;
  }

  // process the new trace

  void Generate::compute (void) {
    // first fill the pipeline
    for (long i = 0; i < d_brpty; i++)
      p_tracer->add (new Tracer::t_vrcd);
    // let's the ball rolling
    Record* rcd = p_reader->getrcd ();
    while (rcd) {
      push (*rcd);
      delete rcd;
      rcd = p_reader->getrcd ();
    }
  }

  // push a record into the record vector

  void Generate::push (const Record& rcd) {
    // ignore record that is not an instruction or a cancelled instruction
    if (rcd.gettype () != Record::RINSTR) return;
    if (rcd.iscancel () == true) return;
    // allocate a new record vector and push record
    if (!p_cvrcd) p_cvrcd = new Tracer::t_vrcd;
    p_cvrcd->push_back (rcd);
    // set pre-record latency value
    if ((rcd.getinst ()).getsunit () == FUNIT) d_maxl = d_flat;
    // if the instruction is a branch, we have to add some branch penalty
    if ((rcd.getinst ()).isbr () == true) {
      // add pre-record penalty (mostly for F unit instruction)
      for (long i = 0; i < (d_maxl - 1); i++)
	p_tracer->add (new Tracer::t_vrcd);
      // add the record vector
      p_tracer->add (p_cvrcd);
      p_cvrcd = 0;
      d_maxl = 1;
      // add the branch penalty
      for (long i = 0; i < d_brpty; i++)
	p_tracer->add (new Tracer::t_vrcd);
    } else if ((rcd.getinst().getstop () == true) || 
	       (p_cvrcd->size () == (unsigned long) d_iwmax)) {
      // if the instruction has a stop bit or the instruction window
      // is full, then push the record vector with the pre-record penalty
      for (long i = 0; i < (d_maxl - 1); i++)
	p_tracer->add (new Tracer::t_vrcd);
      // add the record vector
      p_tracer->add (p_cvrcd);
      p_cvrcd = 0;
      d_maxl = 1;
    }
  }
}
