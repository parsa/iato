// ---------------------------------------------------------------------------
// - Option.hpp                                                              -
// - iato:iaoo client - option class definition                              -
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

#ifndef  IATO_STX_HPP
#include "Stx.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Options class is a simple class that handle the "iato" program
  /// options. Most of the options are later transfered to the program context
  /// class that is used during the construction of the simualtion engine.

  class Option {
  private:
    /// the program name to execute
    string d_name;
    /// program arguments
    vector<string> d_argv;
    /// context parameters
    vector<string> d_cprm;
    /// the dump context flag
    bool d_cdump;
    /// the tracer flag
    bool d_tflag;
    /// the tracer branch flag
    bool d_tbflg;
    /// the tracer reschedule flag
    bool d_thflg;
    /// the tracer verbose flag
    bool d_vflag;
    /// the record type to trace
    string d_trcty;
    /// the record source name
    string d_trsrc;
    /// the tracer file name
    string d_tname;
    /// the tracer threshold
    long   d_tthrs;
    /// the checker mode flag
    bool d_cflag;
    /// the stat mode flag
    bool d_sflag;
    /// maximum number of cycles
    t_long d_maxcc;
    /// the begin tracing cycle count
    t_long d_btrcc;
    /// the end tracing cycle count
    t_long d_etrcc;
    /// the report flag
    bool d_rflag;
    /// the ignore nop flag
    bool d_inops;
    /// issue width
    long d_winsz;
    /// number of M units
    long d_munit;
    /// number of I units
    long d_iunit;
    /// number of F units
    long d_funit;
    /// number of B units
    long d_bunit;
    /// number of gr
    long d_grnum;
    /// the branch predictor
    string d_bprd;
    /// the predicate predictor
    string d_pprd;
    /// enable partial flushing
    bool   d_pfls;

  public:
    /// create a new option from the argument list
    Option (int argc, char** argv);
    
    /// @return the program name
    string getpname (void) const {
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
    bool getcdump (void) const {
      return d_cdump;
    }

    /// @return the tracer flag
    bool gettflag (void) const {
      return d_tflag;
    }

    /// @return the tracer branch flag
    bool gettbflg (void) const {
      return d_tbflg;
    }

    /// @return the tracer reschedule flag
    bool getthflg (void) const {
      return d_thflg;
    }

    /// @return the verbose flag
    bool getvflag (void) const {
      return d_vflag;
    }

    /// @return the record type string
    string gettrcty (void) const {
      return d_trcty;
    }

    /// @return the record source string
    string gettrsrc (void) const {
      return d_trsrc;
    }

    /// @return the tracer file name
    string gettname (void) const {
      return d_tname;
    }

    /// @return the tracer threshold
    long gettthrs (void) const {
      return d_tthrs;
    }

    /// @return the report flag flag
    bool getrflag (void) const {
      return d_rflag;
    }

    /// @return the number of maximum cycle
    long getmaxcc (void) const {
      return d_maxcc;
    }
 
    /// @return the begin tracing cycle count
    t_long getbtrcc (void) const {
      return d_btrcc;
    }

    /// @return the end tracing cycle count
    t_long getetrcc (void) const {
      return d_etrcc;
    }

    /// @return the checker flag
    bool getcflag (void) const {
      return d_cflag;
    }

    /// @return the stat flag
    bool getsflag (void) const {
      return d_sflag;
    }

    /// @return the ignore nop flag
    bool getinops (void) const {
      return d_inops;
    }

    /// @return the issue width
    long getwinsz (void) const {
      return d_winsz;
    }

    /// @return the number of M units
    long getmunit (void) const {
      return d_munit;
    }
    
    /// @return the number of I units
    long getiunit (void) const {
      return d_iunit;
    }
    
    /// @return the number of F units
    long getfunit (void) const {
      return d_funit;
    }
    
    /// @return the number of B units
    long getbunit (void) const {
      return d_bunit;
    }

    /// @return the number of gr
    long getgrnum (void) const {
      return d_grnum;
    }

    /// @return the branch predictor name
    string getabprd (void) const {
      return d_bprd;
    }

    /// @return the predicate predictor name
    string getapprd (void) const {
      return d_pprd;
    }

    /// @return the partial flushing flag
    bool getapfls (void) const {
      return d_pfls;
    }
  };
}

#endif

