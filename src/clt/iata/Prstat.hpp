// ---------------------------------------------------------------------------
// - Prstat.hpp                                                              -
// - iato:iata client - predicate predictor analyzer class definition        -
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

#ifndef  IATO_PRSTAT_HPP
#define  IATO_PRSTAT_HPP

#ifndef  IATO_ATX_HPP
#include "Atx.hpp"
#endif

#ifndef  IATO_TRACER_HPP
#include "Tracer.hpp"
#endif

#ifndef  IATO_FILTERING_HPP
#include "Filtering.hpp"
#endif

#ifndef  IATO_PREDICATE_HPP
#include "Predicate.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Prstat class is a predicate predictor analyzer. The class operates 
  /// by reading a trace file and simulating a predicate predictor. When all
  /// traces have been read, a predictor statistics is returned. 

  class Prstat {
  private:
    /// the trace reader
    Tracer::Reader* p_reader;
    /// the filter
    Filtering*      p_filter;
    /// the predicate predictor
    Predicate*      p_pred;
    /// use branch instruction only
    bool d_ibro;
    // use non-branch instruction only
    bool d_nbro; 
    /// the number of predicated instructions
    t_long d_nins;
    /// number of predicted predicates
    t_long d_nppr;
    /// number of predicated branches
    t_long d_npbr;
    /// number of predicated non branches
    t_long d_npnb;
    /// number of successfull predicate prediction
    t_long d_npps;
    /// number of succesful branch pp
    t_long d_npbs;
    /// number of succesful non branch pp
    t_long d_nnbs;    

  public:
    /// create a new analyzer by name and context
    /// @param name the trace file name
    /// @param atx  the analysis context
    Prstat (const string& name, Atx* atx);

    /// destroy this analyzer
    virtual ~Prstat (void);
    
    /// reset this analyzer
    virtual void reset (void);

    /// set the instrcution use flags
    /// @param ibro the branch only flag
    /// @param nbro the non-branch only flah
    void setbro (const bool ibro, const bool nbro);

    /// analyze an instruction for statistics
    /// @param inst the instruction to analyze
    /// @param pflg the predicate flag to analze
    virtual void analyze (const Instr& inst, const bool pflg);

    /// compute the predictor statistics
    virtual void compute (void);

    /// print the statistic information
    virtual void print (void) const;

    /// dump the data in the form of table
    virtual void mkdata (void) const;

  private:
    // make the copy constructor private
    Prstat (const Prstat&);
    // make the assignment operator private
    Prstat& operator = (const Prstat&);
  };
}

#endif
