// ---------------------------------------------------------------------------
// - Pshare.hpp                                                              -
// - iato:mac library - global history predicate predictor class definition  -
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

#ifndef  IATO_PSHARE_HPP
#define  IATO_PSHARE_HPP

#ifndef  IATO_HTR_HPP
#include "Htr.hpp"
#endif

#ifndef  IATO_PHT_HPP
#include "Pht.hpp"
#endif

#ifndef  IATO_PREDICATE_HPP
#include "Predicate.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Pshare class is a global history predicate prediction system. Given
  /// an address, this address is combined (xor) with the global history
  /// register value (and eventually the predicate number) to produce an 
  /// entry into the pht. The pht value is the predicate prediction value.

  class Pshare : public Predicate {
  private:
    /// the use confidence flag
    bool d_usec;
    /// branch history update only
    bool d_bhuo;
    /// the predictor htr
    Htr* p_htr;
    /// the predictor pht
    Pht* p_pht;

  public:
    /// create a default pshare predictor
    Pshare (void);

    /// create a new pshare predictor by context
    /// @param mtx  the architectural context
    Pshare (Mtx* mtx);

    /// create a new pshare predictor by context and name
    /// @param mtx  the architectural context
    /// @param name the branch resource name
    Pshare (Mtx* mtx, const string& name);

    /// destroy this predictor
    ~Pshare (void);

    /// reset this predictor
    void reset (void);

    /// report some resource information
    void report (void) const;

    /// @return the predictor hash address by address, slot and predicate
    virtual t_octa hash (const t_octa ip, const long slot, 
			 const long pred) const;

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
    Pshare (const Pshare&);
    // make the assignment oerator private
    Pshare& operator = (const Pshare&);
  };
}

#endif
