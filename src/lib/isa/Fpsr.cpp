// ---------------------------------------------------------------------------
// - Fpsr.cpp                                                                -
// - iato:isa library - floating point status register class implementation  -
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

#include "Isa.hpp"
#include "Bits.hpp"
#include "Fpsr.hpp"

namespace iato {

  // this procedure returns the field position in the fpsr register
  static t_byte get_fld_pos (const Fpsr::t_mfield mfld, 
			     const Fpsr::t_field fld) {
    t_byte pos = BYTE_0;
    if (mfld == Fpsr::TRAPS) {
      pos = 0;
      assert ((fld == Fpsr::VD) || (fld == Fpsr::DD) || (fld == Fpsr::ZD) ||
	      (fld == Fpsr::OD) || (fld == Fpsr::UD) || (fld == Fpsr::ID));
    } else {
      if (mfld == Fpsr::SF0) pos = 6;
      else if (mfld == Fpsr::SF1) pos = 19;
      else if (mfld == Fpsr::SF2) pos = 32;
      else if (mfld == Fpsr::SF3) pos = 45;
      assert (!((fld == Fpsr::VD) || (fld == Fpsr::DD) || (fld == Fpsr::ZD)
	      || (fld == Fpsr::OD) || (fld == Fpsr::UD) || (fld == Fpsr::ID)));
    }
    switch (fld) {
    case Fpsr::VD:
      pos = 0;
      break;
    case Fpsr::DD:
      pos = 1;
      break;
    case Fpsr::ZD:
      pos = 2;
      break;
    case Fpsr::OD:
      pos = 3;
      break;
    case Fpsr::UD:
      pos = 4;
      break;
    case Fpsr::ID:
      pos = 5;
      break;
    case Fpsr::FTZ:
      break;
    case Fpsr::WRE:
      pos += 1;
      break;
    case Fpsr::PC:
      pos += 2;
      break;
    case Fpsr::RC:
      pos +=4;
      break;
    case Fpsr::TD:
      pos += 6;
      break;
    case Fpsr::V:
      pos += 7;
      break;
    case Fpsr::D:
      pos += 8;
      break;
    case Fpsr::Z:
      pos += 9;
      break;
    case Fpsr::O:
      pos += 10;
      break;
    case Fpsr::U:
      pos += 11;
      break;
    case Fpsr::I:
      pos += 12;
      break;
    }
    return pos;
  }

  // create a default fpsr

  Fpsr::Fpsr (void) {
    setfpsr (DEF_FPSR);
  }

  // create a fpsr with a value

  Fpsr::Fpsr (const t_octa value) {
    setfpsr (value);
  }

  // copy construct this fpsr

  Fpsr::Fpsr (const Fpsr& that) {
    d_fpsr = that.d_fpsr;
  }

  // assign a fpsr to this one

  Fpsr& Fpsr::operator = (const Fpsr& that) {
    d_fpsr = that.d_fpsr;
    return *this;
  }

  // reset this fpsr

  void Fpsr::reset (void) {
    setfpsr (DEF_FPSR);
  }

  // set this fpsr by value
  void Fpsr::setfpsr (const t_octa value) {
    d_fpsr = value;
  }
  
  // get the current fpsr
  t_octa Fpsr::getfpsr (void) const {
    return d_fpsr;
  }

  // set the fpsr by mainfield, field and value
  void Fpsr::setfld (const t_mfield mfld,const t_field fld,const bool value) {
    t_byte pos = get_fld_pos (mfld, fld);
    d_fpsr = bsetocta (d_fpsr, pos, value);
  } 

  // set the fpsr by mainfield, field and value

  void Fpsr::setfld (const t_mfield mfld, const t_field fld, 
		     const t_byte value) {
    t_byte pos = get_fld_pos (mfld, fld);
    bool val = (value & 0x01) == 0x01;
    d_fpsr = bsetocta (d_fpsr, pos, val);
    val = (value & 0x02) == 0x02;
    d_fpsr = bsetocta (d_fpsr, pos+1, val);
  }
  
  // return the bool field value
  bool Fpsr::getbfld (const t_mfield mfld, const t_field fld) const {
    t_byte pos = get_fld_pos (mfld, fld);
    return bsetget (d_fpsr, pos);
  } 

  // return the bool field value
  t_byte Fpsr::getbsfld (const t_mfield mfld, const t_field fld) const {
    t_byte pos = get_fld_pos (mfld, fld);
    t_byte result = 0x00;
    result = bsetbyte (result, 0, bsetget (d_fpsr, pos));
    result = bsetbyte (result, 1, bsetget (d_fpsr, pos+1));
    return result;
  }

  // convert ia floating point register accordiny to ipc and fpsr

  void Fpsr::convert (const t_fpipc ipc, const Fpsr::t_mfield field, t_real& val) {
    t_byte pc  = getbsfld (field, PC);
    t_byte rc  = getbsfld (field, RC);
    bool   wre = getbfld  (field, WRE);
    if (wre == true) {
      if ((ipc == S) || (pc == BYTE_0))
	val.convert (t_real::SGFD24B, t_real::EXP17B, rc);
      else if ((ipc == iato::D) || (pc == 0x02))
	val.convert (t_real::SGFD53B, t_real::EXP17B, rc);
    } else {
      switch (ipc) {
      case S:
	val.convert (t_real::SGFD24B, t_real::EXP8B, rc); 
	break;
      case iato::D:
	val.convert (t_real::SGFD53B, t_real::EXP11B, rc);
	break;
      case NONEPC:
	assert (pc != 0x01);
	if (pc == 0x00)	val.convert (t_real::SGFD24B, t_real::EXP15B, rc); 
	if (pc == 0x10) val.convert (t_real::SGFD53B, t_real::EXP15B, rc);
	if (pc == 0x11) val.convert (t_real::SGFD64B, t_real::EXP15B, rc);
	break;
      }
    }
  }

  // convert ia floating point register for parallel computing

  void Fpsr::pconvert (const Fpsr::t_mfield field, t_real& val1, t_real& val2) {
    t_byte rc  = getbsfld (field, RC);
    val1.convert (t_real::SGFD24B, t_real::EXP8B, rc);
    val2.convert (t_real::SGFD24B, t_real::EXP8B, rc);
  }
}
