// ---------------------------------------------------------------------------
// - Pimodal.hpp                                                             -
// - iato:mac library - simple predicate prediction class definition         -
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

#ifndef  IATO_PIMODAL_HPP
#define  IATO_PIMODAL_HPP

#ifndef  IATO_PHT_HPP
#include "Pht.hpp"
#endif

#ifndef  IATO_PREDICATE_HPP
#include "Predicate.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Pimodal class is a simple predicate prediction system. Given
  /// a predicate number, the pht table is accesed and a prediction is
  /// returned. This system does not use the instruction address or slot.
  /// The term 'pimodal' is cooked from 'bimodal' with the B (branch)
  /// replaced with a P (predicate).

  class Pimodal : public Predicate {
  private:
    /// the predictor size
    long d_size;
    /// the use confidence flag
    bool d_usec;
    /// the pht
    Pht* p_pht;

  public:
    /// create a default pimodal predictor
    Pimodal (void);

    /// create a new pimodal predictor by context
    /// @param mtx  the architectural context
    Pimodal (Mtx* mtx);

    /// create a new pimodal predictor by context and name
    /// @param mtx  the architectural context
    /// @param name the branch resource name
    Pimodal (Mtx* mtx, const string& name);

    /// destroy this predictor
    ~Pimodal (void);

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
    Pimodal (const Pimodal&);
    // make the assignment oerator private
    Pimodal& operator = (const Pimodal&);
  };
}

#endif
