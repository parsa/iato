// ---------------------------------------------------------------------------
// - Bimodal.hpp                                                             -
// - iato:mac library - simple bimodal branch prediction class definition    -
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

#ifndef  IATO_BIMODAL_HPP
#define  IATO_BIMODAL_HPP

#ifndef  IATO_PHT_HPP
#include "Pht.hpp"
#endif

#ifndef  IATO_BTB_HPP
#include "Btb.hpp"
#endif

#ifndef  IATO_BRANCH_HPP
#include "Branch.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Bimodal class is a simple bimodal branch prediction system. Given
  /// an address, a pattern history buffer is consulted to detect whether or
  /// not the branch is taken.

  class Bimodal : public Branch {
  private:
    /// the predictor pht
    Pht* p_pht;
    /// the predictor btb
    Btb* p_btb;

  public:
    /// create a default bimodal predictor
    Bimodal (void);

    /// create a new bimodal predictor by context
    /// @param mtx  the architectural context
    Bimodal (Mtx* mtx);

    /// create a new bimodal predictor by context and name
    /// @param mtx  the architectural context
    /// @param name the branch resource name
    Bimodal (Mtx* mtx, const string& name);

    /// destroy this predictor
    ~Bimodal (void);

    /// reset this prodictor
    void reset (void);

    /// report some resource information
    void report (void) const;
    
    /// @return true if the branch is predicted taken
    bool istaken (const t_octa cip) const;

    /// @return true if the branch can be predicted
    bool ispredict (const t_octa cip) const;

    /// predict the next ip from the current ip
    /// @param cip  the current instruction pointer
    t_octa predict (const t_octa cip);

    /// update the branch prediction with a current and next ip
    /// @param cip the current ip to update
    /// @param btk the branch taken flag
    /// @param nip the next ip to use
    /// @param hst the history to use
    void update (const t_octa cip, const bool btk, const t_octa nip, 
		 const t_octa hst);

  private:
    // make the copy constructor private
    Bimodal (const Bimodal&);
    // make the assignment oerator private
    Bimodal& operator = (const Bimodal&);
  };
}

#endif
