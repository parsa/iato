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
    d_midx = -1;
    d_cnlf = false;
    d_ppfl = false;
    d_icfm.reset ();
    d_scfm.reset ();
  }

  // copy construct this ssi

  Ssi::Ssi (const Ssi& that) : Instr (that) {
    d_ridx = that.d_ridx;
    d_iidx = that.d_iidx;
    d_midx = that.d_midx;
    d_cnlf = that.d_cnlf;
    d_ppfl = that.d_ppfl;
    d_icfm = that.d_icfm;
    d_scfm = that.d_scfm;
  }

  // assign a ssi to this one

  Ssi& Ssi::operator = (const Ssi& that) {
    this->Instr::operator = (that);
    d_ridx = that.d_ridx;
    d_iidx = that.d_iidx;
    d_midx = that.d_midx;
    d_cnlf = that.d_cnlf;
    d_ppfl = that.d_ppfl;
    d_icfm = that.d_icfm;
    d_scfm = that.d_scfm;
    return *this;
  }

  // assign an instruction to this ssi

  Ssi& Ssi::operator = (const Instr& inst) {
    this->Instr::operator = (inst);
    d_ridx = -1;
    d_iidx = -1;
    d_midx = -1;
    d_cnlf = false;
    d_ppfl = false;
    d_icfm.reset ();
    d_scfm.reset ();
    return *this;
  }

  // reset this ssi

  void Ssi::reset (void) {
    Instr::reset ();
    d_ridx = -1;
    d_iidx = -1;
    d_midx = -1;
    d_cnlf = false;
    d_ppfl = false;
    d_icfm.reset ();
    d_scfm.reset ();
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

  // set the mob index
  
  void Ssi::setmob (const long index) {
    assert (index >= 0);
    d_midx = index;
  }

  // get the mob index

  long Ssi::getmob (void) const {
    return d_midx;
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

  // set the instruction cfm

  void Ssi::seticfm (const Cfm& cfm) {
    d_icfm = cfm;
  }

  // return the instruction cfm

  Cfm Ssi::geticfm (void) const {
    return d_icfm;
  }

  // set the speculative cfm

  void Ssi::setscfm (const Cfm& cfm) {
    d_scfm = cfm;
  }

  // return the speculative cfm

  Cfm Ssi::getscfm (void) const {
    return d_scfm;
  }

  // set the predicate prediction flag

  void Ssi::setppfl (const bool ppfl) {
    d_ppfl = d_valid ? ppfl : false;
  }

  // return the predicate prediction flag

  bool Ssi::getppfl (void) const {
    return d_valid ? d_ppfl : false;
  }
}
