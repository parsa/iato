// ---------------------------------------------------------------------------
// - Filtering.cpp                                                           -
// - iato:iata client - filter class implementation                          -
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
#include "Filtering.hpp"
#include "Exception.hpp"

namespace iato {

  // this procecure check a record source name against the valid name
  static bool is_valid_name (const Record& rcd, const vector<string>& rnam) {
    long size = rnam.size ();
    if (size == 0) return true;
    string rsrc = rcd.getname ();
    for (long i = 0; i < size; i++) {
      if (rnam[i] == rsrc) return true;
    }
    return false;
  }

  // this procedure extract pairs of record source record type
  // pair are separated by "," whereas source and type are separated by ":"
  static void extr_pairs (vector<Record::t_tynm>& dst, const string src) {
    vector<string> pairs = split (src, ",");
    long size = pairs.size();
    for (long i = 0; i < size; i++) {
      vector<string> vec = split (pairs[i], ":");
      if (vec.size () != 2) {
	string msg = "invalid pair of source and type : " + pairs[i];
	throw Exception ("extract error", msg);
      }
      Record::t_tynm pair;
      pair.d_rcdt = Record::totype (vec[1]);
      pair.d_rcdn = vec[0];
      dst.push_back (pair);
    }
  }

  // create a default filter

  Filtering::Filtering (void) {
    reset ();
  }

  // create a filter with a context

  Filtering::Filtering (Atx* atx) {
    reset ();
    // mark the record type that have to be selected
    string rcd = atx->getstr ("TRACER-RECORD-TYPE");
    if (rcd == "") {
      for (long i = 0; i < Record::TYPMAX; i++) d_rctp[i] = true;
    } else {
      for (long i = 0; i < Record::TYPMAX; i++) d_rctp[i] = false;
      vector<string> vec = split (rcd, ",");
      long size = vec.size();
      for (long i = 0; i < size; i++) d_rctp[Record::totype (vec[i])] = true;
    }    
    // initialize record name
    string rnam = atx->getstr ("TRACER-RECORD-NAME");
    d_rnam = split (rnam, ",");
    // initialize record pair that have to be selected
    string strpair = atx->getstr ("TRACER-RECORD-PAIR");
    extr_pairs (d_rpair, strpair);
    // initialize the instruction group to trace
    string strgrup = atx->getstr ("TRACER-RECORD-GROUP");
    setig (strgrup);
  }

  // the assignment operator

  Filtering& Filtering::operator = (const Filtering& that) {
    this->Filter::operator = (that);
    for (long i = 0; i < Record::TYPMAX; i++) d_rctp[i] = that.d_rctp[i];
    d_rnam  = that.d_rnam;
    d_rpair = that.d_rpair;
    return *this;
  }

  // reset this filter

  void Filtering::reset (void) {
    Filter::reset ();
    for (long i = 0; i < Record::TYPMAX; i++) d_rctp[i] = true;
    d_rnam.clear  ();
    d_rpair.clear ();
  }

  // return true if the record have to be selected
  
  bool Filtering::check (const Record& rcd) const {
    if (d_rctp[rcd.gettype ()] == true) return true;
    if (is_valid_name (rcd.getname (), d_rnam) == true)
      return true;
    long size = d_rpair.size ();
    for (long i = 0; i < size; i++)
      if ((d_rpair[i].d_rcdt == rcd.gettype ()) && 
	  (d_rpair[i].d_rcdn == rcd.getname ()))
	return true;
    if (Filter::check (rcd) == true) return true;
    return false;
  }
}
