// ---------------------------------------------------------------------------
// - Filter.hpp                                                              -
// - iato:isa library - filter class definition                              -
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

#ifndef  IATO_FILTER_HPP
#define  IATO_FILTER_HPP

#ifndef  IATO_TRACER_HPP
#include "Tracer.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Filter class is a base class for record filtering. The base filter
  /// is an instruction group filter that can be used for statistical
  /// computation. At this level, the desired instruction group are added
  /// and a test method is provided to select a particular record as an
  /// active one or not.
  
  class Filter {
  private:
    /// the string comparator for the stl set
    struct s_comp {
      bool operator () (const string& s1, const string& s2) const {
	return (s1.compare (s2) < 0);
      }
    };
    /// the instruction group set
    set<string, s_comp> d_set;

  public:
    /// constructor
    Filter (void);

    /// destroy a filter
    virtual ~Filter (void);

    /// assign a filter to this one
    /// @param that the filter to assign
    Filter& operator = (const Filter& that);

    /// reset this filter
    virtual void reset (void);

    /// set the instruction group to trace
    /// @param ig the instruction groups to filter
    virtual void setig (const string& ig);

    /// check a record for selection
    /// @param rcd the record to test
    virtual bool check (const Record& rcd) const;

    /// filter a record vector
    /// @param dst the destination record vector
    /// @param src the source record vector
    virtual void filter (Tracer::t_vrcd* dst, const Tracer::t_vrcd* src) const;

  private:
    // make the copy constructor private
    Filter (const Filter&);
  };
}

#endif
