// ---------------------------------------------------------------------------
// - Dsi.cpp                                                                 -
// - iato:mac library - dynamic scheduled instruction class implementation   -
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

#include "Dsi.hpp"

namespace iato {

  // this procedure returns true if the rid must be physically renamed

  bool Dsi::isprnm (const Rid& rid) {
    // if the rid is not valid, it cannot be renamed
    if (rid.isvalid () == false) return false;
    // check for logical renaming
    if (rid.islrnm () == false) return false;
    // check special registers
    if (rid.gettype () == IPRG) return false;
    if (rid.gettype () == CFMR) return false;
    if (rid.gettype () == PRRG) return false;
    if (rid.gettype () == PROT) return false;
    if (rid.gettype () == UMRG) return false;
    if (rid.gettype () == PSRG) return false;
    // all others can be renamed
    return true;
  }

  // create a default dsi

  Dsi::Dsi (void) {
    reset ();
  }

  // create a dsi from an instruction

  Dsi::Dsi (const Instr& inst) : Ssi (inst) {
    d_midx = -1;
    d_sidx = -1;
    d_igcs = -1;
    d_elat = 0;
    d_rsch = false;
    d_pnrd = false;
  }

  // copy construct this dsi

  Dsi::Dsi (const Dsi& that) : Ssi (that) {
    d_midx = that.d_midx;
    d_sidx = that.d_sidx;
    d_igcs = that.d_igcs;
    d_elat = that.d_elat;
    d_rsch = that.d_rsch;
    d_pnrd = that.d_pnrd;
  }

  // assign a dsi to this one

  Dsi& Dsi::operator = (const Dsi& that) {
    this->Ssi::operator = (that);
    d_midx = that.d_midx;
    d_sidx = that.d_sidx;
    d_igcs = that.d_igcs;
    d_elat = that.d_elat;
    d_rsch = that.d_rsch;
    d_pnrd = that.d_pnrd;
    return *this;
  }

  // assign an instruction to this dsi

  Dsi& Dsi::operator = (const Instr& inst) {
    this->Ssi::operator = (inst);
    d_midx = -1;
    d_sidx = -1;
    d_igcs = -1;
    d_elat = 0;
    d_rsch = false;
    d_pnrd = false;
    return *this;
  }

  // reset this dsi

  void Dsi::reset (void) {
    Ssi::reset ();
    d_midx = -1;
    d_sidx = -1;
    d_igcs = -1;
    d_elat = 0;
    d_rsch = false;
    d_pnrd = false;
  }

  // return true if the instruction is ready for selection
  // the predicate is not used here

  bool Dsi::isready (void) const {
    // make sure that the instruction is valid
    assert (d_valid == true);
    // if the instruction has been cancelled, it is ready
    if (d_cnlf == true) return true;
    // if the instruction has been rescheduled, it is ready
    if (d_rsch == true) return true;
    // check all source operands
    for (long i = 0; i < IA_MSRC; i++) {
      if (d_rsrc[i].isready () == false) return false;
    }
    return true;
  }

  // set an instruction operand ready by rid

  void Dsi::setrdy (const Rid& rid) {
    if (d_valid == false) return;
    // set all source operands
    for (long  i = 0; i < IA_MSRC; i++) d_rsrc[i].seterdy (rid);
  }

  // set the mob index
  
  void Dsi::setmob (const long index) {
    assert (index >= 0);
    d_midx = index;
  }

  // get the mob index

  long Dsi::getmob (void) const {
    return d_midx;
  }

  // set the reschedule flag

  void Dsi::setrsch (const bool rsch) {
    d_rsch = d_valid ? rsch : false;
  }

  // return the reschedule flag

  bool Dsi::getrsch (void) const {
    return isvalid () ? d_rsch : false;
  }

  // set the predicate not ready flag

  void Dsi::setpnrd (const bool pnrd) {
    d_pnrd = d_valid ? pnrd : false;
  }

  // return the reschedule flag

  bool Dsi::getpnrd (void) const {
    return isvalid () ? d_pnrd : false;
  }

  // set the station entry
  
  void Dsi::setsid (const long index) {
    assert (index >= 0);
    d_sidx = index;
  }

  // get the station entry

  long Dsi::getsid (void) const {
    assert (d_sidx != -1);
    return d_sidx;
  }

  // set the gcs index
  
  void Dsi::setgcs (const long index) {
    assert (index >= 0);
    d_igcs = index;
  }

  // get the gcs index

  long Dsi::getgcs (void) const {
    assert (d_igcs != -1);
    return d_igcs;
  }

  // set the execution latency

  void Dsi::setelat (const long elat) {
    if (isvalid () == false) return;
    d_elat = elat;
  }

  // return the execution latency

  long Dsi::getelat (void) const {
    return isvalid () ? d_elat : 0;
  }
}
