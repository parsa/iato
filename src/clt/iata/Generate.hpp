// ---------------------------------------------------------------------------
// - Generate.hpp                                                            -
// - iato:iata client - generate perfect trace class definition              -
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

#ifndef  IATO_GENERATE_HPP
#define  IATO_GENERATE_HPP

#ifndef  IATO_TRACER_HPP
#include "Tracer.hpp"
#endif



namespace iato {
  using namespace std;

  /// The Generate class is a class that produce the trace of an in
  /// order processor taking into account different latencies, branch
  /// policies and dependencies. The emulator trace is used as an input

  class Generate {
  private:
    /// the tracer reader
    Tracer::Reader* p_reader;
    /// the tracer output
    Tracer*         p_tracer;
    /// the current record vector
    Tracer::t_vrcd* p_cvrcd; 
    /// the bundle window size
    long d_iwmax;
    /// the branch penalty value
    long d_brpty;
    /// the f-unit latency
    long d_flat;
    /// the current max latency value
    long d_maxl;

  public:
    /// create a new Generate object by input and output name;
    /// Other values are set to default; 
    /// @param input the input trace name
    /// @param output the output trace name
    Generate (const string input, const string output);

    /// destroy this Generate object
    ~Generate (void);

    /// set generation parameters
    /// @param winsz the bundle window size
    /// @param brpty the branch penalty value
    /// @param flat the unit latencie value
    void setparam (const long winsz, const long brpty, const long flat);

    /// generate the trace
    void compute (void);

  private:
    // make the copy constructor private
    Generate (const Generate&);
    // make the assignment operator private
    Generate& operator = (const Generate&);
    // push a new record in the trace
    void push (const Record& rcd);
  };

  /// the default window size
  const long DEF_WINSIZE = 6;
  /// the default branch penalty
  const long DEF_BRPTY   = 0;
  /// the default pipeline proof;
  const long DEF_PIPRF   = 10;   
  /// the default f_unit latency
  const long DEF_FULAT   = 5;
}

#endif
