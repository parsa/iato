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

  // create a default dsi

  Dsi::Dsi (void) {
    reset ();
  }

  // create a dsi from an instruction

  Dsi::Dsi (const Instr& inst) : Ssi (inst) {
    d_sidx = -1;
    d_igcs = -1;
    d_elat = 0;
    d_rsch = false;
    d_pnrd = false;
  }

  // copy construct this dsi

  Dsi::Dsi (const Dsi& that) : Ssi (that) {
    d_sidx = that.d_sidx;
    d_igcs = that.d_igcs;
    d_elat = that.d_elat;
    d_rsch = that.d_rsch;
    d_pnrd = that.d_pnrd;
  }

  // assign a dsi to this one

  Dsi& Dsi::operator = (const Dsi& that) {
    this->Ssi::operator = (that);
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
    d_sidx = -1;
    d_igcs = -1;
    d_elat = 0;
    d_rsch = false;
    d_pnrd = false;
  }

  // return true if this instruction requires pre-serialization
  
  bool Dsi::ispresr (void) const {
    for (long i = 0; i < IA_MSRC; i++) {
      if (d_rsrc[i].isvalid () == false) continue;
      if (d_rsrc[i].gettype () == PRRG) return true;
      if (d_rsrc[i].gettype () == PROT) return true;
      if (d_rsrc[i].gettype () == UMRG) return true;
      if (d_rsrc[i].gettype () == PSRG) return true;
    }
    return false;
  }

  // return true if this instruction requires post-serialization
  
  bool Dsi::ispostsr (void) const {
    for (long i = 0; i < IA_MDST; i++) {
      if (d_rdst[i].isvalid () == false) continue;
      if (d_rdst[i].gettype () == PRRG) return true;
      if (d_rdst[i].gettype () == PROT) return true;
      if (d_rdst[i].gettype () == UMRG) return true;
      if (d_rdst[i].gettype () == PSRG) return true;
    }
    return false;
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

  void Dsi::setready (const Rid& rid) {
    if (d_valid == false) return;
    // set the predicate
    d_rprd.seterdy (rid);
    // set all source operands
    for (long  i = 0; i < IA_MSRC; i++) d_rsrc[i].seterdy (rid);
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
