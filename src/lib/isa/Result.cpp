// ---------------------------------------------------------------------------
// - Result.cpp                                                              -
// - iato:isa library - instruction result value class implementation        -
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

#include "Result.hpp"
#include "Exception.hpp"

namespace iato {
  
  // create a new result

  Result::Result (void) {
    reset ();
  }

  // create a result by index and value

  Result::Result (const long index, const bool value) {
    assert ((index >= 0) && (index < IA_MDST));
    reset ();
    d_valid       = true;
    d_drop[index] = REG_UPD;
    d_bval[index] = value;
  }

  // create a result by index and value

  Result::Result (const long index, const t_octa value) {
    assert ((index >= 0) && (index < IA_MDST));
    reset ();
    d_valid       = true;
    d_drop[index] = REG_UPD;
    d_oval[index] = value;
  }

  // create a result by index and value

  Result::Result (const long index, const t_real& value) {
    assert ((index >= 0) && (index < IA_MDST));
    reset ();
    d_valid       = true;
    d_drop[index] = REG_UPD;
    d_rval[index] = value;
  }

  // copy construct this result

  Result::Result (const Result& that) {
    d_valid = that.d_valid;
    for (long i = 0; i < IA_MDST; i++) {
      d_addr[i] = that.d_addr[i];
      d_drop[i] = that.d_drop[i];
      d_drid[i] = that.d_drid[i];
      d_bval[i] = that.d_bval[i];
      d_oval[i] = that.d_oval[i];
      d_rval[i] = that.d_rval[i];
      d_aset[i] = that.d_aset[i];
      d_achk[i] = that.d_achk[i];
      d_aclr[i] = that.d_aclr[i];
      d_spec[i] = that.d_spec[i];
    }
  }

  // assign an result to this one

  Result& Result::operator = (const Result& that) {
    d_valid = that.d_valid;
    for (long i = 0; i < IA_MDST; i++) {
      d_addr[i] = that.d_addr[i];
      d_drop[i] = that.d_drop[i];
      d_drid[i] = that.d_drid[i];
      d_bval[i] = that.d_bval[i];
      d_oval[i] = that.d_oval[i];
      d_rval[i] = that.d_rval[i];
      d_aset[i] = that.d_aset[i];
      d_achk[i] = that.d_achk[i];
      d_spec[i] = that.d_spec[i];
    }
    return *this;
  }
   
  // reset this result

  void Result::reset (void) {
    d_valid = false;
    for (long i = 0; i < IA_MDST; i++) {
      d_addr[i] = 0;
      d_drid[i].reset ();
      d_drop[i] = ROP_NOP;
      d_bval[i] = false;
      d_oval[i] = 0;
      d_aset[i] = false;
      d_achk[i] = false;
      d_aclr[i] = false;
      d_spec[i] = false;
    }
  }

  // return true if a rid as the right type

  bool Result::isreg (const long index, t_lreg lreg) const {
    assert ((index >= 0) && (index < IA_MDST));
    if (d_valid == false) return false;
    if (d_drop[index] == ROP_NOP) return false;
    if (d_drid[index].isvalid () == false) return false;
    return (d_drid[index].gettype () == lreg);
  }
  
  // return true if the result has the register type
  
  bool Result::isreg (t_lreg lreg) const {
    for (long i = 0; i < IA_MDST; i++) {
      if (isreg (i, lreg) == true) return true;
    }
    return false;
  }

  // return the result ip

  t_octa Result::getrip (void) const {
    for (long i = 0; i < IA_MDST; i++) {
      if (isreg (i, IPRG) == true) return getoval (i);
    }
    string msg = "cannot find ip register with result";
    throw Exception ("result-error", msg);
  }

  // set the reroute flag by index - this apply only to predicate
  // registers

  void Result::setrrt (const long index, const bool flag) {
    assert ((index >= 0) && (index < IA_MDST));
    assert ((d_drid[index].gettype () == PREG) || 
	    (d_drid[index].gettype () == FREG));
    d_valid = true;
    d_drop[index] = REG_RRT;
    d_bval[index] = flag;
  }

