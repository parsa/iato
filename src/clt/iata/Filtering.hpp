// ---------------------------------------------------------------------------
// - Filtering.hpp                                                           -
// - iato:iata client - filter class definition                              -
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

#ifndef  IATO_FILTERING_HPP
#define  IATO_FILTERING_HPP

#ifndef  IATO_ATX_HPP
#include "Atx.hpp"
#endif

#ifndef  IATO_FILTER_HPP
#include "Filter.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Filtering class is a simple class that selects record.
  /// The record selection is done  according to record filter.
  /// Filtering is done by type or by name or by type and name.
  /// Note that the defaults filter does not filter anything.

  class Filtering : public Filter {
  private:
    /// record type to trace
    bool d_rctp[Record::TYPMAX];
    /// record name to trace
    vector<string> d_rnam;
    /// record to trace by type and name
    vector<Record::t_tynm> d_rpair;

  public:
    /// create a default filter
    Filtering (void);

    /// create a filter with a context
    /// @param atx the analysis context
    Filtering (Atx* atx);
    
    /// assign a filter to this one
    /// @param that the filter to assign
    Filtering& operator = (const Filtering& that);

    /// reset this filter
    void reset (void);

    /// test a record for selection
    /// @param rcd the record to test
    bool check (const Record& rcd) const;

  private:
    // make the copy constructor private
    Filtering (const Filtering&);
  };
}

#endif
