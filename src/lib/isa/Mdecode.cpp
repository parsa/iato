// ---------------------------------------------------------------------------
// - Mdecode.cpp                                                             -
// - iato:isa library - M unit instruction decode implementation             -
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

  // return the load hint
  static t_ldhint get_lhint (const t_octa inst) {
    t_byte lh = ((t_byte) (inst >> 28)) & 0x03;
    assert (lh != 0x02);
    if (lh == 0x00) return LDNONE;
    if (lh == 0x01) return LDNT1;
    return LDNTA;
  }

  // return the store hint
  static t_sthint get_shint (const t_octa inst) {
    t_byte sh = ((t_byte) (inst >> 28)) & 0x03;
    assert (sh != 0x02);
    assert (sh != 0x01);
    if (sh == 0x00) return STNONE;
    return STNTA;
  }

  // return the line prefetch hint
  static t_lfhint get_lfhint (const t_octa inst) {
    t_byte lfh = ((t_byte) (inst >> 28)) & 0x03;
    if (lfh == 0x00) return LFNONE;
    if (lfh == 0x01) return LFNT1;
    if (lfh == 0x02) return LFNT2;
    return LFNTA;
  }
  
  // decode major opcode 0 - M22 M23 M24 M25 M30 M37
  static t_iopc get_opcode_group_0 (t_octa inst) {
    // check X3 bits (33:35) X2 bits (31:32) and X4 bits (27:30)
    t_byte x3 = ((t_byte) (inst >> 33)) & 0x07;
    t_byte x2 = ((t_byte) (inst >> 31)) & 0x03;
    t_byte x4 = ((t_byte) (inst >> 27)) & 0x0F;
    // decode integer advanced load
    if (x3 == 0x04) return M_CHK_A_NC_I;
    if (x3 == 0x05) return M_CHK_A_CLR_I;
    // decode floating point advanced load
    if (x3 == 0x06) return M_CHK_A_NC_F;
    if (x3 == 0x07) return M_CHK_A_CLR_F;
    // decode memory misc
    if ((x3 == 0x00) && (x4 == 0x00) && (x2 == 0x00)) return M_BREAK;
    if ((x3 == 0x00) && (x4 == 0x01) && (x2 == 0x00)) return M_NOP;
    // decode mov to ar
    if ((x3 == 0x00) && (x4 == 0x08) && (x2 == 0x02)) return M_MOV_TO_AR_I;
    // decode invalid alat
    if ((x3 == 0x00) && (x4 == 0x00) && (x2 == 0x01)) return M_INVALA;
    // decode integer alat entry invalidate
    if ((x3 == 0x00) && (x4 == 0x02) && (x2 == 0x01)) return M_INVALA_E;
    // decode floating-point alat entry invalidate
    if ((x3 == 0x00) && (x4 == 0x03) && (x2 == 0x01)) return M_INVALA_E_F;
    // decode flush write buffers
    if ((x3 == 0x00) && (x4 == 0x00) && (x2 == 0x02)) return M_FWB;
    // decode memory fence
    if ((x3 == 0x00) && (x4 == 0x02) && (x2 == 0x02)) return M_MF;
    if ((x3 == 0x00) && (x4 == 0x03) && (x2 == 0x02)) return M_MF_A;
    // decode serialize
    if ((x3 == 0x00) && (x4 == 0x00) && (x2 == 0x03)) return M_SRLZ_D;
    if ((x3 == 0x00) && (x4 == 0x01) && (x2 == 0x03)) return M_SRLZ_I;
    // decode synchronization
    if ((x3 == 0x00) && (x4 == 0x03) && (x2 == 0x03)) return M_SYNC_I;
    // decode flushrs
    if ((x3 == 0x00) && (x4 == 0x0C) && (x2 == 0x00)) return M_FLUSHRS;
    // decode loadrs
    if ((x3 == 0x00) && (x4 == 0x0A) && (x2 == 0x00)) return M_LOADRS;
    // decode set/reset user/system mask
    if ((x3 == 0x00) && (x4 == 0x04))                 return M_SUM;
    if ((x3 == 0x00) && (x4 == 0x05))                 return M_RUM;
    if ((x3 == 0x00) && (x4 == 0x06))                 return M_SSM;
    if ((x3 == 0x00) && (x4 == 0x07))                 return M_RSM;
    // mark as invalid to cancel update
    return U_INVALID;
  }

  // decode major opcode 1 - M28 M29 M31->M34
  static t_iopc get_opcode_group_1 (t_octa inst) {
    // check X3 bits (33:35) X6 bits (27:32)
    t_byte x3 = ((t_byte) (inst >> 33)) & 0x07;
    t_byte x6 = ((t_byte) (inst >> 27)) & 0x3F;
    // decode alloc
    if (x3 == 0x06) return M_ALLOC;
    // decode mov to ar register (M-unit)
    if ((x3 == 0x00) && (x6 == 0x2A)) return M_MOV_TO_AR_R;
    // decode for mov to ar register (M-unit)
    if ((x3 == 0x00) && (x6 == 0x22)) return M_MOV_FROM_AR;
    // decode mov to cr
    if ((x3 == 0x00) && (x6 == 0x2C)) return M_MOV_TO_CR;
    // decode mov from cr
    if ((x3 == 0x00) && (x6 == 0x24)) return M_MOV_FROM_CR;
    // decode integer speculation check
    if (x3 == 0x01) return M_CHK_S_M;
    // decode floating point speculation check
    if (x3 == 0x03) return M_CHK_S;
    // decode flush cache/purge translation cache entry
    if ((x3 == 0x00) && (x6 == 0x30)) return M_FC;
    if ((x3 == 0x00) && (x6 == 0x34)) return M_PTC_E;
    // decode move to psr
    if ((x3 == 0x00) && (x6 == 0x2D)) return M_MOV_TO_PSR_L;
    if ((x3 == 0x00) && (x6 == 0x29)) return M_MOV_TO_PSR_UM;
    // decode move from psr
    if ((x3 == 0x00) && (x6 == 0x25)) return M_MOV_FROM_PSR;
    if ((x3 == 0x00) && (x6 == 0x21)) return M_MOV_FROM_PSR_UM;
    // decode probe-register
    if ((x3 == 0x00) && (x6 == 0x38)) return M_PROBE_R_R;
    if ((x3 == 0x00) && (x6 == 0x39)) return M_PROBE_W_R;
    // decode probe-immediate2
    if ((x3 == 0x00) && (x6 == 0x18)) return M_PROBE_R_I;
    if ((x3 == 0x00) && (x6 == 0x19)) return M_PROBE_W_I;
    // decode probe
    if ((x3 == 0x00) && (x6 == 0x31)) return M_PROBE_RW_FAULT;
    if ((x3 == 0x00) && (x6 == 0x32)) return M_PROBE_R_FAULT;
    if ((x3 == 0x00) && (x6 == 0x33)) return M_PROBE_W_FAULT;
    // decode translation cache insert
    if ((x3 == 0x00) && (x6 == 0x2E)) return M_ITC_D;
    if ((x3 == 0x00) && (x6 == 0x2F)) return M_ITC_I;
    // decode translation purge
    if ((x3 == 0x00) && (x6 == 0x09)) return M_PTC_L; 
    if ((x3 == 0x00) && (x6 == 0x0A)) return M_PTC_G;
    if ((x3 == 0x00) && (x6 == 0x0B)) return M_PTC_GA;
    if ((x3 == 0x00) && (x6 == 0x0C)) return M_PTR_D;
    if ((x3 == 0x00) && (x6 == 0x0D)) return M_PTR_I;
    // decode translation access
    if ((x3 == 0x00) && (x6 == 0x1A)) return M_THASH;
    if ((x3 == 0x00) && (x6 == 0x1B)) return M_TTAG;
    if ((x3 == 0x00) && (x6 == 0x1E)) return M_TPA;
    if ((x3 == 0x00) && (x6 == 0x1F)) return M_TAK;
    // mark as invalid to cancel update
    return U_INVALID;
  }

  // decode major opcode 4 - M1 M2 M4 M16 M19
  static t_iopc get_opcode_group_4 (t_octa inst) {
    // check m bit (36) x bit (27) x6 (30:35)
    bool      m = ((t_byte) (inst >> 36) & 0x01) == 0x01;
    bool      x = ((t_byte) (inst >> 27) & 0x01) == 0x01;
    t_byte   x6 = (t_byte) (inst >> 30) & 0x3F;
    // decode load
    if ((m == false) && (x == false) && (x6 == 0x00)) return M_LD1;
    if ((m == false) && (x == false) && (x6 == 0x01)) return M_LD2;
    if ((m == false) && (x == false) && (x6 == 0x02)) return M_LD4;
    if ((m == false) && (x == false) && (x6 == 0x03)) return M_LD8;
    if ((m == false) && (x == false) && (x6 == 0x04)) return M_LD1_S;
    if ((m == false) && (x == false) && (x6 == 0x05)) return M_LD2_S;
    if ((m == false) && (x == false) && (x6 == 0x06)) return M_LD4_S;
    if ((m == false) && (x == false) && (x6 == 0x07)) return M_LD8_S;
    if ((m == false) && (x == false) && (x6 == 0x08)) return M_LD1_A;
    if ((m == false) && (x == false) && (x6 == 0x09)) return M_LD2_A;
    if ((m == false) && (x == false) && (x6 == 0x0A)) return M_LD4_A;
    if ((m == false) && (x == false) && (x6 == 0x0B)) return M_LD8_A;
    if ((m == false) && (x == false) && (x6 == 0x0C)) return M_LD1_SA;
    if ((m == false) && (x == false) && (x6 == 0x0D)) return M_LD2_SA;
    if ((m == false) && (x == false) && (x6 == 0x0E)) return M_LD4_SA;
    if ((m == false) && (x == false) && (x6 == 0x0F)) return M_LD8_SA;
    if ((m == false) && (x == false) && (x6 == 0x10)) return M_LD1_BIAS;
    if ((m == false) && (x == false) && (x6 == 0x11)) return M_LD2_BIAS;
    if ((m == false) && (x == false) && (x6 == 0x12)) return M_LD4_BIAS;
    if ((m == false) && (x == false) && (x6 == 0x13)) return M_LD8_BIAS;
    if ((m == false) && (x == false) && (x6 == 0x14)) return M_LD1_ACQ;
    if ((m == false) && (x == false) && (x6 == 0x15)) return M_LD2_ACQ;
    if ((m == false) && (x == false) && (x6 == 0x16)) return M_LD4_ACQ;
    if ((m == false) && (x == false) && (x6 == 0x17)) return M_LD8_ACQ;
    if ((m == false) && (x == false) && (x6 == 0x1B)) return M_LD8_FILL;
    if ((m == false) && (x == false) && (x6 == 0x20)) return M_LD1_C_CLR;
    if ((m == false) && (x == false) && (x6 == 0x21)) return M_LD2_C_CLR;
    if ((m == false) && (x == false) && (x6 == 0x22)) return M_LD4_C_CLR;
    if ((m == false) && (x == false) && (x6 == 0x23)) return M_LD8_C_CLR;
    if ((m == false) && (x == false) && (x6 == 0x24)) return M_LD1_C_NC;
    if ((m == false) && (x == false) && (x6 == 0x25)) return M_LD2_C_NC;
    if ((m == false) && (x == false) && (x6 == 0x26)) return M_LD4_C_NC;
    if ((m == false) && (x == false) && (x6 == 0x27)) return M_LD8_C_NC;
    if ((m == false) && (x == false) && (x6 == 0x28)) return M_LD1_C_CLR_ACQ;
    if ((m == false) && (x == false) && (x6 == 0x29)) return M_LD2_C_CLR_ACQ;
    if ((m == false) && (x == false) && (x6 == 0x2A)) return M_LD4_C_CLR_ACQ;
    if ((m == false) && (x == false) && (x6 == 0x2B)) return M_LD8_C_CLR_ACQ;
    // decode load with update by register
    if ((m == true) && (x == false) && (x6 == 0x00)) return M_LD1_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x01)) return M_LD2_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x02)) return M_LD4_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x03)) return M_LD8_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x04)) return M_LD1_S_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x05)) return M_LD2_S_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x06)) return M_LD4_S_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x07)) return M_LD8_S_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x08)) return M_LD1_A_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x09)) return M_LD2_A_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x0A)) return M_LD4_A_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x0B)) return M_LD8_A_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x0C)) return M_LD1_SA_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x0D)) return M_LD2_SA_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x0E)) return M_LD4_SA_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x0F)) return M_LD8_SA_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x10)) return M_LD1_BIAS_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x11)) return M_LD2_BIAS_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x12)) return M_LD4_BIAS_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x13)) return M_LD8_BIAS_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x14)) return M_LD1_ACQ_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x15)) return M_LD2_ACQ_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x16)) return M_LD4_ACQ_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x17)) return M_LD8_ACQ_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x1B)) return M_LD8_FILL_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x20)) return M_LD1_C_CLR_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x21)) return M_LD2_C_CLR_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x22)) return M_LD4_C_CLR_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x23)) return M_LD8_C_CLR_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x24)) return M_LD1_C_NC_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x25)) return M_LD2_C_NC_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x26)) return M_LD4_C_NC_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x27)) return M_LD8_C_NC_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x28)) 
      return M_LD1_C_CLR_ACQ_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x29))
      return M_LD2_C_CLR_ACQ_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x2A))
      return M_LD4_C_CLR_ACQ_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x2B))
      return M_LD8_C_CLR_ACQ_UPD_R;
    // decode store
    if ((m == false) && (x == false) && (x6 == 0x30)) return M_ST1;
    if ((m == false) && (x == false) && (x6 == 0x31)) return M_ST2;
    if ((m == false) && (x == false) && (x6 == 0x32)) return M_ST4;
    if ((m == false) && (x == false) && (x6 == 0x33)) return M_ST8;
    if ((m == false) && (x == false) && (x6 == 0x34)) return M_ST1_REL;
    if ((m == false) && (x == false) && (x6 == 0x35)) return M_ST2_REL;
    if ((m == false) && (x == false) && (x6 == 0x36)) return M_ST4_REL;
    if ((m == false) && (x == false) && (x6 == 0x37)) return M_ST8_REL;
    if ((m == false) && (x == false) && (x6 == 0x3B)) return M_ST8_SPILL;
    // decode getf
    if ((m == false) && (x == true) && (x6 == 0x1C)) return M_GETF_SIG;
    if ((m == false) && (x == true) && (x6 == 0x1D)) return M_GETF_EXP;
    if ((m == false) && (x == true) && (x6 == 0x1E)) return M_GETF_S;
    if ((m == false) && (x == true) && (x6 == 0x1F)) return M_GETF_D;
    // decode compare and exchange
    if ((m == false) && (x == true) && (x6 == 0x00)) return M_CMPXCHG1_ACQ;
    if ((m == false) && (x == true) && (x6 == 0x01)) return M_CMPXCHG2_ACQ;
    if ((m == false) && (x == true) && (x6 == 0x02)) return M_CMPXCHG4_ACQ;
    if ((m == false) && (x == true) && (x6 == 0x03)) return M_CMPXCHG8_ACQ;
    if ((m == false) && (x == true) && (x6 == 0x04)) return M_CMPXCHG1_REL;
    if ((m == false) && (x == true) && (x6 == 0x05)) return M_CMPXCHG2_REL;
    if ((m == false) && (x == true) && (x6 == 0x06)) return M_CMPXCHG4_REL;
    if ((m == false) && (x == true) && (x6 == 0x07)) return M_CMPXCHG8_REL;
    // decode exchange
    if ((m == false) && (x == true) && (x6 == 0x08)) return M_XCHG1;
    if ((m == false) && (x == true) && (x6 == 0x09)) return M_XCHG2;
    if ((m == false) && (x == true) && (x6 == 0x0A)) return M_XCHG4;
    if ((m == false) && (x == true) && (x6 == 0x0B)) return M_XCHG8;
    // decode fetch and add immediate
    if ((m == false) && (x == true) && (x6 == 0x12)) return M_FETCHADD4_ACQ;
    if ((m == false) && (x == true) && (x6 == 0x13)) return M_FETCHADD8_ACQ;
    if ((m == false) && (x == true) && (x6 == 0x16)) return M_FETCHADD4_REL;
    if ((m == false) && (x == true) && (x6 == 0x17)) return M_FETCHADD8_REL;
    // mark as invalid to cancel update
    return U_INVALID;
  }

  // decode major opcode 5 - M3
  static t_iopc get_opcode_group_5 (t_octa inst) {
    // check x6 (30:35)
    t_byte   x6 = (t_byte) (inst >> 30) & 0x3F;
    // check for load with update by immediate
    if (x6 == 0x00) return M_LD1_UPD_I;
    if (x6 == 0x01) return M_LD2_UPD_I;
    if (x6 == 0x02) return M_LD4_UPD_I;
    if (x6 == 0x03) return M_LD8_UPD_I;
    if (x6 == 0x04) return M_LD1_S_UPD_I;
    if (x6 == 0x05) return M_LD2_S_UPD_I;
    if (x6 == 0x06) return M_LD4_S_UPD_I;
    if (x6 == 0x07) return M_LD8_S_UPD_I;
    if (x6 == 0x08) return M_LD1_A_UPD_I;
    if (x6 == 0x09) return M_LD2_A_UPD_I;
    if (x6 == 0x0A) return M_LD4_A_UPD_I;
    if (x6 == 0x0B) return M_LD8_A_UPD_I;
    if (x6 == 0x0C) return M_LD1_SA_UPD_I;
    if (x6 == 0x0D) return M_LD2_SA_UPD_I;
    if (x6 == 0x0E) return M_LD4_SA_UPD_I;
    if (x6 == 0x0F) return M_LD8_SA_UPD_I;
    if (x6 == 0x10) return M_LD1_BIAS_UPD_I;
    if (x6 == 0x11) return M_LD2_BIAS_UPD_I;
    if (x6 == 0x12) return M_LD4_BIAS_UPD_I;
    if (x6 == 0x13) return M_LD8_BIAS_UPD_I;
    if (x6 == 0x14) return M_LD1_ACQ_UPD_I;
    if (x6 == 0x15) return M_LD2_ACQ_UPD_I;
    if (x6 == 0x16) return M_LD4_ACQ_UPD_I;
    if (x6 == 0x17) return M_LD8_ACQ_UPD_I;
    if (x6 == 0x1B) return M_LD8_FILL_UPD_I;
    if (x6 == 0x20) return M_LD1_C_CLR_UPD_I;
    if (x6 == 0x21) return M_LD2_C_CLR_UPD_I;
    if (x6 == 0x22) return M_LD4_C_CLR_UPD_I;
    if (x6 == 0x23) return M_LD8_C_CLR_UPD_I;
    if (x6 == 0x24) return M_LD1_C_NC_UPD_I;
    if (x6 == 0x25) return M_LD2_C_NC_UPD_I;
    if (x6 == 0x26) return M_LD4_C_NC_UPD_I;
    if (x6 == 0x27) return M_LD8_C_NC_UPD_I;
    if (x6 == 0x28) return M_LD1_C_CLR_ACQ_UPD_I;
    if (x6 == 0x29) return M_LD2_C_CLR_ACQ_UPD_I;
    if (x6 == 0x2A) return M_LD4_C_CLR_ACQ_UPD_I;
    if (x6 == 0x2B) return M_LD8_C_CLR_ACQ_UPD_I;
    // check for store with update by immediate
    if (x6 == 0x30) return M_ST1_UPD_I;
    if (x6 == 0x31) return M_ST2_UPD_I;
    if (x6 == 0x32) return M_ST4_UPD_I;
    if (x6 == 0x33) return M_ST8_UPD_I;
    if (x6 == 0x34) return M_ST1_REL_UPD_I;
    if (x6 == 0x35) return M_ST2_REL_UPD_I;
    if (x6 == 0x36) return M_ST4_REL_UPD_I;
    if (x6 == 0x37) return M_ST8_REL_UPD_I;
    if (x6 == 0x3B) return M_ST8_SPILL_UPD_I;
    return U_INVALID;
  }

  // decode major opcode 6 - M6 M7 M11->M14 M18 
  static t_iopc get_opcode_group_6 (t_octa inst) {
    // check m bit (36) x bit (27) x6 (30:35)
    bool    m = ((t_byte) (inst >> 36) & 0x01) == 0x01;
    bool    x = ((t_byte) (inst >> 27) & 0x01) == 0x01;
    t_byte x6 = (t_byte) (inst >> 30) & 0x3F;
    // check for setf
    if ((m == false) && (x == true) && (x6 == 0x1C)) return M_SETF_SIG;
    if ((m == false) && (x == true) && (x6 == 0x1D)) return M_SETF_EXP;
    if ((m == false) && (x == true) && (x6 == 0x1E)) return M_SETF_S;
    if ((m == false) && (x == true) && (x6 == 0x1F)) return M_SETF_D;
    // decode floating point load
    if ((m == false) && (x == false) && (x6 == 0x02)) return M_LDFS;
    if ((m == false) && (x == false) && (x6 == 0x03)) return M_LDFD;
    if ((m == false) && (x == false) && (x6 == 0x01)) return M_LDF8;
    if ((m == false) && (x == false) && (x6 == 0x00)) return M_LDFE;
    if ((m == false) && (x == false) && (x6 == 0x06)) return M_LDFS_S;
    if ((m == false) && (x == false) && (x6 == 0x07)) return M_LDFD_S;
    if ((m == false) && (x == false) && (x6 == 0x05)) return M_LDF8_S;
    if ((m == false) && (x == false) && (x6 == 0x04)) return M_LDFE_S;
    if ((m == false) && (x == false) && (x6 == 0x0A)) return M_LDFS_A;
    if ((m == false) && (x == false) && (x6 == 0x0B)) return M_LDFD_A;
    if ((m == false) && (x == false) && (x6 == 0x09)) return M_LDF8_A;
    if ((m == false) && (x == false) && (x6 == 0x08)) return M_LDFE_A;
    if ((m == false) && (x == false) && (x6 == 0x0E)) return M_LDFS_SA;
    if ((m == false) && (x == false) && (x6 == 0x0F)) return M_LDFD_SA;
    if ((m == false) && (x == false) && (x6 == 0x0D)) return M_LDF8_SA;
    if ((m == false) && (x == false) && (x6 == 0x0C)) return M_LDFE_SA;
    if ((m == false) && (x == false) && (x6 == 0x1B)) return M_LDF_FILL;
    if ((m == false) && (x == false) && (x6 == 0x22)) return M_LDFS_C_CLR;
    if ((m == false) && (x == false) && (x6 == 0x23)) return M_LDFD_C_CLR;
    if ((m == false) && (x == false) && (x6 == 0x21)) return M_LDF8_C_CLR;
    if ((m == false) && (x == false) && (x6 == 0x20)) return M_LDFE_C_CLR;
    if ((m == false) && (x == false) && (x6 == 0x26)) return M_LDFS_C_NC;
    if ((m == false) && (x == false) && (x6 == 0x27)) return M_LDFD_C_NC;
    if ((m == false) && (x == false) && (x6 == 0x25)) return M_LDF8_C_NC;
    if ((m == false) && (x == false) && (x6 == 0x24)) return M_LDFE_C_NC;
    // decode ldf with update by register
    if ((m == true)  && (x == false) && (x6 == 0x02)) return M_LDFS_UPD_R;
    if ((m == true)  && (x == false) && (x6 == 0x03)) return M_LDFD_UPD_R;
    if ((m == true)  && (x == false) && (x6 == 0x01)) return M_LDF8_UPD_R;
    if ((m == true)  && (x == false) && (x6 == 0x00)) return M_LDFE_UPD_R;
    if ((m == true)  && (x == false) && (x6 == 0x06)) return M_LDFS_S_UPD_R;
    if ((m == true)  && (x == false) && (x6 == 0x07)) return M_LDFD_S_UPD_R;
    if ((m == true)  && (x == false) && (x6 == 0x05)) return M_LDF8_S_UPD_R;
    if ((m == true)  && (x == false) && (x6 == 0x04)) return M_LDFE_S_UPD_R;
    if ((m == true)  && (x == false) && (x6 == 0x0A)) return M_LDFS_A_UPD_R;
    if ((m == true)  && (x == false) && (x6 == 0x0B)) return M_LDFD_A_UPD_R;
    if ((m == true)  && (x == false) && (x6 == 0x09)) return M_LDF8_A_UPD_R;
    if ((m == true)  && (x == false) && (x6 == 0x08)) return M_LDFE_A_UPD_R;
    if ((m == true)  && (x == false) && (x6 == 0x0E)) return M_LDFS_SA_UPD_R;
    if ((m == true)  && (x == false) && (x6 == 0x0F)) return M_LDFD_SA_UPD_R;
    if ((m == true)  && (x == false) && (x6 == 0x0D)) return M_LDF8_SA_UPD_R;
    if ((m == true)  && (x == false) && (x6 == 0x0C)) return M_LDFE_SA_UPD_R;
    if ((m == true)  && (x == false) && (x6 == 0x1B)) return M_LDF_FILL_UPD_R;
    if ((m == true)  && (x == false) && (x6 == 0x22))
      return M_LDFS_C_CLR_UPD_R;
    if ((m == true)  && (x == false) && (x6 == 0x23))
      return M_LDFD_C_CLR_UPD_R;
    if ((m == true)  && (x == false) && (x6 == 0x21))
      return M_LDF8_C_CLR_UPD_R;
    if ((m == true)  && (x == false) && (x6 == 0x20))
      return M_LDFE_C_CLR_UPD_R;
    if ((m == true)  && (x == false) && (x6 == 0x26)) return M_LDFS_C_NC_UPD_R;
    if ((m == true)  && (x == false) && (x6 == 0x27)) return M_LDFD_C_NC_UPD_R;
    if ((m == true)  && (x == false) && (x6 == 0x25)) return M_LDF8_C_NC_UPD_R;
    if ((m == true)  && (x == false) && (x6 == 0x24)) return M_LDFE_C_NC_UPD_R;
    // decode floating point load pair
    if ((m == false) && (x == true) && (x6 == 0x02)) return M_LDFPS;
    if ((m == false) && (x == true) && (x6 == 0x03)) return M_LDFPD;
    if ((m == false) && (x == true) && (x6 == 0x01)) return M_LDFP8;
    if ((m == false) && (x == true) && (x6 == 0x06)) return M_LDFPS_S;
    if ((m == false) && (x == true) && (x6 == 0x07)) return M_LDFPD_S;
    if ((m == false) && (x == true) && (x6 == 0x05)) return M_LDFP8_S;
    if ((m == false) && (x == true) && (x6 == 0x0A)) return M_LDFPS_A;
    if ((m == false) && (x == true) && (x6 == 0x0B)) return M_LDFPD_A;
    if ((m == false) && (x == true) && (x6 == 0x09)) return M_LDFP8_A;
    if ((m == false) && (x == true) && (x6 == 0x0E)) return M_LDFPS_SA;
    if ((m == false) && (x == true) && (x6 == 0x0F)) return M_LDFPD_SA;
    if ((m == false) && (x == true) && (x6 == 0x0D)) return M_LDFP8_SA;
    if ((m == false) && (x == true) && (x6 == 0x22)) return M_LDFPS_C_CLR;
    if ((m == false) && (x == true) && (x6 == 0x23)) return M_LDFPD_C_CLR;
    if ((m == false) && (x == true) && (x6 == 0x21)) return M_LDFP8_C_CLR;
    if ((m == false) && (x == true) && (x6 == 0x26)) return M_LDFPS_C_NC;
    if ((m == false) && (x == true) && (x6 == 0x27)) return M_LDFPD_C_NC;
    if ((m == false) && (x == true) && (x6 == 0x25)) return M_LDFP8_C_NC;
    // decode floating point load pair increment by immediate
    if ((m == true) && (x == true) && (x6 == 0x02)) return M_LDFPS_UPD;
    if ((m == true) && (x == true) && (x6 == 0x03)) return M_LDFPD_UPD;
    if ((m == true) && (x == true) && (x6 == 0x01)) return M_LDFP8_UPD;
    if ((m == true) && (x == true) && (x6 == 0x06)) return M_LDFPS_S_UPD;
    if ((m == true) && (x == true) && (x6 == 0x07)) return M_LDFPD_S_UPD;
    if ((m == true) && (x == true) && (x6 == 0x05)) return M_LDFP8_S_UPD;
    if ((m == true) && (x == true) && (x6 == 0x0A)) return M_LDFPS_A_UPD;
    if ((m == true) && (x == true) && (x6 == 0x0B)) return M_LDFPD_A_UPD;
    if ((m == true) && (x == true) && (x6 == 0x09)) return M_LDFP8_A_UPD;
    if ((m == true) && (x == true) && (x6 == 0x0E)) return M_LDFPS_SA_UPD;
    if ((m == true) && (x == true) && (x6 == 0x0F)) return M_LDFPD_SA_UPD;
    if ((m == true) && (x == true) && (x6 == 0x0D)) return M_LDFP8_SA_UPD;
    if ((m == true) && (x == true) && (x6 == 0x22)) return M_LDFPS_C_CLR_UPD;
    if ((m == true) && (x == true) && (x6 == 0x23)) return M_LDFPD_C_CLR_UPD;
    if ((m == true) && (x == true) && (x6 == 0x21)) return M_LDFP8_C_CLR_UPD;
    if ((m == true) && (x == true) && (x6 == 0x26)) return M_LDFPS_C_NC_UPD;
    if ((m == true) && (x == true) && (x6 == 0x27)) return M_LDFPD_C_NC_UPD;
    if ((m == true) && (x == true) && (x6 == 0x25)) return M_LDFP8_C_NC_UPD;
    // decode line prefetch
    if ((m == false) && (x == false) && (x6 == 0x2C)) return M_LFETCH;
    if ((m == false) && (x == false) && (x6 == 0x2D)) return M_LFETCH_EXCL;
    if ((m == false) && (x == false) && (x6 == 0x2E)) return M_LFETCH_FAULT;
    if ((m == false) && (x == false) && (x6 == 0x2F)) 
      return M_LFETCH_FAULT_EXCL;
    // decode line prefetch increment by register
    if ((m == true) && (x == false) && (x6 == 0x2C))
      return M_LFETCH_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x2D)) 
      return M_LFETCH_EXCL_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x2E)) 
      return M_LFETCH_FAULT_UPD_R;
    if ((m == true) && (x == false) && (x6 == 0x2F))
      return M_LFETCH_FAULT_EXCL_UPD_R;
    // decode for stf
    if ((m == false) && (x == false) && (x6 == 0x32)) return M_STFS;
    if ((m == false) && (x == false) && (x6 == 0x33)) return M_STFD;
    if ((m == false) && (x == false) && (x6 == 0x31)) return M_STF8;
    if ((m == false) && (x == false) && (x6 == 0x30)) return M_STFE;
    if ((m == false) && (x == false) && (x6 == 0x3B)) return M_STF_SPILL;
    // mark as invalid to cancel update
    return U_INVALID;
  }

  // decode major opcode 7 - M8 
  static t_iopc get_opcode_group_7 (t_octa inst) {
    // check x6 (30:35)
    t_byte x6 = (t_byte) (inst >> 30) & 0x3F;
    // check for ldf with update by immediate
    if (x6 == 0x02) return M_LDFS_UPD_I;
    if (x6 == 0x03) return M_LDFD_UPD_I;
    if (x6 == 0x01) return M_LDF8_UPD_I;
    if (x6 == 0x00) return M_LDFE_UPD_I;
    if (x6 == 0x06) return M_LDFS_S_UPD_I;
    if (x6 == 0x07) return M_LDFD_S_UPD_I;
    if (x6 == 0x05) return M_LDF8_S_UPD_I;
    if (x6 == 0x04) return M_LDFE_S_UPD_I;
    if (x6 == 0x0A) return M_LDFS_A_UPD_I;
    if (x6 == 0x0B) return M_LDFD_A_UPD_I;
    if (x6 == 0x09) return M_LDF8_A_UPD_I;
    if (x6 == 0x08) return M_LDFE_A_UPD_I;
    if (x6 == 0x0E) return M_LDFS_SA_UPD_I;
    if (x6 == 0x0F) return M_LDFD_SA_UPD_I;
    if (x6 == 0x0D) return M_LDF8_SA_UPD_I;
    if (x6 == 0x0C) return M_LDFE_SA_UPD_I;
    if (x6 == 0x1B) return M_LDF_FILL_UPD_I;
    if (x6 == 0x22) return M_LDFS_C_CLR_UPD_I;
    if (x6 == 0x23) return M_LDFD_C_CLR_UPD_I;
    if (x6 == 0x21) return M_LDF8_C_CLR_UPD_I;
    if (x6 == 0x20) return M_LDFE_C_CLR_UPD_I;
    if (x6 == 0x26) return M_LDFS_C_NC_UPD_I;
    if (x6 == 0x27) return M_LDFD_C_NC_UPD_I;
    if (x6 == 0x25) return M_LDF8_C_NC_UPD_I;
    if (x6 == 0x24) return M_LDFE_C_NC_UPD_I;
    // check for stf with update by immediate
    if (x6 == 0x32) return M_STFS_UPD_I;
    if (x6 == 0x33) return M_STFD_UPD_I;
    if (x6 == 0x31) return M_STF8_UPD_I;
    if (x6 == 0x30) return M_STFE_UPD_I;
    if (x6 == 0x3B) return M_STF_SPILL_UPD_I;
    // decode line prefetch increment by immediate
    if (x6 == 0x2C) return M_LFETCH_UPD_I;
    if (x6 == 0x2D) return M_LFETCH_EXCL_UPD_I;
    if (x6 == 0x2E) return M_LFETCH_FAULT_UPD_I;    
    if (x6 == 0x2F) return M_LFETCH_FAULT_EXCL_UPD_I;
    // mark as invalid to cancel update
    return U_INVALID;
  }

  // update a M unit instruction

  void Instr::mupdate (void) {
    if ((d_valid == true) || (d_opcd == U_INVALID)) return;
    // update predicate, source and destination registers
    switch (d_opcd) {

      // M01 instruction group
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
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_lhint = get_lhint (d_inst);
      d_ildb  = true;
      d_group = "M01";
      d_valid = true;
      break;
    case M_LD8_FILL:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[2].setlnum (AREG, AR_UNAT);
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_lhint = get_lhint (d_inst);
      d_ildb  = true;
      d_group = "M01";
      d_valid = true;
      break;

      // M02 instruction group
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
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_rdst[1] = d_rsrc[1];
      d_lhint   = get_lhint (d_inst);
      d_ildb    = true;
      d_group   = "M02";
      d_valid   = true;
      break;
    case M_LD8_FILL_UPD_R:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[2].setlnum (AREG, AR_UNAT);
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_rdst[1] = d_rsrc[1];
      d_lhint   = get_lhint (d_inst);
      d_ildb    = true;
      d_group   = "M02";
      d_valid   = true;
      break;

      // M03 instruction group
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
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_immv[0] = get_im09b (d_inst);
      d_rdst[1] = d_rsrc[1];
      d_lhint   = get_lhint (d_inst);
      d_ildb    = true;
      d_group   = "M03";
      d_valid   = true;
      break;
    case M_LD8_FILL_UPD_I:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[2].setlnum (AREG, AR_UNAT);
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_rdst[1] = d_rsrc[1];
      d_immv[0] = get_im09b (d_inst);
      d_lhint   = get_lhint (d_inst);
      d_ildb    = true;
      d_group   = "M03";
      d_valid   = true;
      break;

      //M04 instruction group
    case M_ST1:
    case M_ST2:
    case M_ST4:
    case M_ST8:
    case M_ST1_REL:
    case M_ST2_REL:
    case M_ST4_REL:
    case M_ST8_REL:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_shint = get_shint (d_inst);
      d_istb  = true;
      d_group = "M04";
      d_valid = true;
      break;
    case M_ST8_SPILL:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_rsrc[2].setlnum (AREG, AR_UNAT);
      d_rdst[2].setlnum (AREG, AR_UNAT);
      d_shint = get_shint (d_inst);
      d_istb  = true;
      d_group = "M04";
      d_valid = true;
      break;

      // M05 instruction group
    case M_ST1_UPD_I:
    case M_ST2_UPD_I:
    case M_ST4_UPD_I:
    case M_ST8_UPD_I:
    case M_ST1_REL_UPD_I:
    case M_ST2_REL_UPD_I:
    case M_ST4_REL_UPD_I:
    case M_ST8_REL_UPD_I:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_immv[0] = get_im09a (d_inst);
      d_rdst[1] = d_rsrc[1];
      d_shint   = get_shint (d_inst);
      d_istb    = true;
      d_group   = "M05";
      d_valid   = true;
      break;
    case M_ST8_SPILL_UPD_I:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_rsrc[2].setlnum (AREG, AR_UNAT);
      d_rdst[2].setlnum (AREG, AR_UNAT);
      d_rdst[1] = d_rsrc[1];
      d_immv[0] = get_im09a (d_inst);
      d_shint   = get_shint (d_inst);
      d_istb    = true;
      d_group   = "M05";
      d_valid   = true;
      break;

      // M06 instruction group
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
      d_rprd.setlnum      (PREG, get_pred (d_inst));
      d_rsrc[1].setlnum   (GREG, get_src1 (d_inst));
      d_rdst[0].setlnum   (FREG, get_dst0 (d_inst));
      d_lhint = get_lhint (d_inst);
      d_ildb  = true;
      d_group = "M06";
      d_valid = true;
      break;

      // M07 instruction group
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
      d_rprd.setlnum      (PREG, get_pred (d_inst));
      d_rsrc[1].setlnum   (GREG, get_src1 (d_inst));
      d_rsrc[0].setlnum   (GREG, get_src0 (d_inst));
      d_rdst[0].setlnum   (FREG, get_dst0 (d_inst));
      d_rdst[1] = d_rsrc[1];
      d_lhint   = get_lhint (d_inst);
      d_ildb    = true;
      d_group   = "M07";
      d_valid   = true;
      break;

      // M08 instruction group
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
      d_rprd.setlnum      (PREG, get_pred (d_inst));
      d_rsrc[1].setlnum   (GREG, get_src1 (d_inst));
      d_rdst[0].setlnum   (FREG, get_dst0 (d_inst));
      d_rdst[1] = d_rsrc[1];
      d_immv[0] = get_im09b (d_inst);
      d_lhint   = get_lhint (d_inst);
      d_ildb    = true;
      d_group   = "M08";
      d_valid   = true;
      break;

      // M09 instruction group
    case M_STFS:
    case M_STFD:
    case M_STF8:
    case M_STFE:
    case M_STF_SPILL:
      d_rprd.setlnum      (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum   (FREG, get_src0 (d_inst));
      d_rsrc[1].setlnum   (GREG, get_src1 (d_inst));
      d_shint = get_shint (d_inst);
      d_istb  = true;
      d_group = "M09";
      d_valid = true;
      break;

      // M10 instruction group
    case M_STFS_UPD_I:
    case M_STFD_UPD_I:
    case M_STF8_UPD_I:
    case M_STFE_UPD_I:
    case M_STF_SPILL_UPD_I:
      d_rprd.setlnum      (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum   (FREG, get_src0 (d_inst));
      d_rsrc[1].setlnum   (GREG, get_src1 (d_inst));
      d_rdst[1] = d_rsrc[1];
      d_immv[0] = get_im09a (d_inst);
      d_shint   = get_shint (d_inst);
      d_istb    = true;
      d_group   = "M10";
      d_valid   = true;
      break;

      // M11 instruction group
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
      d_rprd.setlnum      (PREG, get_pred (d_inst));
      d_rsrc[1].setlnum   (GREG, get_src1 (d_inst));
      d_rdst[0].setlnum   (FREG, get_dst0 (d_inst));
      d_rdst[1].setlnum   (FREG, get_dst1 (d_inst));
      d_lhint = get_lhint (d_inst);
      d_ildb  = true;
      d_group = "M11";
      d_valid = true;
      break;

      // M12 instruction group
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
      d_rprd.setlnum      (PREG, get_pred (d_inst));
      d_rsrc[1].setlnum   (GREG, get_src1 (d_inst));
      d_rdst[0].setlnum   (FREG, get_dst0 (d_inst));
      d_rdst[1].setlnum   (FREG, get_dst1 (d_inst));
      d_rdst[2] = d_rsrc[1];
      d_immv[0] = 16;
      d_lhint   = get_lhint (d_inst);
      d_ildb    = true;
      d_group   = "M12";
      d_valid   = true;
      break;
    case M_LDFPS_UPD:
    case M_LDFPS_S_UPD:
    case M_LDFPS_A_UPD:
    case M_LDFPS_SA_UPD:
    case M_LDFPS_C_CLR_UPD:
    case M_LDFPS_C_NC_UPD:
      d_rprd.setlnum      (PREG, get_pred (d_inst));
      d_rsrc[1].setlnum   (GREG, get_src1 (d_inst));
      d_rdst[0].setlnum   (FREG, get_dst0 (d_inst));
      d_rdst[1].setlnum   (FREG, get_dst1 (d_inst));
      d_rdst[2] = d_rsrc[1];
      d_immv[0] = 8;
      d_lhint   = get_lhint (d_inst);
      d_ildb    = true;
      d_group   = "M12";
      d_valid   = true;
      break;

      // M13 instruction group
    case M_LFETCH:
    case M_LFETCH_EXCL:
    case M_LFETCH_FAULT:
    case M_LFETCH_FAULT_EXCL:
      d_rprd.setlnum        (PREG, get_pred (d_inst));
      d_rsrc[1].setlnum     (GREG, get_src1 (d_inst));
      d_lfhint = get_lfhint (d_inst);
      d_group  = "M13";
      d_valid  = true;
      break;

      // M14 instruction group
    case M_LFETCH_UPD_R:
    case M_LFETCH_EXCL_UPD_R:
    case M_LFETCH_FAULT_UPD_R:
    case M_LFETCH_FAULT_EXCL_UPD_R:
      d_rprd.setlnum      (PREG, get_pred (d_inst));
      d_rdst[0].setlnum   (GREG, get_src1 (d_inst));
      d_rsrc[0].setlnum   (GREG, get_src0 (d_inst));
      d_rsrc[1].setlnum   (GREG, get_src1 (d_inst));
      d_lfhint = get_lfhint (d_inst);
      d_group  = "M14";
      d_valid  = true;
      break;

      // M15 instruction group
    case M_LFETCH_UPD_I:
    case M_LFETCH_EXCL_UPD_I:
    case M_LFETCH_FAULT_UPD_I:
    case M_LFETCH_FAULT_EXCL_UPD_I:
      d_rprd.setlnum         (PREG, get_pred (d_inst));
      d_rdst[0].setlnum      (GREG, get_src1 (d_inst));
      d_rsrc[1].setlnum      (GREG, get_src1 (d_inst));
      d_immv[0] = get_im09b  (d_inst);
      d_lfhint  = get_lfhint (d_inst);
      d_group   = "M15";
      d_valid   = true;
      break;

      // M16 instruction group
    case M_CMPXCHG1_ACQ:
    case M_CMPXCHG2_ACQ:
    case M_CMPXCHG4_ACQ:
    case M_CMPXCHG8_ACQ:
    case M_CMPXCHG1_REL:
    case M_CMPXCHG2_REL:
    case M_CMPXCHG4_REL:
    case M_CMPXCHG8_REL:
      d_rprd.setlnum      (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum   (FREG, get_src0 (d_inst));
      d_rsrc[1].setlnum   (GREG, get_src1 (d_inst));
      d_rdst[0].setlnum   (GREG, get_dst0 (d_inst));
      d_rsrc[2].setlnum   (AREG, AR_CCV);
      d_lhint = get_lhint (d_inst);
      d_ildb  = true;
      d_istb  = true;
      d_group = "M16";
      d_valid = true;
      break;
    case M_XCHG1:
    case M_XCHG2:
    case M_XCHG4:
    case M_XCHG8:
      d_rprd.setlnum      (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum   (FREG, get_src0 (d_inst));
      d_rsrc[1].setlnum   (GREG, get_src1 (d_inst));
      d_rdst[0].setlnum   (GREG, get_dst0 (d_inst));
      d_lhint = get_lhint (d_inst);
      d_ildb  = true;
      d_istb  = true;
      d_group = "M16";
      d_valid = true;
      break;

      // M17 instruction group
    case M_FETCHADD4_ACQ:
    case M_FETCHADD8_ACQ:
    case M_FETCHADD4_REL:
    case M_FETCHADD8_REL:
      d_rprd.setlnum        (PREG, get_pred (d_inst));
      d_rdst[0].setlnum     (GREG, get_dst0 (d_inst));
      d_rsrc[1].setlnum     (GREG, get_src1 (d_inst));
      d_immv[0] = get_inc3  (d_inst);
      d_lhint   = get_lhint (d_inst);
      d_group   = "M17";
      d_valid   = true;
      break;

     // M18 instruction group
    case M_SETF_SIG:
    case M_SETF_EXP:
    case M_SETF_S:
    case M_SETF_D:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_rdst[0].setlnum (FREG, get_dst0 (d_inst));
      d_group = "M18";
      d_valid = true;
      break;

      // M19 instruction group
    case M_GETF_SIG:
    case M_GETF_EXP:
    case M_GETF_S:
    case M_GETF_D:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (FREG, get_src0 (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_group = "M19";
      d_valid = true;
      break;

      // M20 instrucion group
    case M_CHK_S_M:
      d_rprd.setlnum         (PREG, get_pred (d_inst));
      d_rdst[0].setlnum      (IPRG, 0);
      d_rsrc[0].setlnum      (GREG, get_src0 (d_inst));
      d_immv[0] = get_spectg (d_inst);
      d_group   = "M20";
      d_valid   = true;
      break;

      // M21 instruction group
    case M_CHK_S:
      d_rprd.setlnum         (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum      (FREG, get_src0 (d_inst));
      d_immv[0] = get_spectg (d_inst);
      d_group   = "M21";
      d_valid   = true;
      break;

      // M22 instruction group
    case M_CHK_A_NC_I:
    case M_CHK_A_CLR_I:
      d_rprd.setlnum       (PREG, get_pred (d_inst));
      d_rdst[0].setlnum    (IPRG, 0);
      d_rdst[1].setlnum    (GREG, get_dst0 (d_inst));
      d_immv[0] = get_i25b (d_inst);
      d_group   = "M22";
      d_valid   = true;
      break;

      // M23 instruction group
    case M_CHK_A_NC_F:
    case M_CHK_A_CLR_F:
      d_rprd.setlnum       (PREG, get_pred (d_inst));
      d_rdst[0].setlnum    (IPRG, 0);
      d_rdst[1].setlnum    (FREG, get_dst0 (d_inst));
      d_immv[0] = get_i25b (d_inst);
      d_group   = "M23";
      d_valid   = true;
      break;

      // M24 instruction group
    case M_INVALA:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      // there is normaly no destination register, this
      // register will be later use to carry the result type
      d_rdst[0].setlnum (GREG, 0);
      d_group = "M24";
      d_valid = true;
      break;
    case M_FWB:
    case M_MF:
    case M_MF_A:
    case M_SRLZ_D:
    case M_SRLZ_I:
    case M_SYNC_I:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_group = "M24";
      d_valid = true;
      break;

      // M25 instruction group
    case M_FLUSHRS:
    case M_LOADRS:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_group = "M25";
      d_valid = true;
      break;
  
      // M26 instruction group
    case M_INVALA_E:
      d_rprd.setlnum      (PREG, get_pred (d_inst));
      d_rdst[0].setlnum   (GREG, get_dst0 (d_inst));
      d_group = "M26";
      d_valid = true;
      break;
      
      // M27 instruction group
    case M_INVALA_E_F:
      d_rprd.setlnum      (PREG, get_pred (d_inst));
      d_rdst[0].setlnum   (FREG, get_dst0 (d_inst));
      d_group = "M27";
      d_valid = true;
      break;
      
      // M28 instrcution group
    case M_FC:
    case M_PTC_E:
      d_rprd.setlnum      (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum   (PSRG, 0);
      d_rsrc[1].setlnum   (GREG, get_src1 (d_inst));
      d_group = "M28";
      d_valid = true;
      break;

      // M29 instruction group
    case M_MOV_TO_AR_R:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (AREG, get_src1 (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_group = "M29";
      d_valid = true;
      break;

      // M30 instruction group
    case M_MOV_TO_AR_I:
      d_rprd.setlnum        (PREG, get_pred (d_inst));
      d_rdst[0].setlnum     (AREG, get_src1 (d_inst));
      d_immv[0] = get_im08b (d_inst);
      d_group   = "M30";
      d_valid   = true;
      break;

      // M31 instruction group
    case M_MOV_FROM_AR:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[1].setlnum (AREG, get_src1 (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_group = "M31";
      d_valid = true;
      break;

      // M32 instruction group
    case M_MOV_TO_CR:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_rsrc[1].setlnum (PSRG, 0);
      d_rdst[0].setlnum (CREG, get_src1 (d_inst));
      d_group = "M32";
      d_valid = true;
      break;

      // M33 instruction group
    case M_MOV_FROM_CR:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (PSRG, 0);
      d_rsrc[1].setlnum (CREG, get_src1 (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_group = "M33";
      d_valid = true;
      break;

      // M34 instruction group
    case M_ALLOC:
      d_rprd.setlnum        (PREG, get_pred (d_inst));
      d_rdst[0].setlnum     (GREG, get_dst0 (d_inst));
      d_rdst[1].setlnum     (CFMR, 0);
      d_rsrc[0].setlnum     (AREG, AR_PFS);
      d_rsrc[1].setlnum     (CFMR, 0);
      d_immv[0] = get_src0  (d_inst);                // sof
      d_immv[1] = get_src1  (d_inst);                // sol
      d_immv[2] = ((t_byte) (d_inst >> 27)) & 0x0F;  // sor
      d_group   = "M34";
      d_valid   = true;
      break;

      // M35 instruction group
    case  M_MOV_TO_PSR_L:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_rsrc[1].setlnum (PSRG, 0);
      d_rdst[0].setlnum (PSRG, 0);
      d_group = "M35";
      d_valid = true;
      break;
    case  M_MOV_TO_PSR_UM:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_rdst[0].setlnum (UMRG, 0);
      d_group = "M35";
      d_valid = true;
      break;
      
   // M36 instruction group
    case  M_MOV_FROM_PSR:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (PSRG, 0);
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_group = "M36";
      d_valid = true;
      break;
    case  M_MOV_FROM_PSR_UM:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (UMRG, 0);
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_group = "M36";
      d_valid = true;
      break;

      // M37 instruction group
    case M_BREAK:
    case M_NOP:
      d_rprd.setlnum       (PREG, get_pred (d_inst));
      d_immv[0] = get_i20a (d_inst);
      d_group   = "M37";
      d_valid   = true;
      break;

      // M38 instruction group
    case M_PROBE_R_R:
    case M_PROBE_W_R:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_rsrc[2].setlnum (PSRG, 0);
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_group = "M38";
      d_valid = true;
      break;
      
      // M39 instruction group
    case M_PROBE_R_I:
    case M_PROBE_W_I:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_rsrc[2].setlnum (PSRG, 0);
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_immv[0] = get_im2 (d_inst);
      d_group   = "M39";
      d_valid   = true;
      break;

      // M40 instruction group
    case M_PROBE_RW_FAULT:
    case M_PROBE_R_FAULT:
    case M_PROBE_W_FAULT:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_rsrc[2].setlnum (PSRG, 0);
      d_immv[0] = get_im2 (d_inst);
      d_group   = "M40";
      d_valid   = true;
      break;

      // M41 instruction group
    case M_ITC_D:
    case M_ITC_I:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_rsrc[1].setlnum (PSRG, 0);
      d_group = "M41";
      d_valid = true;
      break;
     
      // M44 instruction group
    case  M_SUM:
    case  M_RUM:
    case  M_SSM:
    case  M_RSM:
      d_rprd.setlnum       (PREG, get_pred (d_inst));
      d_rdst[0].setlnum    (PSRG, 0);
      d_rsrc[0].setlnum    (PSRG, 0);
      d_immv[0] = get_im24 (d_inst);
      d_group   = "M44";
      d_valid   = true;
      break;
   
      // M45 instruction group
    case M_PTC_L:
    case M_PTC_G:
    case M_PTC_GA:
    case M_PTR_D:
    case M_PTR_I:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rsrc[0].setlnum (GREG, get_src0 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_rsrc[2].setlnum (PSRG, 0);
      d_group = "M45";
      d_valid = true;
      break;
      
      // M46 instruction group
    case M_TAK:
    case M_TPA:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_rsrc[0].setlnum (PSRG, 0);
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_group = "M46";
      d_valid = true;
      break;
    case M_TTAG:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_group = "M46";
      d_valid = true;
      break;
    case M_THASH:
      d_rprd.setlnum    (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (GREG, get_dst0 (d_inst));
      d_rsrc[0].setlnum (CREG, CR_PTA);
      d_rsrc[1].setlnum (GREG, get_src1 (d_inst));
      d_group = "M46";
      d_valid = true;
      break;

    default:
      break;
    }
  }

  // decode a M unit instruction
  
  void Instr::mdecode (void) {
    assert ((d_valid == false) && (d_bunit == MUNIT));
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
    case 0x06:
      d_opcd = get_opcode_group_6 (d_inst);
      break;
    case 0x07:
      d_opcd = get_opcode_group_7 (d_inst);
      break;
    default:
      break;
    
    }    
    // update instruction
    mupdate ();
    // mark as M unit
    d_funit = MUNIT;
  }
}
