// ---------------------------------------------------------------------------
// - Statistic.cpp                                                           -
// - iato:iata client - statistic printing class implementation              -
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

#include "Utils.hpp"
#include "Statistic.hpp"

namespace iato {
  using namespace std;
   
  // create a new trace statistic object
  
  Statistic::Statistic (const string& name, Atx* atx) {
    p_reader = new Tracer::Reader (name);
    p_filter = new Filtering (atx);
    reset ();
  }

  // destroy a statistic object

  Statistic::~Statistic (void) {
    delete p_reader;
    delete p_filter;
  }

  // reset this statistic

  void Statistic::reset (void) {
    Stat::reset ();
    d_tbgn = OCTA_0;
    d_tend = OCTA_A;
    d_neti = 0;
    d_etrc = 0;
  }

  // set the trace begin number

  void Statistic::settbgn (const t_long value) {
    d_tbgn = value;
  }
  
  // set the trace end number

  void Statistic::settend (const t_long value) {
    d_tend = value;
  } 

  // set the number of empty traces to ignore
  void Statistic::setneti (const t_long neti) {
    d_neti = neti;
  }
  
  // add a record to this collection

  void Statistic::addrcd (const Record& rcd) {
    // ignore non instruction records
    if (rcd.gettype () != Record::RINSTR) return;
    // process from instruction type
    Instr inst = rcd.getinst ();
    if (inst.isvalid () == false) return;
    // check for cancel instruction. if the instruction is canceled
    // a check is made to determine if the instruction was a branch
    if (rcd.iscancel () == true) {
      d_ncan++; 
      if (inst.isbr () == false) d_nbcn++;
    }
  }

  // add a record vector to this collection.

  void Statistic::addvrcd (const Tracer::t_vrcd& vrcd) {
    long size = vrcd.size ();
    // process empty trace
    if (size == 0) {
      // if ignore is 0, do not ignore empty trace
      if (d_neti == 0) {
	d_ncyc++;
	return;
      }
      // here the ignore flag is not 0, check for current state
      // so add the current emty state
      d_etrc++;
      if (d_etrc == d_neti) d_etrc = 0;
      return;
    }
    // fix the number of traces
    d_ncyc += d_etrc;
    d_etrc    = 0;
    // update number of traces
    d_ncyc++;
    // update with all records
    for (long i = 0; i < size; i++) {
      Record rcd = vrcd[i];
      addrcd   (rcd);
      updbstat (rcd);
      updistat (rcd);
    }
  }
  
  // compute statistics

  void Statistic::compute (void) {    
    t_octa tcnt = 0;
    Tracer::t_vrcd* vrcd  = p_reader->trcrd ();
    Tracer::t_vrcd* fvrcd = new Tracer::t_vrcd;
    while (vrcd) {
      if (tcnt > d_tend) break;
      if (tcnt++ >= d_tbgn) {
	p_filter->filter (fvrcd, vrcd);
	addvrcd (*fvrcd);
      }
      delete vrcd;
      vrcd = p_reader->trcrd ();
    }
    delete fvrcd;
  }
  
  // update bundle stat

  void Statistic::updbstat (const Record& rcd) {
    if (rcd.gettype () == Record::BUNDLE)
      addbndl (rcd.getbnd ());
  }

  // update instruction stat

  void Statistic::updistat (const Record& rcd) {
    if (rcd.gettype () == Record::RINSTR) addinst (rcd.getinst ());
  }
}
