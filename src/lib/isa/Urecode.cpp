// ---------------------------------------------------------------------------
// - Urecode.cpp                                                             -
// - iato:isa library - utility function for instruction recode              -
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

#include "Urecode.hxx"

namespace iato {
  using namespace std;

  // return a string representation of a PR

  string to_pred (const long pred) {
    string result;
    if (pred != 0x00) {
      ostringstream os;
      os << '(' << 'p' ;
      if (pred < 10) os << '0';
      os << (int) pred << ')' << ' ';
      result = os.str ();
    }
    long len = result.size ();
    long pad = 6 - len;
    for (long i = 0; i < pad; i++) result += ' ';
    return result;
  }

  // return a string representation of GR

  string to_greg (const long greg) {
    ostringstream os;
    os << 'r' << (int) greg;
    return os.str ();
  }

  // return a string representation of FR

  string to_freg (const long freg) {
    ostringstream os;
    os << 'f' << (int) freg;
    return os.str ();
  } 

  // return a string representation of PR

  string to_preg (const long preg) {
    ostringstream os;
    os << 'p' << (int) preg;
    return os.str ();
  }

  // return a string representation of BR
  
  string to_breg (const long breg) {
    ostringstream os;
    os << 'b' << (int) breg;
    return os.str ();
  }

  // return a string representation of AR

  string to_areg (const long areg) {
    ostringstream os;
    os << "ar" ;
    switch (areg){
    case 0x00:
      os << ".k0";
      break;
    case 0x01:
      os << ".k1";
      break;
    case 0x02:
      os << ".k2";
      break;
    case 0x03:
      os << ".k3";
      break;
    case 0x04:
      os << ".k4";
      break;
    case 0x05:
      os << ".k5";
      break;
    case 0x06:
      os << ".k6";
      break;
    case 0x07:
      os << ".k7";
      break;
    case 0x10:
      os << ".rsc";
      break;
    case 0x11:
      os << ".bsp";
      break;
    case 0x12:
      os << ".bspstore";
      break;
    case 0x13:
      os << ".rnat";
      break;
    case 0x15:
      os << ".fcr";
      break;
    case 0x18:
      os << ".eflag";
      break;
    case 0x19:
      os << ".csd";
      break;
    case 0x1A:
      os << ".ssd";
      break;
    case 0x1B:
      os << ".cflg";
      break;
    case 0x1C:
      os << ".fsr";
      break;
    case 0x1D:
      os << ".fir";
      break;
    case 0x1E:
      os << ".fdr";
      break;
    case 0x20:
      os << ".ccv";
      break;
    case 0x24:
      os << ".unat";
      break;
    case 0x28:
      os << ".fpsr";
      break;
    case 0x2C:
      os << ".itc";
      break;
    case 0x40:
      os << ".pfs";
      break;
    case 0x41:
      os << ".lc";
      break;
    case 0x42:
      os << ".ec";
      break;
    default:
      os << int (areg);
      break;
    }
    return os.str ();
  }
 
  // return a string representation of CR
  
  string to_creg (const long creg) {
    ostringstream os;
    os << "cr" << (int) creg;
    return os.str ();
  }


  // return a string representation of an immediate

  string to_immv (const t_octa immv, const bool sign, const bool hflg) {
    ostringstream os;
    if (sign == true) {
      os << (t_long) immv;
    } else {
      if (hflg == true) {
	os << "0x" << hex << immv;
      } else {
	os << immv;
      }
    }
    return os.str ();
  }

  // return a string representation of a branch target

  string to_brtg (const t_octa immv, const t_octa ip) {
    t_long aoff = (t_long) immv;
    t_octa addr = aoff > 0 ? ip + (t_octa) aoff : ip - (t_octa) (-aoff);
    ostringstream os;
    os << "0x" << hex << addr;
    return os.str ();
  }

}
