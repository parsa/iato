// ---------------------------------------------------------------------------
// - Ghpp.hpp                                                                -
// - iato:mac library - GHPP predicate predictor class definition            -
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

#ifndef  IATO_GHPP_HPP
#define  IATO_GHPP_HPP

#ifndef  IATO_HTR_HPP
#include "Htr.hpp"
#endif

#ifndef  IATO_PREDICATE_HPP
#include "Predicate.hpp"
#endif

#ifndef  IATO_PERCEPTRON_HPP
#include "Perceptron.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Ghpp class is a global history percetron based with predicate
  /// access, predicate predictor. The predictor operates with a global
  /// history and an array of perceptrons. The perceptron is access by
  /// using the predicate index.

  class Ghpp : public Predicate {
  private:
    /// the predictor size
    long d_size;
    /// the predictor htr
    Htr* p_htr;
    /// the perceptron array
    Perceptron* p_ppt;

  public:
    /// create a default predictor
    Ghpp (void);

    /// create a new ghpp predictor by context
    /// @param mtx  the architectural context
    Ghpp (Mtx* mtx);

    /// create a new ghpp predictor by context and name
    /// @param mtx  the architectural context
    /// @param name the branch resource name
    Ghpp (Mtx* mtx, const string& name);

    /// destroy this predictor
    ~Ghpp (void);

    /// reset this prodictor
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
    Ghpp (const Ghpp&);
    // make the assignment oerator private
    Ghpp& operator = (const Ghpp&);
  };
}

#endif
