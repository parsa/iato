// ---------------------------------------------------------------------------
// - Prstat.hpp                                                              -
// - iato:iata client - predicate predictor analyzer class implementation    -
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

#include "Prstat.hpp"

namespace iato {
  using namespace std;

  // this fucntion decides whether or not the instruction is elegible
  // for analysis, based on the use flags
  static bool iscomp (const Instr& inst, const bool ibro, const bool nbro) {
    // check for valid instrcution
    if (inst.isvalid () == false) return false;
    // check for branch
    if (ibro == true) return inst.isbr ();
    // check for non-branch
    if (nbro == true) return !inst.isbr ();
    // valid, after all
    return true;
  }

  // create a new trace predictor analyzer object

  Prstat::Prstat (const string& name, Atx* atx) {
    p_reader = new Tracer::Reader (name);
    p_filter = new Filtering      (atx);
    p_pred   = Predicate::mkpr    (atx);
    d_ibro   = false;
    d_nbro   = false;
    reset ();
  }

  // destroy this analyzer

  Prstat::~Prstat (void) {
    delete p_reader;
    delete p_filter;
    delete p_pred;
  }

  // reset this analyzer

  void Prstat::reset (void) {
    d_nins = 0;
    d_nppr = 0;
    d_npbr = 0;
    d_npnb = 0;
    d_npps = 0;
    d_npbs = 0;
    d_nnbs = 0;
  }

  // set the instruction use flags

  void Prstat::setbro (const bool ibro, const bool nbro) {
    d_ibro   = ibro;
    d_nbro   = nbro;
  }

  // analyze the predictor statistics

  void Prstat::analyze (const Instr& inst, const bool pflg) {
    if ((inst.isvalid () == false) || (inst.ispred () == false)) return;
    d_nins++;
    // check if the instruction can be predicted
    if (p_pred->ispredict (inst) == true) {
      d_nppr++;
      // check for branches or not
      if (inst.isbr () == true) {
	d_npbr++;
      } else {
	d_npnb++;
      }
      // evaluate the predicted predicate
      bool pval = p_pred->predict (inst);
      // update the predictor
      p_pred->markpp (inst, pflg);
      // update the statistics
      if (pval == pflg) {
	d_npps++;
	// check for branches or not
	if (inst.isbr () == true) {
	  d_npbs++;
	} else {
	  d_nnbs++;
	}
      }
    } else {
      // update the predictor
      p_pred->markpp (inst, pflg);
    }	  
  }

  // compute the predictor statistics
  
  void Prstat::compute (void) {
    Tracer::t_vrcd* src = p_reader->trcrd ();
    Tracer::t_vrcd* dst = new Tracer::t_vrcd;
    while (src) {
      p_filter->filter (dst, src);
      long size = dst->size ();
      // iterate in the record vector
      for (long i = 0; i < size; i++) {
	// get the record and check for instruction
	Record rcd = (*dst)[i];
	if (rcd.gettype () != Record::RINSTR) continue;
	// get the instruction and check for predicate
	Instr inst = rcd.getinst ();
	// check for compute
	if (iscomp (inst, d_ibro, d_nbro) == false) continue;
	// get the actual result
	bool pflg = !rcd.iscancel ();
	// anylze the instruction
	analyze (inst, pflg);
      }
      delete src;
      src = p_reader->trcrd ();
    }
    delete dst;
  }

  // report the analyzer results

  void Prstat::print (void) const {
    // report predictor type
    p_pred->report ();
    // report instruction
    cout << "number of predicate instruction: " << d_nins << endl;
    // report predicate prediction
    if ((d_nins != 0) && (d_nppr != 0)) {
      double pppr = 100.0 * (double) d_nppr / (double) d_nins;
      double ppbr = 100.0 * (double) d_npbr / (double) d_nppr;
      double pibr = 100.0 * (double) d_npbr / (double) d_nins;
      double ppnb = 100.0 * (double) d_npnb / (double) d_nppr;
      double pinb = 100.0 * (double) d_npnb / (double) d_nins;
      double ppps = 100.0 * (double) d_npps / (double) d_nppr;
      double pppi = 100.0 * (double) d_npps / (double) d_nins;
      double ppbp = 100.0 * (double) d_npbs / (double) d_npbr;
      double ppbs = 100.0 * (double) d_npbs / (double) d_nppr;
      double ppbi = 100.0 * (double) d_npbs / (double) d_nins;
      double pnbp = 100.0 * (double) d_nnbs / (double) d_npnb;
      double pnbs = 100.0 * (double) d_nnbs / (double) d_nppr;
      double pnbi = 100.0 * (double) d_nnbs / (double) d_nins;
      cout << "number of predicate prediction : " << d_nppr;
      cout << "\t(" << setprecision (3) << pppr << "%)" << endl;
      if (d_npbr > 0) {
	cout << "number of branch prediction    : " << d_npbr;
	cout << "\t(" << setprecision (3) << ppbr << "%)";
	cout << "\t(" << setprecision (3) << pibr << "%)" << endl;
      }
      if (d_npnb > 0) {
	cout << "number of non branch prediction: " << d_npnb;
	cout << "\t(" << setprecision (3) << ppnb << "%)";
	cout << "\t(" << setprecision (3) << pinb << "%)" << endl;
      }
      if (d_npps > 0) {
	cout << "successfull predicates         : " << d_npps;
	cout << "\t(" << setprecision (3) << ppps << "%)";
	cout << "\t(" << setprecision (3) << pppi << "%)" << endl;
      }
      if (d_npbs > 0) {
	cout << "successfull branch predicates  : " << d_npbs;
	cout << "\t(" << setprecision (3) << ppbp << "%)";
	cout << "\t(" << setprecision (3) << ppbs << "%)";
	cout << "\t(" << setprecision (3) << ppbi << "%)" << endl;
      }
      if (d_nnbs > 0) {
	cout << "successfull non branch pred    : " << d_nnbs;
	cout << "\t(" << setprecision (3) << pnbp << "%)";
	cout << "\t(" << setprecision (3) << pnbs << "%)";
	cout << "\t(" << setprecision (3) << pnbi << "%)" << endl;
      }
    }
  }

  // build a table data
  
  void Prstat::mkdata (void) const {
    Tracer::t_vrcd* src = p_reader->trcrd ();
    Tracer::t_vrcd* dst = new Tracer::t_vrcd;
    while (src) {
      p_filter->filter (dst, src);
      long size = dst->size ();
      // iterate in the record vector
      for (long i = 0; i < size; i++) {
	// get the record and check for instruction
	Record rcd = (*dst)[i];
	if (rcd.gettype () != Record::RINSTR) continue;
	// get the instruction and check for predicate
	Instr inst = rcd.getinst ();
	if ((inst.isvalid () == false) || (inst.ispred () == false)) continue;
	// get the instruction info
	t_octa addr = inst.getiip  ();
	long   slot = inst.getslot ();
	Rid    prid = inst.getpnum (); assert (prid.isvalid () == true);
	long   pred = prid.getpnum ();
	bool   brfl = inst.isbr    ();
	// get the actual result
	bool pflg = !rcd.iscancel ();
	// format output result
	cout << "0x" << hex << setw (16) << setfill ('0') << addr << dec;
	cout << ' '  << slot;
	if (brfl == true) cout << " B"; else cout << " O";
	if (pflg == true) cout << " T"; else cout << " F";
	cout << ' ' << pred << endl;
      }
      delete src;
      src = p_reader->trcrd ();
    }
    delete dst;
  } 
}
