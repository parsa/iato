// ---------------------------------------------------------------------------
// - Option.hpp                                                              -
// - iato:iata client - option class definition                              -
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

#ifndef  IATO_RECORD_HPP
#include "Record.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Options class is a simple class that handle the "iata" program
  /// options. The basic options are the program name to query the trace,
  /// query statistics and various other things.

  class Option {
  private:
    /// context parameters
    vector<string> d_cprm;
    /// the trace name
    string d_name;
    /// the record type to trace
    string d_rcty;
    /// the record source to trace
    string d_rsrc;
    /// the record source and type to trace
    string d_rpair;
    /// the instruction groups to trace
    string d_group;
    /// the dump flag
    bool   d_dflg;
    /// the write header flag
    bool   d_hflg;
    /// the stat summary flag
    bool   d_sflg;
    /// the bundle distribution flag
    bool   d_bflg;
    /// the instruction distribution flag
    bool   d_iflg;
    /// the nop distribution flag
    bool   d_nflg;
    /// the number of empty traces to ignore
    long   d_neti;
    /// the generate trace flag
    bool   d_gflg;
    /// the start point for statistic computation
    t_octa d_tbgn;
    /// the stop point for statistic computation
    t_octa d_tend;
    /// the generated trace name
    string d_gname;
    /// the bundle window size
    long d_gwin;
    /// the branch penalty
    long d_gbrp;
    /// the f-unit latencie
    long d_gflat;
    /// the predicate analyze flag
    bool d_pflg;
    /// the predictor name
    string d_pnam;
    /// the make data analyze flag
    bool d_mdat;
    /// use branch instruction only
    bool d_ibro;
    // use non-branch instruction only
    bool d_nbro;

  public:
    /// create a new option from the argument list
    /// @param argc the number of arguments
    /// @param argv the vector of arguments
    Option (int argc, char** argv);

    /// @return the vector of parameters
    const vector<string>& getcprm (void) const {
      return d_cprm;
    }

    /// @return the trace name
    string getname (void) const {
      return d_name;
    }

    /// @return the record type string
    string gettftr (void) const {
      return d_rcty;
    }

    /// @return the record source string
    string getnftr (void) const {
      return d_rsrc;
    }

    /// @return the record source and type string
    string getpair (void) const {
      return d_rpair;
    }

    /// @return the instruction group string
    string getgroup (void) const {
      return d_group;
    }
    
    /// @return the dump flag
    bool getdflg (void) const {
      return d_dflg;
    }

    /// @return the write header flag
    bool gethflg (void) const {
      return d_hflg;
    }

    /// @return the stat flag
    bool getsflg (void) const {
      return d_sflg;
    }

    /// @return the bundle flag
    bool getbflg (void) const {
      return d_bflg;
    }

    /// @return the instruction flag
    bool getiflg (void) const {
      return d_iflg;
    }

    /// @return the nop flag
    bool getnflg (void) const {
      return d_nflg;
    }

    /// @return the number of ignore empty traces
    long getneti (void) const {
      return d_neti;
    }

    /// @return the generate flag
    bool getgflg (void) const {
      return d_gflg;
    }

    /// @return the trace begin number
    t_octa gettbgn (void) const {
      return d_tbgn;
    }

    /// @return the trace end number
    t_octa gettend (void) const {
      return d_tend;
    }

    /// @return the generated trace name
    string getgname (void) const {
      return d_gname;
    }

    /// @return the bundle window size
    long getwin (void) const {
      return d_gwin;
    }

    /// @return the branch penalty
    long getbrp (void) const {
      return d_gbrp;
    }
    
    /// @return the F unit latency
    long getflat (void) const {
      return d_gflat;
    }

    /// @return the predicate analyze flag
    long getpflg (void) const {
      return d_pflg;
    }

    /// @return the predictor name
    string getpnam (void) const {
      return d_pnam;
    }

    /// @return the make data flag
    bool getmdat (void) const {
      return d_mdat;
    }

    /// @return the use branch instruction flag
    bool getibro (void) const {
      return d_ibro;
    }

    /// @return the use non-branch instruction flag
    bool getnbro (void) const {
      return d_nbro;
    }

  private:
    // make the copy constructor private
    Option (const Option&);
    // make the assignment operator private
    Option& operator = (const Option&);
  };
}

#endif

