// ---------------------------------------------------------------------------
// - Btb.hpp                                                                 -
// - iato:mac library - branch target buffer class definition                -
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

#ifndef  IATO_BTB_HPP
#define  IATO_BTB_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

namespace iato {

  /// The Btb class is a branch target buffer class. The btb is used to
  /// to store for a given address, the associated target address. A given
  /// btb uses a valid bit, the instruction address abd the target address.

  class Btb {
  private:
    /// the btb size
    long d_size;
    /// the btb array
    struct s_btb* p_vbtb;

  public:
    /// create a default btb
    Btb (void);

    /// create a btb with a size
    /// @param size the btb size
    Btb (const long size);

    /// create a btb with a context
    /// @param mtx the architectural context
    Btb (Mtx* mtx);

    /// destroy this btb
    ~Btb (void);

    /// reset this btb
    void reset (void);

    /// @return the btb size
    long getsize (void) const;

    /// @return true if the address is valid
    bool isvalid (const t_octa addr) const;

    /// get the btb target by address
    /// @param addr the address tag
    t_octa gettrg (const t_octa addr) const;

    /// update the btb with an address and a target
    /// @param addr the address used for update
    /// @param targ the target address to store
    void update (const t_octa addr, const t_octa targ);

  private:
    // make the copy constructor private
    Btb (const Btb&);
    // make the assignment operator private
    Btb& operator = (const Btb&);
  };
}

#endif
