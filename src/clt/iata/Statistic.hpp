// ---------------------------------------------------------------------------
// - Statistic.hpp                                                           -
// - iato:iata client - trace statistic class definition                     -
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

#ifndef  IATO_STATISTIC_HPP
#define  IATO_STATISTIC_HPP

#ifndef  IATO_ATX_HPP
#include "Atx.hpp"
#endif

#ifndef  IATO_STAT_HPP
#include "Stat.hpp"
#endif

#ifndef  IATO_TRACER_HPP
#include "Tracer.hpp"
#endif

#ifndef  IATO_FILTERING_HPP
#include "Filtering.hpp"
#endif

namespace iato {
  using namespace std;
  
  /// The Statistic class is a class that handle the statistic computation.
  /// It operates with a file name, a type, an operation and a trace reader
  /// option

  class Statistic : public Stat {
  private:
    /// the trace reader
    Tracer::Reader* p_reader;
    /// the trace filter
    Filtering*      p_filter;
    /// the trace begin number
    t_octa d_tbgn;
    /// the trace end number
    t_octa d_tend;
    /// min number of empty trace to ignore
    t_long d_neti;
    /// current number of empty traces
    t_long d_etrc;

  public:
    /// create a new statistic object by name
    /// @param name the trace file name
    /// @param atx  the analysis context
    Statistic (const string& name, Atx* atx);

    /// destroy this statistic object
    ~Statistic (void);

    /// reset this statistic
    void reset (void);

    /// set the number of empty traces to ignore
    void setneti (const t_long neti);

    /// set the trace begin number
    void settbgn (const t_long value);

    /// set the trace end number
    void settend (const t_long value);

    /// update statistic with a record
    /// @param rcd the record to add
    void addrcd (const Record& rcd);
   
    /// update statistic with a vector of record
    /// @param vrcd the record vector to add
    void addvrcd(const Tracer::t_vrcd& vrcd);
    
    /// update the bundle statistic
    /// @param rcd the record use to update stat
    void updbstat (const Record& rcd);

    /// update the instruction statistic
    /// @param rcd the record use to update stat
    void updistat (const Record& rcd);

    /// compute statistics according to filter
    void compute (void);

  private:
    // make the copy constructor private
    Statistic (const Statistic&);
    // make the assignment operator private
    Statistic& operator = (const Statistic&);
  };
}

#endif
