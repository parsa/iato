// ---------------------------------------------------------------------------
// - Hypp.hpp                                                                -
// - iato:mac library - hybrid predicate predictor definition                -
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

#ifndef  IATO_HYPP_HPP
#define  IATO_HYPP_HPP

#ifndef  IATO_PREDICATE_HPP
#include "Predicate.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Hypp class is a hybrid predicate predictor that is built with
  /// two predictors. The confidence is obtained by comparing for differences
  /// the two predictors.

  class Hypp : public Predicate {
  private:
    /// the master predictor
    Predicate* p_mpp;
    /// the slave predictor
    Predicate* p_spp;

  public:
    /// create a default hybrid predictor
    Hypp (void);

    /// create a new hybrid predictor by context
    /// @param mtx  the architectural context
    Hypp (Mtx* mtx);

    /// create a new bybrid predictor by context and name
    /// @param mtx  the architectural context
    /// @param name the branch resource name
    Hypp (Mtx* mtx, const string& name);

    /// destroy this predictor
    ~Hypp (void);

    /// reset this predictor
    void reset (void);

    /// report some resource information
    void report (void) const;

    /// @return true if the predicate can be predicted
    bool isvalid (const t_octa ip, const long slot, const long pred) const;

    /// compute the predicate value
    /// @param ip   the instruction ip
    /// @param slot the instruction slot
    /// @param pred the predicate number
    bool compute (const t_octa ip, const long slot, const long pred) const;

    /// update the predicate prediction by index and value
    /// @param ip   the instruction ip
    /// @param slot the instruction slot
    /// @param pred the predicate index
    /// @param pval the predicate value
    /// @param bflg the branch flag
    void update (const t_octa ip, const long slot, const long pred, 
		 const bool pval, const bool bflg);

  private:
    // make the copy constructor private
    Hypp (const Hypp&);
    // make the assignment oerator private
    Hypp& operator = (const Hypp&);
  };
}

#endif
