// ---------------------------------------------------------------------------
// - Rid.cpp                                                                 -
// - iato:isa library - register number class implementation                 -
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

#include "Rid.hpp"

namespace iato {

  // create a new register number

  Rid::Rid (void) {
    reset ();
  }

  // copy construct a register number

  Rid::Rid (const Rid& that) {
    d_valid = that.d_valid;
    d_type  = that.d_type;
    d_lnum  = that.d_lnum;
    d_pnum  = that.d_pnum;
    d_vbit  = that.d_vbit;
    d_vnum  = that.d_vnum;
    d_onum  = that.d_onum;
    d_erdy  = that.d_erdy;
  }

  // assign a register number to this one

  Rid& Rid::operator = (const Rid& that) {
    d_valid = that.d_valid;
    d_type  = that.d_type;
    d_lnum  = that.d_lnum;
    d_pnum  = that.d_pnum;
    d_vbit  = that.d_vbit;
    d_vnum  = that.d_vnum;
    d_onum  = that.d_onum;
    d_erdy  = that.d_erdy;
    return *this;
  }

  // return true if two rid are equal, the ready bit is ignored

  bool Rid::operator == (const Rid& rid) const {
    if ((d_valid == false) || (rid.d_valid == false)) return false;
    if (d_type != rid.d_type) return false;
    if (d_lnum != rid.d_lnum) return false;
    if (d_pnum != rid.d_pnum) return false;
    if (d_vbit != rid.d_vbit) return false;
    if (d_vbit == true) {
      if (d_vnum != rid.d_vnum) return false;
    }
    return true;
  }

  // reset this register number

  void Rid::reset (void) {
    d_valid = false;
    d_type  = GREG;
    d_lnum  = 0;
    d_pnum  = 0;
    d_vbit  = false;
    d_vnum  = 0;
    d_onum  = 0;
    d_erdy  = false;
  }

  // return true if the register is valid

  bool Rid:: isvalid (void) const {
    return d_valid;
  }

  // return true if the rid is a predicate

  bool Rid::ispred (void) const {
    if (d_valid == false) return false;
    if (d_type != PREG) return false;
    if (d_pnum == 0) return false;
    return true;
  }

  // return true if the rid are equal physically

  bool Rid::isequal (const Rid& rid) const {
    if ((d_valid == false) || (rid.d_valid == false)) return false;
    // type and physical must match
    if (d_type != rid.d_type) return false;
    if (d_pnum != rid.d_pnum) return false;
    // special case for virtual bits
    if (d_vbit != rid.d_vbit) return false;
    if (d_vbit == true) {
      if (d_vnum != rid.d_vnum) return false;
    }
    return true;
  }

  // set the eval ready bit

  void Rid::seterdy (const bool erdy) {
    if (d_valid == false) return;
    d_erdy = erdy;
  }

  // set the eval ready bit if both rid are equal

  void Rid::seterdy (const Rid& rid) {
    if (isequal (rid) == false) return;
    d_erdy = true;
  }

  // clear the eval ready bit if both rid are equal

  void Rid::clrerdy (const Rid& rid) {
    if (isequal (rid) == false) return;
    d_erdy = false;
  }

  // return the eval ready bit

  bool Rid::geterdy (void) const {
    return d_erdy;
  }

  // return true if the rid must be logicaly renamed

  bool Rid::islrnm (void) const {
    assert (d_valid == true);
    if ((d_type == GREG) && (d_lnum == 0)) return false;
    if ((d_type == FREG) && (d_lnum == 0)) return false;
    if ((d_type == FREG) && (d_lnum == 1)) return false;
    if ((d_type == PREG) && (d_lnum == 0)) return false;
    return true;
  }

  // return true if the rid is ready

  bool Rid::isready (void) const {
    // if the rid is false, in some sense, it is ready
    if (d_valid == false) return true;
    // if the virtual bit is false, the rid is ready
    if (d_vbit == false) return true;
    // here is the eval ready bit
    return d_erdy;
  }

  // clear the virtual bit

  void Rid::clrvbit (void) {
    d_vbit = false;
    d_vnum = 0;
    d_onum = 0;
  }

  // clear the virtual bit by rid

  void Rid::clrvbit (const Rid& rid) {
    if (isequal (rid) == true) clrvbit ();
  }

  // return the register virtual bit
  
  bool Rid::getvbit (void) const {
    return d_vbit;
  }

  // return the register type

  t_lreg Rid::gettype (void) const {
    return d_type;
  }

  // set the logical number by type

  void Rid::setlnum (t_lreg type, const long lnum) {
    d_valid = true;
    d_type  = type;
    d_lnum  = lnum;
    d_pnum  = lnum;
  }  

  // return the logical register number

  long Rid::getlnum (void) const {
    return d_lnum;
  }

  // set the physical register number

  void Rid::setpnum (const long pnum) {
    d_pnum = pnum;
  }

  // return the physical register number

  long Rid::getpnum (void) const {
    return d_pnum;
  }
  
  // return the register virtual number

  long Rid::getvnum (void) const {
    return d_vnum;
  }

  // set the virtual register number with a virtual bit

  void Rid::setvnum (const long vnum) {
    d_vbit = vnum < 0 ? false : true;
    d_vnum = vnum;
    d_onum = -1;
  }

  // set the virtual register number with a virtual bit

  void Rid::setvnum (const long vnum, const long onum) {
    d_vbit = vnum < 0 ? false : true;
    d_vnum = vnum;
    d_onum = onum;
  }

  // return the target register number
    
  long Rid::gettnum (void) const {
    return d_vbit ? d_vnum : d_pnum;
  }

  // set the register number by type and number

  void Rid::setreg (t_lreg type, const long lnum, const long pnum) {
    d_valid = true;
    d_type  = type;
    d_lnum  = lnum;
    d_pnum  = pnum;
  }

  // return a string representation of this rid

  string Rid::tostring (void) const {
    using namespace std;
    assert (d_valid == true);
    ostringstream os;
    switch (d_type) {
    case GREG:
      os << "type = GREG ";
      break;
    case NREG:
      os << "type = NREG ";
      break;
    case FREG:
      os << "type = FREG ";
      break;
    case PREG:
      os << "type = PREG ";
      break;
    case BREG:
      os << "type = BREG ";
      break;
    case AREG:
      os << "type = AREG ";
      break;
    case IPRG:
      os << "type = IREG ";
      break;
    case CFMR:
      os << "type = CFMR ";
      break;
    case PRRG:
      os << "type = PRRG ";
      break;
    case PROT:
      os << "type = PROT ";
      break;
    case UMRG:
      os << "type = UMRG ";
      break;
    case PSRG:
      os << "type = PSRG ";
      break;
    case CREG:
      os << "type = CREG ";
      break;
    default:
      assert (false);
      break;
    }
    os << "lnum = " << d_lnum << ' ';
    os << "pnum = " << d_pnum << ' ';
    if (d_vbit == true) {
      os << "vnum = " << d_vnum << ' ';
      os << "onum = " << d_onum;
    }
    return os.str ();
  }
} 
