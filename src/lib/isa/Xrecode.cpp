// ---------------------------------------------------------------------------
// - Xrecode.cpp                                                             -
// - iato:isa library - X unit instruction recode implementation             -
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

#include "Instr.hpp"
#include "Urecode.hxx"

namespace iato {
  using namespace std;

  // recode a X01 instruction

  string Instr::xrecode_01 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case X_BREAK: result += "break.x "; break;
    case X_NOP:   result += "nop.x "; break;
    default: 
      break;
    }
    // add immediate value
    result += to_immv (d_immv[0], false, true);
    return result;
  }

  // recode a X02 instruction

  string Instr::xrecode_02 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += "movl ";
    result += to_greg (d_rdst[0].getlnum ()) + '=';
    result += to_immv (d_immv[0], false, true);
    return result;
  }

  // recode a X unit instruction

  string Instr::xrecode (void) const {
    assert (d_valid == true);
    string result;
    // select instruction group
    switch (d_opcd) {
    case X_BREAK:
    case X_NOP:
      result = xrecode_01 ();
      break;
    case X_MOVL:
      result = xrecode_02 ();
      break;
    default:
      break;
    }
    return result;
  }
}
