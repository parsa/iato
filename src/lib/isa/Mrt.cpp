// ---------------------------------------------------------------------------
// - Mrt.cpp                                                                 -
// - iato:isa library - memory request object class implementation           -
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

#include "Mrt.hpp"

namespace iato {

  // create a default mrt
  
  Mrt::Mrt (void) {
    reset ();
  }

  // create a mrt by type and address

  Mrt::Mrt (t_mrtt type, const t_octa addr) {
    reset ();
    d_type = type;
    d_addr = addr;
  }

  // copy construct this mrt

  Mrt::Mrt (const Mrt& that) {
    d_type = that.d_type;
    d_addr = that.d_addr;
    d_oval = that.d_oval;
    d_lval = that.d_lval;
    d_hval = that.d_hval;
    d_lrid = that.d_lrid;
    d_hrid = that.d_hrid;
  }

  // assign  mrt to this one

  Mrt& Mrt::operator = (const Mrt& that) {
    d_type = that.d_type;
    d_addr = that.d_addr;
    d_oval = that.d_oval;
    d_lval = that.d_lval;
    d_hval = that.d_hval;
    d_lrid = that.d_lrid;
    d_hrid = that.d_hrid;
    return *this;
  }

  // reset this mrt
  
  void Mrt::reset (void) {
    d_type = REQ_NUL;
    d_addr = OCTA_0;
    d_oval = OCTA_0;
    d_lval = 0.0L;
    d_hval = 0.0L;
    d_lrid.reset ();
    d_hrid.reset ();
  }

  // return true if the mrt is valid

  bool Mrt::isvalid (void) const {
    return (d_type != REQ_NUL);
  }

  // return true if the mrt is a load

  bool Mrt::isload (void) const {
    bool result = false;
    switch (d_type) {
    case REQ_LD1:
    case REQ_LD2:
    case REQ_LD4:
    case REQ_LD8:
    case REQ_LDS:
    case REQ_LDD:
    case REQ_LDE:
    case REQ_LDF:
    case REQ_LDI:
    case REQ_LPS:
    case REQ_LPD:
    case REQ_LPI:
      result = true;
      break;
    default:
      break;
    }
    return result;
  }

  // return true if the mrt is a store

  bool Mrt::isstore (void) const {
    bool result = false;
    switch (d_type) {
    case REQ_ST1:
    case REQ_ST2:
    case REQ_ST4:
    case REQ_ST8:
    case REQ_STS:
    case REQ_STD:
    case REQ_STE:
    case REQ_STF:
    case REQ_STI:
      result = true;
      break;
    default:
      break;
    }
    return result;
  }

  // create a bundle mrt by address

  void Mrt::setbnd (const t_octa addr) {
    reset ();
    d_type = REQ_BND;
    d_addr = addr;
  }

  // return the mrt type

  Mrt::t_mrtt Mrt::gettype (void) const {
    return d_type;
  }

  // return the mrt address

  t_octa Mrt::getaddr (void) const {
    assert (isvalid () == true);
    return d_addr;
  }

  // set the mrt byte value

  void Mrt::setbval (const t_byte bval) {
    assert (isvalid () == true);
    d_bval = bval;
  }

  // return the mrt byte value

  t_byte Mrt::getbval (void) const {
    assert (isvalid () == true);
    return d_bval;
  }

  // set the mrt word value

  void Mrt::setwval (const t_word wval) {
    assert (isvalid () == true);
    d_wval = wval;
  }

  // return the mrt word value

  t_word Mrt::getwval (void) const {
    assert (isvalid () == true);
    return d_wval;
  }

  // set the mrt quad value

  void Mrt::setqval (const t_quad qval) {
    assert (isvalid () == true);
    d_qval = qval;
  }

  // return the mrt quad value

  t_quad Mrt::getqval (void) const {
    assert (isvalid () == true);
    return d_qval;
  }

  // set the mrt octa value

  void Mrt::setoval (const t_octa oval) {
    assert (isvalid () == true);
    d_oval = oval;
  }

  // return the mrt octa value

  t_octa Mrt::getoval (void) const {
    assert (isvalid () == true);
    return d_oval;
  }
 
  // set the mrt low real value

  void Mrt::setlval (const t_real& rval) {
    assert (isvalid () == true);
    d_lval = rval;
  }

  // return the mrt low real value

  t_real Mrt::getlval (void) const {
    assert (isvalid () == true);
    return d_lval;
  }  

  // set the mrt high real value

  void Mrt::sethval (const t_real& rval) {
    assert (isvalid () == true);
    d_hval = rval;
  }

  // return the mrt high real value

  t_real Mrt::gethval (void) const {
    assert (isvalid () == true);
    return d_hval;
  }  

  // set the mrt low rid

  void Mrt::setlrid (const Rid& rid) {
    d_lrid = rid;
  }

  // return the load low register

  Rid Mrt::getlrid (void) const {
    return d_lrid;
  }

  // set the mrt high rid

  void Mrt::sethrid (const Rid& rid) {
    d_hrid = rid;
  }

  // return the load high register

  Rid Mrt::gethrid (void) const {
    return d_hrid;
  }

  // set the load information by type, address and rid

  void Mrt::setld (t_mrtt type, const t_octa addr, const Rid& rid) {
    reset ();
    d_type = type;
    d_addr = addr;
    d_lrid = rid; 
  }

  // set the load information by type, address and rid pair

  void Mrt::setld (t_mrtt type, const t_octa addr, const Rid& lrid,
		   const Rid& hrid) {
    reset ();
    d_type = type;
    d_addr = addr;
    d_lrid = lrid; 
    d_hrid = hrid; 
  }

  // set the store information by type, address and value

  void Mrt::setst (t_mrtt type, const t_octa addr, const t_octa oval) {
    reset ();
    d_type = type;
    d_addr = addr;
    d_oval = oval;
  }

  // set the store information by type, address and value

  void Mrt::setst (t_mrtt type, const t_octa addr, const t_real& rval) {
    reset ();
    d_type = type;
    d_addr = addr;
    d_lval = rval;
  }

  // set the store information by type, address and pair value

  void Mrt::setst (t_mrtt type, const t_octa addr, const t_real& lval,
		   const t_real& hval) {
    reset ();
    d_type = type;
    d_addr = addr;
    d_lval = lval;
    d_hval = hval;
  }
}
