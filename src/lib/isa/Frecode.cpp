// ---------------------------------------------------------------------------
// - Frecode.cpp                                                             -
// - iato:isa library - F unit instruction recode implementation             -
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

  // return a string representation of the floatting point field completer
  static string to_fpcomp (t_fpcomp fpc) {
    string result;
    if (fpc == S0) result += ".s0";
    if (fpc == S1) result += ".s1";
    if (fpc == S2) result += ".s2";
    if (fpc == S3) result += ".s3";
    return result;
  }

  // recode a F01 instruction
  string Instr::frecode_01 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case F_FMA:
      result += "fma" + to_fpcomp(d_fpcomp) + ' ';
      break;
    case F_FMA_S:
      result += "fma.s" + to_fpcomp(d_fpcomp) + ' ';
      break;
    case F_FMA_D:
      result += "fma.d" + to_fpcomp(d_fpcomp) + ' ';
      break;
    case F_FPMA:
      result += "fpma" + to_fpcomp(d_fpcomp) + ' ';
      break;
    case F_FMS:
      result += "fms" + to_fpcomp(d_fpcomp) + ' ';
      break;
    case F_FMS_S:
      result += "fms.s" + to_fpcomp(d_fpcomp) + ' ';
      break;
    case F_FMS_D:
      result += "fms.d" + to_fpcomp(d_fpcomp) + ' ';
      break;
    case F_FPMS:
      result += "fpms" + to_fpcomp(d_fpcomp) + ' ';
      break;
    case F_FNMA:
      result += "fnma" + to_fpcomp(d_fpcomp) + ' ';
      break;
    case F_FNMA_S:
      result += "fnma.s" + to_fpcomp(d_fpcomp) + ' ';
      break;
    case F_FNMA_D:
      result += "fnma.d" + to_fpcomp(d_fpcomp) + ' ';
      break;
    case F_FPNMA:
      result += "fpnma" + to_fpcomp(d_fpcomp) + ' ';
      break;
    default:
      break;
    }
    result += to_freg(d_rdst[0].getlnum ()) + '=';
    result += to_freg(d_rsrc[1].getlnum ()) + ',';
    result += to_freg(d_rsrc[2].getlnum ()) + ',';
    result += to_freg(d_rsrc[0].getlnum ());
    return result;
  }

  // recode a F01 pseudo instruction
  string Instr::frecode_01_pseudo (void) const {
    string result = to_pred (d_rprd.getlnum ());
    bool flag = (d_rsrc[2].getlnum () == 1);
    switch (d_opcd) {
    case F_FMA:
      if (flag)	result += "fnorm" + to_fpcomp(d_fpcomp) + ' ';
      else result += "fmpy" + to_fpcomp(d_fpcomp) + ' ';
      break;
    case F_FMA_S:
      if (flag)	result += "fnorm.s" + to_fpcomp(d_fpcomp) + ' ';
      else result += "fmpy.s" + to_fpcomp(d_fpcomp) + ' ';
      break;
    case F_FMA_D:
      if (flag)	result += "fnorm.d" + to_fpcomp(d_fpcomp) + ' ';
      else result += "fmpy.d" + to_fpcomp(d_fpcomp) + ' ';
      break;
    default:
      break;
    }
    result += to_freg(d_rdst[0].getlnum ()) + '=';
    result += to_freg(d_rsrc[1].getlnum ());
    if (!flag) result += ',' + to_freg(d_rsrc[2].getlnum ());
    return result;
  }

  // recode a F02 instruction
  string Instr::frecode_02 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case F_XMA_L:
      result += "xma.l ";
      break;
    case F_XMA_H:
      result += "xma.h ";
      break;
    case F_XMA_HU:
      result += "xma.hu ";
      break;
    default:
      break;
    }
    result += to_freg(d_rdst[0].getlnum ()) + '=';
    result += to_freg(d_rsrc[1].getlnum ()) + ',';
    result += to_freg(d_rsrc[2].getlnum ()) + ',';
    result += to_freg(d_rsrc[0].getlnum ());
    return result;
  }

  // recode a F03 instruction
  string Instr::frecode_03 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case F_FSELECT:
      result += "fselect ";
      break;
    default:
      break;
    }
    result += to_freg(d_rdst[0].getlnum ()) + '=';
    result += to_freg(d_rsrc[1].getlnum ()) + ',';
    result += to_freg(d_rsrc[2].getlnum ()) + ',';
    result += to_freg(d_rsrc[0].getlnum ());
    return result;
  }

  // recode a F04 instruction
  string Instr::frecode_04 (void) const {
    string result;
     switch (d_opcd) {
     case F_FCMP_EQ:
       result = to_pred (d_rprd.getlnum ());
       result += "fcmp.eq" + to_fpcomp(d_fpcomp) + ' ';
       break;
     case F_FCMP_LT:
       result = to_pred (d_rprd.getlnum ());
       result += "fcmp.lt" + to_fpcomp(d_fpcomp) + ' ';
       break;
     case F_FCMP_LE:
       result = to_pred (d_rprd.getlnum ());
       result += "fcmp.le" + to_fpcomp(d_fpcomp) + ' ';
       break;
     case F_FCMP_UNORD:
       result = to_pred (d_rprd.getlnum ());
       result += "fcmp.unord" + to_fpcomp(d_fpcomp) + ' ';
       break;
     case F_FCMP_EQ_UNC:
       result = to_pred (d_rsrc[2].getlnum ());
       result += "fcmp.eq.unc" + to_fpcomp(d_fpcomp) + ' ';
       break;
     case F_FCMP_LT_UNC:
       result = to_pred (d_rsrc[2].getlnum ());
       result += "fcmp.lt.unc" + to_fpcomp(d_fpcomp) + ' ';
       break;
     case F_FCMP_LE_UNC:
       result = to_pred (d_rsrc[2].getlnum ());
       result += "fcmp.le.unc" + to_fpcomp(d_fpcomp) + ' ';
       break;
     case F_FCMP_UNORD_UNC:
       result = to_pred (d_rsrc[2].getlnum ());
       result += "fcmp.unord.unc" + to_fpcomp(d_fpcomp) + ' ';
       break;
     default:
       break;
     }
    result += to_preg (d_rdst[0].getlnum ()) + ',';
    result += to_preg (d_rdst[1].getlnum ());
    result += '=' + to_freg(d_rsrc[0].getlnum ()) + ',';
    result += to_freg(d_rsrc[1].getlnum ());
    return result;
  }

  // recode a F05 instruction
  string Instr::frecode_05 (void) const {
    string result;
    switch (d_opcd){
    case F_FCLASS_M:
      result = to_pred (d_rprd.getlnum ()) + "fclass.m ";
      break;
    case F_FCLASS_M_UNC:
      result = to_pred (d_rsrc[1].getlnum ()) + "fclass.m.unc ";
      break;
    default:
      assert (false);
      break;
    }
    result += to_preg (d_rdst[0].getlnum ()) + ',';
    result += to_preg (d_rdst[1].getlnum ());
    result += '=' + to_freg(d_rsrc[0].getlnum ()) + ',';
    result += to_immv (d_immv[0], false, true);
    return result;
  }

  // recode a F06 instruction
  string Instr::frecode_06 (void) const {
    string result = to_pred (d_rsrc[2].getlnum ());
    switch (d_opcd) {
    case F_FRCPA:
      result += "frpca" + to_fpcomp(d_fpcomp) + ' ';
      break;
    default:
      assert (false);
      break;
    }
    result += to_freg (d_rdst[0].getlnum ()) + ',';
    result += to_preg (d_rdst[1].getlnum ()) + '=';
    result += to_freg (d_rsrc[0].getlnum ()) + ',';
    result += to_freg (d_rsrc[1].getlnum ());
    return result;
  }

  // recode a F07 instruction
  string Instr::frecode_07 (void) const {
    string result = to_pred (d_rsrc[2].getlnum ());
    switch (d_opcd) {
    case F_FRSQRTA:
      result += "frsqrta" + to_fpcomp(d_fpcomp) + ' ';
      break;
    case F_FPRSQRTA:
      result += "fprsqrta" + to_fpcomp(d_fpcomp) + ' ';
      break;
    default:
      assert (false);
      break;
    }
    result += to_freg (d_rdst[0].getlnum ()) + ',';
    result += to_preg (d_rdst[1].getlnum ()) + '=';
    result += to_freg (d_rsrc[1].getlnum ());
    return result;
  }

  // recode a F08 instruction
  string Instr::frecode_08 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case F_FMIN:
      result += "fmin" + to_fpcomp (d_fpcomp) + ' ';
      break;
    case F_FMAX:
      result += "fmax" + to_fpcomp (d_fpcomp) + ' ';
      break;
    case F_FPMIN:
      result += "fpmin" + to_fpcomp (d_fpcomp) + ' ';
      break;
    case F_FPMAX:
      result += "fpmax" + to_fpcomp (d_fpcomp) + ' ';
      break;
    case F_FPCMP_EQ:
      result += "fpcmp.eq" + to_fpcomp (d_fpcomp) + ' ';
      break;
    case F_FPCMP_LT:
      result += "fpcmp.lt" + to_fpcomp (d_fpcomp) + ' ';
      break;
    case F_FPCMP_LE:
      result += "fpcmp.le" + to_fpcomp (d_fpcomp) + ' ';
      break;
    case F_FPCMP_UNORD:
      result += "fpcmp.unord" + to_fpcomp (d_fpcomp) + ' ';
      break;
    case F_FPCMP_NEQ:
      result += "fpcmp.neq" + to_fpcomp (d_fpcomp) + ' ';
      break;
    case F_FPCMP_NLT:
      result += "fpcmp.nlt" + to_fpcomp (d_fpcomp) + ' ';
      break;
    case F_FPCMP_NLE:
      result += "fpcmp.nle" + to_fpcomp (d_fpcomp) + ' ';
      break;
    case F_FPCMP_ORD:
      result += "fpcmp.ord" + to_fpcomp (d_fpcomp) + ' ';
      break;
    default:
      assert (false);
      break;
    }
    result += to_freg (d_rdst[0].getlnum ()) + '=';
    result += to_freg (d_rsrc[0].getlnum ()) + ',';
    result += to_freg (d_rsrc[1].getlnum ());
    return result;
  }

  // recode a F09 instruction
  string Instr::frecode_09 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case F_FMERGE_S:
      result += "fmerge.s ";
      break;
    case F_FMERGE_NS:
      result += "fmerge.ns ";
      break;
    case F_FMERGE_SE:
      result += "fmerge.se ";
      break;
    case F_FMIX_LR:
      result += "fmix.lr ";
      break;
    case F_FMIX_R:
      result += "fmix.r ";
      break;
    case F_FMIX_L:
      result += "fmix.l ";
      break;
    case F_FSXT_R:
      result += "fsxt.r ";
      break;
    case F_FSXT_L:
      result += "fsxt.l ";
      break;
    case F_FPACK:
      result += "fpack ";
      break;
    case F_FSWAP:
      result += "fswap ";
      break;
    case F_FSWAP_NL:
      result += "fswap.nl ";
      break;
    case F_FSWAP_NR:
      result += "fswap.nr ";
      break;
    case F_FAND:
      result += "fand ";
      break;
    case F_FANDCM:
      result += "fandcm ";
      break;
    case F_FOR:
      result += "for ";
      break;
    case F_FXOR:
      result += "fxor ";
      break;
    case F_FPMERGE_S:
      result += "fmerge.s ";
      break;
    case F_FPMERGE_NS:
      result += "fmerge.ns ";
      break;
    case F_FPMERGE_SE:
      result += "fmerge.se ";
      break;
    default:
      break;
    }
    result += to_freg (d_rdst[0].getlnum ());
    result += '=' + to_freg (d_rsrc[0].getlnum ());
    result += ',' + to_freg (d_rsrc[1].getlnum ());
    return result;
  }

  // recode a F09 pseudo instruction
  string Instr::frecode_09_pseudo (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd){
    case F_FMERGE_S:
      result += "mov ";
      break;
    default:
      break;
    }
    result += to_freg (d_rdst[0].getlnum ());
    result += '=' + to_freg (d_rsrc[0].getlnum ());
    return result;
  }

  // recode a F10 instruction
  string Instr::frecode_10 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd){
    case F_FCVT_FX:
      result += "fcvt.fx" + to_fpcomp (d_fpcomp) +' ';
      break;
    case F_FCVT_FXU:
      result += "fcvt.fxu" + to_fpcomp (d_fpcomp) +' ';
      break;
    case F_FCVT_FX_TRUNC:
      result += "fcvt.fx.trunc" + to_fpcomp (d_fpcomp) +' ';
      break;
    case F_FCVT_FXU_TRUNC:
      result += "fcvt.fxu.trunc" + to_fpcomp (d_fpcomp) +' ';
      break;
    case F_FPCVT_FX:
      result += "fpcvt.fx" + to_fpcomp (d_fpcomp) +' ';
      break;
    case F_FPCVT_FXU:
      result += "fpcvt.fxu" + to_fpcomp (d_fpcomp) +' ';
      break;
    case F_FPCVT_FX_TRUNC:
      result += "fpcvt.fx.trunc" + to_fpcomp (d_fpcomp) +' ';
      break;
    case F_FPCVT_FXU_TRUNC:
      result += "fpcvt.fxu.trunc" + to_fpcomp (d_fpcomp) +' ';
      break;
    default:
      break;
    }
    result += to_freg (d_rdst[0].getlnum ());
    result += '=' + to_freg (d_rsrc[0].getlnum ());
    return result;
  }

  // recode a F11 instruction
  string Instr::frecode_11 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += "fcvt.xf " + to_freg (d_rdst[0].getlnum ());
    result += '=' + to_freg (d_rsrc[0].getlnum ());
    return result;
  }

  // recode a F12 instruction
  string Instr::frecode_12 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += "fsetc" +  to_fpcomp(d_fpcomp);
    result += ' ' + to_immv (d_immv[1], false, true);
    result += ',' + to_immv (d_immv[0], false, true);
    return result;
  }

  // recode a F13 instruction
  string Instr::frecode_13 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += "fclrf" +  to_fpcomp(d_fpcomp);
    return result;
  }

 // recode a F14 instruction
  string Instr::frecode_14 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += "fchkf" + to_fpcomp(d_fpcomp);
    result += ' ' +  to_brtg (d_immv[0], d_iip);
    return result;
  }

  // recode a F15 instruction
  string Instr::frecode_15 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case F_BREAK: result += "break.f "; break;
    case F_NOP:   result += "nop.f "; break;
    default: 
      break;
    }
    // add immediate value
    result += to_immv (d_immv[0], false, true);
    return result;
  }

  // recode a F unit instruction

  string Instr::frecode (void) const {
    assert (d_valid == true);
    string result;
    // select instruction group
    switch (d_opcd) {
    case F_FMA:
    case F_FMA_S:
    case F_FMA_D:
      if (d_rsrc[0].getlnum () == 0)
	result = frecode_01_pseudo ();
      else
	result = frecode_01 ();
      break;
    case F_FPMA:
    case F_FMS:
    case F_FMS_S:
    case F_FMS_D:
    case F_FPMS:
    case F_FNMA:
    case F_FNMA_S:
    case F_FNMA_D:
    case F_FPNMA:
      result = frecode_01 ();
      break;
    case F_XMA_L:
    case F_XMA_H:
    case F_XMA_HU:
      result = frecode_02 ();
      break;
    case F_FSELECT:
      result = frecode_03 ();
      break;
    case F_FCMP_EQ:
    case F_FCMP_LT:
    case F_FCMP_LE:
    case F_FCMP_UNORD:
    case F_FCMP_EQ_UNC:
    case F_FCMP_LT_UNC:
    case F_FCMP_LE_UNC:
    case F_FCMP_UNORD_UNC:
      result = frecode_04 ();
      break;
    case F_FCLASS_M:
    case F_FCLASS_M_UNC:
      result = frecode_05 ();
      break;
    case F_FRCPA:
      result = frecode_06 ();
      break;
    case F_FRSQRTA:
    case F_FPRSQRTA:
      result = frecode_07 ();
      break;
    case F_FMIN:
    case F_FMAX:
    case F_FPMIN:
    case F_FPMAX:
    case F_FPCMP_EQ:
    case F_FPCMP_LT:
    case F_FPCMP_LE:
    case F_FPCMP_UNORD:
    case F_FPCMP_NEQ:
    case F_FPCMP_NLT:
    case F_FPCMP_NLE:
    case F_FPCMP_ORD:
      result = frecode_08 ();
      break;
    case F_FMERGE_S:
      if (d_rsrc[1].getlnum () == d_rsrc[0].getlnum ())
	result = frecode_09_pseudo ();
      else
	result = frecode_09 ();
      break;
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
      result = frecode_09 ();
      break;
    case F_FCVT_FX:
    case F_FCVT_FXU:
    case F_FCVT_FX_TRUNC:
    case F_FCVT_FXU_TRUNC:
    case F_FPCVT_FX:
    case F_FPCVT_FXU:
    case F_FPCVT_FX_TRUNC:
    case F_FPCVT_FXU_TRUNC:
      result = frecode_10 ();
      break;
    case F_FCVT_XF:
      result = frecode_11 ();
      break;
    case F_FSETC:
      result = frecode_12 ();
      break;
    case F_FCLRF:
      result = frecode_13 ();
      break;
    case F_FCHKF:
      result = frecode_14 ();
      break;
    case F_BREAK:
    case F_NOP:
      result = frecode_15 ();
      break;
    default:
      break;
    }
    return result;
  }
}
