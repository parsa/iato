// ---------------------------------------------------------------------------
// - Filter.cpp                                                              -
// - iato:isa library - filter class implementation                          -
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
#include "Filter.hpp"

namespace iato {

  // construct a filter

  Filter::Filter (void) {
    reset ();
  }
  
  // destroy a filter

  Filter::~Filter (void) {
  }
    
  // the assignment operator
  Filter& Filter::operator = (const Filter& that) {
    d_set = that.d_set;
    return *this;
  }
  
  // reset this filter

  void Filter::reset (void) {
    d_set.clear ();
  }
  
  // set the instruction group to trace
  
  void Filter::setig (const string& ig) {
    vector<string> vec = split (ig, " \t,");
    long size = vec.size ();
    for (long j = 0; j < size; j++) {
      if (Instr::isvig (vec[j]) == true) d_set.insert (vec[j]);
    }
  }

  // test a record for selection with an instruction group
  
  bool Filter::check (const Record& rcd) const {
    if (rcd.gettype () != Record::RINSTR) return false;
    if (d_set.size () == 0) return true;
    if (d_set.find (rcd.getinst().getgroup ()) != d_set.end ()) return true;
    return false;
  }

  // filter this record vector
  
  void Filter::filter (Tracer::t_vrcd* dst, const Tracer::t_vrcd* src) const {
    if ((!src) || (!dst)) return;
    dst->clear ();
    long size = src ? src->size () : 0;
    if (size == 0) return;
    for (long i = 0; i < size; i++) {
      Record rcd = (*src)[i];
      if (check (rcd) == true) dst->push_back (rcd);
    }
  }
}