  // set the load/store address by index

  void Result::setaddr (const long index, t_rop rop, const t_octa addr) {
    assert ((index >= 0) && (index < IA_MDST));
    switch (rop) {
    case REG_LD1:
    case REG_LD2:
    case REG_LD4:
    case REG_LD8:
    case REG_LDS:
    case REG_LDD:
    case REG_LDE:
    case REG_LDF:
    case REG_LDI:
    case RPL_LDS:
    case RPH_LDS:
    case RPL_LDD:
    case RPH_LDD:
    case RPL_LDI:
    case RPH_LDI:
    case REG_ST1:
    case REG_ST2:
    case REG_ST4:
    case REG_ST8:
    case REG_STS:
    case REG_STD:
    case REG_STE:
    case REG_STF:
    case REG_STI:
    case REG_CX1:
    case REG_CX2:
    case REG_CX4:
    case REG_CX8:
      d_drop[index] = rop;
      d_addr[index] = addr;
      d_valid       = true;
      break;
    default:
      assert (false);
      break;
    }
  }

  // return the load/store address

  t_octa Result::getaddr (const long index) const {
    assert ((index >= 0) && (index < IA_MDST));
    return d_addr[index];
  }

  // set the store immediate value

  void Result::setimmv (const long index, const t_octa value) {
    assert ((index >= 0) && (index < IA_MDST));
    switch (d_drop[index]) {
    case REG_ST1:
    case REG_ST2:
    case REG_ST4:
    case REG_ST8:
    case REG_CX1:
    case REG_CX2:
    case REG_CX4:
    case REG_CX8:
      d_bval[index] = true;
      d_oval[index] = value;
      break;
    default:
      assert (false);
      break;
    }
  }

  // set the store real value

  void Result::setrimv (const long index, const t_real value) {
    assert ((index >= 0) && (index < IA_MDST));
    switch (d_drop[index]) {
    case REG_STS:
    case REG_STD:
    case REG_STE:
    case REG_STF:
    case REG_STI:
      d_bval[index] = true;
      d_rval[index] = value;
      break;
    default:
      assert (false);
      break;
    }
  }

  // get the store immediate value

  t_octa Result::getimmv (const long index) const {
    assert ((index >= 0) && (index < IA_MDST));
    t_octa result = OCTA_0;
    switch (d_drop[index]) {
    case REG_ST1:
    case REG_ST2:
    case REG_ST4:
    case REG_ST8:
    case REG_CX1:
    case REG_CX2:
    case REG_CX4:
    case REG_CX8:
      assert (d_bval[index] == true);
      result = d_oval[index];
      break;
    default:
      assert (false);
      break;
    }
    return result;
  }

  // get the store real value

  t_real Result::getrimv (const long index) const {
    assert ((index >= 0) && (index < IA_MDST));
    t_real result = 0.0L;
    switch (d_drop[index]) {
    case REG_STS:
    case REG_STD:
    case REG_STE:
    case REG_STF:
    case REG_STI:
      assert (d_bval[index] == true);
      result = d_rval[index];
      break;
    default:
      assert (false);
      break;
    }
    return result;
  }

  // return the result value by index

  Uvr Result::getuvr (const long index) const {
    assert ((index >= 0) && (index < IA_MDST));
    // check for valid result
    Uvr result;
    if (d_valid == false) return result;
    if (d_drop[index] == ROP_NOP) return result;
    if (d_drid[index].isvalid () == false) return result;
    // the rid tells us what kind of data we have
    t_lreg lreg = d_drid[index].gettype ();
    switch (lreg) {
    case GREG:
      result.setdval (getoval (index), getbval (index));
      break;
    case NREG:
    case PREG:
      result.setbval (getbval (index));
      break;
    case FREG:
      result.setrval (getrval (index));
      break;  
    default:
      result.setoval (getoval (index));
      break; 
    }
    return result;
  }

