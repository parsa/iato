// ---------------------------------------------------------------------------
// - Sct.hpp                                                                 -
// - iato:mac library - saturating 2 bits counter class definition           -
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

#ifndef  IATO_SCT_HPP
#define  IATO_SCT_HPP

#ifndef  IATO_TYPES_HPP
#include "Types.hpp"
#endif

namespace iato {

  /// The Sct class is a two bit saturating counter used within
  /// prediction systems. The 'update' method can be used to adjust the
  /// counter value while the 'istrue' method returns true if the counter
  /// is in a true state.

  class Sct {
  public:
    /// the two bits states
    enum t_sct {
      STRONG_F, // strongly false
      WEAKLY_F, // weakly false
      WEAKLY_T, // weakly true
      STRONG_T  // strongly true
    };
  
  private:
    /// the counter state
    t_sct d_state;
    /// the initial state
    t_sct d_sinit;

  public:
    /// create a default counter
    Sct (void);

    /// create a counter with an initial state
    /// @param state the initial state
    Sct (t_sct state);

    /// copy construct this counter
    /// @param that the counter to copy
    Sct (const Sct& that);

    /// reset this counter
    void reset (void);

    /// assign a counter to this one
    /// @param that the counter to assign
    Sct& operator = (const Sct& that);

    /// @return true if the state is strong
    bool isstrong (void) const;

    /// @return true if the state is weak
    bool isweak (void) const;

    /// @return true if the state is true
    bool istrue (void) const;

    /// update a counter with a flag
    /// @param flag the update flag
    void update (const bool flag
);
  };
}

#endif
