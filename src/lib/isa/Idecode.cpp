// ---------------------------------------------------------------------------
// - Idecode.cpp                                                             -
// - iato:isa library - I unit instruction decode implementation             -
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

  //return ct2d value
  static inline t_octa get_ct2d (const t_octa inst) {
    t_octa result = 1;
    t_octa count2 = ((inst >> 30) & 0x0000000000000003LL);
    if (count2 == 0) result = 0;
    else if (count2 == 1) result = 7;
    else if (count2 == 2) result = 15;
    else if (count2 == 3) result = 16;
    return result;
  }

  // return the mbt4c value
  static inline t_octa get_mbt4c (const t_octa inst) {
    return ((inst >> 20) & 0x000000000000000FLL);
  }

  // return the mht8c value
  static inline t_octa get_mht8c (const t_octa inst) {
    return ((inst >> 20) & 0x00000000000000FFLL);
  }

  // return the count5b value
  static inline t_octa get_count5b (const t_octa inst) {
    return ((inst >> 14) & 0x000000000000001FLL);
  }

 // return the count5c value
  static inline t_octa get_ccount5c (const t_octa inst) {
    return ((inst >> 20) & 0x000000000000001FLL);
  }


  // return the pos6b and cpos6b value
  static inline t_octa get_pos6b (const t_octa inst) {
    return ((inst >> 14) & 0x000000000000003FLL);
  }

  // return the cpos6c value
  static inline t_octa get_pos6c (const t_octa inst) {
    return ((inst >> 20) & 0x000000000000003FLL);
  }

  // return the cpos6d value
  static inline t_octa get_pos6d (const t_octa inst) {
    return ((inst >> 31) & 0x000000000000003FLL);
  }

  // return the len4d value
  static inline t_octa get_len4d (const t_octa inst) {
    return ((inst >> 27) & 0x000000000000000FLL);
  }

  // return the len6d and count6d value
  static inline t_octa get_len6d (const t_octa inst) {
    return ((inst >> 27) & 0x000000000000003FLL);
  }

  // return the tag13 value
  static t_octa get_tag13 (const t_octa inst) {
    t_octa result = (inst >> 24) & 0x00000000000000FFLL;
    bool      s = ((t_byte) (inst >> 32) & 0x01 == 0x01);
    if (s) result |= ~0x00000000000000FF;
    return (result << 4);
  }

  // returm the mask 17 value
  static inline t_octa get_mask17 (const t_octa inst) {
    t_octa result = ((inst >> 5) & 0x00000000000000FELL) |
      ((inst >> 16) & 0x000000000000FF00);
    if (get_sbit (inst) == true) result |= ~0x000000000000FFFFLL;
    return result;
  }

  // return the imm44 value
  static inline t_octa get_imm44 (const t_octa inst) {
    t_octa result = (inst & 0x00000001FFFFFFC0LL) << 10;
    if (get_sbit (inst) == true) result |= ~0x000007FFFFFFFFFFLL;
    return result;
  }

  // return the mov to br hint
  static t_mhint get_mhint (const t_octa inst) {
    t_byte mh = ((t_byte) (inst >> 20)) & 0x03;
    assert (mh != 0x03);
    if (mh == 0x00) return MSPTK;
    if (mh == 0x01) return MNONE;
    return MDPTK;
  }

  // return the mov to br important hint
  static t_ihint get_ihint (const t_octa inst) {
    t_byte ph = ((t_byte) (inst >> 23)) & 0x01;
    return (ph == 0x00) ? INONE : IIMP;
  }

  // decode major opcode 0 - I19/I29
  static t_iopc get_opcode_group_0 (t_octa inst) {
    // check X3 bits (33:35) and X6 bits (27:32)
    t_byte x  = ((t_byte) (inst >> 22)) & 0x01;
    t_byte x3 = ((t_byte) (inst >> 33)) & 0x07;
    t_byte x6 = ((t_byte) (inst >> 27)) & 0x3F;
    // decode integer misc
    if ((x3 == 0x00) && (x6 == 0x00)) return I_BREAK;
    if ((x3 == 0x00) && (x6 == 0x01)) return I_NOP;
    if ((x3 == 0x00) && (x6 == 0x30)) return I_MOV_FROM_IP;
    if ((x3 == 0x00) && (x6 == 0x31)) return I_MOV_FROM_B;
    if ((x3 == 0x00) && (x6 == 0x33)) return I_MOV_FROM_PR;
    if ((x3 == 0x07) && (x  == 0x00)) return I_MOV_TO_B; 
    if ((x3 == 0x07) && (x  == 0x01)) return I_MOV_RET_TO_B;
    if ((x3 == 0x00) && (x6 == 0x2A)) return I_MOV_TO_AR_R;
    if ((x3 == 0x00) && (x6 == 0x0A)) return I_MOV_TO_AR_I;
    if ((x3 == 0x00) && (x6 == 0x32)) return I_MOV_FROM_AR;
    if  (x3 == 0x03)                  return I_MOV_TO_PR;
    if  (x3 == 0x02)                  return I_MOV_TO_PR_ROT;
    // decode sign/zero extend/compute zero index
    if ((x3 == 0x00) && (x6 == 0x10)) return I_ZXT1;
    if ((x3 == 0x00) && (x6 == 0x11)) return I_ZXT2;
    if ((x3 == 0x00) && (x6 == 0x12)) return I_ZXT4;
    if ((x3 == 0x00) && (x6 == 0x14)) return I_SXT1;
    if ((x3 == 0x00) && (x6 == 0x15)) return I_SXT2;
    if ((x3 == 0x00) && (x6 == 0x16)) return I_SXT4;
    if ((x3 == 0x00) && (x6 == 0x18)) return I_CZX1_L;
    if ((x3 == 0x00) && (x6 == 0x19)) return I_CZX2_L;
    if ((x3 == 0x00) && (x6 == 0x1C)) return I_CZX1_R;
    if ((x3 == 0x00) && (x6 == 0x1D)) return I_CZX2_R;
    // decode integer speculation check
    if (x3 == 0x01) return I_CHK_S_I;
    // mark as invalid to cancel update
    return U_INVALID;
  }

   // decode major opcode 4 - I15
  static t_iopc get_opcode_group_4 (t_octa inst) {
    // decode deposit
    return I_DEP;
  }

  // decode major opcode 5 - I10 I11 I14 I16
  static t_iopc get_opcode_group_5 (t_octa inst) {
    // check tb bit (36) ta bit (33)
    // check x2 bits (34:35) x bit (33) y bit (13) c bit (12) 
    bool    tb = ((((t_byte) (inst >> 36)) & 0x01) == 0x01);
    t_byte  x2 =   ((t_byte) (inst >> 34)) & 0x03;
    bool     x = ((((t_byte) (inst >> 33)) & 0x01) == 0x01);
    bool    ta = ((((t_byte) (inst >> 33)) & 0x01) == 0x01);
    bool     y = ((((t_byte) (inst >> 13)) & 0x01) == 0x01);
    bool     c = ((((t_byte) (inst >> 12)) & 0x01) == 0x01);
    // decode shift right pair
    if ((x2 == 0x03) && (x == false)) return I_SHRP;
    // decode extract
    if ((x2 == 0x01) && (x == false) && (y == false)) return I_EXTR_U;
    if ((x2 == 0x01) && (x == false) && (y == true )) return I_EXTR;
    // decode test bit
    if ((x2 == 0x00) && (ta == false) && (tb == false) && (y == false) 
	&& (c == false)) return I_TBIT_Z;
    if ((x2 == 0x00) && (ta == false) && (tb == false) && (y == false) 
	&& (c == true)) return I_TBIT_Z_UNC;
    if ((x2 == 0x00) && (ta == false) && (tb == true) && (y == false) 
	&& (c == false)) return I_TBIT_Z_AND;
    if ((x2 == 0x00) && (ta == false) && (tb == true) && (y == false) 
	&& (c == true)) return I_TBIT_NZ_AND;
    if ((x2 == 0x00) && (ta == true) && (tb == false) && (y == false) 
	&& (c == false)) return I_TBIT_Z_OR;
    if ((x2 == 0x00) && (ta == true) && (tb == false) && (y == false) 
	&& (c == true)) return I_TBIT_NZ_OR;
    if ((x2 == 0x00) && (ta == true) && (tb == true) && (y == false) 
	&& (c == false)) return I_TBIT_Z_OR_ANDCM;
    if ((x2 == 0x00) && (ta == true) && (tb == true) && (y == false) 
	&& (c == true)) return I_TBIT_NZ_OR_ANDCM;
    // decode test nat
    if ((x2 == 0x00) && (ta == false) && (tb == false) && (y == true) 
	&& (c == false)) return I_TNAT_Z;
    if ((x2 == 0x00) && (ta == false) && (tb == false) && (y == true) 
	&& (c == true)) return I_TNAT_Z_UNC;
    if ((x2 == 0x00) && (ta == false) && (tb == true) && (y == true) 
	&& (c == false)) return I_TNAT_Z_AND;
    if ((x2 == 0x00) && (ta == false) && (tb == true) && (y == true) 
	&& (c == true)) return I_TNAT_NZ_AND;
    if ((x2 == 0x00) && (ta == true) && (tb == false) && (y == true) 
	&& (c == false)) return I_TNAT_Z_OR;
    if ((x2 == 0x00) && (ta == true) && (tb == false) && (y == true) 
	&& (c == true)) return I_TNAT_NZ_OR;
    if ((x2 == 0x00) && (ta == true) && (tb == true) && (y == true) 
	&& (c == false)) return I_TNAT_Z_OR_ANDCM;
    if ((x2 == 0x00) && (ta == true) && (tb == true) && (y == true) 
	&& (c == true)) return I_TNAT_NZ_OR_ANDCM;
    // decode zero and deposit / recompute y at bit 26
    y = ((((t_byte) (inst >> 26)) & 0x01) == 0x01);
    if ((x2 == 0x01) && (x == true ) && (y == false)) return I_DEP_Z;
    // decode zero and deposit immediate8
    if ((x2 == 0x01) && (x == true ) && (y == true )) return I_DEP_Z_I;
    // decode deposit immediate1
    if ((x2 == 0x03) && (x == true ))                 return I_DEP_I;
    return U_INVALID;
  }

  // decode major opcode 7 - I1->I9 I19 I29
  static t_iopc get_opcode_group_7 (t_octa inst) {
    // check za bit (36)  x2a bits (34:35) zb bit (33) ve bit (32)
    // x2c bits (30:31) and x2b bits (28:29)
    bool    za = ((((t_byte) (inst >> 36)) & 0x01) == 0x01);
    bool    zb = ((((t_byte) (inst >> 33)) & 0x01) == 0x01);
    bool    ve = ((((t_byte) (inst >> 32)) & 0x01) == 0x01);
    t_byte x2a =   ((t_byte) (inst >> 34)) & 0x03;
    t_byte x2b =   ((t_byte) (inst >> 28)) & 0x03;
    t_byte x2c =   ((t_byte) (inst >> 30)) & 0x03;
    // decode multimedia multiply and shift
    if ((za == false) && (zb == true ) && (ve == false) && (x2a == 0x00)
	&& (x2b == 0x03)) return I_PMPYSHR2;
    if ((za == false) && (zb == true ) && (ve == false) && (x2a == 0x00)
	&& (x2b == 0x01)) return I_PMPYSHR2_U;
    // decode parallel multiply
    if ((za == false) && (zb == true ) && (ve == false) && (x2a == 0x02)
	&& (x2b == 0x01) && (x2c == 0x03)) return I_PMPY2_R;
    if ((za == false) && (zb == true ) && (ve == false) && (x2a == 0x02)
	&& (x2b == 0x03) && (x2c == 0x03)) return I_PMPY2_L;
    // decode mix
    if ((za == false) && (zb == false) && (ve == false) && (x2a == 0x02)
	&& (x2b == 0x00) && (x2c == 0x02)) return I_MIX1_R;
    if ((za == false) && (zb == true ) && (ve == false) && (x2a == 0x02)
	&& (x2b == 0x00) && (x2c == 0x02)) return I_MIX2_R;
    if ((za == true ) && (zb == false) && (ve == false) && (x2a == 0x02)
	&& (x2b == 0x00) && (x2c == 0x02)) return I_MIX4_R;
    if ((za == false) && (zb == false) && (ve == false) && (x2a == 0x02)
	&& (x2b == 0x02) && (x2c == 0x02)) return I_MIX1_L;
    if ((za == false) && (zb == true ) && (ve == false) && (x2a == 0x02)
	&& (x2b == 0x02) && (x2c == 0x02)) return I_MIX2_L;
    if ((za == true ) && (zb == false) && (ve == false) && (x2a == 0x02)
	&& (x2b == 0x02) && (x2c == 0x02)) return I_MIX4_L;
    // decode pack
    if ((za == false) && (zb == true ) && (ve == false) && (x2a == 0x02)
	&& (x2b == 0x00) && (x2c == 0x00)) return I_PACK2_USS;
    if ((za == false) && (zb == true ) && (ve == false) && (x2a == 0x02)
	&& (x2b == 0x02) && (x2c == 0x00)) return I_PACK2_SSS;
    if ((za == true ) && (zb == false) && (ve == false) && (x2a == 0x02)
	&& (x2b == 0x02) && (x2c == 0x00)) return I_PACK4_SSS;
    // decode unpack
    if ((za == false) && (zb == false) && (ve == false) && (x2a == 0x02)
	&& (x2b == 0x00) && (x2c == 0x01)) return I_UNPACK1_H;
    if ((za == false) && (zb == true ) && (ve == false) && (x2a == 0x02)
	&& (x2b == 0x00) && (x2c == 0x01)) return I_UNPACK2_H;
    if ((za == true ) && (zb == false) && (ve == false) && (x2a == 0x02)
	&& (x2b == 0x00) && (x2c == 0x01)) return I_UNPACK4_H;
    if ((za == false) && (zb == false) && (ve == false) && (x2a == 0x02)
	&& (x2b == 0x02) && (x2c == 0x01)) return I_UNPACK1_L;
    if ((za == false) && (zb == true ) && (ve == false) && (x2a == 0x02)
	&& (x2b == 0x02) && (x2c == 0x01)) return I_UNPACK2_L;
    if ((za == true ) && (zb == false) && (ve == false) && (x2a == 0x02)
	&& (x2b == 0x02) && (x2c == 0x01)) return I_UNPACK4_L;
    // decode parallel minimum
    if ((za == false) && (zb == false) && (ve == false) && (x2a == 0x02)
	&& (x2b == 0x01) && (x2c == 0x00)) return I_PMIN1_U;
    if ((za == false) && (zb == true ) && (ve == false) && (x2a == 0x02)
	&& (x2b == 0x03) && (x2c == 0x00)) return I_PMIN2;
    // decode parallel maximum
    if ((za == false) && (zb == false) && (ve == false) && (x2a == 0x02)
	&& (x2b == 0x01) && (x2c == 0x01)) return I_PMAX1_U;
    if ((za == false) && (zb == true ) && (ve == false) && (x2a == 0x02)
	&& (x2b == 0x03) && (x2c == 0x01)) return I_PMAX2;
    // decode parallel sum of absolute difference
    if ((za == false) && (zb == false) && (ve == false) && (x2a == 0x02)
	&& (x2b == 0x03) && (x2c == 0x02)) return I_PSAD1;
    // decode multimedia mux1
    if ((za == false) && (zb == false) && (ve == false) && (x2a == 0x03)
	&& (x2b == 0x02) && (x2c == 0x02)) return I_MUX1;
    // decode multimedia mux2
    if ((za == false) && (zb == true ) && (ve == false) && (x2a == 0x03)
	&& (x2b == 0x02) && (x2c == 0x02)) return I_MUX2;
    // decode shift right variable
    if ((za == false) && (zb == true ) && (ve == false) && (x2a == 0x00)
	&& (x2b == 0x02) && (x2c == 0x00)) return I_PSHR2;
    if ((za == true ) && (zb == false) && (ve == false) && (x2a == 0x00)
	&& (x2b == 0x02) && (x2c == 0x00)) return I_PSHR4;
    if ((za == true ) && (zb == true ) && (ve == false) && (x2a == 0x00)
	&& (x2b == 0x02) && (x2c == 0x00)) return I_SHR;
    if ((za == false) && (zb == true ) && (ve == false) && (x2a == 0x00)
	&& (x2b == 0x00) && (x2c == 0x00)) return I_PSHR2_U;
    if ((za == true ) && (zb == false) && (ve == false) && (x2a == 0x00)
	&& (x2b == 0x00) && (x2c == 0x00)) return I_PSHR4_U;
    if ((za == true ) && (zb == true ) && (ve == false) && (x2a == 0x00)
	&& (x2b == 0x00) && (x2c == 0x00)) return I_SHR_U;
    // decode multimedia shift right - fixed
    if ((za == false) && (zb == true ) && (ve == false) && (x2a == 0x01)
	&& (x2b == 0x03) && (x2c == 0x00)) return I_PSHR2_I;
    if ((za == true ) && (zb == false) && (ve == false) && (x2a == 0x01)
	&& (x2b == 0x03) && (x2c == 0x00)) return I_PSHR4_I;
    if ((za == false) && (zb == true ) && (ve == false) && (x2a == 0x01)
	&& (x2b == 0x01) && (x2c == 0x00)) return I_PSHR2_U_I;
    if ((za == true ) && (zb == false) && (ve == false) && (x2a == 0x01)
	&& (x2b == 0x01) && (x2c == 0x00)) return I_PSHR4_U_I;
    // decode shift left variable
    if ((za == false) && (zb == true ) && (ve == false) && (x2a == 0x00)
	&& (x2b == 0x00) && (x2c == 0x01)) return I_PSHL2;
    if ((za == true ) && (zb == false) && (ve == false) && (x2a == 0x00)
	&& (x2b == 0x00) && (x2c == 0x01)) return I_PSHL4;
    if ((za == true ) && (zb == true ) && (ve == false) && (x2a == 0x00)
	&& (x2b == 0x00) && (x2c == 0x01)) return I_SHL;
    // decode multimedia shift left - fixed
    if ((za == false) && (zb == true ) && (ve == false) && (x2a == 0x03)
	&& (x2b == 0x01) && (x2c == 0x01)) return I_PSHL2_I;
    if ((za == true ) && (zb == false) && (ve == false) && (x2a == 0x03)
	&& (x2b == 0x01) && (x2c == 0x01)) return I_PSHL4_I;
    // decode population count
    if ((za == false) && (zb == true ) && (ve == false) && (x2a == 0x01)
	&& (x2b == 0x01) && (x2c == 0x02)) return I_POPCNT;
    // mark as invalid to cancel update
    return U_INVALID;
  }

  // update a I unit instruction

  void Instr::iupdate (void) {
    if ((d_valid == true) || (d_opcd == U_INVALID)) return;
    // update predicate, source and destination registers
    switch (d_opcd) {

      // I1 instruction group
    case I_PMPYSHR2_U:
    case I_PMPYSHR2:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_immv[0] = get_ct2d (d_inst);
      d_group   = "I01";
      d_valid   = true;
      break;

      // I2 instruction group
    case I_PMPY2_R:
    case I_PMPY2_L:
    case I_MIX1_R:
    case I_MIX2_R:
    case I_MIX4_R:
    case I_MIX1_L:
    case I_MIX2_L:
    case I_MIX4_L:
    case I_PACK2_USS:
    case I_PACK2_SSS:
    case I_PACK4_SSS:
    case I_UNPACK1_H:
    case I_UNPACK2_H:
    case I_UNPACK4_H:
    case I_UNPACK1_L:
    case I_UNPACK2_L:
    case I_UNPACK4_L:
    case I_PMIN1_U:
    case I_PMAX1_U:
    case I_PMIN2:
    case I_PMAX2:
    case I_PSAD1:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_group = "I02";
      d_valid = true;
      break;

      // I3 instruction group
    case I_MUX1:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_immv[0] = get_mbt4c (d_inst);
      d_group   = "I03";
      d_valid   = true;
      break;

      // I4 instruction group
    case I_MUX2:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_immv[0] = get_mht8c (d_inst);
      d_group   = "I04";
      d_valid   = true;
      break;

      // I5 instruction group
    case I_PSHR2:
    case I_PSHR4:
    case I_SHR:
    case I_PSHR2_U:
    case I_PSHR4_U:
    case I_SHR_U:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_group = "I05";
      d_valid = true;
      break;

      // I6 instruction group
    case I_PSHR2_I:
    case I_PSHR4_I:
    case I_PSHR2_U_I:
    case I_PSHR4_U_I:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_immv [0] = get_count5b (d_inst);
      d_group    = "I06";
      d_valid    = true;
      break;

      // I7 instruction group
    case I_PSHL2:
    case I_PSHL4:
    case I_SHL:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_group = "I07";
      d_valid = true;
      break;

      // I8 instruction group
    case I_PSHL2_I:
    case I_PSHL4_I:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_immv[0] = 31 - get_ccount5c (d_inst);
      d_group   = "I08";
      d_valid   = true;
      break;

      // I9 instruction group
    case I_POPCNT:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_immv[0] = get_src0 (d_inst);
      d_group   = "I09";
      d_valid   = true;
      assert (get_src0 (d_inst) == 0);
      break;

      // I10 instruction group
    case I_SHRP:
       d_rprd.setlnum (PREG, get_pred (d_inst));
       d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
       d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
       d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
       d_immv[0] = get_len6d (d_inst);
       d_group   = "I10";
       d_valid   = true;
      break;

      // I11 instruction group
    case I_EXTR:
    case I_EXTR_U:
       d_rprd.setlnum (PREG, get_pred (d_inst));
       d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
       d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
       d_immv[0] = get_pos6b (d_inst);
       d_immv[1] = get_len6d (d_inst) + 1;
       d_group   = "I11";
       d_valid   = true;
      break;

      // I12 instruction group
    case I_DEP_Z:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_immv[0] = 63 - get_pos6c (d_inst);
      d_immv[1] = get_len6d (d_inst) + 1;
      d_group   = "I12";
      d_valid   = true;
      break;

      // I13 instruction group
    case I_DEP_Z_I:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_immv[0] = get_im08b (d_inst);
      d_immv[1] = 63 - get_pos6c (d_inst);
      d_immv[2] = get_len6d (d_inst) + 1;
      d_group   = "I13";
      d_valid   = true;
      break;

      // I14 instruction group
    case I_DEP_I:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_immv[0] = 63 - get_pos6b (d_inst);
      d_immv[1] = get_len6d (d_inst) + 1;
      d_immv[2] = get_sbit(d_inst)? -1: 0;
      d_group   = "I14";
      d_valid   = true;
      break;

      // I15 instruction group
    case I_DEP:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_immv[0] = get_len4d (d_inst) + 1;
      d_immv[1] = 63 - get_pos6d (d_inst);
      d_group   = "I15";
      d_valid   = true;
      break;

      // I16 instruction group
    case I_TBIT_Z:
    case I_TBIT_Z_AND:
    case I_TBIT_NZ_AND:
    case I_TBIT_Z_OR:
    case I_TBIT_NZ_OR:
    case I_TBIT_Z_OR_ANDCM:
    case I_TBIT_NZ_OR_ANDCM:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (PREG, get_dst_pr0 (d_inst));
      d_rdst[1].setlnum (PREG, get_dst_pr1 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_immv[0] = get_pos6b (d_inst);
      d_group   = "I16";
      d_valid   = true;
      break;
    case I_TBIT_Z_UNC:
      d_rprd.setlnum (PREG, 0);
      d_rdst[0].setlnum (PREG, get_dst_pr0 (d_inst));
      d_rdst[1].setlnum (PREG, get_dst_pr1 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_rsrc[2].setlnum (PREG, get_pred (d_inst));
      d_immv[0] = get_pos6b (d_inst);
      d_group   = "I16";
      d_valid   = true;
      break;

      // I17 instruction group
    case I_TNAT_Z:
    case I_TNAT_Z_AND:
    case I_TNAT_NZ_AND:
    case I_TNAT_Z_OR:
    case I_TNAT_NZ_OR:
    case I_TNAT_Z_OR_ANDCM:
    case I_TNAT_NZ_OR_ANDCM:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (PREG, get_dst_pr0 (d_inst));
      d_rdst[1].setlnum (PREG, get_dst_pr1 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_group = "I17";
      d_valid = true;
      break;
    case I_TNAT_Z_UNC:
      d_rprd.setlnum (PREG, 0);
      d_rdst[0].setlnum (PREG, get_dst_pr0 (d_inst));
      d_rdst[1].setlnum (PREG, get_dst_pr1 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_rsrc[2].setlnum (PREG, get_pred (d_inst));
      d_group = "I17";
      d_valid = true;
      break;

      // I19 instruction group
    case I_BREAK:
    case I_NOP:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_immv[0] = get_i20a (d_inst);
      d_group   = "I19";
      d_valid   = true;
      break;

      // I20 instruction group
    case I_CHK_S_I:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (IPRG, 0);
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_immv[0] = get_spectg (d_inst);
      d_group   = "I20";
      d_valid   = true;
      break;

      // I21 instruction group
    case I_MOV_TO_B:
    case I_MOV_RET_TO_B:
       d_rprd.setlnum (PREG, get_pred (d_inst));
       d_rdst[0].setlnum (BREG, get_dst_br (d_inst));
       d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
       d_immv[0] = get_tag13 (d_inst);
       d_mhint = get_mhint(d_inst);
       d_ihint = get_ihint(d_inst);
       d_group = "I21";
       d_valid = true;
      break;

      // I22 instruction group
    case I_MOV_FROM_B:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_rsrc[0].setlnum (BREG, get_src_br (d_inst));
      d_group = "I22";
      d_valid = true;
      break;

      // I23 instruction group
    case I_MOV_TO_PR:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (PRRG, 0);
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_rsrc[1].setlnum (PRRG, 0);
      d_immv[0] = get_mask17 (d_inst);
      d_group   = "I23";
      d_valid   = true;
      break;

      // I24 instruction group
    case I_MOV_TO_PR_ROT:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (PROT, 0);
      d_immv[0] = get_imm44 (d_inst);
      d_group   = "I24";
      d_valid   = true;
      break;

      // I25 instruction group
    case I_MOV_FROM_IP:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_group = "I25";
      d_valid = true;
      break;
    case I_MOV_FROM_PR:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (PRRG, 0);      
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_group = "I25";
      d_valid = true;
      break;

      // I26 instruction group
    case I_MOV_TO_AR_R:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (AREG, get_src1 (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_group = "I26";
      d_valid = true;
      break;

      // I27 instruction group
    case I_MOV_TO_AR_I:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (AREG, get_src1 (d_inst));
      d_immv[0] =  get_im08b (d_inst);
      d_group = "I27";
      d_valid = true;
      break;

      // I28 instruction group
    case I_MOV_FROM_AR:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_rsrc[1].setlnum (AREG, get_src1 (d_inst));
      d_group = "I28";
      d_valid = true;
      break;

      // I29 instruction group
    case I_ZXT1:
    case I_ZXT2:
    case I_ZXT4:
    case I_SXT1:
    case I_SXT2:
    case I_SXT4:
    case I_CZX1_L:
    case I_CZX2_L:
    case I_CZX1_R:
    case I_CZX2_R:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_valid = true;
      d_group = "I29";
      break;
    default:
      break;
    }
  }

  // decode a I unit instruction
  
  void Instr::idecode (void) {
    assert ((d_valid == false) && (d_bunit == IUNIT));
    // get instruction major opcode
    d_code = slot_getmopc (d_inst);
    switch (d_code) {
    case 0x00:
      d_opcd = get_opcode_group_0 (d_inst);
      break;
    case 0x04:
      d_opcd = get_opcode_group_4 (d_inst);
      break;
    case 0x05:
      d_opcd = get_opcode_group_5 (d_inst);
      break;
    case 0x07:
      d_opcd = get_opcode_group_7 (d_inst);
      break;
    default:
      break;
    }    
    // update instruction
    iupdate ();
    // mark as I unit
    d_funit = IUNIT;
  }
}
