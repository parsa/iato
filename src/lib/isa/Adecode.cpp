// ---------------------------------------------------------------------------
// - Adecode.cpp                                                             -
// - iato:isa library - A unit instruction decode implementation             -
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

  // extract a shift count value for shladd, shladdp4
  static inline t_octa get_shct (const t_octa inst) {
    t_octa result = (inst >> 27) & 0x0000000000000003LL;
    return result;
  }

  // decode alu opcode group 8 / x2a = 0, ve = 0
  static t_iopc get_opcode_group_8_0_0 (t_octa inst) {
    t_byte x4  = ((t_byte) (inst >> 29)) & 0x0F;
    t_byte x2b = ((t_byte) (inst >> 27)) & 0x03;
    if ((x4 == 0x00) && (x2b == 0x00)) return A_ADD;
    if ((x4 == 0x00) && (x2b == 0x01)) return A_ADD_1;
    if ((x4 == 0x01) && (x2b == 0x01)) return A_SUB;
    if ((x4 == 0x01) && (x2b == 0x00)) return A_SUB_1;
    if ((x4 == 0x02) && (x2b == 0x00)) return A_ADDP4;
    if ((x4 == 0x03) && (x2b == 0x00)) return A_AND;
    if ((x4 == 0x03) && (x2b == 0x01)) return A_ANDCM;
    if ((x4 == 0x03) && (x2b == 0x02)) return A_OR;
    if ((x4 == 0x03) && (x2b == 0x03)) return A_XOR;	  
    if (x4 == 0x04)                    return A_SHLADD;
    if (x4 == 0x06)                    return A_SHLADDP4;
    if ((x4 == 0x09) && (x2b == 0x01)) return A_SUB_I;
    if ((x4 == 0x0B) && (x2b == 0x00)) return A_AND_I;
    if ((x4 == 0x0B) && (x2b == 0x01)) return A_ANDCM_I;
    if ((x4 == 0x0B) && (x2b == 0x02)) return A_OR_I;
    if ((x4 == 0x0B) && (x2b == 0x03)) return A_XOR_I;
    return U_INVALID;
  }

  // decode major opcode 8 - A1/A4 - A9/A10
  static t_iopc get_opcode_group_8 (t_octa inst) {
    // check za (36) X2A bits (34:35) ve/zb bit (33) 
    // X4  bits (29:32) X2B (27:28)
    bool    za = ((((t_byte) (inst >> 36)) & 0x01) == 0x01);
    t_byte x2a = ((t_byte) (inst >> 34)) & 0x03;
    bool    ve = ((((t_byte) (inst >> 33)) & 0x01) == 0x01);
    bool    zb = ((((t_byte) (inst >> 33)) & 0x01) == 0x01);
    t_byte  x4 = ((t_byte) (inst >> 29)) & 0x0F;
    t_byte x2b = ((t_byte) (inst >> 27)) & 0x03;
    // decode integer alu
    if ((x2a == 0x00) && (ve == false)) return get_opcode_group_8_0_0 (inst);
    if ((x2a == 0x02) && (ve == false)) return A_ADDS_I;
    if ((x2a == 0x03) && (ve == false)) return A_ADDP4_I;
    // decode multimedia alu
    if ((x2a == 0x01) && (za == false) && (zb == false) && (x4 == 0x00) 
	&& (x2b == 0x00)) return A_PADD1;
    if ((x2a == 0x01) && (za == false) && (zb == true) && (x4 == 0x00) 
	&& (x2b == 0x00)) return A_PADD2;
    if ((x2a == 0x01) && (za == true) && (zb == false) && (x4 == 0x00) 
	&& (x2b == 0x00)) return A_PADD4;
    if ((x2a == 0x01) && (za == false) && (zb == false) && (x4 == 0x00) 
	&& (x2b == 0x01)) return A_PADD1_SSS;
    if ((x2a == 0x01) && (za == false) && (zb == true) && (x4 == 0x00) 
	&& (x2b == 0x01)) return A_PADD2_SSS;
    if ((x2a == 0x01) && (za == false) && (zb == false) && (x4 == 0x00) 
	&& (x2b == 0x02)) return A_PADD1_UUU;
    if ((x2a == 0x01) && (za == false) && (zb == true) && (x4 == 0x00) 
	&& (x2b == 0x02)) return A_PADD2_UUU;
    if ((x2a == 0x01) && (za == false) && (zb == false) && (x4 == 0x00) 
	&& (x2b == 0x03)) return A_PADD1_UUS;
    if ((x2a == 0x01) && (za == false) && (zb == true) && (x4 == 0x00) 
	&& (x2b == 0x03)) return A_PADD2_UUS;
    if ((x2a == 0x01) && (za == false) && (zb == false) && (x4 == 0x01) 
	&& (x2b == 0x00)) return A_PSUB1;
    if ((x2a == 0x01) && (za == false) && (zb == true) && (x4 == 0x01) 
	&& (x2b == 0x00)) return A_PSUB2;
    if ((x2a == 0x01) && (za == true) && (zb == false) && (x4 == 0x01) 
	&& (x2b == 0x00)) return A_PSUB4;
    if ((x2a == 0x01) && (za == false) && (zb == false) && (x4 == 0x01) 
	&& (x2b == 0x01)) return A_PSUB1_SSS;
    if ((x2a == 0x01) && (za == false) && (zb == true) && (x4 == 0x01) 
	&& (x2b == 0x01)) return A_PSUB2_SSS;
    if ((x2a == 0x01) && (za == false) && (zb == false) && (x4 == 0x01) 
	&& (x2b == 0x02)) return A_PSUB1_UUU;
    if ((x2a == 0x01) && (za == false) && (zb == true) && (x4 == 0x01) 
	&& (x2b == 0x02)) return A_PSUB2_UUU;
    if ((x2a == 0x01) && (za == false) && (zb == false) && (x4 == 0x01) 
	&& (x2b == 0x03)) return A_PSUB1_UUS;
    if ((x2a == 0x01) && (za == false) && (zb == true) && (x4 == 0x01) 
	&& (x2b == 0x03)) return A_PSUB2_UUS;
    if ((x2a == 0x01) && (za == false) && (zb == false) && (x4 == 0x02) 
	&& (x2b == 0x02)) return A_PAVG1;
    if ((x2a == 0x01) && (za == false) && (zb == true) && (x4 == 0x02) 
	&& (x2b == 0x02)) return A_PAVG2;
    if ((x2a == 0x01) && (za == false) && (zb == false) && (x4 == 0x02) 
	&& (x2b == 0x03)) return A_PAVG1_RAZ;
    if ((x2a == 0x01) && (za == false) && (zb == true) && (x4 == 0x02) 
	&& (x2b == 0x03)) return A_PAVG2_RAZ;
    if ((x2a == 0x01) && (za == false) && (zb == false) && (x4 == 0x03) 
	&& (x2b == 0x02)) return A_PAVGSUB1;
    if ((x2a == 0x01) && (za == false) && (zb == true) && (x4 == 0x03) 
	&& (x2b == 0x02)) return A_PAVGSUB2;
    if ((x2a == 0x01) && (za == false) && (zb == false) && (x4 == 0x09) 
	&& (x2b == 0x00)) return A_PCMP1_EQ;
    if ((x2a == 0x01) && (za == false) && (zb == true) && (x4 == 0x09) 
	&& (x2b == 0x00)) return A_PCMP2_EQ;
    if ((x2a == 0x01) && (za == true) && (zb == false) && (x4 == 0x09) 
	&& (x2b == 0x00)) return A_PCMP4_EQ;
    if ((x2a == 0x01) && (za == false) && (zb == false) && (x4 == 0x09) 
	&& (x2b == 0x01)) return A_PCMP1_GT;
    if ((x2a == 0x01) && (za == false) && (zb == true) && (x4 == 0x09) 
	&& (x2b == 0x01)) return A_PCMP2_GT;
    if ((x2a == 0x01) && (za == true) && (zb == false) && (x4 == 0x09) 
	&& (x2b == 0x01)) return A_PCMP4_GT;
    // decode multimedia shift and add
    if ((x2a == 0x01) && (za == false) && (zb == true) && (x4 == 0x04))
      return A_PSHLADD2;
    if ((x2a == 0x01) && (za == false) && (zb == true) && (x4 == 0x06))
      return A_PSHRADD2;
    // mark as invalid to cancel update
    return U_INVALID;
  }

  // decode major opcode C - A6 A7 A8
  static t_iopc get_opcode_group_C (t_octa inst) {
    t_byte x2 = ((t_byte) (inst >> 34)) &0x03;
    bool   tb = ((((t_byte) (inst >> 36)) & 0x01) == 0x01);
    bool   ta = ((((t_byte) (inst >> 33)) & 0x01) == 0x01);
    bool    c = ((((t_byte) (inst >> 12)) & 0x01) == 0x01);
    // decode cmp reg-reg
    if ((x2 == 0x00) && (tb == false) && (ta == false) && (c == false))    
      return A_CMP_LT_RR;
    if ((x2 == 0x00) && (tb == false) && (ta == false) && (c == true))    
      return A_CMP_LT_UNC_RR;
    if ((x2 == 0x00) && (tb == false) && (ta == true) && (c == false))    
      return A_CMP_EQ_AND_RR;
    if ((x2 == 0x00) && (tb == false) && (ta == true) && (c == true))    
      return A_CMP_NE_AND_RR;
    if ((x2 == 0x01) && (tb == false) && (ta == false) && (c == false))    
      return A_CMP4_LT_RR;
    if ((x2 == 0x01) && (tb == false) && (ta == false) && (c == true))    
      return A_CMP4_LT_UNC_RR;
    if ((x2 == 0x01) && (tb == false) && (ta == true) && (c == false))    
      return A_CMP4_EQ_AND_RR;
    if ((x2 == 0x01) && (tb == false) && (ta == true) && (c == true))    
      return A_CMP4_NE_AND_RR;
    // decode cmp r0-reg
    if ((x2 == 0x00) && (tb == true) && (ta == false) && (c == false))    
      return A_CMP_GT_AND_ZR;
    if ((x2 == 0x00) && (tb == true) && (ta == false) && (c == true))    
      return  A_CMP_LE_AND_ZR;
    if ((x2 == 0x00) && (tb == true) && (ta == true) && (c == false))    
      return A_CMP_GE_AND_ZR;
    if ((x2 == 0x00) && (tb == true) && (ta == true) && (c == true))    
      return A_CMP_LT_AND_ZR;
    if ((x2 == 0x01) && (tb == true) && (ta == false) && (c == false))    
      return A_CMP4_GT_AND_ZR;
    if ((x2 == 0x01) && (tb == true) && (ta == false) && (c == true))    
      return A_CMP4_LE_AND_ZR;
    if ((x2 == 0x01) && (tb == true) && (ta == true) && (c == false))    
      return A_CMP4_GE_AND_ZR;
    if ((x2 == 0x01) && (tb == true) && (ta == true) && (c == true))    
      return A_CMP4_LT_AND_ZR;
    // decode cmp imm-reg
    if ((x2 == 0x02) && (ta == false) && (c == false))    
      return A_CMP_LT_IR;
    if ((x2 == 0x02) && (ta == false) && (c == true))    
      return A_CMP_LT_UNC_IR;
    if ((x2 == 0x02) && (ta == true) && (c == false))    
      return A_CMP_EQ_AND_IR;
    if ((x2 == 0x02) && (ta == true) && (c == true))    
      return A_CMP_NE_AND_IR;
    if ((x2 == 0x03) && (ta == false) && (c == false))    
      return A_CMP4_LT_IR;
    if ((x2 == 0x03) && (ta == false) && (c == true))    
      return A_CMP4_LT_UNC_IR;
    if ((x2 == 0x03) && (ta == true) && (c == false))    
      return A_CMP4_EQ_AND_IR;
    if ((x2 == 0x03) && (ta == true) && (c == true))    
      return A_CMP4_NE_AND_IR;
    return U_INVALID;
  }

  // decode major opcode D - A6 A7 A8
  static t_iopc get_opcode_group_D (t_octa inst) {
    t_byte x2 = ((t_byte) (inst >> 34)) &0x03;
    bool   tb = ((((t_byte) (inst >> 36)) & 0x01) == 0x01);
    bool   ta = ((((t_byte) (inst >> 33)) & 0x01) == 0x01);
    bool    c = ((((t_byte) (inst >> 12)) & 0x01) == 0x01);
    // decode cmp reg-reg
    if ((x2 == 0x00) && (tb == false) && (ta == false) && (c == false))    
      return A_CMP_LTU_RR;
    if ((x2 == 0x00) && (tb == false) && (ta == false) && (c == true))    
      return A_CMP_LTU_UNC_RR;
    if ((x2 == 0x00) && (tb == false) && (ta == true) && (c == false))    
      return A_CMP_EQ_OR_RR;
    if ((x2 == 0x00) && (tb == false) && (ta == true) && (c == true))    
      return A_CMP_NE_OR_RR;
    if ((x2 == 0x01) && (tb == false) && (ta == false) && (c == false))    
      return A_CMP4_LTU_RR;
    if ((x2 == 0x01) && (tb == false) && (ta == false) && (c == true))    
      return A_CMP4_LTU_UNC_RR;
    if ((x2 == 0x01) && (tb == false) && (ta == true) && (c == false))    
      return A_CMP4_EQ_OR_RR;
    if ((x2 == 0x01) && (tb == false) && (ta == true) && (c == true))    
      return A_CMP4_NE_OR_RR;
    // decode cmp r0-reg
    if ((x2 == 0x00) && (tb == true) && (ta == false) && (c == false))    
      return A_CMP_GT_OR_ZR;
    if ((x2 == 0x00) && (tb == true) && (ta == false) && (c == true))    
      return  A_CMP_LE_OR_ZR;
    if ((x2 == 0x00) && (tb == true) && (ta == true) && (c == false))    
      return A_CMP_GE_OR_ZR;
    if ((x2 == 0x00) && (tb == true) && (ta == true) && (c == true))    
      return A_CMP_LT_OR_ZR;
    if ((x2 == 0x01) && (tb == true) && (ta == false) && (c == false))    
      return A_CMP4_GT_OR_ZR;
    if ((x2 == 0x01) && (tb == true) && (ta == false) && (c == true))    
      return A_CMP4_LE_OR_ZR;
    if ((x2 == 0x01) && (tb == true) && (ta == true) && (c == false))    
      return A_CMP4_GE_OR_ZR;
    if ((x2 == 0x01) && (tb == true) && (ta == true) && (c == true))    
      return A_CMP4_LT_OR_ZR;
    // decode cmp imm-reg
    if ((x2 == 0x02) && (ta == false) && (c == false))    
      return A_CMP_LTU_IR;
    if ((x2 == 0x02) && (ta == false) && (c == true))    
      return A_CMP_LTU_UNC_IR;
    if ((x2 == 0x02) && (ta == true) && (c == false))    
      return A_CMP_EQ_OR_IR;
    if ((x2 == 0x02) && (ta == true) && (c == true))    
      return A_CMP_NE_OR_IR;
    if ((x2 == 0x03) && (ta == false) && (c == false))    
      return A_CMP4_LTU_IR;
    if ((x2 == 0x03) && (ta == false) && (c == true))    
      return A_CMP4_LTU_UNC_IR;
    if ((x2 == 0x03) && (ta == true) && (c == false))    
      return A_CMP4_EQ_OR_IR;
    if ((x2 == 0x03) && (ta == true) && (c == true))    
      return A_CMP4_NE_OR_IR;
    return U_INVALID;
  }

  // decode major opcode E - A6 A7 A8
  static t_iopc get_opcode_group_E (t_octa inst) {
     t_byte x2 = ((t_byte) (inst >> 34)) &0x03;
     bool   tb = ((((t_byte) (inst >> 36)) & 0x01) == 0x01);
     bool   ta = ((((t_byte) (inst >> 33)) & 0x01) == 0x01);
     bool    c = ((((t_byte) (inst >> 12)) & 0x01) == 0x01);
     // decode cmp reg-reg
     if ((x2 == 0x00) && (tb == false) && (ta == false) && (c == false))    
       return A_CMP_EQ_RR;
     if ((x2 == 0x00) && (tb == false) && (ta == false) && (c == true))    
       return A_CMP_EQ_UNC_RR;
     if ((x2 == 0x00) && (tb == false) && (ta == true) && (c == false))    
       return A_CMP_EQ_OR_ANDCM_RR;
     if ((x2 == 0x00) && (tb == false) && (ta == true) && (c == true))    
       return A_CMP_NE_OR_ANDCM_RR;
     if ((x2 == 0x01) && (tb == false) && (ta == false) && (c == false))    
       return A_CMP4_EQ_RR;
     if ((x2 == 0x01) && (tb == false) && (ta == false) && (c == true))    
       return A_CMP4_EQ_UNC_RR;     
     if ((x2 == 0x01) && (tb == false) && (ta == true) && (c == false))    
       return A_CMP4_EQ_OR_ANDCM_RR;
     if ((x2 == 0x01) && (tb == false) && (ta == true) && (c == true))    
       return A_CMP4_NE_OR_ANDCM_RR;
     // decode cmp r0-reg
     if ((x2 == 0x00) && (tb == true) && (ta == false) && (c == false))    
       return A_CMP_GT_OR_ANDCM_ZR;
     if ((x2 == 0x00) && (tb == true) && (ta == false) && (c == true))    
       return  A_CMP_LE_OR_ANDCM_ZR;
     if ((x2 == 0x00) && (tb == true) && (ta == true) && (c == false))    
       return A_CMP_GE_OR_ANDCM_ZR;
     if ((x2 == 0x00) && (tb == true) && (ta == true) && (c == true))    
       return A_CMP_LT_OR_ANDCM_ZR;
     if ((x2 == 0x01) && (tb == true) && (ta == false) && (c == false))    
       return A_CMP4_GT_OR_ANDCM_ZR;
     if ((x2 == 0x01) && (tb == true) && (ta == false) && (c == true))    
       return A_CMP4_LE_OR_ANDCM_ZR;
     if ((x2 == 0x01) && (tb == true) && (ta == true) && (c == false))    
       return A_CMP4_GE_OR_ANDCM_ZR;
     if ((x2 == 0x01) && (tb == true) && (ta == true) && (c == true))    
       return A_CMP4_LT_OR_ANDCM_ZR;
     // decode cmp imm-reg
     if ((x2 == 0x02) && (ta == false) && (c == false))    
       return A_CMP_EQ_IR;
     if ((x2 == 0x02) && (ta == false) && (c == true))    
       return A_CMP_EQ_UNC_IR;
     if ((x2 == 0x02) && (ta == true) && (c == false))    
       return A_CMP_EQ_OR_ANDCM_IR;
     if ((x2 == 0x02) && (ta == true) && (c == true))    
       return A_CMP_NE_OR_ANDCM_IR;
     if ((x2 == 0x03) && (ta == false) && (c == false))    
      return A_CMP4_EQ_IR;
     if ((x2 == 0x03) && (ta == false) && (c == true))    
       return A_CMP4_EQ_UNC_IR;     
     if ((x2 == 0x03) && (ta == true) && (c == false))    
       return A_CMP4_EQ_OR_ANDCM_IR;
     if ((x2 == 0x03) && (ta == true) && (c == true))    
       return A_CMP4_NE_OR_ANDCM_IR;
     return U_INVALID;
  }

  // update a A unit instruction

  void Instr::aupdate (void) {
    if ((d_valid == true) || (d_opcd == U_INVALID)) return;
    // update predicate, source and destination registers
    switch (d_opcd) {
      
      // A1 instruction group
    case A_ADD:
    case A_ADD_1:
    case A_SUB:
    case A_SUB_1:
    case A_ADDP4:
    case A_AND:
    case A_ANDCM:
    case A_OR:
    case A_XOR:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_group = "A01";
      d_valid = true;
      break;

      // A2 instruction group
    case A_SHLADD:
    case A_SHLADDP4:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_immv[0] = get_shct (d_inst) + 1;
      d_group   = "A02";
      d_valid   = true;
      break;
	
      // A3 instruction group
    case A_SUB_I:
    case A_AND_I:
    case A_ANDCM_I:
    case A_OR_I:
    case A_XOR_I:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_immv[0] = get_im08b (d_inst);
      d_group   = "A03";
      d_valid   = true;
      break;
	
      // A4 instruction group
    case A_ADDS_I:
    case A_ADDP4_I:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_immv[0] = get_im14 (d_inst);
      d_group   = "A04";
      d_valid   = true;
      break;
	
      // A5 instruction group
    case A_ADDL_I:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_immv[0] = get_im22 (d_inst);
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_rsrc[0].setlnum (GREG, get_small_src (d_inst));
      d_group = "A05";
      d_valid = true;
      break;
	
      // A6 instruction group
    case A_CMP_LT_RR:
    case A_CMP_LTU_RR:
    case A_CMP_EQ_RR:
    case A_CMP_EQ_AND_RR:
    case A_CMP_EQ_OR_RR:
    case A_CMP_EQ_OR_ANDCM_RR:
    case A_CMP_NE_AND_RR:
    case A_CMP_NE_OR_RR:
    case A_CMP_NE_OR_ANDCM_RR:
    case A_CMP4_LT_RR:
    case A_CMP4_LTU_RR:
    case A_CMP4_EQ_RR:
    case A_CMP4_EQ_AND_RR:
    case A_CMP4_EQ_OR_RR:
    case A_CMP4_EQ_OR_ANDCM_RR:
    case A_CMP4_NE_AND_RR:
    case A_CMP4_NE_OR_RR:
    case A_CMP4_NE_OR_ANDCM_RR:
    d_rprd.setlnum    (PREG, get_pred (d_inst));
    d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
    d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
    d_rdst[0].setlnum (PREG, get_dst_pr0 (d_inst));
    d_rdst[1].setlnum (PREG, get_dst_pr1 (d_inst));
    d_group = "A06";
    d_valid = true;
      break;
    case A_CMP_LT_UNC_RR:
    case A_CMP_LTU_UNC_RR:
    case A_CMP_EQ_UNC_RR:
    case A_CMP4_LT_UNC_RR:
    case A_CMP4_LTU_UNC_RR:
    case A_CMP4_EQ_UNC_RR:
      d_rprd.setlnum    (PREG, 0);
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_rsrc[2].setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (PREG, get_dst_pr0 (d_inst));
      d_rdst[1].setlnum (PREG, get_dst_pr1 (d_inst));
      d_group = "A06";
      d_valid = true;
      break;
	
      // A7 instruction group
    case A_CMP_GT_AND_ZR:
    case A_CMP_GT_OR_ZR:
    case A_CMP_GT_OR_ANDCM_ZR:
    case A_CMP_LE_AND_ZR:
    case A_CMP_LE_OR_ZR:
    case A_CMP_LE_OR_ANDCM_ZR:
    case A_CMP_GE_AND_ZR:
    case A_CMP_GE_OR_ZR:
    case A_CMP_GE_OR_ANDCM_ZR:
    case A_CMP_LT_AND_ZR:
    case A_CMP_LT_OR_ZR:
    case A_CMP_LT_OR_ANDCM_ZR:
    case A_CMP4_GT_AND_ZR:
    case A_CMP4_GT_OR_ZR:
    case A_CMP4_GT_OR_ANDCM_ZR:
    case A_CMP4_LE_AND_ZR:
    case A_CMP4_LE_OR_ZR:
    case A_CMP4_LE_OR_ANDCM_ZR:
    case A_CMP4_GE_AND_ZR:
    case A_CMP4_GE_OR_ZR:
    case A_CMP4_GE_OR_ANDCM_ZR:
    case A_CMP4_LT_AND_ZR:
    case A_CMP4_LT_OR_ZR:
    case A_CMP4_LT_OR_ANDCM_ZR:
      // make sure we have source 0 set to 0
      if (get_src0 (d_inst) == 0) {
	d_rprd.setlnum    (PREG, get_pred (d_inst));
	d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
	d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
	d_rdst[0].setlnum (PREG, get_dst_pr0 (d_inst));
	d_rdst[1].setlnum (PREG, get_dst_pr1 (d_inst));
	d_group = "A07";
	d_valid = true;
      } else {
	d_opcd  = U_INVALID;
	d_valid = false;
      }
    break;
	
      // A8 instruction group
    case A_CMP_LT_IR:
    case A_CMP_LTU_IR:
    case A_CMP_EQ_IR:
    case A_CMP_EQ_AND_IR:
    case A_CMP_EQ_OR_IR:
    case A_CMP_EQ_OR_ANDCM_IR:
    case A_CMP_NE_AND_IR:
    case A_CMP_NE_OR_IR:
    case A_CMP_NE_OR_ANDCM_IR:
    case A_CMP4_LT_IR:
    case A_CMP4_LTU_IR:
    case A_CMP4_EQ_IR:
    case A_CMP4_EQ_AND_IR:
    case A_CMP4_EQ_OR_IR:
    case A_CMP4_EQ_OR_ANDCM_IR:
    case A_CMP4_NE_AND_IR:
    case A_CMP4_NE_OR_IR:
    case A_CMP4_NE_OR_ANDCM_IR:
    d_rprd.setlnum    (PREG, get_pred (d_inst));
    d_immv[0] = get_im08b (d_inst);
    d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
    d_rdst[0].setlnum (PREG, get_dst_pr0 (d_inst));
    d_rdst[1].setlnum (PREG, get_dst_pr1 (d_inst));
    d_group = "A08";
    d_valid = true;
      break;
    case A_CMP_LT_UNC_IR:
    case A_CMP_LTU_UNC_IR:
    case A_CMP_EQ_UNC_IR:
    case A_CMP4_LT_UNC_IR:
    case A_CMP4_LTU_UNC_IR:
    case A_CMP4_EQ_UNC_IR:
      d_rprd.setlnum    (PREG, 0);
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_rsrc[2].setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (PREG, get_dst_pr0 (d_inst));
      d_rdst[1].setlnum (PREG, get_dst_pr1 (d_inst));
      d_immv[0] = get_im08b (d_inst);
      d_group   = "A08";
      d_valid   = true;
      break;
	
      // A9 instruction group
    case A_PADD1:
    case A_PADD2:
    case A_PADD4:
    case A_PADD1_SSS:
    case A_PADD2_SSS:
    case A_PADD1_UUU:
    case A_PADD2_UUU:
    case A_PADD1_UUS:
    case A_PADD2_UUS:
    case A_PSUB1:
    case A_PSUB2:
    case A_PSUB4:
    case A_PSUB1_SSS:
    case A_PSUB2_SSS:
    case A_PSUB1_UUU:
    case A_PSUB2_UUU:
    case A_PSUB1_UUS:
    case A_PSUB2_UUS:
    case A_PAVG1:
    case A_PAVG2:
    case A_PAVG1_RAZ:
    case A_PAVG2_RAZ:
    case A_PAVGSUB1:
    case A_PAVGSUB2:
    case A_PCMP1_EQ:
    case A_PCMP2_EQ:
    case A_PCMP4_EQ:
    case A_PCMP1_GT:
    case A_PCMP2_GT:
    case A_PCMP4_GT:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_group = "A09";
      d_valid = true;
      break;
	
      // A10 instruction group
    case A_PSHLADD2:
    case A_PSHRADD2:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_immv[0] = get_shct (d_inst) + 1;
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_group = "A10";
      d_valid = true;
      break;
    default:
      break;
    }
  }

  // decode a A unit instruction

  void Instr::adecode (void) {
    assert (d_valid == false);
    assert ((d_bunit == MUNIT) || (d_bunit == IUNIT));
    // get instruction major opcode
    d_code = slot_getmopc (d_inst);
    switch (d_code) {
    case 0x08:
      // get instruction opcode
      d_opcd = get_opcode_group_8 (d_inst);
      break;
    case 0x09:
      d_opcd = A_ADDL_I;
      break;
    case 0x0C:
      d_opcd = get_opcode_group_C (d_inst);
      break;
    case 0x0D:
      d_opcd = get_opcode_group_D (d_inst);
      break;
    case 0x0E:
      d_opcd = get_opcode_group_E (d_inst);
      break;
    default:
      break;
    }
    // update instruction
    aupdate ();
    // mark as A unit
    d_funit = AUNIT;
  }
}