  // set the result nat value by rid

  void Result::setnval (const Rid& rid, const bool value) {
    // check for valid rid
    if (rid.isvalid () == false) return;
    // loop to match rid
    for (long i = 0; i < IA_MDST; i++) {
      if (d_drid[i] == rid) {
	if (rid.gettype () == GREG) {
	  setbval (i, true);
	  return;
	}
	if (rid.gettype () == FREG) {
	  t_real rval = 0.0L; rval.setnat ();
	  setrval (i, rval);
	  return;
	}   
      }
    }
  }

  // set the result value by index

  void Result::setbval (const long index, const bool value) {
    assert ((index >= 0) && (index < IA_MDST));
    assert (d_drid[index].isvalid () == true);
    d_valid       = true;
    d_drop[index] = REG_UPD;
    d_bval[index] = value;
  }

  // set the result value by rid

  void Result::setbval (const Rid& rid, const bool value) {
    // check for valid rid
    if (rid.isvalid () == false) return;
    // loop to match rid
    for (long i = 0; i < IA_MDST; i++) {
      if (d_drid[i] == rid) {
	setbval (i, value);
	return;
      }
    }
  }

  // update the result value by index but do not touch the rop

  void Result::updbval (const long index, const bool value) {
    assert ((index >= 0) && (index < IA_MDST));
    assert (d_drid[index].isvalid () == true);
    assert (d_valid == true);
    assert (d_drop[index] != ROP_NOP);
    d_bval[index] = value;
  }

  // update the result value by rid but do not touch the rop

  void Result::updbval (const Rid& rid, const bool value) {
    // check for valid rid
    if (rid.isvalid () == false) return;
    // loop to match rid
    for (long i = 0; i < IA_MDST; i++) {
      if (d_drid[i] == rid) {
	updbval (i, value);
	return;
      }
    }
  }

  // set the result value by index

  void Result::setoval (const long index, const t_octa value) {
    assert ((index >= 0) && (index < IA_MDST));
    assert (d_drid[index].isvalid () == true);
    d_valid       = true;
    d_drop[index] = REG_UPD;
    d_oval[index] = value;
  }

  // set the result value by rop and index

  void Result::setoval (const long index, t_rop rop, const t_octa value) {
    assert ((index >= 0) && (index < IA_MDST));
    assert (d_drid[index].isvalid () == true);
    d_valid       = true;
    d_drop[index] = rop;
    d_oval[index] = value;
  }

  // set the result value by rid

  void Result::setoval (const Rid& rid, const t_octa value) {
    // check for valid rid
    if (rid.isvalid () == false) return;
    // loop to match rid
    for (long i = 0; i < IA_MDST; i++) {
      if (d_drid[i] == rid) {
	setoval (i, value);
	return;
      }
    }
  }

  // update the result value by index but do not touch the rop

  void Result::updoval (const long index, const t_octa value) {
    assert ((index >= 0) && (index < IA_MDST));
    assert (d_drid[index].isvalid () == true);
    assert (d_valid == true);
    assert (d_drop[index] != ROP_NOP);
    d_oval[index] = value;
  }

  // update the result value by rid but do not touch the rop

  void Result::updoval (const Rid& rid, const t_octa value) {
    // check for valid rid
    if (rid.isvalid () == false) return;
    // loop to match rid
    for (long i = 0; i < IA_MDST; i++) {
      if (d_drid[i] == rid) {
	updoval (i, value);
	return;
      }
    }
  }

  // set the result value by index
  
  void Result::setrval (const long index, const t_real& value) {
    assert ((index >= 0) && (index < IA_MDST));
    assert (d_drid[index].isvalid () == true);
    d_valid       = true;
    d_drop[index] = REG_UPD;
    d_rval[index] = value;
  }

  // set the result value by rid

  void Result::setrval (const Rid& rid, const t_real& value) {
    // check for valid rid
    if (rid.isvalid () == false) return;
    // loop to match rid
    for (long i = 0; i < IA_MDST; i++) {
      if (d_drid[i] == rid) {
	setrval (i, value);
	return;
      }
    }
  }

