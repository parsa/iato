// ---------------------------------------------------------------------------
// - Irecode.cpp                                                             -
// - iato:isa library - I unit instruction recode implementation             -
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

  // return a string representation of the permutation to perform
  static string get_mbtype4 (t_octa mbt4c){
    string result = "invalid";
    if (mbt4c == 0x0000000000000000ULL) result = "@brcst";
    if (mbt4c == 0x0000000000000008ULL) result = "@mix";
    if (mbt4c == 0x0000000000000009ULL) result = "@shuf";
    if (mbt4c == 0x000000000000000AULL) result = "@alt";
    if (mbt4c == 0x000000000000000BULL) result = "@rev";
    return result;
  }

  // return a string representation of the mov to br hint
  static string to_mhint (t_mhint mh, t_ihint ih) {
    string result;
    if (mh == MSPTK) result += ".sptk";
    if (mh == MNONE) result += "";
    if (mh == MDPTK) result += ".dptk";
    if (ih == INONE) result += "";
    if (ih == IIMP)  result += ".imp";
    return result;
  }

  // recode a I01 instruction
  string Instr::irecode_01 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case I_PMPYSHR2:
      result += "pmpyshr2 ";
      break;
    case I_PMPYSHR2_U:
      result += "pmpyshr2.u ";
      break;
    default:
      break;
    }
    result += to_greg (d_rdst[0].getlnum ()) + "=";
    result += to_greg (d_rsrc[0].getlnum ()) + ",";
    result += to_greg (d_rsrc[1].getlnum ()) + ",";
    result += to_immv (d_immv[0], false, false);
    return result;
  }

  // recode a I02 instruction
  string Instr::irecode_02 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case I_PMPY2_R:
      result += "pmpy2.r ";
      break;
    case I_PMPY2_L:
      result += "pmpy2.l ";
      break;
    case I_MIX1_R:
      result += "mix1.r ";
      break;
    case I_MIX2_R:
      result += "mix2.r ";
      break;
    case I_MIX4_R:
      result += "mix4.r ";
      break;
    case I_MIX1_L:
      result += "mix1.l ";
      break;
    case I_MIX2_L:
      result += "mix2.l ";
      break;
    case I_MIX4_L:
      result += "mix4.l ";
      break;
    case I_PACK2_USS:
      result += "pack2.uss ";
      break;
    case I_PACK2_SSS:
      result += "pack2.sss ";
      break;
    case I_PACK4_SSS:
      result += "pack4.uss ";
      break;
    case I_UNPACK1_H:
      result += "unpack1.h ";
      break;
    case I_UNPACK2_H:
      result += "unpack2.h ";
      break;
    case I_UNPACK4_H:
      result += "unpack4.h ";
      break;
    case I_UNPACK1_L:
      result += "unpack1.l ";
      break;
    case I_UNPACK2_L:
      result += "unpack2.l ";
      break;
    case I_UNPACK4_L:
      result += "unpack4.l ";
      break;
    case I_PMIN1_U:
      result += "pmin1.u ";
      break;
    case I_PMAX1_U:
      result += "pmax1.u ";
      break;
    case I_PMIN2:
      result += "pmin2 ";
      break;
    case I_PMAX2:
      result += "pmax2 ";
      break;
    case I_PSAD1:
      result += "psad1 ";
      break;
    default:
      break;
    }
    result += to_greg (d_rdst[0].getlnum ());
    result += '=' + to_greg (d_rsrc[0].getlnum ());
    result += ',' + to_greg (d_rsrc[1].getlnum ());
    return result;
  }

  // recode a I03 instruction
  string Instr::irecode_03 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += "mux1 " + to_greg (d_rdst[0].getlnum ()) + "=";
    result +=  to_greg (d_rsrc[0].getlnum ()) + "," + get_mbtype4 (d_immv[0]);
    return result;
  }

  // recode a I04 instruction
  string Instr::irecode_04 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += "mux2 " + to_greg (d_rdst[0].getlnum ()) + "=";
    result +=  to_greg (d_rsrc[0].getlnum ()) + ",";
    result += to_immv (d_immv[0], false, true);
    return result;
  }

  // recode a I05 instruction
  string Instr::irecode_05 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case I_PSHR2:
      result += "pshr2 ";
      break;
    case I_PSHR4:
      result += "pshr4 ";
      break;
    case I_SHR:
      result += "shr ";
      break;
    case I_PSHR2_U:
      result += "pshr2.u ";
      break;
    case I_PSHR4_U:
      result += "pshr4.u ";
      break;
    case I_SHR_U:
      result += "shr.u ";
      break;
    default:
      break;
    }
    result += to_greg (d_rdst[0].getlnum ());
    result += '=' + to_greg (d_rsrc[1].getlnum ());
    result += ',' + to_greg (d_rsrc[0].getlnum ());
    return result;
  }

  // recode a I05 instruction
  string Instr::irecode_06 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case I_PSHR2_I:
      result += "pshr2 ";
      break;
    case I_PSHR4_I:
      result += "pshr4 ";
      break;
    case I_PSHR2_U_I:
      result += "pshr2.u ";
      break;
    case I_PSHR4_U_I:
      result += "pshr4.u ";
      break;
    default:
      break;
    }
    result += to_greg (d_rdst[0].getlnum ());
    result += '=' + to_greg (d_rsrc[1].getlnum ());
    result += ',' + to_immv (d_immv[0], false, false);
    return result;
  }

  // recode a I07 instruction
  string Instr::irecode_07 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case I_PSHL2:
      result += "pshl2 ";
      break;
    case I_PSHL4:
      result += "pshl4 ";
      break;
    case I_SHL:
      result += "shl ";
      break;
    default:
      break;
    }
    result += to_greg (d_rdst[0].getlnum ());
    result += '=' + to_greg (d_rsrc[0].getlnum ());
    result += ',' + to_greg (d_rsrc[1].getlnum ());
    return result;
  }

  // recode a I08 instruction
  string Instr::irecode_08 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case I_PSHL2_I:
      result += "pshl2 ";
      break;
    case I_PSHL4_I:
      result += "pshl4 ";
      break;
    default:
      break;
    }
    result += to_greg (d_rdst[0].getlnum ());
    result += '=' + to_greg (d_rsrc[0].getlnum ());
    result += ',' + to_immv (d_immv[0], false, false);
    return result;
  }

  // recode a I09 instruction
  string Instr::irecode_09 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += "popcnt " + to_greg (d_rdst[0].getlnum ()) + "=";
    result += to_greg (d_rsrc[1].getlnum ()) + ",";
    return result;
  }

  // recode a I10 instruction
  string Instr::irecode_10 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += "shrp " + to_greg (d_rdst[0].getlnum ()) + "=";
    result += to_greg (d_rsrc[0].getlnum ()) + ",";
    result += to_greg (d_rsrc[1].getlnum ()) + ",";
    result += to_immv (d_immv[0], false, false);
    return result;
  }

  // recode a I11 instruction
  string Instr::irecode_11 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    t_long cnt = d_immv[0] + d_immv[1];
    if (cnt == 64) result += ipseudo_11 ();
    else {
    switch (d_opcd) {
    case I_EXTR:
      result += "extr ";
      break;
    case I_EXTR_U:
      result += "extr.u ";
      break;
    default:
      break;
    }
    result += to_greg (d_rdst[0].getlnum ()) + '=';
    result += to_greg (d_rsrc[1].getlnum ()) + ',';
    result += to_immv (d_immv[0], false, false) + ',';
    result += to_immv (d_immv[1], false, false);
    }
    return result;
  }

  // recode a I11 pseudo instruction
  string Instr::ipseudo_11 (void) const {
    assert (d_immv[0] + d_immv[1] == 64);
    string result = "";
    switch (d_opcd) {
    case I_EXTR:
      result += "shr ";
      break;
    case I_EXTR_U:
      result += "shr.u ";
      break;
    default:
      break;
    }
    result += to_greg (d_rdst[0].getlnum ()) + '=';
    result += to_greg (d_rsrc[1].getlnum ()) + ',';
    result += to_immv (d_immv[0], false, false);
    return result;
  }

  // recode a I12 instruction
  string Instr::irecode_12 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += "dep.z " + to_greg (d_rdst[0].getlnum ()) + '=';
    result += to_greg (d_rsrc[0].getlnum ()) + ',';
    result += to_immv (d_immv[0], false, false) + ',';
    result += to_immv (d_immv[1], false, false); 
    return result;
  }

  // recode a I13 instruction
  string Instr::irecode_13 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += "dep.z " + to_greg (d_rdst[0].getlnum ()) + '=';
    result += to_immv (d_immv[0], true, false) + ',';
    result += to_immv (d_immv[1], false, false) + ',';
    result += to_immv (d_immv[2], false, false); 
    return result;
  }

  // recode a I14 instruction
  string Instr::irecode_14 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += "dep " + to_greg (d_rdst[0].getlnum ()) + '=';
    result += to_immv (d_immv[2], true, false) + ',';
    result += to_greg (d_rsrc[1].getlnum ()) + ',';
    result += to_immv (d_immv[0], false, false) + ',';
    result += to_immv (d_immv[1], false, false);
    return result;
  }

  // recode a I15 instruction
  string Instr::irecode_15 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += "dep " + to_greg (d_rdst[0].getlnum ()) + '=';
    result += to_greg (d_rsrc[0].getlnum ()) + ',';
    result += to_greg (d_rsrc[1].getlnum ()) + ',';
    result += to_immv (d_immv[1], false, false) + ',';
    result += to_immv (d_immv[0], false, false);
    return result;
  }

  // recode a I16 instruction
  string Instr::irecode_16 (void) const {
    string result;
    if (d_opcd == I_TBIT_Z_UNC) result += to_pred (d_rsrc[2].getlnum ());
    else result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case I_TBIT_Z:
      result += "tbit.z ";
      break;
    case I_TBIT_Z_UNC:
      result += "tbit.z.unc ";
      break;
    case I_TBIT_Z_AND:
      result += "tbit.z.and ";
      break;
    case I_TBIT_NZ_AND:
      result += "tbit.nz.and ";
      break;
    case I_TBIT_Z_OR:
      result += "tbit.z.or ";
      break;
    case I_TBIT_NZ_OR:
      result += "tbit.nz.or ";
      break;
    case I_TBIT_Z_OR_ANDCM:
      result += "tbit.z.or.andcm ";
      break;
    case I_TBIT_NZ_OR_ANDCM:
      result += "tbit.nz.or.andcm ";
      break;
    default:
      break;
    }
    result += to_preg (d_rdst[0].getlnum ()) + ",";      
    result += to_preg (d_rdst[1].getlnum ()) + "=";
    result += to_greg (d_rsrc[1].getlnum ()) + "," ;
    result += to_immv (d_immv[0], false, false);
    return result;
  }

  // recode a I17 instruction
  string Instr::irecode_17 (void) const {
    string result;
    if (d_opcd == I_TNAT_Z_UNC) result += to_pred (d_rsrc[2].getlnum ());
    else result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case I_TNAT_Z:
      result += "tnat.z ";
      break;
    case I_TNAT_Z_UNC:
      result += "tnat.z.unc ";
      break;
    case I_TNAT_Z_AND:
      result += "tnat.z.and ";
      break;
    case I_TNAT_NZ_AND:
      result += "tnat.nz.and ";
      break;
    case I_TNAT_Z_OR:
      result += "tnat.z.or ";
      break;
    case I_TNAT_NZ_OR:
      result += "tnat.nz.or ";
      break;
    case I_TNAT_Z_OR_ANDCM:
      result += "tnat.z.or.andcm ";
      break;
    case I_TNAT_NZ_OR_ANDCM:
      result += "tnat.nz.or.andcm ";
      break;
    default:
      break;
    }
    result += to_preg (d_rdst[0].getlnum ()) + ",";
    result += to_preg (d_rdst[1].getlnum ()) + "=";
    result += to_greg (d_rsrc[1].getlnum ());
    return result;
  }

  // recode a I19 instruction
  string Instr::irecode_19 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case I_BREAK: result += "break.i "; break;
    case I_NOP:   result += "nop.i "; break;
    default: 
      break;
    }
    // add immediate value
    result += to_immv (d_immv[0], false, true);
    return result;
  }

  // recode a I20 instruction
  string Instr::irecode_20 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += "chk.s.i " + to_greg (d_rsrc[0].getlnum ());
    result += "," + to_brtg (d_immv[0], d_iip);
    return result;
  }

  // recode a I21 instruction
  string Instr::irecode_21 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case I_MOV_TO_B:
      result += "mov";
      break;
    case I_MOV_RET_TO_B:
      result += "mov.ret";
       break;
    default:
      break;
    }
    result += to_mhint(d_mhint, d_ihint) + ' ' + to_breg(d_rdst[0].getlnum ());
    result += '=' + to_greg(d_rsrc[0].getlnum ());
    if (d_immv[0] != 0)
      result += ',' + to_brtg(d_immv[0], true);
    return result;
  }

  // recode a I22 instruction
  string Instr::irecode_22 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += "mov " + to_greg (d_rdst[0].getlnum ());
    result += '=' + to_breg (d_rsrc[0].getlnum ());
    return result;
  }

  // recode a I23 instruction
  string Instr::irecode_23 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += "mov pr=" + to_greg (d_rsrc[0].getlnum ());
    result += ',' + to_immv (d_immv[0], false, true);
    return result;
  }

  // recode a I24 instruction
  string Instr::irecode_24 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += "mov pr.rot=" + to_immv (d_immv[0], false, true);
    return result;
  }

  // recode a I25 instruction
  string Instr::irecode_25 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case I_MOV_FROM_IP:
      result += "mov " + to_greg (d_rdst[0].getlnum ()) + "=ip";
      break;
    case I_MOV_FROM_PR:
      result += "mov " + to_greg (d_rdst[0].getlnum ()) + "=pr";
      break;
    default:
      break;
    }
    return result;
  }

  // recode a I26 instruction
  string Instr::irecode_26 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += "mov.i " + to_areg (d_rdst[0].getlnum ());
    result += '=' + to_greg (d_rsrc[0].getlnum ());
    return result;
  }

  // recode a I27 instruction
  string Instr::irecode_27 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += "mov.i " + to_areg (d_rdst[0].getlnum ());
    result += '=' + to_immv (d_immv[0], true, false);
    return result;
  }

  // recode a I28 instruction
  string Instr::irecode_28 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += "mov.i " + to_greg (d_rdst[0].getlnum ());
    result += '=' + to_areg (d_rsrc[1].getlnum ());
    return result;
  }

  // recode a I29 instruction
  string Instr::irecode_29 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case I_ZXT1:
      result += "zxt1 ";
      break;
    case I_ZXT2:
      result += "zxt2 ";
      break;
    case I_ZXT4:
      result += "zxt4 ";
      break;
    case I_SXT1:
      result += "sxt1 ";
      break;
    case I_SXT2:
      result += "sxt2 ";
      break;
    case I_SXT4:
      result += "sxt4 ";
      break;
    case I_CZX1_L:
      result += "czx1.l ";
      break;
    case I_CZX2_L:
      result += "czx2.l ";
      break;
    case I_CZX1_R:
      result += "czx1.r ";
      break;
    case I_CZX2_R:
      result += "czx2.r ";
      break;
    default:
      break; 
    }
    result += to_greg (d_rdst[0].getlnum ());
    result += '=' + to_greg (d_rsrc[1].getlnum ());
    return result;
  }

  // recode a I unit instruction

  string Instr::irecode (void) const {
    assert (d_valid == true);
    string result;
    // select instruction group
    switch (d_opcd) {
    case I_PMPYSHR2:
    case I_PMPYSHR2_U:
      result += irecode_01 ();
      break;
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
      result += irecode_02 ();
      break;
    case I_MUX1:
      result = irecode_03 ();
      break;
    case I_MUX2:
      result = irecode_04 ();
      break;
    case I_PSHR2:
    case I_PSHR4:
    case I_SHR:
    case I_PSHR2_U:
    case I_PSHR4_U:
    case I_SHR_U:
      result += irecode_05 ();
      break;
    case I_PSHR2_I:
    case I_PSHR4_I:
    case I_PSHR2_U_I:
    case I_PSHR4_U_I:
      result += irecode_06 ();
      break;
    case I_PSHL2:
    case I_PSHL4:
    case I_SHL:
      result = irecode_07 ();
      break;
    case I_PSHL2_I:
    case I_PSHL4_I:
      result = irecode_08 ();
      break;
    case I_POPCNT:
      result = irecode_09 ();
      break;
    case I_SHRP:
      result = irecode_10 ();
      break;
    case I_EXTR:
    case I_EXTR_U:
      result = irecode_11 ();
      break;
    case I_DEP_Z:
      result = irecode_12 ();
      break;
    case I_DEP_Z_I:
      result = irecode_13 ();
      break;
    case  I_DEP_I:
      result = irecode_14 ();
      break;
    case I_DEP:
      result = irecode_15 ();
      break;
    case I_TBIT_Z:
    case I_TBIT_Z_UNC:
    case I_TBIT_Z_AND:
    case I_TBIT_NZ_AND:
    case I_TBIT_Z_OR:
    case I_TBIT_NZ_OR:
    case I_TBIT_Z_OR_ANDCM:
    case I_TBIT_NZ_OR_ANDCM:
      result = irecode_16 ();
      break;
    case I_TNAT_Z:
    case I_TNAT_Z_UNC:
    case I_TNAT_Z_AND:
    case I_TNAT_NZ_AND:
    case I_TNAT_Z_OR:
    case I_TNAT_NZ_OR:
    case I_TNAT_Z_OR_ANDCM:
    case I_TNAT_NZ_OR_ANDCM:
      result = irecode_17 ();
      break;
    case I_BREAK:
    case I_NOP:
      result = irecode_19 ();
      break;
    case I_CHK_S_I:
      result = irecode_20 ();
      break;
    case I_MOV_TO_B:
    case I_MOV_RET_TO_B:
      result = irecode_21();
     break;
    case I_MOV_FROM_B:
      result = irecode_22();
      break;
    case I_MOV_TO_PR:
      result = irecode_23();
      break;
    case I_MOV_TO_PR_ROT:
      result = irecode_24();
      break;
    case I_MOV_FROM_IP:
    case I_MOV_FROM_PR:
      result = irecode_25();
      break;
    case I_MOV_TO_AR_R:
      result = irecode_26();
      break;
    case I_MOV_TO_AR_I:
      result = irecode_27();
      break;
    case I_MOV_FROM_AR:
      result = irecode_28();
      break;
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
      result = irecode_29();
      break;
    default:
      break;
    }
    return result;
  }
}
