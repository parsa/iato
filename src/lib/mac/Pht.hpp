// ---------------------------------------------------------------------------
// - Pht.hpp                                                                 -
// - iato:mac library - pattern history table class definition               -
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

#ifndef  IATO_PHT_HPP
#define  IATO_PHT_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_SCT_HPP
#include "Sct.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

namespace iato {

  /// The Pht class is a pattern history table class. The class is defined
  /// as an array of 2 bits saturating counters (Sct). Given an index
  /// the pht can be updated or queried to get a prediction status.

  class Pht : public Resource {
  private:
    /// the pht size
    long d_size;
    /// the sct array
    Sct* p_vsct;

  public:
    /// create a default pht
    Pht (void);

    /// create a pht with a size
    /// @param size the pht size
    Pht (const long size);

    /// create a pht with a context
    /// @param mtx the architectural context
    Pht (Mtx* mtx);

    /// destroy this pht
    ~Pht (void);

    /// reset this pht
    void reset (void);

    /// @return the pht size
    long getsize (void) const;

    /// @return a hashed index by address and slot
    virtual long hash (const t_octa ip, const long slot) const;

    /// @return true if the prediction is strong by index
    virtual bool isstrong (const long index) const;

    /// @return true if the prediction is strong by address and slot
    virtual bool isstrong (const t_octa ip, const long slot) const;

    /// @return true if the prediction is weak by index
    virtual bool isweak (const long index) const;

    /// @return true if the prediction is weak by address and slot
    virtual bool isweak (const t_octa ip, const long slot) const;

    /// @return true if the prediction is true by index
    virtual bool istrue (const long index) const;

    /// @return true if the prediction is true by address and slot
    virtual bool istrue (const t_octa ip, const long slot) const;

    /// update the pht by index
    /// @param index the pht index
    /// @param flag the flag used for update
    virtual void update (const long index, const bool flag);

    /// update the pht by address and slot
    /// @param ip   the pht address
    /// @param slot the pht slot
    /// @param flag the flag used for update
    virtual void update (const t_octa ip, const long slot, const bool flag);

  private:
    // make the copy constructor private
    Pht (const Pht&);
    // make the assignment operator private
    Pht& operator = (const Pht&);
  };
}

#endif
