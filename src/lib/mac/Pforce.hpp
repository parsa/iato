// ---------------------------------------------------------------------------
// - Pforce.hpp                                                              -
// - iato:mac library - forced predicate prediction class definition         -
// ---------------------------------------------------------------------------
// - (c) inria 2002-2004                                                     -
// ---------------------------------------------------------------------------
// - authors                                      Amaury Darsch    2002:2004 -
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

#ifndef  IATO_PFORCE_HPP
#define  IATO_PFORCE_HPP

#ifndef  IATO_PREDICATE_HPP
#include "Predicate.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Pforce class is a simple predicate prediction system. The predicate
  /// is always predicted to a boolean value. The update is ignored.

  class Pforce : public Predicate {
  private:
    /// the forcing value
    bool d_fval;

  public:
    /// create a default pforce predictor
    Pforce (void);

    /// create a new pforce predictor by context
    /// @param mtx  the architectural context
    Pforce (Mtx* mtx);

    /// create a new pforce predictor by context and name
    /// @param mtx  the architectural context
    /// @param name the branch resource name
    Pforce (Mtx* mtx, const string& name);

    /// report some resource information
    void report (void) const;

    /// @return true if the predicate can be predicted
    bool isvalid (const t_octa ip, const long slot, const long pred) const;

    /// compute the predicate value
    /// @param ip   the instruction ip
    /// @param slot the instruction slot
    /// @param pred the predicate number
    bool compute (const t_octa ip, const long slot, const long pred) const;

  private:
    // make the copy constructor private
    Pforce (const Pforce&);
    // make the assignment oerator private
    Pforce& operator = (const Pforce&);
  };
}

#endif
