// ---------------------------------------------------------------------------
// - Xdecode.cpp                                                             -
// - iato:isa library - X unit instruction decode implementation             -
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
#include "Udecode.hxx"

namespace iato {

  // decode major opcode 0 - X1
  static t_iopc get_opcode_group_0 (t_octa inst) {
    // check X3 bits (33:35) and X6 bits (27:32)
    t_byte x3 = ((t_byte) (inst >> 33)) & 0x07;
    t_byte x6 = ((t_byte) (inst >> 27)) & 0x3F;
    // decode x misc
    if ((x3 == 0x00) && (x6 == 0x00)) return X_BREAK;
    if ((x3 == 0x00) && (x6 == 0x01)) return X_NOP;
    // mark as invalid to cancel update
    return U_INVALID;
  }

  // decode major opcode 6 - X2
  static t_iopc get_opcode_group_6 (t_octa inst) {
    // get extension bit
    bool vc = ((t_byte) (inst >> 20)) & 0x01;
    // decode x movl
    if (vc == false) return X_MOVL;
    // mark as invalid to cancel update
    return U_INVALID;
  }  
  // update a X unit instruction

  void Instr::xupdate (void) {
    if ((d_valid == true) || (d_opcd == U_INVALID)) return;
    // update predicate, source and destination registers
    switch (d_opcd) {

      // X1 instruction group
    case X_BREAK:
    case X_NOP:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_immv[0] = get_im62 (d_inst, d_extd);
      d_funit   = IUNIT;
      d_group   = "X01";
      d_valid   = true;
      break;

      // X2 instruction group
    case X_MOVL:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_immv[0] = get_im64 (d_inst, d_extd);
      d_funit   = IUNIT;
      d_group   = "X02";
      d_valid   = true;
      break;

    default:
      break;
    }
  }

  // decode a X unit instruction
  
  void Instr::xdecode (void) {
    assert ((d_valid == false) && (d_bunit == XUNIT));
    // get instruction major opcode
    d_code = slot_getmopc (d_inst);
    switch (d_code) {
    case 0x00:
      d_opcd = get_opcode_group_0 (d_inst);
      break;
    case 0x06:
      d_opcd = get_opcode_group_6 (d_inst);
      break;
    default:
      break;
    }    
    // update instruction
    xupdate ();
  }
}
