// ---------------------------------------------------------------------------
// - Tracer.hpp                                                              -
// - iato:isa library - cycle based record tracing class definition          -
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

#ifndef  IATO_TRACER_HPP
#define  IATO_TRACER_HPP

#ifndef  IATO_CTX_HPP
#include "Ctx.hpp"
#endif

#ifndef  IATO_RECORD_HPP
#include "Record.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Tracer class is a cycle based trace recorder. For a given cycle
  /// the tracer accumulates records until a new trace is started. Traces
  /// are accumulated in a vector and can be saved (in a binary form) in a
  /// file. The 'setname' method can be used to set that file name and
  /// open it. When used, the file is opened and a header is written. When
  /// number of traces reaches a certain threshold, the traces are saved in
  /// the file. Note that record are saved according to record type and name.
  /// Note also that traces are saved according to their cycle number.

  class Tracer {
  public:
    /// record vector type
    typedef vector<Record>  t_vrcd;
    /// trace vector type
    typedef vector<t_vrcd*> t_trcd;

  private:
    /// the verbose flag
    bool    d_vflg;
    /// the record vector counter
    t_long  d_tcnt;
    /// the begin trace index
    t_long  d_bicc;
    /// the end trace index
    t_long  d_eicc;
    /// the program traced name
    string  d_ptnm;
    /// the current trace
    t_vrcd* p_ctrc;
    /// the trace vector
    t_trcd* p_vtrc;
    /// the trace vector threshold
    long    d_tthr;
    /// the tracer file name
    string  d_name;
    /// the tracer file descriptor
    int     d_fd;
    /// record type to trace
    bool    d_rctp[Record::TYPMAX];
    /// record name to trace
    vector<string> d_rnam;

  public:
    /// create a default tracer
    Tracer (void);

    /// create a tracer with a context
    /// @param ctx the current context
    Tracer (Ctx* ctx);

    /// create a tracer with a verbose flag
    /// @param vflg the verbose flag
    Tracer (const bool vflg);

    /// destroy this tracer
    virtual ~Tracer (void);

    /// reset this tracer
    virtual void reset (void);

    /// set the tracer file name and open it
    /// @param name the file name to set and open
    virtual void setname (const string& name);

    /// add a record type to trace type vector
    /// @param val the record type to add  
    virtual void addtype (Record::t_rctp val);

    /// start a new trace
    virtual void newtrace (void);

    /// add several empty traces
    virtual void newtraces (const long count);

    /// add a record to the current cycle
    /// @param rcd the record to add
    virtual void add (const Record& rcd);

    /// add a record vector to the tracer
    /// @param vrcd the record vector  to add
    virtual void add (t_vrcd* vrcd);

    /// print the current record vector
    virtual void print (void) const;

  private:
    // make the copy constructor private
    Tracer (const Tracer&);
    // make the assignment operator private
    Tracer& operator = (const Tracer&);
    // write the current trace in a file
    void trcwr (void);
    // add the current trace to the trace vector if needed
    void addtrc (void);
    // check if the cycle count is in the desire range
    bool inrange (void) const;


  public:
    /// the trace reader class
    class Reader {
    private:
      /// the file name
      string            d_name;
      /// the file descriptor
      int               d_fd;
      /// the file header
      struct t_theader* p_hd;
      /// the record queue
      queue<Record>     d_rqueue;

    public:
      /// create a new trace reader by name
      /// @param name the trace file name
      Reader (const string& name);

      /// destroy this trace reader
      ~Reader (void);

      /// @return a new tracer record
      t_vrcd* trcrd (void);

      /// @return a new record
      Record* getrcd (void);

      /// print header information
      void hdinfo (void);

      /// @return the begin trace index
      t_long getbicc (void) const;

      /// @return the end trace index
      t_long geteicc (void) const;

    private:
      // make the copy constructor private
      Reader (const Reader&);
      // make the assignment operator private
      Reader& operator = (const Reader&);
    };
  };
}

#endif