  // update the result value by index but do not touch the rop

  void Result::updrval (const long index, const t_real& value) {
    assert ((index >= 0) && (index < IA_MDST));
    assert (d_drid[index].isvalid () == true);
    assert (d_valid == true);
    assert (d_drop[index] != ROP_NOP);
    d_rval[index] = value;
  }

  // update the result value by rid but do not touch the rop

  void Result::updrval (const Rid& rid, const t_real& value) {
    // check for valid rid
    if (rid.isvalid () == false) return;
    // loop to match rid
    for (long i = 0; i < IA_MDST; i++) {
      if (d_drid[i] == rid) {
	updrval (i, value);
	return;
      }
    }
  }

  // set the result value by rid and uvr

  void Result::setuval (const Rid& rid, const Uvr& uvr) {
    // check for valid rid
    if (rid.isvalid () == false) return;
    // check for valid uvr
    switch (uvr.gettype ()) {
    case Uvr::OBV:
      setoval (rid, uvr.getoval ());
      break;
    case Uvr::ONV:
      setoval (rid, uvr.getoval ());
      setbval (rid, uvr.getbval ());
      break;
    case Uvr::BBV:
      setbval (rid, uvr.getbval ());
      break;
    case Uvr::RBV:
      setrval (rid, uvr.getoval ());
      break;
    default:
      break;
    }
  }
  
  // set the result value by rid and uvr but do not touch the rop

  void Result::upduval (const Rid& rid, const Uvr& uvr) {
    // check for valid rid
    if (rid.isvalid () == false) return;
    // check for valid uvr
    switch (uvr.gettype ()) {
    case Uvr::OBV:
      updoval (rid, uvr.getoval ());
      break;
    case Uvr::ONV:
      updoval (rid, uvr.getoval ());
      updbval (rid, uvr.getbval ());
      break;
    case Uvr::BBV:
      updbval (rid, uvr.getbval ());
      break;
    case Uvr::RBV:
      updrval (rid, uvr.getoval ());
      break;
    default:
      break;
    }
  }
   
  // return the result mrt by index

