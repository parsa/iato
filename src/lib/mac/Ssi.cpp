// ---------------------------------------------------------------------------
// - Ssi.cpp                                                                 -
// - iato:mac library - statically scheduled instr class implementation      -
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

#include "Ssi.hpp"

namespace iato {

  // create a default ssi

  Ssi::Ssi (void) {
    reset ();
  }

  // create a ssi from an instruction

  Ssi::Ssi (const Instr& inst) : Instr (inst) {
    d_ridx = -1;
    d_iidx = -1;
    d_cnlf = false;
    d_intr = false;
    d_vspf = true;
    d_ppvl = true;
    d_ppfl = false;
    d_xflg = false;
    d_iste.reset ();
    d_sste.reset ();
  }

  // copy construct this ssi

  Ssi::Ssi (const Ssi& that) : Instr (that) {
    d_ridx = that.d_ridx;
    d_iidx = that.d_iidx;
    d_cnlf = that.d_cnlf;
    d_intr = that.d_intr;
    d_vspf = that.d_vspf;
    d_ppvl = that.d_ppvl;
    d_ppfl = that.d_ppfl;
    d_xflg = that.d_xflg;
    d_iste = that.d_iste;
    d_sste = that.d_sste;
  }

  // assign a ssi to this one

  Ssi& Ssi::operator = (const Ssi& that) {
    this->Instr::operator = (that);
    d_ridx = that.d_ridx;
    d_iidx = that.d_iidx;
    d_cnlf = that.d_cnlf;
    d_intr = that.d_intr;
    d_vspf = that.d_vspf;
    d_ppvl = that.d_ppvl;
    d_ppfl = that.d_ppfl;
    d_xflg = that.d_xflg;
    d_iste = that.d_iste;
    d_sste = that.d_sste;
    return *this;
  }

  // assign an instruction to this ssi

  Ssi& Ssi::operator = (const Instr& inst) {
    this->Instr::operator = (inst);
    d_ridx = -1;
    d_iidx = -1;
    d_cnlf = false;
    d_intr = false;
    d_vspf = true;
    d_ppvl = true;
    d_ppfl = false;
    d_xflg = false;
    d_iste.reset ();
    d_sste.reset ();
    return *this;
  }

  // reset this ssi

  void Ssi::reset (void) {
    Instr::reset ();
    d_ridx = -1;
    d_iidx = -1;
    d_cnlf = false;
    d_intr = false;
    d_vspf = true;
    d_ppvl = true;
    d_ppfl = false;
    d_xflg = false;
    d_iste.reset ();
    d_sste.reset ();
  }

  // return true if this instruction requires pre-serialization
  
  bool Ssi::ispresr (void) const {
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
  
  bool Ssi::ispostsr (void) const {
    for (long i = 0; i < IA_MDST; i++) {
      if (d_rdst[i].isvalid () == false) continue;
      if (d_rdst[i].gettype () == PRRG) return true;
      if (d_rdst[i].gettype () == PROT) return true;
      if (d_rdst[i].gettype () == UMRG) return true;
      if (d_rdst[i].gettype () == PSRG) return true;
    }
    return false;
  }

  // set the reorder index
  
  void Ssi::setrix (const long index) {
    assert (index >= 0);
    d_ridx = index;
  }

  // get the reorder index

  long Ssi::getrix (void) const {
    assert (d_ridx != -1);
    return d_ridx;
  }

  // set the iib index
  
  void Ssi::setiib (const long index) {
    assert (index >= 0);
    d_iidx = index;
  }

  // get the iib index

  long Ssi::getiib (void) const {
    return d_iidx;
  }

  // set the cancellation flag

  void Ssi::setcnlf (const bool cnlf) {
    d_cnlf = d_valid ? cnlf : false;
  }

  // set the cancellation flag by rid

  void Ssi::setcnlf (const Rid& rid, const bool cnlf) {
    if (rid.islrnm () == false) return;
    if (d_rprd == rid) setcnlf (cnlf);
  }

  // return the cancellation flag

  bool Ssi::getcnlf (void) const {
    return isvalid () ? d_cnlf : false;
  }

  // set the interrupt flag

  void Ssi::setintr (const bool intr) {
    d_intr = d_valid ? intr : false;
  }

  // return the interrupt flag

  bool Ssi::getintr (void) const {
    return d_valid ? d_intr : false;
  }

  // set the valid speculation flag

  void Ssi::setvspf (const bool vspf) {
    d_vspf = d_valid && d_sfl ? vspf : true;
  }

  // return the valid speculation flag

  bool Ssi::getvspf (void) const {
    return d_valid && d_sfl ? d_vspf : true;
  }

  // set the predicted predicate value

  void Ssi::setppvl (const bool ppvl) {
    if (d_valid == true) {
      d_ppvl = ppvl;
      d_ppfl = true;
    } else {
      d_ppvl = true;
      d_ppfl = false;
    }
  }

  // return the predicted predicate value

  bool Ssi::getppvl (void) const {
    return d_valid ? d_ppvl : true;
  }

  // set the predicate prediction flag

  void Ssi::setppfl (const bool ppfl) {
    d_ppfl = d_valid ? ppfl : false;
  }

  // return the predicate prediction flag

  bool Ssi::getppfl (void) const {
    return d_valid ? d_ppfl : false;
  }

  // set the stat extra flag

  void Ssi::setxflg (const bool xflg) {
    d_xflg = d_valid ? xflg : false;
  }

  // return the stat extra flag

  bool Ssi::getxflg (void) const {
    return d_valid ? d_xflg : false;
  }

  // set the instruction rse state

  void Ssi::setiste (const Rse::State& state) {
    d_iste = state;
  }

  // return the instruction rse state

  Rse::State Ssi::getiste (void) const {
    return d_iste;
  }

  // return the instruction cfm

  Cfm Ssi::geticfm (void) const {
    return d_iste.getcfm ();
  }

  // set the speculative rse state

  void Ssi::setsste (const Rse::State& state) {
    d_sste = state;
  }

  // return the speculative rse state

  Rse::State Ssi::getsste (void) const {
    return d_sste;
  }

  // return the speculative cfm

  Cfm Ssi::getscfm (void) const {
    return d_sste.getcfm ();
  }
}
