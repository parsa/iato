// ---------------------------------------------------------------------------
// - Gskew.hpp                                                               -
// - iato:mac library - skewed branch predictor class definition             -
// ---------------------------------------------------------------------------
// - (c) inria 2002-2004                                                     -
// ---------------------------------------------------------------------------
// - authors                                      Andre  Seznec    2003:2004 -
// - adapted for iato                             Amaury Darsch    2002:2004 -
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

#ifndef  IATO_GSKEW_HPP
#define  IATO_GSKEW_HPP

#ifndef  IATO_HTR_HPP
#include "Htr.hpp"
#endif

#ifndef  IATO_BTB_HPP
#include "Btb.hpp"
#endif

#ifndef  IATO_PHT_HPP
#include "Pht.hpp"
#endif

#ifndef  IATO_BRANCH_HPP
#include "Branch.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Gskew class is a global history branch prediction system based on
  /// the original implementation of the skewed branch predictor designed
  /// by Andre Seznec. The predictor is augmented with a branch target buffer
  /// in order to satisfy the branch class interface. The global history
  /// is a speculative history.

  class Gskew : public Branch {
  private:
    /// the predictor history
    Htr* p_htr;
    /// the predictor btb
    Btb* p_btb;
    /// the pht tables
    Pht* p_pht;

  public:
    /// create a default gskew predictor
    Gskew (void);

    /// create a new gskew predictor by context
    /// @param mtx  the architectural context
    Gskew (Mtx* mtx);

    /// create a new gskew predictor by context and name
    /// @param mtx  the architectural context
    /// @param name the branch resource name
    Gskew (Mtx* mtx, const string& name);

    /// destroy this predictor
    ~Gskew (void);

    /// reset this prodictor
    void reset (void);

    /// report some resource information
    void report (void) const;
 
    /// @return true if the branch is predicted taken
    bool istaken (const t_octa cip) const;

    /// @return true if the branch can be predicted
    bool ispredict (const t_octa cip) const;

    /// set the predictor history
    /// @param hist the history to set
    void sethist (const t_octa hist);

    /// @return the predictor history
    t_octa gethist (void) const;

    /// predict the next ip from the current ip
    /// @param cip  the current instruction pointer
    t_octa predict (const t_octa cip);

    /// update the branch prediction with a current and next ip
    /// @param cip  the current ip to update
    /// @param btk  the branch taken flag
    /// @param nip  the next ip to use
    /// @param hst the history to use
    void update (const t_octa cip, const bool btk, const t_octa nip, 
		 const t_octa hst);

  private:
    // make the copy constructor private
    Gskew (const Gskew&);
    // make the assignment oerator private
    Gskew& operator = (const Gskew&);
  };
}

#endif
