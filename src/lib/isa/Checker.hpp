// ---------------------------------------------------------------------------
// - Checker.hpp                                                             -
// - iato:isa library - record checker class definition                      -
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

#ifndef  IATO_CHECKER_HPP
#define  IATO_CHECKER_HPP

#ifndef  IATO_CTX_HPP
#include "Ctx.hpp"
#endif

#ifndef  IATO_RECORD_HPP
#include "Record.hpp"
#endif

#ifndef  IATO_REGISTER_HPP
#include "Register.hpp"
#endif

namespace iato {

  /// The Checker class is a simple record checker. The idea is to store
  /// a suite of record (typed register check) and compare them with
  /// a register bank. If a difference is found an error can be reported.
  /// The idea behind the checker is to automate the test suite process
  /// with simple verification.

  class Checker {
  private:
    /// the checker verbose flag
    bool d_vchk;
    /// the record vector
    vector<Record> d_vrcd;
    
  public:
    /// create a default checker
    Checker (void);

    /// create a checker with a context
    /// @param ctx the current context
    Checker (Ctx* ctx);

    /// destroy this checker
    virtual ~Checker (void);

    /// reset this checker
    virtual void reset (void);

    /// add a record to the check vector
    /// @param rcd the record to add
    virtual void add (const Record& rcd);

    /// check all record against the register bank
    virtual bool check (Register* rbk) const;

  private:
    // make the copy constructor private
    Checker (const Checker&);
    // make the assignment operator private
    Checker& operator = (const Checker&);
  };
}

#endif
