// ---------------------------------------------------------------------------
// - Option.hpp                                                              -
// - iato:iaka client - option class definition                              -
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

#ifndef  IATO_OPTION_HPP
#define  IATO_OPTION_HPP

#ifndef  IATO_TYPES_HPP
#include "Types.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Options class is a simple class that handle the "iaka" program
  /// options. Most of the options are later transfered to the program context
  /// class that is used to construction the execution engine.

  class Option {
  private:
    /// the program name to execute
    string d_name;
    /// the program arguments
    vector<string> d_argv;
    /// context parameters
    vector<string> d_cprm;
    /// the dump context flag
    bool d_cdmp;
    /// the trace mode flag
    bool d_tflg;
    // the trace branch flag
    bool d_tbfl;
    /// the tracer branch only
    bool d_tbof;
    /// the tracer predicate only
    bool d_tpof;
    /// the tracer predicate target only
    bool d_tpgf;
    /// the tracer verbose flag
    bool d_tvfl;
    /// the disperse flag
    bool d_dflg;
    /// the record type to trace
    string d_rcty;
    /// the record source name
    string d_rsrc;
    /// the tracer file name
    string d_tnam;
    /// the tracer threshold
    long d_tthr;
    /// the begin tracing cycle value
    t_long d_btrc;
    /// the end tracing cycle value
    t_long d_etrc;
    /// the checker mode flag
    bool d_cflg;
    /// the stat mode flag
    bool d_sflg;
    /// maximum number of cycles
    long d_mbnd;
    /// issue width
    long d_iwsz;
    /// number of general register
    long d_gnum;
    /// the branch pipeline refill
    long d_pfil;
    /// the branch predictor
    string d_bprd;

  public:
    /// create a new option from the argument list
    Option (int argc, char** argv);
    
    /// @return the program name
    string getname (void) const {
      return d_name;
    }

    /// @return the vector of arguments
    const vector<string>& getargv (void) const {
      return d_argv;
    }

    /// @return the vector of parameters
    const vector<string>& getcprm (void) const {
      return d_cprm;
    }

    /// @return the dump context flag
    bool getcdmp (void) const {
      return d_cdmp;
    }

    /// @return the tracer flag
    bool gettflg (void) const {
      return d_tflg;
    }

    /// @return the tracer branch flag
    bool gettbfl (void) const {
      return d_tbfl;
    }

    /// @return the tracer branch only
    bool gettbof (void) const {
      return d_tbof;
    }

    /// @return the tracer predicate only
    bool gettpof (void) const {
      return d_tpof;
    }

    /// @return the tracer predicate generate only
    bool gettpgf (void) const {
      return d_tpgf;
    }

    /// @return the disperse flag
    bool getdflg (void) const {
      return d_dflg;
    }

    /// @return the verbose flag
    bool gettvfl (void) const {
      return d_tvfl;
    }

    /// @return the record type string
    string getrcty (void) const {
      return d_rcty;
    }

    /// @return the record source string
    string getrsrc (void) const {
      return d_rsrc;
    }

    /// @return the tracer file name
    string gettnam (void) const {
      return d_tnam;
    }

    /// @return the tracer threshold
    long gettthr (void) const {
      return d_tthr;
    }

    /// @return the begin trace index
    t_long getbtrc (void) const {
      return d_btrc;
    }

    /// @return the end trace index
    t_long getetrc (void) const {
      return d_etrc;
    }

    /// @return the checker flag
    bool getcflg (void) const {
      return d_cflg;
    }
 
    /// @return the stat flag
    bool getsflg (void) const {
      return d_sflg;
    }
 
    /// @return the number of maximum bundle count
    long getmbnd (void) const {
      return d_mbnd;
    }

    /// @return the issue width
    long getiwsz (void) const {
      return d_iwsz;
    }

    /// @return the number of general registers
    long getgnum (void) const {
      return d_gnum;
    }

    /// @return the pipeline refill
    long getpfil (void) const {
      return d_pfil;
    }

    /// @return the branch predictor name
    string getbprd (void) const {
      return d_bprd;
    }

  private:
    // make the copy constructor private
    Option (const Option&);
    // make the assignement operqtor private
    Option& operator =  (const Option&);
  };
}

#endif
