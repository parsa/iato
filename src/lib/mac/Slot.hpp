// ---------------------------------------------------------------------------
// - Slot.hpp                                                                -
// - iato:mac library - instruction slot class definition                    -
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

#ifndef  IATO_SLOT_HPP
#define  IATO_SLOT_HPP

#ifndef  IATO_SSI_HPP
#include "Ssi.hpp"
#endif

namespace iato {

  /// the Slot class is an issue port slot. It is used to store a particular
  /// instruction beeing routed. The slot object holds the slot type as well
  /// as the slot index.

  class Slot {
  private:
    /// the slot type
    t_unit d_unit;
    /// the slot index
    long   d_spos;
    /// the instruction
    Ssi    d_inst;

  public:
    /// create a slot by unit
    /// @param unit the slot unit type
    Slot (t_unit unit);

    /// create a slot by unit and position
    /// @param unit the slot unit type
    /// @param spos the slot position
    Slot (t_unit unit, const long spos);

    /// copy construct this slot
    /// @param that the slot to copy
    Slot (const Slot& that);

    /// reset this slot
    void reset (void);

    /// assign a slot to this one
    /// @param that the slot to assign
    Slot& operator = (const Slot& that);

    /// @return true if the slot is free
    bool isfree (void) const;

    /// @return the slot unit
    t_unit getunit (void) const;

    /// @return the slot position
    long getspos (void) const;

    /// set the slot position
    /// @param spos the slot position to set
    void setspos (const long spos);

    /// @return the slot instruction
    Ssi getinst (void) const;

    /// @return the slot instruction and clear the slot
    Ssi grabinst (void);

    /// set the slot instruction
    /// @param ssi the instruction to set
    void setinst (const Ssi& ssi);
  };
}

#endif
