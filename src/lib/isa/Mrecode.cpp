// ---------------------------------------------------------------------------
// - Mrecode.cpp                                                             -
// - iato:isa library - M unit instruction recode implementation             -
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

  // return a string representation of the load extension
  static string to_lhint (t_ldhint lh) {
    string result;
    if (lh == LDNONE) result += "";
    if (lh == LDNT1)  result += ".nt1";
    if (lh == LDNTA)  result += ".nta";
    return result;
  }

  // return a string representation of the store extension
  static string to_shint (t_sthint sh) {
    string result;
    if (sh == STNONE) result += "";
    if (sh == STNTA)  result += ".nta";
    return result;
  }

  // return a string representation of the line prefetch hint
  static string to_lfhint (t_lfhint lfh) {
    string result;
    if (lfh == LFNONE) result += "";
    if (lfh == LFNT1)  result += ".nt1";
    if (lfh == LFNT2)  result += ".nt2";
    if (lfh == LFNTA)  result += ".nta";
    return result;
  }

  // recode a M01 instruction
  string Instr::mrecode_01 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case M_LD1:
      result += "ld1" + to_lhint(d_lhint) + " ";
      break;	
    case M_LD2:
      result += "ld2" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4:
      result += "ld4" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8:
      result += "ld8" + to_lhint(d_lhint) + " ";
      break;
    case M_LD1_S:
      result += "ld1.s" + to_lhint(d_lhint) + " ";
      break;
    case M_LD2_S:
      result += "ld2.s" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_S:
      result += "ld4.s" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_S:
      result += "ld8.s" + to_lhint(d_lhint) + " ";
      break;
    case M_LD1_A:
      result += "ld1.a" + to_lhint(d_lhint) + " ";
      break;
    case M_LD2_A:
      result += "ld2.a" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_A:
      result += "ld4.a" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_A:
      result += "ld8.a" + to_lhint(d_lhint) + " ";
      break;
    case M_LD1_SA:
      result += "ld1.sa" + to_lhint(d_lhint) + " ";
      break;
    case M_LD2_SA:
      result += "ld2.sa" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_SA:
      result += "ld4.sa" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_SA:
      result += "ld8.sa" + to_lhint(d_lhint) + " ";
      break;
    case M_LD1_BIAS:
      result += "ld1.bias" + to_lhint(d_lhint) + " ";
      break;
    case M_LD2_BIAS:
      result += "ld2.bias" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_BIAS:
      result += "ld4.bias" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_BIAS:
      result += "ld8.bias" + to_lhint(d_lhint) + " ";
      break;
    case M_LD1_ACQ:
      result += "ld1.acq" + to_lhint(d_lhint) + " ";
      break;
    case M_LD2_ACQ:
      result += "ld2.acq" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_ACQ:
      result += "ld4.acq" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_ACQ:
      result += "ld8.acq" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_FILL:
      result += "ld8.fill" + to_lhint(d_lhint) + " ";
      break;
    case M_LD1_C_CLR:
      result += "ld1.c.clr" + to_lhint(d_lhint) + " ";
      break;	
    case M_LD2_C_CLR:
      result += "ld2.c.clr" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_C_CLR:
      result += "ld4.c.clr" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_C_CLR:
      result += "ld8.c.clr" + to_lhint(d_lhint) + " ";
      break; 
    case M_LD1_C_NC:
      result += "ld1.c.nc" + to_lhint(d_lhint) + " ";
      break;	
    case M_LD2_C_NC:
      result += "ld2.c.nc" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_C_NC:
      result += "ld4.c.nc" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_C_NC:
      result += "ld8.c.nc" + to_lhint(d_lhint) + " ";
      break;
    case M_LD1_C_CLR_ACQ:
      result += "ld1.c.clr.acq" + to_lhint(d_lhint) + " ";
      break;	
    case M_LD2_C_CLR_ACQ:
      result += "ld2.c.clr.acq" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_C_CLR_ACQ:
      result += "ld4.c.clr.acq" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_C_CLR_ACQ:
      result += "ld8.c.clr.acq" + to_lhint(d_lhint) + " ";
      break;
    default:
      assert (false);
      break;
    }
    result += to_greg (d_rdst[0].getlnum ()) + "=[";
    result += to_greg (d_rsrc[1].getlnum ()) + ']';
    return result;
  }

  // recode a M02 instruction
  string Instr::mrecode_02 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case M_LD1_UPD_R:
      result += "ld1" + to_lhint(d_lhint) + " ";
      break;	
    case M_LD2_UPD_R:
      result += "ld2" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_UPD_R:
      result += "ld4" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_UPD_R:
      result += "ld8" + to_lhint(d_lhint) + " ";
      break;
    case M_LD1_S_UPD_R:
      result += "ld1.s" + to_lhint(d_lhint) + " ";
      break;
    case M_LD2_S_UPD_R:
      result += "ld2.s" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_S_UPD_R:
      result += "ld4.s" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_S_UPD_R:
      result += "ld8.s" + to_lhint(d_lhint) + " ";
      break;
    case M_LD1_A_UPD_R:
      result += "ld1.a" + to_lhint(d_lhint) + " ";
      break;
    case M_LD2_A_UPD_R:
      result += "ld2.a" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_A_UPD_R:
      result += "ld4.a" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_A_UPD_R:
      result += "ld8.a" + to_lhint(d_lhint) + " ";
      break;
    case M_LD1_SA_UPD_R:
      result += "ld1.sa" + to_lhint(d_lhint) + " ";
      break;
    case M_LD2_SA_UPD_R:
      result += "ld2.sa" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_SA_UPD_R:
      result += "ld4.sa" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_SA_UPD_R:
      result += "ld8.sa" + to_lhint(d_lhint) + " ";
      break;
    case M_LD1_BIAS_UPD_R:
      result += "ld1.bias" + to_lhint(d_lhint) + " ";
      break;
    case M_LD2_BIAS_UPD_R:
      result += "ld2.bias" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_BIAS_UPD_R:
      result += "ld4.bias" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_BIAS_UPD_R:
      result += "ld8.bias" + to_lhint(d_lhint) + " ";
      break;
    case M_LD1_ACQ_UPD_R:
      result += "ld1.acq" + to_lhint(d_lhint) + " ";
      break;
    case M_LD2_ACQ_UPD_R:
      result += "ld2.acq" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_ACQ_UPD_R:
      result += "ld4.acq" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_ACQ_UPD_R:
      result += "ld8.acq" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_FILL_UPD_R:
      result += "ld8.fill" + to_lhint(d_lhint) + " ";
      break;
    case M_LD1_C_CLR_UPD_R:
      result += "ld1.c.clr" + to_lhint(d_lhint) + " ";
      break;	
    case M_LD2_C_CLR_UPD_R:
      result += "ld2.c.clr" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_C_CLR_UPD_R:
      result += "ld4.c.clr" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_C_CLR_UPD_R:
      result += "ld8.c.clr" + to_lhint(d_lhint) + " ";
      break; 
    case M_LD1_C_NC_UPD_R:
      result += "ld1.c.nc" + to_lhint(d_lhint) + " ";
      break;	
    case M_LD2_C_NC_UPD_R:
      result += "ld2.c.nc" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_C_NC_UPD_R:
      result += "ld4.c.nc" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_C_NC_UPD_R:
      result += "ld8.c.nc" + to_lhint(d_lhint) + " ";
      break;
    case M_LD1_C_CLR_ACQ_UPD_R:
      result += "ld1.c.clr.acq" + to_lhint(d_lhint) + " ";
      break;	
    case M_LD2_C_CLR_ACQ_UPD_R:
      result += "ld2.c.clr.acq" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_C_CLR_ACQ_UPD_R:
      result += "ld4.c.clr.acq" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_C_CLR_ACQ_UPD_R:
      result += "ld8.c.clr.acq" + to_lhint(d_lhint) + " ";
      break;
    default:
      assert (false);
      break;
    }
    result += to_greg (d_rdst[0].getlnum ()) + "=[";
    result += to_greg (d_rsrc[1].getlnum ()) + "],";
    result += to_greg (d_rsrc[0].getlnum ());
    return result;
  }

  // recode a M03 instruction
  string Instr::mrecode_03 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case M_LD1_UPD_I:
      result += "ld1" + to_lhint(d_lhint) + " ";
      break;	
    case M_LD2_UPD_I:
      result += "ld2" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_UPD_I:
      result += "ld4" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_UPD_I:
      result += "ld8" + to_lhint(d_lhint) + " ";
      break;
    case M_LD1_S_UPD_I:
      result += "ld1.s" + to_lhint(d_lhint) + " ";
      break;
    case M_LD2_S_UPD_I:
      result += "ld2.s" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_S_UPD_I:
      result += "ld4.s" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_S_UPD_I:
      result += "ld8.s" + to_lhint(d_lhint) + " ";
      break;
    case M_LD1_A_UPD_I:
      result += "ld1.a" + to_lhint(d_lhint) + " ";
      break;
    case M_LD2_A_UPD_I:
      result += "ld2.a" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_A_UPD_I:
      result += "ld4.a" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_A_UPD_I:
      result += "ld8.a" + to_lhint(d_lhint) + " ";
      break;
    case M_LD1_SA_UPD_I:
      result += "ld1.sa" + to_lhint(d_lhint) + " ";
      break;
    case M_LD2_SA_UPD_I:
      result += "ld2.sa" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_SA_UPD_I:
      result += "ld4.sa" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_SA_UPD_I:
      result += "ld8.sa" + to_lhint(d_lhint) + " ";
      break;
    case M_LD1_BIAS_UPD_I:
      result += "ld1.bias" + to_lhint(d_lhint) + " ";
      break;
    case M_LD2_BIAS_UPD_I:
      result += "ld2.bias" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_BIAS_UPD_I:
      result += "ld4.bias" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_BIAS_UPD_I:
      result += "ld8.bias" + to_lhint(d_lhint) + " ";
      break;
    case M_LD1_ACQ_UPD_I:
      result += "ld1.acq" + to_lhint(d_lhint) + " ";
      break;
    case M_LD2_ACQ_UPD_I:
      result += "ld2.acq" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_ACQ_UPD_I:
      result += "ld4.acq" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_ACQ_UPD_I:
      result += "ld8.acq" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_FILL_UPD_I:
      result += "ld8.fill" + to_lhint(d_lhint) + " ";
      break;
    case M_LD1_C_CLR_UPD_I:
      result += "ld1.c.clr" + to_lhint(d_lhint) + " ";
      break;	
    case M_LD2_C_CLR_UPD_I:
      result += "ld2.c.clr" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_C_CLR_UPD_I:
      result += "ld4.c.clr" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_C_CLR_UPD_I:
      result += "ld8.c.clr" + to_lhint(d_lhint) + " ";
      break; 
    case M_LD1_C_NC_UPD_I:
      result += "ld1.c.nc" + to_lhint(d_lhint) + " ";
      break;	
    case M_LD2_C_NC_UPD_I:
      result += "ld2.c.nc" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_C_NC_UPD_I:
      result += "ld4.c.nc" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_C_NC_UPD_I:
      result += "ld8.c.nc" + to_lhint(d_lhint) + " ";
      break;
    case M_LD1_C_CLR_ACQ_UPD_I:
      result += "ld1.c.clr.acq" + to_lhint(d_lhint) + " ";
      break;	
    case M_LD2_C_CLR_ACQ_UPD_I:
      result += "ld2.c.clr.acq" + to_lhint(d_lhint) + " ";
      break;
    case M_LD4_C_CLR_ACQ_UPD_I:
      result += "ld4.c.clr.acq" + to_lhint(d_lhint) + " ";
      break;
    case M_LD8_C_CLR_ACQ_UPD_I:
      result += "ld8.c.clr.acq" + to_lhint(d_lhint) + " ";
      break;
    default:
      assert (false);
      break;
    }
    result += to_greg (d_rdst[0].getlnum ()) + "=[";
    result += to_greg (d_rsrc[1].getlnum ()) + "],";
    result += to_immv (d_immv[0], true, false);
    return result;
  }

  // recode a M04 instruction
  string Instr::mrecode_04 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case M_ST1:
      result += "st1" + to_shint(d_shint) + " ";
      break;
    case M_ST2:
      result += "st2" + to_shint(d_shint) + " ";    
      break;
    case M_ST4:
      result += "st4" + to_shint(d_shint) + " ";
      break;
    case M_ST8:
      result += "st8" + to_shint(d_shint) + " ";
      break;
    case M_ST1_REL:
      result += "st1.rel" + to_shint(d_shint) + " ";
      break;
    case M_ST2_REL:
      result += "st2.rel" + to_shint(d_shint) + " ";
      break;
    case M_ST4_REL:
      result += "st4.rel" + to_shint(d_shint) + " ";
      break;
    case M_ST8_REL:
      result += "st8.rel" + to_shint(d_shint) + " ";
      break;
    case M_ST8_SPILL:
      result += "st8.spill" + to_shint(d_shint) + " ";
      break;
    default:
      assert (false);
      break;
    }
    result += '[' + to_greg (d_rsrc[1].getlnum ()) + "]=";
    result += to_greg (d_rsrc[0].getlnum ());
    return result;
  }

  // recode a M05 instruction
  string Instr::mrecode_05 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case M_ST1_UPD_I:
      result += "st1" + to_shint(d_shint) + " ";
      break;
    case M_ST2_UPD_I:
      result += "st2" + to_shint(d_shint) + " ";    
      break;
    case M_ST4_UPD_I:
      result += "st4" + to_shint(d_shint) + " ";
      break;
    case M_ST8_UPD_I:
      result += "st8" + to_shint(d_shint) + " ";
      break;
    case M_ST1_REL_UPD_I:
      result += "st1.rel" + to_shint(d_shint) + " ";
      break;
    case M_ST2_REL_UPD_I:
      result += "st2.rel" + to_shint(d_shint) + " ";
      break;
    case M_ST4_REL_UPD_I:
      result += "st4.rel" + to_shint(d_shint) + " ";
      break;
    case M_ST8_REL_UPD_I:
      result += "st8.rel" + to_shint(d_shint) + " ";
      break;
    case M_ST8_SPILL_UPD_I:
      result += "st8.spill" + to_shint(d_shint) + " ";
      break;
    default:
      assert (false);
      break;
    }
    result += '[' + to_greg (d_rdst[1].getlnum ()) + "]=";
    result += to_greg (d_rsrc[0].getlnum ());
    result += ','  + to_immv (d_immv[0], true, false);
    return result;
  }

  // recode a M06 instruction
  string Instr::mrecode_06 (void) const {
    string result = to_pred (d_rprd.getlnum ());
   switch (d_opcd) {
   case M_LDFS:
     result += "ldfs" + to_lhint(d_lhint) + " "; 
     break;
   case M_LDFD:
     result += "ldfd" + to_lhint(d_lhint) + " ";
     break;
   case M_LDF8:
     result += "ldf8" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFE:
     result += "ldfe" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFS_S:
     result += "ldfs.s" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFD_S:
     result += "ldfd.s" + to_lhint(d_lhint) + " ";
     break;
   case M_LDF8_S:
     result += "ldf8.s" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFE_S:
     result += "ldfe.s" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFS_A:
     result += "ldfs.a" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFD_A:
     result += "ldfd.a" + to_lhint(d_lhint) + " ";
     break;
   case M_LDF8_A:
     result += "ldf8.a" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFE_A:
     result += "ldfe.a" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFS_SA:
     result += "ldfs.sa" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFD_SA:
     result += "ldfd.sa" + to_lhint(d_lhint) + " ";
     break;
   case M_LDF8_SA:
     result += "ldf8.sa" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFE_SA:
     result += "ldfe.sa" + to_lhint(d_lhint) + " ";
     break;
   case M_LDF_FILL:
     result += "ldf.fill" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFS_C_CLR:
     result += "ldfs.c.clr" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFD_C_CLR:
     result += "ldfd.c.clr" + to_lhint(d_lhint) + " ";
     break;
   case M_LDF8_C_CLR:
     result += "ldf8.c.clr" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFE_C_CLR:
     result += "ldfe.c.clr" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFS_C_NC:
     result += "ldfs.c.nc" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFD_C_NC:
     result += "ldfd.c.nc" + to_lhint(d_lhint) + " ";
     break;
   case M_LDF8_C_NC:
     result += "ldf8.c.nc" + to_lhint(d_lhint) + " ";
     break; 
   case M_LDFE_C_NC:
     result += "ldfe.c.nc" + to_lhint(d_lhint) + " ";
     break;
   default:
     break;
   }
   result += to_freg (d_rdst[0].getlnum ()) + "=[";
   result += to_greg (d_rsrc[1].getlnum ()) + ']';
    return result;
  }

  // recode a M07 instruction
  string Instr::mrecode_07 (void) const {
    string result = to_pred (d_rprd.getlnum ());
   switch (d_opcd) {
   case M_LDFS_UPD_R:
     result += "ldfs" + to_lhint(d_lhint) + " "; 
     break;
   case M_LDFD_UPD_R:
     result += "ldfd" + to_lhint(d_lhint) + " ";
     break;
   case M_LDF8_UPD_R:
     result += "ldf8" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFE_UPD_R:
     result += "ldfe" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFS_S_UPD_R:
     result += "ldfs.s" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFD_S_UPD_R:
     result += "ldfd.s" + to_lhint(d_lhint) + " ";
     break;
   case M_LDF8_S_UPD_R:
     result += "ldf8.s" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFE_S_UPD_R:
     result += "ldfe.s" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFS_A_UPD_R:
     result += "ldfs.a" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFD_A_UPD_R:
     result += "ldfd.a" + to_lhint(d_lhint) + " ";
     break;
   case M_LDF8_A_UPD_R:
     result += "ldf8.a" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFE_A_UPD_R:
     result += "ldfe.a" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFS_SA_UPD_R:
     result += "ldfs.sa" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFD_SA_UPD_R:
     result += "ldfd.sa" + to_lhint(d_lhint) + " ";
     break;
   case M_LDF8_SA_UPD_R:
     result += "ldf8.sa" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFE_SA_UPD_R:
     result += "ldfe.sa" + to_lhint(d_lhint) + " ";
     break;
   case M_LDF_FILL_UPD_R:
     result += "ldf.fill" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFS_C_CLR_UPD_R:
     result += "ldfs.c.clr" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFD_C_CLR_UPD_R:
     result += "ldfd.c.clr" + to_lhint(d_lhint) + " ";
     break;
   case M_LDF8_C_CLR_UPD_R:
     result += "ldf8.c.clr" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFE_C_CLR_UPD_R:
     result += "ldfe.c.clr" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFS_C_NC_UPD_R:
     result += "ldfs.c.nc" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFD_C_NC_UPD_R:
     result += "ldfd.c.nc" + to_lhint(d_lhint) + " ";
     break;
   case M_LDF8_C_NC_UPD_R:
     result += "ldf8.c.nc" + to_lhint(d_lhint) + " ";
     break; 
   case M_LDFE_C_NC_UPD_R:
     result += "ldfe.c.nc" + to_lhint(d_lhint) + " ";
     break;
   default:
     break;
   }
   result += to_freg (d_rdst[0].getlnum ()) + "=[";
   result += to_greg (d_rsrc[1].getlnum ()) + "],";
   result += to_greg (d_rsrc[0].getlnum ());
   return result;
  }

  // recode a M08 instruction
  string Instr::mrecode_08 (void) const {
    string result = to_pred (d_rprd.getlnum ());
   switch (d_opcd) {
   case M_LDFS_UPD_I:
     result += "ldfs" + to_lhint(d_lhint) + " "; 
     break;
   case M_LDFD_UPD_I:
     result += "ldfd" + to_lhint(d_lhint) + " ";
     break;
   case M_LDF8_UPD_I:
     result += "ldf8" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFE_UPD_I:
     result += "ldfe" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFS_S_UPD_I:
     result += "ldfs.s" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFD_S_UPD_I:
     result += "ldfd.s" + to_lhint(d_lhint) + " ";
     break;
   case M_LDF8_S_UPD_I:
     result += "ldf8.s" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFE_S_UPD_I:
     result += "ldfe.s" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFS_A_UPD_I:
     result += "ldfs.a" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFD_A_UPD_I:
     result += "ldfd.a" + to_lhint(d_lhint) + " ";
     break;
   case M_LDF8_A_UPD_I:
     result += "ldf8.a" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFE_A_UPD_I:
     result += "ldfe.a" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFS_SA_UPD_I:
     result += "ldfs.sa" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFD_SA_UPD_I:
     result += "ldfd.sa" + to_lhint(d_lhint) + " ";
     break;
   case M_LDF8_SA_UPD_I:
     result += "ldf8.sa" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFE_SA_UPD_I:
     result += "ldfe.sa" + to_lhint(d_lhint) + " ";
     break;
   case M_LDF_FILL_UPD_I:
     result += "ldf.fill" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFS_C_CLR_UPD_I:
     result += "ldfs.c.clr" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFD_C_CLR_UPD_I:
     result += "ldfd.c.clr" + to_lhint(d_lhint) + " ";
     break;
   case M_LDF8_C_CLR_UPD_I:
     result += "ldf8.c.clr" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFE_C_CLR_UPD_I:
     result += "ldfe.c.clr" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFS_C_NC_UPD_I:
     result += "ldfs.c.nc" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFD_C_NC_UPD_I:
     result += "ldfd.c.nc" + to_lhint(d_lhint) + " ";
     break;
   case M_LDF8_C_NC_UPD_I:
     result += "ldf8.c.nc" + to_lhint(d_lhint) + " ";
     break; 
   case M_LDFE_C_NC_UPD_I:
     result += "ldfe.c.nc" + to_lhint(d_lhint) + " ";
     break;
   default:
     break;
   }
   result += to_freg (d_rdst[0].getlnum ()) + "=[";
   result += to_greg (d_rsrc[1].getlnum ()) + "],";
   result += to_immv (d_immv[0], true, false);
   return result;
  }

  // recode a M09 instruction
  string Instr::mrecode_09 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case M_STFS:
      result += "stfs" + to_shint(d_shint) + " [";
      break;
    case M_STFD:
      result += "stfd" + to_shint(d_shint) + " [";
      break;
    case M_STF8:
      result += "stf8" + to_shint(d_shint) + " [";
      break;
    case M_STFE:
      result += "stfe" + to_shint(d_shint) + " [";
      break;
    case M_STF_SPILL:
      result += "stf.spill" + to_shint(d_shint) + " [";
      break;
    default:
	break;
    }
    result += to_greg(d_rsrc[1].getlnum ()) +"]=";
    result += to_freg(d_rsrc[0].getlnum ());
    return result;
  }

  // recode a M10 instruction
  string Instr::mrecode_10 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case M_STFS_UPD_I:
      result += "stfs" + to_shint(d_shint) + " [";
      break;
    case M_STFD_UPD_I:
      result += "stfd" + to_shint(d_shint) + " [";
      break;
    case M_STF8_UPD_I:
      result += "stf8" + to_shint(d_shint) + " [";
      break;
    case M_STFE_UPD_I:
      result += "stfe" + to_shint(d_shint) + " [";
      break;
    case M_STF_SPILL_UPD_I:
      result += "stf.spill" + to_shint(d_shint) + " [";
      break;
    default:
	break;
    }
    result += to_greg(d_rsrc[1].getlnum ()) + "]=";
    result += to_freg(d_rsrc[0].getlnum ());
    result += ',' + to_immv(d_immv[0], true, false);
    return result;
  }

  // recode a M11 instruction
  string Instr::mrecode_11 (void) const {
  string result = to_pred (d_rprd.getlnum ());
   switch (d_opcd) {
   case M_LDFPS:
     result += "ldfps" + to_lhint(d_lhint) + " "; 
     break;
   case M_LDFPD:
     result += "ldfpd" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFP8:
     result += "ldfp8" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFPS_S:
     result += "ldfps.s" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFPD_S:
     result += "ldfpd.s" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFP8_S:
     result += "ldfp8.s" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFPS_A:
     result += "ldfps.a" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFPD_A:
     result += "ldfpd.a" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFP8_A:
     result += "ldfp8.a" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFPS_SA:
     result += "ldfps.sa" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFPD_SA:
     result += "ldfpd.sa" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFP8_SA:
     result += "ldfp8.sa" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFPS_C_CLR:
     result += "ldfps.c.clr" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFPD_C_CLR:
     result += "ldfpd.c.clr" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFP8_C_CLR:
     result += "ldfp8.c.clr" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFPS_C_NC:
     result += "ldfps.c.nc" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFPD_C_NC:
     result += "ldfpd.c.nc" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFP8_C_NC:
     result += "ldfp8.c.nc" + to_lhint(d_lhint) + " ";
     break; 
   default:
     break;
   }
   result += to_freg (d_rdst[0].getlnum ()) + ",";
   result += to_freg (d_rdst[1].getlnum ()) + "=[";
   result += to_greg (d_rsrc[1].getlnum ()) + "]";
   return result;
  }

  // recode a M12 instruction
  string Instr::mrecode_12 (void) const {
  string result = to_pred (d_rprd.getlnum ());
   switch (d_opcd) {
   case M_LDFPS_UPD:
     result += "ldfps" + to_lhint(d_lhint) + " "; 
     break;
   case M_LDFPD_UPD:
     result += "ldfpd" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFP8_UPD:
     result += "ldfp8" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFPS_S_UPD:
     result += "ldfps.s" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFPD_S_UPD:
     result += "ldfpd.s" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFP8_S_UPD:
     result += "ldfp8.s" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFPS_A_UPD:
     result += "ldfps.a" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFPD_A_UPD:
     result += "ldfpd.a" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFP8_A_UPD:
     result += "ldfp8.a" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFPS_SA_UPD:
     result += "ldfps.sa" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFPD_SA_UPD:
     result += "ldfpd.sa" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFP8_SA_UPD:
     result += "ldfp8.sa" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFPS_C_CLR_UPD:
     result += "ldfps.c.clr" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFPD_C_CLR_UPD:
     result += "ldfpd.c.clr" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFP8_C_CLR_UPD:
     result += "ldfp8.c.clr" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFPS_C_NC_UPD:
     result += "ldfps.c.nc" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFPD_C_NC_UPD:
     result += "ldfpd.c.nc" + to_lhint(d_lhint) + " ";
     break;
   case M_LDFP8_C_NC_UPD:
     result += "ldfp8.c.nc" + to_lhint(d_lhint) + " ";
     break; 
   default:
     break;
   }
   result += to_freg (d_rdst[0].getlnum ()) + ",";
   result += to_freg (d_rdst[1].getlnum ()) + "=[";
   result += to_greg (d_rsrc[1].getlnum ()) + "],";
   result += to_immv (d_immv[0], false, false);
   return result;
  }

  // recode a M13 instruction
  string Instr::mrecode_13 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case M_LFETCH: 
      result += "lfetch" + to_lfhint (d_lfhint) + " ";
      break;
    case M_LFETCH_EXCL:
      result += "lfetch.excl" + to_lfhint (d_lfhint) + " ";
      break;
    case M_LFETCH_FAULT:
      result += "lfetch.fault" + to_lfhint (d_lfhint) + " ";
      break;
    case M_LFETCH_FAULT_EXCL:
      result += "lfetch.fault.excl" + to_lfhint (d_lfhint) + " ";
      break;
    default:
      break;
    }
    result += "[" + to_greg (d_rsrc[1].getlnum ()) + "]";
    return result;
  }

  // recode a M14 instruction
  string Instr::mrecode_14 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case M_LFETCH_UPD_R: 
      result += "lfetch" + to_lfhint (d_lfhint) + " ";
      break;
    case M_LFETCH_EXCL_UPD_R:
      result += "lfetch.excl" + to_lfhint (d_lfhint) + " ";
      break;
    case M_LFETCH_FAULT_UPD_R:
      result += "lfetch.fault" + to_lfhint (d_lfhint) + " ";
      break;
    case M_LFETCH_FAULT_EXCL_UPD_R:
      result += "lfetch.fault.excl" + to_lfhint (d_lfhint) + " ";
      break;
    default:
      break;
    }
    result += "[" + to_greg (d_rsrc[1].getlnum ()) + "],";
    result += to_greg (d_rsrc[0].getlnum ());
    return result;
  }

  // recode a M15 instruction
  string Instr::mrecode_15 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case M_LFETCH_UPD_I: 
      result += "lfetch" + to_lfhint (d_lfhint) + " ";
      break;
    case M_LFETCH_EXCL_UPD_I:
      result += "lfetch.excl" + to_lfhint (d_lfhint) + " ";
      break;
    case M_LFETCH_FAULT_UPD_I:
      result += "lfetch.fault" + to_lfhint (d_lfhint) + " ";
      break;
    case M_LFETCH_FAULT_EXCL_UPD_I:
      result += "lfetch.fault.excl" + to_lfhint (d_lfhint) + " ";
      break;
    default:
      break;
    }
    result += "[" + to_greg (d_rsrc[1].getlnum ()) + "],";
    result += to_immv (d_immv[0], true, false);
    return result;
  }

  // recode a M16 instruction
  string Instr::mrecode_16 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case M_CMPXCHG1_ACQ:
      result += "cmpxchg1.acq";
      break;
    case M_CMPXCHG2_ACQ:
      result += "cmpxchg2.acq";
      break;
    case M_CMPXCHG4_ACQ:
      result += "cmpxchg4.acq";
      break;
    case M_CMPXCHG8_ACQ:
      result += "cmpxchg8.acq";
      break;
    case M_CMPXCHG1_REL:
      result += "cmpxchg1.rel";
      break;
    case M_CMPXCHG2_REL:
      result += "cmpxchg2.rel";
      break;
    case M_CMPXCHG4_REL:
      result += "cmpxchg4.rel";
      break;
    case M_CMPXCHG8_REL:
      result += "cmpxchg8.rel";
      break;
    case M_XCHG1:
      result += "xchg1";
      break;
    case M_XCHG2:
      result += "xchg2";
      break;
    case M_XCHG4:
      result += "xchg4";
      break;
    case M_XCHG8:
      result += "xchg8";
      break;
    default:
      break;
    }
    result += to_lhint (d_lhint) + " ";
    result += to_greg  (d_rdst[0].getlnum ()) + "=[";
    result += to_greg  (d_rsrc[1].getlnum ()) + "],";
    result += to_greg  (d_rsrc[0].getlnum ());
    switch (d_opcd){
    case M_CMPXCHG1_ACQ:
    case M_CMPXCHG2_ACQ:
    case M_CMPXCHG4_ACQ:
    case M_CMPXCHG8_ACQ:
    case M_CMPXCHG1_REL:
    case M_CMPXCHG2_REL:
    case M_CMPXCHG4_REL:
    case M_CMPXCHG8_REL:
      result += "," + to_areg(d_rsrc[2].getlnum ());
      break;
    default:
      break;
    }
    return result;
  }

  // recode a M17 instruction
  string Instr::mrecode_17 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case M_FETCHADD4_ACQ:
      result += "fetchadd4.acq";
      break;
    case M_FETCHADD8_ACQ:
      result += "fetchadd8.acq";
      break;
    case M_FETCHADD4_REL:
      result += "fetchadd4.rel";
      break;
    case M_FETCHADD8_REL:
      result += "fetchadd8.rel";
      break; 
    default:
      break;
    }
    result += to_lhint (d_lhint) + " ";
    result += to_greg (d_rdst[0].getlnum ()) + "=[";
    result += to_greg (d_rsrc[1].getlnum ()) + "],";
    result += to_immv (d_immv[0], true, false);
    return result;
  }
  
  // recode a M18 instruction
  string Instr::mrecode_18 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case M_SETF_SIG:
      result += "setf.sig ";
      break;
    case M_SETF_EXP:
      result += "setf.exp ";
      break;
    case M_SETF_S:
      result += "setf.s ";
      break;
    case M_SETF_D:
      result += "setf.d ";
      break;
    default:
      assert (false);
      break;
    }
    result += to_freg (d_rdst[0].getlnum ()) + '=';
    result += to_greg (d_rsrc[0].getlnum ());
    return result;
  }

  // recode a M19 instruction
  string Instr::mrecode_19 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case M_GETF_SIG:
      result += "getf.sig ";
      break;
    case M_GETF_EXP:
      result += "getf.exp ";
      break;
    case M_GETF_S:
      result += "getf.s ";
      break; 
    case M_GETF_D:
      result += "getf.d ";
      break;
    default:
      assert (false);
      break;
    }
    result += to_greg (d_rdst[0].getlnum ()) + '=';
    result += to_freg (d_rsrc[0].getlnum ());
    return result;
  }

  // recode a M24 instruction
  string Instr::mrecode_24 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case M_INVALA:
      result += "invala";
      break;
    case M_FWB:
      result += "fwb";
      break;
    case M_MF:
      result += "mf";
      break;
    case M_MF_A:
      result += "mf.a";
      break;
    case M_SRLZ_D:
      result += "srlz.d";
      break;
    case M_SRLZ_I:
      result += "srlz.i";
      break;
    case M_SYNC_I:
      result += "sync.i";
      break;
    default:
      assert (false);
      break;
    }
    return result;
  }

  // recode a M20 instruction
  string Instr::mrecode_20 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += "chk.s.m " + to_greg (d_rsrc[0].getlnum ());
    result += "," + to_brtg (d_immv[0], d_iip);
    return result;
  }

  // recode a M21 instruction
  string Instr::mrecode_21 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += "chk.s " + to_freg (d_rsrc[0].getlnum ());
    result += "," + to_brtg (d_immv[0], d_iip);
    return result;
  }

  // recode a M22 instruction
  string Instr::mrecode_22 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd){
    case M_CHK_A_NC_I:
      result += "chk.a.nc ";
      break;
    case M_CHK_A_CLR_I:
      result += "chk.a.clr ";
      break;
    default:
      break;
    }
    result += to_greg (d_rdst[1].getlnum ());
    result += "," + to_brtg (d_immv[0], d_iip);
    return result;
  }

  // recode a M23 instruction
  string Instr::mrecode_23 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd){
    case M_CHK_A_NC_F:
      result += "chk.a.nc ";
      break;
    case M_CHK_A_CLR_F:
      result += "chk.a.clr ";
      break;
    default:
      break;
    }
    result += to_freg (d_rdst[1].getlnum ());
    result += "," + to_brtg (d_immv[0], d_iip);
    return result;
  }

  // recode a M25 instruction
  string Instr::mrecode_25 (void) const {
    string result = to_pred (0);
    switch (d_opcd){
    case M_FLUSHRS:
      result += "flushrs";
      break;
    case M_LOADRS:
      result += "loadrs";
      break;
    default:
      break;
    }
    return result;
  }

  // recode a M26 instruction
  string Instr::mrecode_26 (void) const {
    string result = to_pred (0);
    switch (d_opcd){
    case M_INVALA_E:
      result += "invala.e ";
      break;
    default:
      break;
    }
    result += to_greg (d_rdst[0].getlnum ());
    return result;
  }

  // recode a M27 instruction
  string Instr::mrecode_27 (void) const {
    string result = to_pred (0);
    switch (d_opcd){
    case M_INVALA_E_F:
      result += "invala.e ";
      break;
    default:
      break;
    }
    result += to_freg (d_rdst[0].getlnum ());
    return result;
  }
   
   // recode a M28 instruction
  string Instr::mrecode_28 (void) const {
    string result = to_pred (0);
    switch (d_opcd){
    case M_FC:
      result += "fc ";
      break;
    case M_PTC_E:
      result += "ptc.e ";
      break;
    default:
      break;
    }
    result += to_greg (d_rsrc[1].getlnum ());
    return result;
  }

  // recode a M29 instruction
  string Instr::mrecode_29 (void) const {
  string result = to_pred (d_rprd.getlnum ());
  result += "mov.m " + to_areg (d_rdst[0].getlnum ());
  result += '=' + to_greg (d_rsrc[0].getlnum ());
  return result;
  }

  // recode a M30 instruction
  string Instr::mrecode_30 (void) const {
  string result = to_pred (d_rprd.getlnum ());
  result += "mov.m " + to_areg (d_rdst[0].getlnum ()) + '=';
  result +=  to_immv (d_immv[0], true, false);
  return result;
  }

  // recode a M31 instruction
  string Instr::mrecode_31 (void) const {
  string result = to_pred (d_rprd.getlnum ());
  result += "mov.m " + to_greg (d_rdst[0].getlnum ()) + '=';
  result += to_areg(d_rsrc[1].getlnum ());
  return result;
  }

  // recode a M32 instruction
  string Instr::mrecode_32 (void) const {
  string result = to_pred (d_rprd.getlnum ());
  result += "mov " + to_creg (d_rdst[0].getlnum ()) + '=';
  result += to_greg(d_rsrc[0].getlnum ());
  return result;
  }

  // recode a M33 instruction
  string Instr::mrecode_33 (void) const {
  string result = to_pred (d_rprd.getlnum ());
  result += "mov " + to_greg (d_rdst[0].getlnum ()) + '=';
  result += to_creg(d_rsrc[1].getlnum ());
  return result;
  }
  
  // recode a M34 instruction
  string Instr::mrecode_34 (void) const {
    long r = d_immv[2] << 3;
    long o = d_immv[0] - d_immv[1];
    long i = d_immv[0] - o;
    string result = to_pred (d_rprd.getlnum ());
    result += "alloc ";
    result += to_greg (d_rdst[0].getlnum ()) + "=ar.pfs,";
    result += to_immv (i, true, false) + ",0,";
    result += to_immv (o, true, false) + ',' + to_immv (r, true, false);
    return result;
  }

  // recode a M35 instruction
  string Instr::mrecode_35 (void) const {
    string result = to_pred (0);
    switch (d_opcd){
    case M_MOV_TO_PSR_L:
      result += "mov psr.l";
      break;
    case M_MOV_TO_PSR_UM:
      result += "mov psr.um";
      break;
    default:
      break;
    }
    result += "=" + to_greg (d_rsrc[0].getlnum ());
    return result;
  }
  
 // recode a M36 instruction
  string Instr::mrecode_36 (void) const {
    string result = to_pred (0);
    switch (d_opcd){
    case M_MOV_FROM_PSR:
      result += "mov " + to_greg (d_rdst[0].getlnum ()) + "=psr";
      break;
    case M_MOV_FROM_PSR_UM:
      result += "mov " + to_greg (d_rdst[0].getlnum ()) + "=psr.um" ;
      break;
    default:
      break;
    }
    return result;
  }

  // recode a M37 instruction
  string Instr::mrecode_37 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case M_BREAK: result += "break.m "; break;
    case M_NOP:   result += "nop.m "; break;
    default: 
      break;
    }
    // add immediate value
    result += to_immv (d_immv[0], false, true);
    return result;
  }

  // recode a M38 instruction
  string Instr::mrecode_38 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case M_PROBE_R_R:
      result += "probe.r ";
      break;
    case M_PROBE_W_R:
      result += "probe.w ";
      break;
    default: 
      break;
    }
  result += to_greg (d_rdst[0].getlnum ()) + "=";
  result += to_greg (d_rsrc[1].getlnum ()) + ',' ;
  result += to_greg (d_rsrc[0].getlnum ());
  return result;
  }
  
  // recode a M39 instruction
  string Instr::mrecode_39 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case M_PROBE_R_I:
      result += "probe.r ";
      break;
    case M_PROBE_W_I:
      result += "probe.w ";
      break;
    default: 
      break;
    }
  result += to_greg (d_rdst[0].getlnum ()) + "=";
  result += to_greg (d_rsrc[1].getlnum ()) + ',' ;
  result += to_immv (d_immv[0], false, false);
  return result;
  }

 // recode a M40 instruction
  string Instr::mrecode_40 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case M_PROBE_RW_FAULT:
      result += "probe.rw.fault ";
      break;
    case M_PROBE_R_FAULT:
      result += "probe.r.fault ";
      break;
    case M_PROBE_W_FAULT:
      result += "probe.w.fault ";
      break;
    default: 
      break;
    }
  result += to_greg (d_rsrc[1].getlnum ()) + ',' ;
  result += to_immv (d_immv[0], false, false);
  return result;
  }

 // recode a M41 instruction
  string Instr::mrecode_41 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case M_ITC_D:
      result += "itc.d ";
      break;
    case M_ITC_I:
      result += "itc.i ";
      break;
    default: 
      break;
    }
    result += to_greg (d_rsrc[0].getlnum ());
    return result;
  }
  
 // recode a M44 instruction
  string Instr::mrecode_44 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case M_SUM:
      result += "sum ";
      break;
    case M_RUM:
      result += "rum ";
      break;
    case M_SSM:
      result += "ssm ";
      break;
    case M_RSM:
      result += "rsm ";
      break;
    default: 
      break;
    }  
    result += to_immv (d_immv[0], false, true);
    return result;
  }

 // recode a M45 instruction
  string Instr::mrecode_45 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case M_PTC_L:
      result += "ptc.l ";
      break;
    case M_PTC_G:
      result += "ptc.g ";
      break;
    case M_PTC_GA:
      result += "ptc.ga ";
      break;
    case M_PTR_D:
      result += "ptr.d ";
      break;
    case M_PTR_I:
      result += "ptr.i ";
      break;
    default: 
      break;
    }  
    result +=  to_greg (d_rsrc[1].getlnum ()) + ',';
    result +=  to_greg (d_rsrc[0].getlnum ());
    return result;
  }

 // recode a M46 instruction
  string Instr::mrecode_46 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case M_THASH:
      result += "thash ";
      break;
    case M_TTAG:
      result += "ttag ";
      break;
    case M_TPA:
      result += "tpa ";
      break;
    case M_TAK:
      result += "tak ";
      break;
    default: 
      break;
    }  
    result +=  to_greg (d_rdst[0].getlnum ()) + '=';
    result +=  to_greg (d_rsrc[1].getlnum ());
    return result;
  }

  // recode a M unit instruction

  string Instr::mrecode (void) const {
    assert (d_valid == true);
    string result;
    // select instruction group
    switch (d_opcd) {
    case M_LD1:
    case M_LD2:
    case M_LD4:
    case M_LD8:
    case M_LD1_S:
    case M_LD2_S:
    case M_LD4_S:
    case M_LD8_S:
    case M_LD1_A:
    case M_LD2_A:
    case M_LD4_A:
    case M_LD8_A:
    case M_LD1_SA:
    case M_LD2_SA:
    case M_LD4_SA:
    case M_LD8_SA:
    case M_LD1_BIAS:
    case M_LD2_BIAS:
    case M_LD4_BIAS:
    case M_LD8_BIAS:  
    case M_LD1_ACQ:
    case M_LD2_ACQ:
    case M_LD4_ACQ:
    case M_LD8_ACQ:
    case M_LD8_FILL:
    case M_LD1_C_CLR:
    case M_LD2_C_CLR:
    case M_LD4_C_CLR:
    case M_LD8_C_CLR:
    case M_LD1_C_NC:
    case M_LD2_C_NC:
    case M_LD4_C_NC:
    case M_LD8_C_NC:
    case M_LD1_C_CLR_ACQ:
    case M_LD2_C_CLR_ACQ:
    case M_LD4_C_CLR_ACQ:
    case M_LD8_C_CLR_ACQ:
      result = mrecode_01();
      break;
    case M_LD1_UPD_R:
    case M_LD2_UPD_R:
    case M_LD4_UPD_R:
    case M_LD8_UPD_R:
    case M_LD1_S_UPD_R:
    case M_LD2_S_UPD_R:
    case M_LD4_S_UPD_R:
    case M_LD8_S_UPD_R:
    case M_LD1_A_UPD_R:
    case M_LD2_A_UPD_R:
    case M_LD4_A_UPD_R:
    case M_LD8_A_UPD_R:
    case M_LD1_SA_UPD_R:
    case M_LD2_SA_UPD_R:
    case M_LD4_SA_UPD_R:
    case M_LD8_SA_UPD_R:
    case M_LD1_BIAS_UPD_R:
    case M_LD2_BIAS_UPD_R:
    case M_LD4_BIAS_UPD_R:
    case M_LD8_BIAS_UPD_R:  
    case M_LD1_ACQ_UPD_R:
    case M_LD2_ACQ_UPD_R:
    case M_LD4_ACQ_UPD_R:
    case M_LD8_ACQ_UPD_R:
    case M_LD8_FILL_UPD_R:
    case M_LD1_C_CLR_UPD_R:
    case M_LD2_C_CLR_UPD_R:
    case M_LD4_C_CLR_UPD_R:
    case M_LD8_C_CLR_UPD_R:
    case M_LD1_C_NC_UPD_R:
    case M_LD2_C_NC_UPD_R:
    case M_LD4_C_NC_UPD_R:
    case M_LD8_C_NC_UPD_R:
    case M_LD1_C_CLR_ACQ_UPD_R:
    case M_LD2_C_CLR_ACQ_UPD_R:
    case M_LD4_C_CLR_ACQ_UPD_R:
    case M_LD8_C_CLR_ACQ_UPD_R:
      result = mrecode_02();
      break;
    case M_LD1_UPD_I:
    case M_LD2_UPD_I:
    case M_LD4_UPD_I:
    case M_LD8_UPD_I:
    case M_LD1_S_UPD_I:
    case M_LD2_S_UPD_I:
    case M_LD4_S_UPD_I:
    case M_LD8_S_UPD_I:
    case M_LD1_A_UPD_I:
    case M_LD2_A_UPD_I:
    case M_LD4_A_UPD_I:
    case M_LD8_A_UPD_I:
    case M_LD1_SA_UPD_I:
    case M_LD2_SA_UPD_I:
    case M_LD4_SA_UPD_I:
    case M_LD8_SA_UPD_I:
    case M_LD1_BIAS_UPD_I:
    case M_LD2_BIAS_UPD_I:
    case M_LD4_BIAS_UPD_I:
    case M_LD8_BIAS_UPD_I:  
    case M_LD1_ACQ_UPD_I:
    case M_LD2_ACQ_UPD_I:
    case M_LD4_ACQ_UPD_I:
    case M_LD8_ACQ_UPD_I:
    case M_LD8_FILL_UPD_I:
    case M_LD1_C_CLR_UPD_I:
    case M_LD2_C_CLR_UPD_I:
    case M_LD4_C_CLR_UPD_I:
    case M_LD8_C_CLR_UPD_I:
    case M_LD1_C_NC_UPD_I:
    case M_LD2_C_NC_UPD_I:
    case M_LD4_C_NC_UPD_I:
    case M_LD8_C_NC_UPD_I:
    case M_LD1_C_CLR_ACQ_UPD_I:
    case M_LD2_C_CLR_ACQ_UPD_I:
    case M_LD4_C_CLR_ACQ_UPD_I:
    case M_LD8_C_CLR_ACQ_UPD_I:
      result = mrecode_03();
      break;
    case M_ST1:
    case M_ST2:
    case M_ST4:
    case M_ST8:
    case M_ST1_REL:
    case M_ST2_REL:
    case M_ST4_REL:
    case M_ST8_REL:
    case M_ST8_SPILL:
      result = mrecode_04();
      break;
    case M_ST1_UPD_I:
    case M_ST2_UPD_I:
    case M_ST4_UPD_I:
    case M_ST8_UPD_I:
    case M_ST1_REL_UPD_I:
    case M_ST2_REL_UPD_I:
    case M_ST4_REL_UPD_I:
    case M_ST8_REL_UPD_I:
    case M_ST8_SPILL_UPD_I:
      result = mrecode_05();
      break;
    case M_LDFS:
    case M_LDFD:
    case M_LDF8:
    case M_LDFE:
    case M_LDFS_S:
    case M_LDFD_S:
    case M_LDF8_S:
    case M_LDFE_S:
    case M_LDFS_A:
    case M_LDFD_A:
    case M_LDF8_A:
    case M_LDFE_A:
    case M_LDFS_SA:
    case M_LDFD_SA:
    case M_LDF8_SA:
    case M_LDFE_SA:
    case M_LDF_FILL:
    case M_LDFS_C_CLR:
    case M_LDFD_C_CLR:
    case M_LDF8_C_CLR:
    case M_LDFE_C_CLR:
    case M_LDFS_C_NC:
    case M_LDFD_C_NC:
    case M_LDF8_C_NC:
    case M_LDFE_C_NC:
      result = mrecode_06();
      break;
    case M_LDFS_UPD_R:
    case M_LDFD_UPD_R:
    case M_LDF8_UPD_R:
    case M_LDFE_UPD_R:
    case M_LDFS_S_UPD_R:
    case M_LDFD_S_UPD_R:
    case M_LDF8_S_UPD_R:
    case M_LDFE_S_UPD_R:
    case M_LDFS_A_UPD_R:
    case M_LDFD_A_UPD_R:
    case M_LDF8_A_UPD_R:
    case M_LDFE_A_UPD_R:
    case M_LDFS_SA_UPD_R:
    case M_LDFD_SA_UPD_R:
    case M_LDF8_SA_UPD_R:
    case M_LDFE_SA_UPD_R:
    case M_LDF_FILL_UPD_R:
    case M_LDFS_C_CLR_UPD_R:
    case M_LDFD_C_CLR_UPD_R:
    case M_LDF8_C_CLR_UPD_R:
    case M_LDFE_C_CLR_UPD_R:
    case M_LDFS_C_NC_UPD_R:
    case M_LDFD_C_NC_UPD_R:
    case M_LDF8_C_NC_UPD_R:
    case M_LDFE_C_NC_UPD_R:
      result = mrecode_07();
      break;
    case M_LDFS_UPD_I:
    case M_LDFD_UPD_I:
    case M_LDF8_UPD_I:
    case M_LDFE_UPD_I:
    case M_LDFS_S_UPD_I:
    case M_LDFD_S_UPD_I:
    case M_LDF8_S_UPD_I:
    case M_LDFE_S_UPD_I:
    case M_LDFS_A_UPD_I:
    case M_LDFD_A_UPD_I:
    case M_LDF8_A_UPD_I:
    case M_LDFE_A_UPD_I:
    case M_LDFS_SA_UPD_I:
    case M_LDFD_SA_UPD_I:
    case M_LDF8_SA_UPD_I:
    case M_LDFE_SA_UPD_I:
    case M_LDF_FILL_UPD_I:
    case M_LDFS_C_CLR_UPD_I:
    case M_LDFD_C_CLR_UPD_I:
    case M_LDF8_C_CLR_UPD_I:
    case M_LDFE_C_CLR_UPD_I:
    case M_LDFS_C_NC_UPD_I:
    case M_LDFD_C_NC_UPD_I:
    case M_LDF8_C_NC_UPD_I:
    case M_LDFE_C_NC_UPD_I:
      result = mrecode_08();
      break;
    case M_STFS:
    case M_STFD:
    case M_STF8:
    case M_STFE:
    case M_STF_SPILL:
      result = mrecode_09();
      break;
    case M_STFS_UPD_I:
    case M_STFD_UPD_I:
    case M_STF8_UPD_I:
    case M_STFE_UPD_I:
    case M_STF_SPILL_UPD_I:
      result = mrecode_10 ();
      break;
    case M_LDFPS:
    case M_LDFPD:
    case M_LDFP8:
    case M_LDFPS_S:
    case M_LDFPD_S:
    case M_LDFP8_S:
    case M_LDFPS_A:
    case M_LDFPD_A:
    case M_LDFP8_A:
    case M_LDFPS_SA:
    case M_LDFPD_SA:
    case M_LDFP8_SA:
    case M_LDFPS_C_CLR:
    case M_LDFPD_C_CLR:
    case M_LDFP8_C_CLR:
    case M_LDFPS_C_NC:
    case M_LDFPD_C_NC:
    case M_LDFP8_C_NC:
      result = mrecode_11 ();
      break;
    case M_LDFPD_UPD:
    case M_LDFP8_UPD:
    case M_LDFPD_S_UPD:
    case M_LDFP8_S_UPD:
    case M_LDFPD_A_UPD:
    case M_LDFP8_A_UPD:
    case M_LDFPD_SA_UPD:
    case M_LDFP8_SA_UPD:
    case M_LDFPD_C_CLR_UPD:
    case M_LDFP8_C_CLR_UPD:
    case M_LDFPD_C_NC_UPD:
    case M_LDFP8_C_NC_UPD:
    case M_LDFPS_UPD:
    case M_LDFPS_S_UPD:
    case M_LDFPS_A_UPD:
    case M_LDFPS_SA_UPD:
    case M_LDFPS_C_CLR_UPD:
    case M_LDFPS_C_NC_UPD:
      result = mrecode_12 ();
      break;
    case M_LFETCH:
    case M_LFETCH_EXCL:
    case M_LFETCH_FAULT:
    case M_LFETCH_FAULT_EXCL:
      result = mrecode_13 ();
      break;
    case M_LFETCH_UPD_R:
    case M_LFETCH_EXCL_UPD_R:
    case M_LFETCH_FAULT_UPD_R:
    case M_LFETCH_FAULT_EXCL_UPD_R:
      result = mrecode_14 ();
      break;
    case M_LFETCH_UPD_I:
    case M_LFETCH_EXCL_UPD_I:
    case M_LFETCH_FAULT_UPD_I:
    case M_LFETCH_FAULT_EXCL_UPD_I:
      result = mrecode_15 ();
      break;
    case M_CMPXCHG1_ACQ:
    case M_CMPXCHG2_ACQ:
    case M_CMPXCHG4_ACQ:
    case M_CMPXCHG8_ACQ:
    case M_CMPXCHG1_REL:
    case M_CMPXCHG2_REL:
    case M_CMPXCHG4_REL:
    case M_CMPXCHG8_REL:
    case M_XCHG1:
    case M_XCHG2:
    case M_XCHG4:
    case M_XCHG8:
      result = mrecode_16 ();
      break;
    case M_FETCHADD4_ACQ:
    case M_FETCHADD8_ACQ:
    case M_FETCHADD4_REL:
    case M_FETCHADD8_REL:
      result = mrecode_17 ();
      break;
    case M_SETF_SIG:
    case M_SETF_EXP:
    case M_SETF_S:
    case M_SETF_D:
      result = mrecode_18 ();      
      break;
    case M_GETF_SIG:
    case M_GETF_EXP:
    case M_GETF_S:
    case M_GETF_D:
      result = mrecode_19 ();      
      break;
    case M_CHK_S_M:
      result = mrecode_20 ();
      break;
    case M_CHK_S:
      result = mrecode_21 ();
      break;
   case M_CHK_A_NC_I:
   case M_CHK_A_CLR_I:
      result = mrecode_22 ();
      break;
    case M_CHK_A_NC_F:
    case M_CHK_A_CLR_F:
      result = mrecode_23 ();
      break;
    case M_INVALA:
    case M_FWB:
    case M_MF:
    case M_MF_A:
    case M_SRLZ_D:
    case M_SRLZ_I:
    case M_SYNC_I:
      result = mrecode_24 ();  
      break;
    case M_FLUSHRS:
    case M_LOADRS:
      result = mrecode_25 ();  
      break;
    case M_INVALA_E:
      result = mrecode_26 ();
      break;
    case M_INVALA_E_F:
      result = mrecode_27 ();
      break;
    case M_FC:
    case M_PTC_E:
      result = mrecode_28 ();
      break;
    case M_MOV_TO_AR_R:
      result = mrecode_29 ();  
      break;
    case M_MOV_TO_AR_I:
      result = mrecode_30 ();  
      break;
    case M_MOV_FROM_AR:
      result = mrecode_31 ();
      break;
    case M_MOV_TO_CR:
      result = mrecode_32 ();
      break;
    case M_MOV_FROM_CR:
      result = mrecode_33 ();
      break;
    case M_ALLOC:
      result = mrecode_34 ();
      break;
    case M_MOV_TO_PSR_L:
    case M_MOV_TO_PSR_UM:
      result = mrecode_35 ();
      break;
    case M_MOV_FROM_PSR:
    case M_MOV_FROM_PSR_UM:
      result = mrecode_36 ();
      break;
    case M_BREAK:
    case M_NOP:
      result = mrecode_37 ();
      break;
    case M_PROBE_R_R:
    case M_PROBE_W_R:
      result = mrecode_38 ();
      break;
    case M_PROBE_R_I:
    case M_PROBE_W_I:
      result = mrecode_39 ();
      break;
    case M_PROBE_RW_FAULT:
    case M_PROBE_R_FAULT:
    case M_PROBE_W_FAULT:
      result = mrecode_40 ();
      break;
    case M_ITC_D:
    case M_ITC_I:
      result = mrecode_41 ();
      break;
    case  M_SUM:
    case  M_RUM:
    case  M_SSM:
    case  M_RSM:
      result = mrecode_44 ();
      break;
    case  M_PTC_L:
    case  M_PTC_G:
    case  M_PTC_GA:
    case  M_PTR_D:
    case  M_PTR_I:
      result = mrecode_45 ();
      break;
    case  M_THASH:
    case  M_TTAG:
    case  M_TPA:
    case  M_TAK:
      result = mrecode_46 ();
      break;
    default:
      break;
    }
    return result;
  }
}
