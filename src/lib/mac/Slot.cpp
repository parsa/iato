// ---------------------------------------------------------------------------
// - Slot.cpp                                                                -
// - iato:mac library - instruction slot class implementation                -
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

#include "Slot.hpp"

namespace iato {

  // create a slot by unit

  Slot::Slot (t_unit unit) {
    d_unit = unit;
    d_spos = -1;
    reset ();
  }

  // create a slot by unit and position

  Slot::Slot (t_unit unit, const long spos) {
    d_unit = unit;
    d_spos = spos;
    reset ();
  }

  // copy construct this slot

  Slot::Slot (const Slot& that) {
    d_unit = that.d_unit;
    d_spos = that.d_spos;
    d_inst = that.d_inst;
  }

  // reset this slot
  
  void Slot::reset (void) {
    d_inst.reset ();
  }

  // assign a slot to this one

  Slot& Slot::operator = (const Slot& that) {
    d_unit = that.d_unit;
    d_spos = that.d_spos;
    d_inst = that.d_inst;
    return *this;
  }

  // return true if the slot is free

  bool Slot::isfree (void) const {
    return d_inst.isvalid () ? false : true;
  }

  // return the slot unit

  t_unit Slot::getunit (void) const {
    return d_unit;
  }

  // return the slot position

  long Slot::getspos (void) const {
    return d_spos;
  }

  // set the slot position

  void Slot::setspos (const long spos) {
    d_spos = spos;
  }

  // return the slot instruction

  Ssi Slot::getinst (void) const {
    return d_inst;
  }

  // return the slot instruction and clear the slot

  Ssi Slot::grabinst (void) {
    Ssi ssi = d_inst;
    reset ();
    return ssi;
  }

  // set the slot instruction

  void Slot::setinst (const Ssi& ssi) {
    if (ssi.isvalid () == true) {
      d_inst = ssi;
    } else {
      reset ();
    }
  }
}