  Mrt Result::getmrt (void) const {
    // prepare mrt result and loop in rop
    Mrt mrt;
    for (long i = 0; i < IA_MDST; i++) {
      switch (d_drop[i]) {
      case REG_LD1:
	assert (mrt.isvalid () == false);
	mrt.setld (Mrt::REQ_LD1, d_addr[i], d_spec[i], d_drid[i]);
	break;
      case REG_LD2:
	assert (mrt.isvalid () == false);
	mrt.setld (Mrt::REQ_LD2, d_addr[i], d_spec[i], d_drid[i]);
	break;
      case REG_LD4:
	assert (mrt.isvalid () == false);
	mrt.setld (Mrt::REQ_LD4, d_addr[i], d_spec[i], d_drid[i]);
	break;
      case REG_LD8:
	assert (mrt.isvalid () == false);
	mrt.setld (Mrt::REQ_LD8, d_addr[i], d_spec[i], d_drid[i]);
	break;
      case REG_LDS:
	assert (mrt.isvalid () == false);
	mrt.setld (Mrt::REQ_LDS, d_addr[i], d_spec[i], d_drid[i]);
	break;
      case REG_LDD:
	assert (mrt.isvalid () == false);
	mrt.setld (Mrt::REQ_LDD, d_addr[i], d_spec[i], d_drid[i]);
	break;
      case REG_LDE:
	assert (mrt.isvalid () == false);
	mrt.setld (Mrt::REQ_LDE, d_addr[i], d_spec[i], d_drid[i]);
	break;
      case REG_LDF:
	assert (mrt.isvalid () == false);
	mrt.setld (Mrt::REQ_LDF, d_addr[i], d_spec[i], d_drid[i]);
	break;
      case REG_LDI:
	assert (mrt.isvalid () == false);
	mrt.setld (Mrt::REQ_LDI, d_addr[i], d_spec[i], d_drid[i]);
	break;
      case RPL_LDS:
	assert (mrt.isvalid () == false);
	mrt.setld (Mrt::REQ_LPS, d_addr[i], d_spec[i], d_drid[i]);
	break;
      case RPL_LDD:
	assert (mrt.isvalid () == false);
	mrt.setld (Mrt::REQ_LPD, d_addr[i], d_spec[i], d_drid[i]);
	break;
      case RPL_LDI:
	assert (mrt.isvalid () == false);
	mrt.setld (Mrt::REQ_LPI, d_addr[i], d_spec[i], d_drid[i]);
	break;
      case RPH_LDS:
      case RPH_LDD:
      case RPH_LDI:
	mrt.sethrid (d_drid[i]);
	break;
      case REG_ST1:
	assert (mrt.isvalid () == false);
	mrt.setst (Mrt::REQ_ST1, d_addr[i], d_oval[i]);
	break;
      case REG_ST2:
	assert (mrt.isvalid () == false);
	mrt.setst (Mrt::REQ_ST2, d_addr[i], d_oval[i]);
	break;
      case REG_ST4:
	assert (mrt.isvalid () == false);
	mrt.setst (Mrt::REQ_ST4, d_addr[i], d_oval[i]);
	break;
      case REG_ST8:
	assert (mrt.isvalid () == false);
	mrt.setst (Mrt::REQ_ST8, d_addr[i], d_oval[i]);
	break;
      case REG_STS:
	assert (mrt.isvalid () == false);
	mrt.setst (Mrt::REQ_STS, d_addr[i], d_rval[i]);
	break;
      case REG_STD:
	assert (mrt.isvalid () == false);
	mrt.setst (Mrt::REQ_STD, d_addr[i], d_rval[i]);
	break;
      case REG_STE:
	assert (mrt.isvalid () == false);
	mrt.setst (Mrt::REQ_STE, d_addr[i], d_rval[i]);
	break;
      case REG_STF:
	assert (mrt.isvalid () == false);
	mrt.setst (Mrt::REQ_STF, d_addr[i], d_rval[i]);
	break;
      case REG_STI:
	assert (mrt.isvalid () == false);
	mrt.setst (Mrt::REQ_STI, d_addr[i], d_rval[i]);
	break;
      case REG_CX1:
      case REG_CX2:
      case REG_CX4:
      case REG_CX8:
	assert (false);
	break;
      default:
	break;
      }
    }
    return mrt;
  }
  
  // update a result mrt
  
  void Result::update (const Mrt& mrt) {
    // check for valid mrt
    if (mrt.isvalid () == false) return;
    // check mrt operation
    switch (mrt.gettype ()) {
    case Mrt::REQ_LD1:
    case Mrt::REQ_LD2:
    case Mrt::REQ_LD4:
    case Mrt::REQ_LD8:
      if (mrt.isnval () == true) {
	setnval (mrt.getlrid (), true);
      } else {
	setoval (mrt.getlrid (), mrt.getoval ());
      }
      break;
    case Mrt::REQ_LDS:
    case Mrt::REQ_LDD:
    case Mrt::REQ_LDE:
    case Mrt::REQ_LDF:
    case Mrt::REQ_LDI:
      if (mrt.isnval () == true) {
	setnval (mrt.getlrid (), true);
      } else {
	setrval (mrt.getlrid (), mrt.getlval ());
      }
      break;
    case Mrt::REQ_LPS:
    case Mrt::REQ_LPD:
    case Mrt::REQ_LPI:
      if (mrt.isnval () == true) {
	setnval (mrt.getlrid (), true);
	setnval (mrt.gethrid (), true);
      } else {
	setrval (mrt.getlrid (), mrt.getlval ());
	setrval (mrt.gethrid (), mrt.gethval ());
      }
      break;
    default:
      break;
    }
  }
}
