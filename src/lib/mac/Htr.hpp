// ---------------------------------------------------------------------------
// - Htr.hpp                                                                 -
// - iato:mac library - history register class definition                    -
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

#ifndef  IATO_HTR_HPP
#define  IATO_HTR_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

namespace iato {

  /// The Htr class is a history class. The class is designed to store the 
  /// a k-bits history that is used later by a predictor to hash the address
  /// (with Ga type branch predictor). Note that the maximum size is 64 bits.
  /// The Htr class is also a good candidate for predicate predictor that is
  /// built around a global history mechanism.

  class Htr {
  private:
    /// the htr size
    long   d_size;
    /// the htr mask
    t_octa d_mask;
    /// the htr value
    t_octa d_hist;

  public:
    /// create a default htr
    Htr (void);

    /// create a htr by size
    /// @param size the htr size
    Htr (const long size);

    /// create a htr by context
    /// @param mtx the architectural context
    Htr (Mtx* mtx);

    /// copy construct this htr
    /// @param that the htr to copy
    Htr (const Htr& that);

    /// reset this htr
    void reset (void);

    /// assign a htr to this one
    /// @param that the htr to assign
    Htr& operator = (const Htr& that);

    /// @return the htr size
    long getsize (void) const;

    /// @return the masked htr value
    t_octa gethist (void) const;

    /// update the history by flag
    /// @param flag the flag used for update
    t_octa update (const bool flag);
  };
}

#endif
