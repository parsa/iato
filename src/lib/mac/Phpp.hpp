// ---------------------------------------------------------------------------
// - Phpp.hpp                                                                -
// - iato:mac library - PHPP predicate predictor class definition            -
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

#ifndef  IATO_PHPP_HPP
#define  IATO_PHPP_HPP

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

  /// The Phpp class is a per predicate history percetron based with predicate
  /// access, predicate predictor. The predictor operates with a predicate
  /// history and an array of perceptrons. The perceptron is access by
  /// using the predicate index.

  class Phpp : public Predicate {
  private:
    /// the predictor size
    long d_size;
    /// the predictor htr
    Htr* p_htr;
    /// the perceptron array
    Perceptron* p_ppt;

  public:
    /// create a default predictor
    Phpp (void);

    /// create a new phpp predictor by context
    /// @param mtx  the architectural context
    Phpp (Mtx* mtx);

    /// create a new phpp predictor by context and name
    /// @param mtx  the architectural context
    /// @param name the branch resource name
    Phpp (Mtx* mtx, const string& name);

    /// destroy this predictor
    ~Phpp (void);

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
    /// @param pval the pred
    /// @param bflg the branch flagicate value
    void update (const t_octa ip, const long slot, const long pred, 
		 const bool pval, const bool bflg);

  private:
    // make the copy constructor private
    Phpp (const Phpp&);
    // make the assignment oerator private
    Phpp& operator = (const Phpp&);
  };
}

#endif
