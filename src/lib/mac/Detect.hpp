// ---------------------------------------------------------------------------
// - Detect.hpp                                                              -
// - iato:mac library - speculative detection logic class definition         -
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

#ifndef  IATO_DETECT_HPP
#define  IATO_DETECT_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_SSI_HPP
#include "Dsi.hpp"
#endif

#ifndef  IATO_REGISTER_HPP
#include "Register.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Detect class is a special resource used to detect speculative
  /// correctness. The main operation performed by the detection logic
  /// is twofold. First, for speculative rse operation, the detection
  /// logic make sure that any cfm part of an execution result has been
  /// correctly speculated. Second, the detection logic make sure that
  /// the speculative next ip associated with branch instruction is also
  /// correct. If an error is found, the associated bit are set in the 
  /// instruction record for further recovery. The detection logic is also
  /// involved in validating the predicate prediction. The speculative
  /// predicate value is checked against the real value found in the register
  /// bank. Note that with an out-of-order engine, the detection logic will
  /// operate only if the register bank is in a correct state with respect
  /// to the checked predicate.

  class Detect : public Resource {
  private:
    Register* p_rbk;

  public:
    /// create a default detect logic
    Detect (void);

    /// create a new detection logic by context
    /// @param mtx  the architectural context
    Detect (Mtx* mtx);

    /// create a new detection logic by context and name
    /// @param mtx  the architectural context
    /// @param name the branch resource name
    Detect (Mtx* mtx, const string& name);
 
    /// reset this detection logic
    void reset (void);

    /// report some resource information
    void report (void) const;

    /// @return true if the ip speculation is correct
    bool chksip (const Ssi& ssi, const Result& resl) const;

    /// @return true if the predicate prediction is valid
    bool chkspp (const Ssi& ssi) const;

    /// bind the register bank
    /// @param rbk the reggister bank to bind
    void bind (Register* rbk);

  private:
    // make the copy constructor private
    Detect (const Detect&);
    // make the assignment oerator private
    Detect& operator = (const Detect&);
  };
}

#endif
