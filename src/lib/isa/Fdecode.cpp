// ---------------------------------------------------------------------------
// - Fdecode.cpp                                                             -
// - iato:isa library - F unit instruction decode implementation             -
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

  // return the floating-point status field completer
  static t_fpcomp get_fpcomp (const t_octa inst) {
    t_byte fpc = ((t_byte) (inst >> 34)) & 0x03;
    if (fpc == 0x00) return S0;
    if (fpc == 0x01) return S1;
    if (fpc == 0x02) return S2;
    return S3;
  }

  // return the fclass9 value
  static t_octa get_fclass9 (const t_octa inst) {
    t_octa result = ((inst >> 18) & 0x00000000000001FCULL);
    return (result |=  ((inst >> 33) & 0x0000000000000003ULL));
  }

  // return the omask7c immediate field for F12
  static t_octa get_omask7c (const t_octa inst) {
    t_octa result = ((inst >> 20) & 0x000000000000007F3ULL);
    return result;
  }

  // return the amask7b immediate field for F12
  static t_octa get_amask7b (const t_octa inst) {
    t_octa result = ((inst >> 13) & 0x000000000000007F3ULL);
    return result;
  }

  // decode major opcode 0 - F07/F09 F15
  static t_iopc get_opcode_group_0 (t_octa inst) {
    // check X bit (33), Q bit (36) and X6 bits (27:32)
    bool   xb = (((t_byte) (inst >> 33)) & 0x01) == 0x01;
    bool   qb = (((t_byte) (inst >> 36)) & 0x01) == 0x01;
    t_byte x6 = ((t_byte) (inst >> 27)) & 0x3F;
    // decode frcpa
    if ((xb == true) && (qb == false)) return F_FRCPA;
    // decode floating point misc
    if ((xb == false) && (x6 == 0x00)) return F_BREAK;
    if ((xb == false) && (x6 == 0x01)) return F_NOP;
    if ((xb == false) && (x6 == 0x1C)) return F_FCVT_XF;
    // decode floating point merge
    if ((xb == false) && (x6 == 0x10)) return F_FMERGE_S;
    if ((xb == false) && (x6 == 0x11)) return F_FMERGE_NS;
    if ((xb == false) && (x6 == 0x12)) return F_FMERGE_SE;
    // decode floating point mix
    if ((xb == false) && (x6 == 0x39)) return F_FMIX_LR;
    if ((xb == false) && (x6 == 0x3A)) return F_FMIX_R;
    if ((xb == false) && (x6 == 0x3B)) return F_FMIX_L;
    if ((xb == false) && (x6 == 0x3C)) return F_FSXT_R;
    if ((xb == false) && (x6 == 0x3D)) return F_FSXT_L;
    // decode floating point pack
    if ((xb == false) && (x6 == 0x28)) return F_FPACK;
    // decode floating point swap
    if ((xb == false) && (x6 == 0x34)) return F_FSWAP;
    if ((xb == false) && (x6 == 0x36)) return F_FSWAP_NL;
    if ((xb == false) && (x6 == 0x35)) return F_FSWAP_NR;
    if ((xb == false) && (x6 == 0x2C)) return F_FAND;
    if ((xb == false) && (x6 == 0x2D)) return F_FANDCM;
    if ((xb == false) && (x6 == 0x2E)) return F_FOR;
    if ((xb == false) && (x6 == 0x2F)) return F_FXOR;
    // decode convert floating point to fixed point
    if ((xb == false) && (x6 == 0x18)) return F_FCVT_FX;
    if ((xb == false) && (x6 == 0x19)) return F_FCVT_FXU;
    if ((xb == false) && (x6 == 0x1A)) return F_FCVT_FX_TRUNC;
    if ((xb == false) && (x6 == 0x1B)) return F_FCVT_FXU_TRUNC;
    // decode floating point reciprocal square root approximation
    if ((xb == true) && (qb == true))  return F_FRSQRTA;
    // decode floating point min and max
    if ((xb == false) && (x6 == 0x14)) return F_FMIN;
    if ((xb == false) && (x6 == 0x15)) return F_FMAX;
    if ((xb == false) && (x6 == 0x16)) return F_FAMIN;
    if ((xb == false) && (x6 == 0x17)) return F_FAMAX;
    // decode floating point set controls
    if ((xb == false) && (x6 == 0x04)) return F_FSETC;
    // decode floating point clear flags
    if ((xb == false) && (x6 == 0x05)) return F_FCLRF ;
    // decode floating point check flags
    if ((xb == false) && (x6 == 0x08)) return F_FCHKF ;
    // mark as invalid to cancel update
    return U_INVALID;
  }

  // decode major opcode 1 - F07/F10
  static t_iopc get_opcode_group_1 (t_octa inst) {
    // check X bit (33) and X6 bits (27:32)
    bool   x  = (((t_byte) (inst >> 33)) & 0x01) == 0x01;
    bool   q  = (((t_byte) (inst >> 36)) & 0x01) == 0x01;
    t_byte x6 = ((t_byte) (inst >> 27)) & 0x3F;
    // decode floating misc
    if ((x == false) && (x6 == 0x10)) return F_FPMERGE_S;
    if ((x == false) && (x6 == 0x11)) return F_FPMERGE_NS;
    if ((x == false) && (x6 == 0x12)) return F_FPMERGE_SE;
    // decode convert floating point to fixed point
    if ((x == false) && (x6 == 0x18)) return F_FPCVT_FX;
    if ((x == false) && (x6 == 0x19)) return F_FPCVT_FXU;
    if ((x == false) && (x6 == 0x1A)) return F_FPCVT_FX_TRUNC;
    if ((x == false) && (x6 == 0x1B)) return F_FPCVT_FXU_TRUNC;
    // decode floating point reciprocal square root approximation
    if ((x == true) && (q == true))   return F_FPRSQRTA;
    // decode floatin point parallel min, max and compare
    if ((x == false) && (x6 == 0x14)) return F_FPMIN;
    if ((x == false) && (x6 == 0x15)) return F_FPMAX;
    if ((x == false) && (x6 == 0x16)) return F_FPAMIN;
    if ((x == false) && (x6 == 0x17)) return F_FPAMAX;
    if ((x == false) && (x6 == 0x30)) return F_FPCMP_EQ;
    if ((x == false) && (x6 == 0x31)) return F_FPCMP_LT;
    if ((x == false) && (x6 == 0x32)) return F_FPCMP_LE;
    if ((x == false) && (x6 == 0x33)) return F_FPCMP_UNORD;
    if ((x == false) && (x6 == 0x34)) return F_FPCMP_NEQ;
    if ((x == false) && (x6 == 0x35)) return F_FPCMP_NLT;
    if ((x == false) && (x6 == 0x36)) return F_FPCMP_NLE;
    if ((x == false) && (x6 == 0x37)) return F_FPCMP_ORD;
    // mark as invalid to cancel update
    return U_INVALID;
  }

  // decode major opcode 4 - F15
  static t_iopc get_opcode_group_4 (t_octa inst) {
    // check ra bit (33) rb bit (36) and ta bit (12)
    bool ra = ((((t_byte) (inst >> 33)) & 0x01) == 0x01);
    bool rb = ((((t_byte) (inst >> 36)) & 0x01) == 0x01);
    bool ta = ((((t_byte) (inst >> 12)) & 0x01) == 0x01);
    // check floating point compare
    if ((ra == false) && (rb == false) && (ta == false)) return F_FCMP_EQ;
    if ((ra == false) && (rb == true ) && (ta == false)) return F_FCMP_LT;
    if ((ra == true ) && (rb == false) && (ta == false)) return F_FCMP_LE;
    if ((ra == true ) && (rb == true ) && (ta == false)) return F_FCMP_UNORD;
    if ((ra == false) && (rb == false) && (ta == true )) return F_FCMP_EQ_UNC;
    if ((ra == false) && (rb == true ) && (ta == true )) return F_FCMP_LT_UNC;
    if ((ra == true ) && (rb == false) && (ta == true )) return F_FCMP_LE_UNC;
    if ((ra == true ) && (rb == true ) && (ta == true )) return F_FCMP_UNORD_UNC;
    // mark as invalid to cancel update
    return U_INVALID;
  }

  // decode major opcode 5 - F05
  static t_iopc get_opcode_group_5 (t_octa inst) {
    // check ta bit (12)
    bool ta = ((((t_byte) (inst >> 12)) & 0x01) == 0x01);
    // check floatting point class
    if (ta == false) return F_FCLASS_M;
    if (ta == true)  return F_FCLASS_M_UNC;
    // mark as invalid to cancel update
    return U_INVALID;
  }

  // decode major opcode 8
  static t_iopc get_opcode_group_8 (t_octa inst) {
    // check x bit (36)
    bool x = ((((t_byte) (inst >> 36)) & 0x01) == 0x01);
    // check floatting point multiply add
    if (x == false) return F_FMA;
    if (x == true)  return F_FMA_S;
    // mark as invalid to cancel update
    return U_INVALID;
  }

  // decode major opcode 9
  static t_iopc get_opcode_group_9 (t_octa inst) {
    // check x bit (36)
    bool x = (((t_byte) (inst >> 36)) & 0x01) == 0x01;
    // check floatting point multiply add
    if (x == false) return F_FMA_D;
    if (x == true)  return F_FPMA;
    // mark as invalid to cancel update
    return U_INVALID;
  }

  // decode major opcode A
  static t_iopc get_opcode_group_A (t_octa inst) {
    // check x bit (36)
    bool x = (((t_byte) (inst >> 36)) & 0x01) == 0x01;
    // check floatting point multiply add
    if (x == false) return F_FMS;
    if (x == true)  return F_FMS_S;
    // mark as invalid to cancel update
    return U_INVALID;
  }
  
  // decode major opcode B
  static t_iopc get_opcode_group_B (t_octa inst) {
    // check x bit (36)
    bool x = (((t_byte) (inst >> 36)) & 0x01) == 0x01;
    // check floatting point multiply add
    if (x == false) return F_FMS_D;
    if (x == true)  return F_FPMS;
    // mark as invalid to cancel update
    return U_INVALID;
  }

  // decode major opcode C
  static t_iopc get_opcode_group_C (t_octa inst) {
    // check x bit (36)
    bool x = (((t_byte) (inst >> 36)) & 0x01) == 0x01;
    // check floatting point multiply add
    if (x == false) return F_FNMA;
    if (x == true)  return F_FNMA_S;
    // mark as invalid to cancel update
    return U_INVALID;
  }

  // decode major opcode D
  static t_iopc get_opcode_group_D (t_octa inst) {
    // check x bit (36)
    bool x = (((t_byte) (inst >> 36)) & 0x01) == 0x01;
    // check floatting point multiply add
    if (x == false) return F_FNMA_D;
    if (x == true)  return F_FPNMA;
    // mark as invalid to cancel update
    return U_INVALID;
  }

  // decode major opcode E
  static t_iopc get_opcode_group_E (t_octa inst) {
    // check x bit (36) and x2 bits (34:35)
    bool   x  = (((t_byte) (inst >> 36)) & 0x01) == 0x01;
    t_byte x2 = ((t_byte) (inst >> 34)) & 0x03;
    // check fixed point multiply add
    if ((x == true ) && (x2 == 0x00)) return F_XMA_L;
    if ((x == true ) && (x2 == 0x03)) return F_XMA_H;
    if ((x == true ) && (x2 == 0x02)) return F_XMA_HU;
    if  (x == false)                  return F_FSELECT;
    // mark as invalid to cancel update
    return U_INVALID;
  }

  // update a F unit instruction

  void Instr::fupdate (void) {
    if ((d_valid == true) || (d_opcd == U_INVALID)) return;
    // update predicate, source and destination registers
    switch (d_opcd) {

      // F01 instruction group
    case F_FMA:
    case F_FMA_S:
    case F_FMA_D:
    case F_FPMA:
    case F_FMS:
    case F_FMS_S:
    case F_FMS_D:
    case F_FPMS:
    case F_FNMA:
    case F_FNMA_S:
    case F_FNMA_D:
    case F_FPNMA:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (FREG, get_dst0 (d_inst));   
      d_rsrc[0].setlnum (FREG, get_src0 (d_inst)); 
      d_rsrc[1].setlnum (FREG, get_src1 (d_inst)); 
      d_rsrc[2].setlnum (FREG, get_src2 (d_inst)); 
      d_rsrc[3].setlnum (AREG, AR_FPSR);
      d_fpcomp = get_fpcomp (d_inst);
      d_group  = "F01";
	d_valid  = true;
      break;

      // F02 instruction group
    case F_XMA_L:
    case F_XMA_H:
    case F_XMA_HU:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (FREG, get_dst0 (d_inst));   
      d_rsrc[0].setlnum (FREG, get_src0 (d_inst)); 
      d_rsrc[1].setlnum (FREG, get_src1 (d_inst)); 
      d_rsrc[2].setlnum (FREG, get_src2 (d_inst)); 
      d_group = "F02";
      d_valid = true;
      break;

      // F03 instruction group
    case F_FSELECT:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (FREG, get_dst0 (d_inst));   
      d_rsrc[0].setlnum (FREG, get_src0 (d_inst)); 
      d_rsrc[1].setlnum (FREG, get_src1 (d_inst)); 
      d_rsrc[2].setlnum (FREG, get_src2 (d_inst)); 
      d_group = "F03";
      d_valid = true;
      break;

      // F04 instruction group
    case F_FCMP_EQ:
    case F_FCMP_LT:
    case F_FCMP_LE:
    case F_FCMP_UNORD:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (PREG, get_dst_pr0 (d_inst));   
      d_rdst[1].setlnum (PREG, get_dst_pr1 (d_inst));   
      d_rsrc[0].setlnum (FREG, get_src0 (d_inst)); 
      d_rsrc[1].setlnum (FREG, get_src1 (d_inst)); 
      d_fpcomp = get_fpcomp (d_inst);
      d_group  = "F04";
      d_valid  = true;
      break;
    case F_FCMP_EQ_UNC:
    case F_FCMP_LT_UNC:
    case F_FCMP_LE_UNC:
    case F_FCMP_UNORD_UNC:
      d_rprd.setlnum (PREG, 0);
      d_rdst[0].setlnum (PREG, get_dst_pr0 (d_inst));   
      d_rdst[1].setlnum (PREG, get_dst_pr1 (d_inst));   
      d_rsrc[0].setlnum (FREG, get_src0 (d_inst)); 
      d_rsrc[1].setlnum (FREG, get_src1 (d_inst)); 
      d_rsrc[2].setlnum (PREG, get_pred (d_inst)); 
      d_fpcomp = get_fpcomp (d_inst);
      d_group  = "F04";
      d_valid = true;
      break;

      // F05 instruction group
    case F_FCLASS_M:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (FREG, get_src0     (d_inst));
      d_rdst[0].setlnum (PREG, get_dst_pr0  (d_inst));
      d_rdst[1].setlnum (PREG, get_dst_pr1  (d_inst));
      d_immv[0] = get_fclass9 (d_inst);
      d_group   = "F05";
      d_valid   = true;
      break;
    case F_FCLASS_M_UNC:
      d_rprd.setlnum (PREG, 0);
      d_rsrc[0].setlnum (FREG, get_src0     (d_inst));
      d_rsrc[1].setlnum (PREG, get_pred     (d_inst));
      d_rdst[0].setlnum (PREG, get_dst_pr0  (d_inst));
      d_rdst[1].setlnum (PREG, get_dst_pr1  (d_inst));
      d_immv[0] = get_fclass9 (d_inst);
      d_group   = "F05";
      d_valid   = true;
      break;
      
      // F06 instruction group
    case F_FRCPA:
      d_rprd.setlnum (PREG, 0);
      d_rdst[0].setlnum (FREG, get_dst0    (d_inst));   
      d_rdst[1].setlnum (PREG, get_dst_pr1 (d_inst));   
      d_rsrc[0].setlnum (FREG, get_src0    (d_inst)); 
      d_rsrc[1].setlnum (FREG, get_src1    (d_inst)); 
      d_rsrc[2].setlnum (PREG, get_pred    (d_inst)); 
      d_fpcomp = get_fpcomp (d_inst);
      d_group  = "F06";
      d_valid  = true;
      break;

      // F07 instruction group
    case F_FRSQRTA:
    case F_FPRSQRTA:
      d_rprd.setlnum    (PREG, 0);
      d_rsrc[1].setlnum (FREG, get_src1    (d_inst));
      d_rsrc[2].setlnum (PREG, get_pred    (d_inst));
      d_rdst[1].setlnum (PREG, get_dst_pr1 (d_inst));
      d_rdst[0].setlnum (FREG, get_dst0    (d_inst));
      d_fpcomp = get_fpcomp (d_inst);
      d_group  = "F07";
      d_valid  = true;
      break;

      // F08 instructtion group
    case F_FMIN:
    case F_FMAX:
    case F_FAMIN:
    case F_FAMAX:
    case F_FPMIN:
    case F_FPMAX:
    case F_FPAMIN:
    case F_FPAMAX:
    case F_FPCMP_EQ:
    case F_FPCMP_LT:
    case F_FPCMP_LE:
    case F_FPCMP_UNORD:
    case F_FPCMP_NEQ:
    case F_FPCMP_NLT:
    case F_FPCMP_NLE:
    case F_FPCMP_ORD:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (FREG, get_dst0 (d_inst));
      d_rsrc[0].setlnum (FREG, get_src0 (d_inst));
      d_rsrc[1].setlnum (FREG, get_src1 (d_inst));
      d_rsrc[2].setlnum (AREG, AR_FPSR);
      d_fpcomp = get_fpcomp (d_inst);
      d_group  = "F08";
      d_valid  = true;
      break;
      
      // F09 instruction group
    case F_FMERGE_S:
    case F_FMERGE_NS:
    case F_FMERGE_SE:
    case F_FMIX_LR:
    case F_FMIX_R:
    case F_FMIX_L:
    case F_FSXT_R:
    case F_FSXT_L:
    case F_FPACK:
    case F_FSWAP:
    case F_FSWAP_NL:
    case F_FSWAP_NR:
    case F_FAND:
    case F_FANDCM:
    case F_FOR:
    case F_FXOR:
    case F_FPMERGE_S:
    case F_FPMERGE_NS:
    case F_FPMERGE_SE:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (FREG, get_dst0 (d_inst));   
      d_rsrc[0].setlnum (FREG, get_src0 (d_inst)); 
      d_rsrc[1].setlnum (FREG, get_src1 (d_inst)); 
      d_group = "F09";
      d_valid = true;
      break;

      // F10 instruction group
    case F_FCVT_FX:
    case F_FCVT_FXU:
    case F_FCVT_FX_TRUNC:
    case F_FCVT_FXU_TRUNC:
    case F_FPCVT_FX:
    case F_FPCVT_FXU:
    case F_FPCVT_FX_TRUNC:
    case F_FPCVT_FXU_TRUNC:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (FREG, get_src0 (d_inst)); 
      d_rsrc[1].setlnum (AREG, AR_FPSR);
      d_rdst[0].setlnum (FREG, get_dst0 (d_inst)); 
      d_fpcomp = get_fpcomp (d_inst);
      d_group  = "F10";
      d_valid  = true;
      break;

      // F11 instruction group
    case F_FCVT_XF:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (FREG, get_src0 (d_inst)); 
      d_rdst[0].setlnum (FREG, get_dst0 (d_inst)); 
      d_group = "F11";
      d_valid = true;
      break;

      // F12 instruction group
    case F_FSETC:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (AREG, AR_FPSR);
      d_rdst[0].setlnum (AREG, AR_FPSR);
      d_immv[0] = get_omask7c (d_inst);
      d_immv[1] = get_amask7b (d_inst);
      d_fpcomp  = get_fpcomp  (d_inst);
      d_group   = "F12";
      d_valid   = true;
      break;
      
      // F13 instruction group
    case F_FCLRF:
     d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (AREG, AR_FPSR);
      d_rdst[0].setlnum (AREG, AR_FPSR);
      d_fpcomp = get_fpcomp (d_inst);
      d_group  = "F13";
      d_valid  = true;
      break;
      
      // F14 instruction group
    case F_FCHKF:
      d_rprd.setlnum(PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (AREG, AR_FPSR);
      d_fpcomp  = get_fpcomp (d_inst);
      d_immv[0] = get_i25a   (d_inst);
      d_group  = "F14";
      d_valid   = true;
      break;
      
      // F15 instruction group
    case F_BREAK:
    case F_NOP:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_immv[0] = get_im21 (d_inst);
      d_group   = "F15";
      d_valid   = true;
      break;

    default:
      break;
    }
  }

  // decode a F unit instruction
  
  void Instr::fdecode (void) {
    assert ((d_valid == false) && (d_bunit == FUNIT));
    // get instruction major opcode
    d_code = slot_getmopc (d_inst);
    switch (d_code) {
    case 0x00:
      d_opcd = get_opcode_group_0 (d_inst);
      break;
    case 0x01:
      d_opcd = get_opcode_group_1 (d_inst);
      break;
    case 0x04:
      d_opcd = get_opcode_group_4 (d_inst);
      break;
    case 0x05:
      d_opcd = get_opcode_group_5 (d_inst);
      break;
    case 0x08:
      d_opcd = get_opcode_group_8 (d_inst);
      break;
    case 0x09:
      d_opcd = get_opcode_group_9 (d_inst);
      break;
    case 0x0A:
      d_opcd = get_opcode_group_A (d_inst);
      break;
    case 0x0B:
      d_opcd = get_opcode_group_B (d_inst);
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
    fupdate ();
    // mark as F unit
    d_funit = FUNIT;
  }
}
