// ---------------------------------------------------------------------------
// - Arecode.cpp                                                             -
// - iato:isa library - A unit instruction recode implementation             -
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

  // recode a A1 instruction
  string Instr::arecode_01 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case A_ADD:   result += "add ";   break;
    case A_ADD_1: result += "add ";   break;
    case A_SUB:   result += "sub ";   break;
    case A_SUB_1: result += "sub ";   break;
    case A_ADDP4: result += "addp4 "; break;
    case A_AND:   result += "and ";   break;
    case A_ANDCM: result += "andcm "; break;
    case A_OR:    result += "or ";    break;
    case A_XOR:   result += "xor ";   break;
    default: 
      break;
    }
    // add source and destination
    result += to_greg (d_rdst[0].getlnum ()) + '=';
    result += to_greg (d_rsrc[0].getlnum ()) + ',' + 
              to_greg (d_rsrc[1].getlnum ());
    // eventuall add extension
    switch (d_opcd) {
    case A_ADD_1:
    case A_SUB_1:
      result += ",1";
      break;
    default:
      break;
    }
    return result;
  }

  // recode a A2 instruction
  string Instr::arecode_02 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case A_SHLADD:   result += "shladd "; break;
    case A_SHLADDP4: result += "shladdp4 "; break;
    default: 
      break;
    }
    // add destination and first source, count and destination
    result += to_greg (d_rdst[0].getlnum ()) + '=';
    result += to_greg (d_rsrc[0].getlnum ()) + ',';
    result += to_immv (d_immv[0], true, false) + ',';
    result += to_greg (d_rsrc[1].getlnum ());
    return result;
  }

  // recode a A3 instruction
  string Instr::arecode_03 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case A_SUB_I:   result += "sub "; break;
    case A_AND_I:   result += "and  "; break;
    case A_ANDCM_I: result += "andcm "; break;
    case A_OR_I:    result += "or "; break;
    case A_XOR_I:   result += "xor "; break;
    default: 
      break;
    }
    // add source, immediate and destination
    result += to_greg (d_rdst[0].getlnum ()) + '=';
    result += to_immv (d_immv[0],true, false) + ',';
    result += to_greg (d_rsrc[1].getlnum ());
    return result;
  }

  // recode a A4 instruction
  string Instr::arecode_04 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case A_ADDS_I:  
      result += (d_immv[0] == 0) ? "mov " : "adds "; 
      break;
    case A_ADDP4_I: 
      result += "addp4 "; 
      break;
    default: 
      break;
    }
    // add source, immediate and destination
    result += to_greg (d_rdst[0].getlnum ()) + '=';
    if ((d_immv[0] != 0) || (d_opcd != A_ADDS_I))
      result += to_immv (d_immv[0], true, false) + ",";
    result += to_greg (d_rsrc[1].getlnum ());
    return result;
  }

  // recode a A5 instruction
  string Instr::arecode_05 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += (d_rsrc[0].getlnum () == 0) ? "mov " : "addl "; 
    result += to_greg (d_rdst[0].getlnum ());
    result += "=" + to_immv (d_immv[0], true, false);
    if (d_rsrc[0].getlnum () != 0) result += "," + to_greg (d_rsrc[0].getlnum ());
    return result;
  }

  // recode a A6 instruction
  string Instr::arecode_06 (void) const {
    string result;
    switch (d_opcd) {
    case A_CMP_LT_UNC_RR:
    case A_CMP_LTU_UNC_RR:
    case A_CMP_EQ_UNC_RR:
    case A_CMP4_LT_UNC_RR:
    case A_CMP4_LTU_UNC_RR:
    case A_CMP4_EQ_UNC_RR:
      result = to_pred (d_rsrc[2].getlnum ());
      break;
    default:
      result = to_pred (d_rprd.getlnum ());
      break;    
    }
    switch (d_opcd) {
    case A_CMP_LT_RR:
      result += "cmp.lt ";
      break;
    case A_CMP_LTU_RR:
      result += "cmp.ltu ";
      break;
    case A_CMP_EQ_RR:
      result += "cmp.eq ";
      break;
    case A_CMP_LT_UNC_RR:
      result += "cmp.lt.unc ";
      break;
    case A_CMP_LTU_UNC_RR:
      result += "cmp.ltu.unc ";
      break;
    case A_CMP_EQ_UNC_RR:
      result += "cmp.eq.unc ";
      break;
    case A_CMP_EQ_AND_RR:
      result += "cmp.eq.and ";
      break;
    case A_CMP_EQ_OR_RR:
      result += "cmp.eq.or ";
      break;
    case A_CMP_EQ_OR_ANDCM_RR:
      result += "cmp.eq.or.andcm ";
      break;
    case A_CMP_NE_AND_RR:
      result += "cmp.ne.and ";
      break;
    case A_CMP_NE_OR_RR:
      result += "cmp.ne.or ";
      break;
    case A_CMP_NE_OR_ANDCM_RR:
      result += "cmp.ne.or.andcm ";
      break;
    case A_CMP4_LT_RR:
      result += "cmp4.lt ";
      break;
    case A_CMP4_LTU_RR:
      result += "cmp4.ltu ";
      break;
    case A_CMP4_EQ_RR:
      result += "cmp4.eq ";
      break;
    case A_CMP4_LT_UNC_RR:
      result += "cmp4.lt.unc ";
      break;
    case A_CMP4_LTU_UNC_RR:
      result += "cmp4.ltu.unc ";
      break;
    case A_CMP4_EQ_UNC_RR:
      result += "cmp4.eq.unc ";
      break;
    case A_CMP4_EQ_AND_RR:
      result += "cmp4.eq.and ";
      break;
    case A_CMP4_EQ_OR_RR:
      result += "cmp4.eq.or ";
      break;
    case A_CMP4_EQ_OR_ANDCM_RR:
      result += "cmp4.eq.or.andcm ";
      break;
    case A_CMP4_NE_AND_RR:
      result += "cmp4.ne.and ";
      break;
    case A_CMP4_NE_OR_RR:
      result += "cmp4.ne.or ";
      break;
    case A_CMP4_NE_OR_ANDCM_RR:
      result += "cmp4.ne.or.andcm ";
      break;
    default:
      break;
    }
    result += to_preg (d_rdst[0].getlnum ()) + ',' ;
    result += to_preg (d_rdst[1].getlnum ()) + '=' ;
    result += to_greg (d_rsrc[0].getlnum ()) + ',' ;
    result += to_greg (d_rsrc[1].getlnum ());
    return result;
  }

  // recode a A7 instruction
  string Instr::arecode_07 (void) const {
    assert (d_rsrc[0].getlnum () == 0);
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case A_CMP_GT_AND_ZR:
      result += "cmp.gt.and ";
      break;
    case A_CMP_GT_OR_ZR:
      result += "cmp.gt.or ";
      break;
    case A_CMP_GT_OR_ANDCM_ZR:
      result += "cmp.gt.or.andcm ";
      break;
    case A_CMP_LE_AND_ZR:
      result += "cmp.le.and ";
      break;
    case A_CMP_LE_OR_ZR:
      result += "cmp.le.or ";
      break;
    case A_CMP_LE_OR_ANDCM_ZR:
      result += "cmp.le.or.andcm ";
      break;
    case A_CMP_GE_AND_ZR:
      result += "cmp.ge.and ";
      break;
    case A_CMP_GE_OR_ZR:
      result += "cmp.ge.or ";
      break;
    case A_CMP_GE_OR_ANDCM_ZR:
      result += "cmp.ge.or.andcm ";
      break;
    case A_CMP_LT_AND_ZR:
      result += "cmp.lt.and ";
      break;
    case A_CMP_LT_OR_ZR:
      result += "cmp.lt.or ";
      break;
    case A_CMP_LT_OR_ANDCM_ZR:
      result += "cmp.lt.or.andcm ";
      break;
    case A_CMP4_GT_AND_ZR:
      result += "cmp4.gt.and ";
      break;
    case A_CMP4_GT_OR_ZR:
      result += "cmp4.gt.or ";
      break;
    case A_CMP4_GT_OR_ANDCM_ZR:
      result += "cmp4.gt.or.andcm ";
      break;
    case A_CMP4_LE_AND_ZR:
      result += "cmp4.le.and ";
      break;
    case A_CMP4_LE_OR_ZR:
      result += "cmp4.le.or ";
      break;
    case A_CMP4_LE_OR_ANDCM_ZR:
      result += "cmp4.le.or.andcm ";
      break;
    case A_CMP4_GE_AND_ZR:
      result += "cmp4.ge.and ";
      break;
    case A_CMP4_GE_OR_ZR:
      result += "cmp4.ge.or ";
      break;
    case A_CMP4_GE_OR_ANDCM_ZR:
      result += "cmp4.ge.or.andcm ";
      break;
    case A_CMP4_LT_AND_ZR:
      result += "cmp4.lt.and ";
      break;
    case A_CMP4_LT_OR_ZR:
      result += "cmp4.lt.or ";
      break;
    case A_CMP4_LT_OR_ANDCM_ZR:
      result += "cmp4.lt.or.andcm ";
      break;
    default:
      break;
    }
    result += to_preg (d_rdst[0].getlnum ()) + ',';
    result += to_preg (d_rdst[1].getlnum ()) + '=';
    result += to_greg (0) + ',' + to_greg (d_rsrc[1].getlnum ());
    return result;
  }

  // recode a A8 instruction
  string Instr::arecode_08 (void) const {
    string result;
    switch (d_opcd) {
    case A_CMP_LT_UNC_IR:
    case A_CMP_LTU_UNC_IR:
    case A_CMP_EQ_UNC_IR:
    case A_CMP4_LT_UNC_IR:
    case A_CMP4_LTU_UNC_IR:
    case A_CMP4_EQ_UNC_IR:
      result += to_pred (d_rsrc[2].getlnum ());
      break;
    default:
      result = to_pred (d_rprd.getlnum ());
      break;
    }
    switch (d_opcd) {
    case A_CMP_LT_IR:
      result += "cmp.lt ";
      break;
    case A_CMP_LTU_IR:
      result += "cmp.ltu ";
      break;
    case A_CMP_EQ_IR:
      result += "cmp.eq ";
      break;
    case A_CMP_LT_UNC_IR:
      result += "cmp.lt.unc ";
      break;
    case A_CMP_LTU_UNC_IR:
      result += "cmp.ltu.unc ";
      break;
    case A_CMP_EQ_UNC_IR:
      result += "cmp.eq.unc ";
      break;
    case A_CMP_EQ_AND_IR:
      result += "cmp.eq.and ";
      break;
    case A_CMP_EQ_OR_IR:
      result += "cmp.eq.or ";
      break;
    case A_CMP_EQ_OR_ANDCM_IR:
      result += "cmp.eq.or.andcm ";
      break;
    case A_CMP_NE_AND_IR:
      result += "cmp.ne.and ";
      break;
    case A_CMP_NE_OR_IR:
      result += "cmp.ne.or ";
      break;
    case A_CMP_NE_OR_ANDCM_IR:
      result += "cmp.ne.or.andcm ";
      break;
    case A_CMP4_LT_IR:
      result += "cmp4.lt ";
      break;
    case A_CMP4_LTU_IR:
      result += "cmp4.ltu ";
      break;
    case A_CMP4_EQ_IR:
      result += "cmp4.eq ";
      break;
    case A_CMP4_LT_UNC_IR:
      result += "cmp4.lt.unc ";
      break;
    case A_CMP4_LTU_UNC_IR:
      result += "cmp4.ltu.unc ";
      break;
    case A_CMP4_EQ_UNC_IR:
      result += "cmp4.eq.unc ";
      break;
    case A_CMP4_EQ_AND_IR:
      result += "cmp4.eq.and ";
      break;
    case A_CMP4_EQ_OR_IR:
      result += "cmp4.eq.or ";
      break;
    case A_CMP4_EQ_OR_ANDCM_IR:
      result += "cmp4.eq.or.andcm ";
      break;
    case A_CMP4_NE_AND_IR:
      result += "cmp4.ne.and ";
      break;
    case A_CMP4_NE_OR_IR:
      result += "cmp4.ne.or ";
      break;
    case A_CMP4_NE_OR_ANDCM_IR:
      result += "cmp4.ne.or.andcm ";
      break;
    default:
      break;
    }
    result += to_preg (d_rdst[0].getlnum ()) + ',';
    result += to_preg (d_rdst[1].getlnum ()) + '=';
    result += to_immv (d_immv[0], true, false) + ',';
    result += to_greg (d_rsrc[1].getlnum ());
    return result;
  }

  // recode a A9 instruction
  string Instr::arecode_09 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case A_PADD1:
      result += "padd1 ";
      break;
    case A_PADD2:
      result += "padd2 ";
      break;
    case A_PADD4:
      result += "padd4 ";
      break;
    case A_PADD1_SSS:
      result += "padd1.sss ";
      break;
    case A_PADD2_SSS:
      result += "padd2.sss ";
      break;
    case A_PADD1_UUU:
      result += "padd1.uuu ";
      break;
    case A_PADD2_UUU:
      result += "padd2.uuu ";
      break;
    case A_PADD1_UUS:
      result += "padd1.uus ";
      break;
    case A_PADD2_UUS:
      result += "padd2.uus ";
      break;
    case A_PSUB1:
      result += "psub1 ";
      break;
    case A_PSUB2:
      result += "psub2 ";
      break;
    case A_PSUB4:
      result += "psub4 ";
      break;
    case A_PSUB1_SSS:
      result += "psub1.sss ";
      break;
    case A_PSUB2_SSS:
      result += "psub2.sss ";
      break;
    case A_PSUB1_UUU:
      result += "psub1.uuu ";
      break;
    case A_PSUB2_UUU:
      result += "psub2.uuu ";
      break;
    case A_PSUB1_UUS:
      result += "psub1.uus ";
      break;
    case A_PSUB2_UUS:
      result += "psub2.uus ";
      break;
    case A_PAVG1:
      result += "pavg1 ";
      break;
    case A_PAVG2:
      result += "pavg2 ";
      break;
    case A_PAVG1_RAZ:
      result += "pavg1.raz ";
      break;
    case A_PAVG2_RAZ:
      result += "pavg2.raz ";
      break;
    case A_PAVGSUB1:
      result += "pavgsub1 ";
      break;
    case A_PAVGSUB2:
      result += "pavgsub2 ";
      break;
    case A_PCMP1_EQ:
      result += "pcmp1.eq ";
      break;
    case A_PCMP2_EQ:
      result += "pcmp2.eq ";
      break;
    case A_PCMP4_EQ:
      result += "pcmp4.eq ";
      break;
    case A_PCMP1_GT:
      result += "pcmp1.gt ";
      break;
    case A_PCMP2_GT:
      result += "pcmp2.gt ";
      break;
    case A_PCMP4_GT:
      result += "pcmp4.gt ";
      break;
    default:
      break;
    }
    result += to_greg (d_rdst[0].getlnum ()) + "=";
    result += to_greg (d_rsrc[0].getlnum ()) + ",";
    result += to_greg (d_rsrc[1].getlnum ());
    return result;
  }

 // recode a A10 instruction
  string Instr::arecode_10 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case A_PSHLADD2:
      result += "pshladd2 ";
      break;
    case A_PSHRADD2:
      result += "pshradd2 ";
      break;
    default:
      break;
    }
    result += to_greg (d_rdst[0].getlnum ()) + "=";
    result += to_greg (d_rsrc[0].getlnum ()) + ",";
    result += to_immv (d_immv[0], false, false) + ",";
    result += to_greg (d_rsrc[1].getlnum ());
    return result;
    }
  // recode a A unit instruction

  string Instr::arecode (void) const {
    assert (d_valid == true);
    string result;
    // select instruction group
    switch (d_opcd) {
    case A_ADD:
    case A_ADD_1:
    case A_SUB:
    case A_SUB_1:
    case A_ADDP4:
    case A_AND:
    case A_ANDCM:
    case A_OR:
    case A_XOR:
      result = arecode_01 ();
      break;
    case A_SHLADD:
    case A_SHLADDP4:
      result = arecode_02 ();
      break;
    case A_SUB_I:
    case A_AND_I:
    case A_ANDCM_I:
    case A_OR_I:
    case A_XOR_I:
      result = arecode_03 ();
      break;
    case A_ADDS_I:
    case A_ADDP4_I:
      result = arecode_04 ();
      break;
    case A_ADDL_I:
      result = arecode_05 ();
      break;
    case A_CMP_LT_RR:
    case A_CMP_LTU_RR:
    case A_CMP_EQ_RR:
    case A_CMP_LT_UNC_RR:
    case A_CMP_LTU_UNC_RR:
    case A_CMP_EQ_UNC_RR:
    case A_CMP_EQ_AND_RR:
    case A_CMP_EQ_OR_RR:
    case A_CMP_EQ_OR_ANDCM_RR:
    case A_CMP_NE_AND_RR:
    case A_CMP_NE_OR_RR:
    case A_CMP_NE_OR_ANDCM_RR:
    case A_CMP4_LT_RR:
    case A_CMP4_LTU_RR:
    case A_CMP4_EQ_RR:
    case A_CMP4_LT_UNC_RR:
    case A_CMP4_LTU_UNC_RR:
    case A_CMP4_EQ_UNC_RR:
    case A_CMP4_EQ_AND_RR:
    case A_CMP4_EQ_OR_RR:
    case A_CMP4_EQ_OR_ANDCM_RR:
    case A_CMP4_NE_AND_RR:
    case A_CMP4_NE_OR_RR:
    case A_CMP4_NE_OR_ANDCM_RR:
      result = arecode_06 ();
      break;
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
      result = arecode_07 ();
      break;
    case A_CMP_LT_IR:
    case A_CMP_LTU_IR:
    case A_CMP_EQ_IR:
    case A_CMP_LT_UNC_IR:
    case A_CMP_LTU_UNC_IR:
    case A_CMP_EQ_UNC_IR:
    case A_CMP_EQ_AND_IR:
    case A_CMP_EQ_OR_IR:
    case A_CMP_EQ_OR_ANDCM_IR:
    case A_CMP_NE_AND_IR:
    case A_CMP_NE_OR_IR:
    case A_CMP_NE_OR_ANDCM_IR:
    case A_CMP4_LT_IR:
    case A_CMP4_LTU_IR:
    case A_CMP4_EQ_IR:
    case A_CMP4_LT_UNC_IR:
    case A_CMP4_LTU_UNC_IR:
    case A_CMP4_EQ_UNC_IR:
    case A_CMP4_EQ_AND_IR:
    case A_CMP4_EQ_OR_IR:
    case A_CMP4_EQ_OR_ANDCM_IR:
    case A_CMP4_NE_AND_IR:
    case A_CMP4_NE_OR_IR:
    case A_CMP4_NE_OR_ANDCM_IR:
      result = arecode_08 ();
      break;
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
      result = arecode_09 ();
      break;
    case A_PSHLADD2:
    case A_PSHRADD2:
      result = arecode_10 ();
      break;
    default:
      break;
    }
    return result;
  }
}
