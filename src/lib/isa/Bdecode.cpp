// ---------------------------------------------------------------------------
// - Bdecode.cpp                                                             -
// - iato:isa library - B unit instruction decode implementation             -
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

#include "Cfm.hpp"
#include "Pfs.hpp"
#include "Instr.hpp"
#include "Udecode.hxx"

namespace iato {
  // this function maps the pfs octa value to a cfm
  static Uvr rpm_pfstocfm (const Uvr& uvr) {
    // check for valid uvr
    if (uvr.gettype () != Uvr::OBV) return uvr;
    // create the pfs
    Pfs pfs = uvr.getoval ();
    // extract the cfm
    Cfm cfm = pfs.getfld (Pfs::PFM);
    // map result
    Uvr result;
    result.setoval (cfm.getcfm ());
    return result;
  }

  // return the prefetch hint
  static t_phint get_phint (const t_octa inst) {
    t_byte ph = ((t_byte) (inst >> 12)) & 0x01;
    return (ph == 0x00) ? FEW : MANY;
  }

  // return the branch hint (2 bits)
  static t_bhint get_bhint_2 (const t_octa inst) {
    t_byte bh = ((t_byte) (inst >> 33)) & 0x03;
    if (bh == 0x00) return SPTK;
    if (bh == 0x01) return SPNT;
    if (bh == 0x02) return DPTK;
    return DPNT;
  }

  // return cache deallocation
  static t_chint get_chint (const t_octa inst) {
    t_byte ch = ((t_byte) (inst >> 35)) & 0x01;
    return (ch == 0x00) ? NONE : CLR;
  }

  // return the branch importance hint (1 bit)
  static t_ihint get_ihint (const t_octa inst) {
    bool ih = (t_byte) (inst >> 35) & 0x01;
    return (ih == 0x00) ? INONE : IIMP;
  }

  // return the ip-relative predict hint (2 bits)
  static t_bphint get_bphint (const t_octa inst) {
    t_byte bph = ((t_byte) (inst >> 3)) & 0x03;
    if (bph == 0x00) return BPSPTK;
    if (bph == 0x01) return BPLOOP;
    if (bph == 0x02) return BPDPTK;
    return BPEXIT;
  } 

  // get the instruction immediate (13) value for branch predict
  // The value is shifted by 4 after the sign extension.
  static inline t_octa get_tag13 (const t_octa inst) {
    t_octa result = (inst >> 6) & 0x000000000000007FULL;
    result |= ((t_byte)(inst >> 26)) & 0x0000000000000180ULL;
    if ((result & 0x0000000000000100ULL) == 0x0000000000000100ULL)
      result |= ~0x00000000000000FFULL;
    return (result << 4);
  }

  // decode major opcode 0 - B4 B8 B9
  static t_iopc get_opcode_group_0 (t_octa inst) {
    // chek X6 bits (27:32) btype bits (6:8)
    t_byte x6    = ((t_byte) (inst >> 27)) & 0x3F;
    t_byte btype = ((t_byte) (inst >> 6))  & 0x07;
    // decode floating misc
    if (x6 == 0x00) return B_BREAK;
    // decode br.cond
    if ((x6 == 0x20) && (btype == 0x00))  return B_COND;
    // decode br.ia
    if ((x6 == 0x20) && (btype == 0x01)) return B_IA;
    // decode br.ret
    if ((x6 == 0x21) && (btype == 0x04)) return B_RET;
    // decode miscellaneous (B-UNIT)
    if (x6 == 0x02) return B_COVER;
    if (x6 == 0x04) return B_CLRRRB;
    if (x6 == 0x05) return B_CLRRRB_PR;
    if (x6 == 0x02) return B_RFI;
    if (x6 == 0x02) return B_BSW_0;
    if (x6 == 0x02) return B_BSW_1;
    if (x6 == 0x02) return B_EPC;
    // mark as invalid to cancel update
    return U_INVALID;
  }

  // decode major opcode 1 - B5
  static t_iopc get_opcode_group_1 (t_octa inst) {
    return B_CALL;
  }

  // decode major opcode 2 - B9
  static t_iopc get_opcode_group_2 (t_octa inst) {
    // check X6 bits (27:32)
    t_byte x6 = ((t_byte) (inst >> 27)) & 0x3F;
    // decode floating misc
    if (x6 == 0x00) return B_NOP;
    // decode indirect predict
    if (x6 == 0x10) return B_BRP;
    if (x6 == 0x11) return B_BRP_RET;
    // mark as invalid to cancel update
    return U_INVALID;
  }

  // decode major opcode 4 - B1 B2 B4
  static t_iopc get_opcode_group_4 (t_octa inst) {
    // get branch type
    t_byte bt = ((t_byte) (inst >> 6)) & 0x07;
    if (bt == 0x00) return B_COND_IP;
    if (bt == 0x02) return B_WEXIT_IP;
    if (bt == 0x03) return B_WTOP_IP;
    if (bt == 0x05) return B_CLOOP_IP;
    if (bt == 0x06) return B_CEXIT_IP;
    if (bt == 0x07) return B_CTOP_IP;
    // mark as invalid to cancel update
    return U_INVALID;
  }

  // decode major opcode 5 - B3
  static t_iopc get_opcode_group_5 (t_octa inst) {
     return B_CALL_IP;
  }

  // decode major opcode 7 - B6
  static t_iopc get_opcode_group_7 (t_octa inst) {
    return B_BRP_IP;
  }

  // update a B unit instruction

  void Instr::bupdate (void) {
    if ((d_valid == true) || (d_opcd == U_INVALID)) return;
    // update predicate, source and destination registers
    switch (d_opcd) {
      
      // B01 instruction group
    case B_COND_IP:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (IPRG, 0);
      d_immv[0] = get_i25b    (d_inst);
      d_phint   = get_phint   (d_inst); 
      d_bhint   = get_bhint_2 (d_inst); 
      d_chint   = get_chint   (d_inst); 
      d_brch    = true;
      d_group   = "B01";
      d_valid   = true;
      break;
    case B_WEXIT_IP:
    case B_WTOP_IP:
      d_rprd.setlnum (PREG, 0);
      d_rdst[0].setlnum (IPRG, 0);
      d_rdst[1].setlnum (AREG, AR_EC);
      d_rdst[2].setlnum (CFMR, 0);
      d_rdst[3].setlnum (PREG, 63);
      d_rsrc[0].setlnum (CFMR, 0);
      d_rsrc[1].setlnum (AREG, AR_EC);
      d_rsrc[2].setlnum (PREG, get_pred (d_inst)); 
      d_immv[0] = get_i25b    (d_inst);
      d_phint   = get_phint   (d_inst); 
      d_bhint   = get_bhint_2 (d_inst); 
      d_chint   = get_chint   (d_inst); 
      d_brch    = true;
      d_group   = "B01";
      d_valid   = true;
      break;

      // B02 instruction group
    case B_CLOOP_IP:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (IPRG, 0);
      d_rdst[1].setlnum (AREG, AR_LC);
      d_rsrc[0].setlnum (AREG, AR_LC);
      d_immv[0] = get_i25b    (d_inst);
      d_phint   = get_phint   (d_inst); 
      d_bhint   = get_bhint_2 (d_inst); 
      d_chint   = get_chint   (d_inst); 
      d_brch    = true;
      d_group   = "B02";
      d_valid   = true;
      break;
    case B_CEXIT_IP:
    case B_CTOP_IP:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (IPRG, 0);
      d_rdst[1].setlnum (AREG, AR_EC);
      d_rdst[2].setlnum (AREG, AR_LC);
      d_rdst[3].setlnum (CFMR, 0);
      d_rdst[4].setlnum (PREG, 63);
      d_rsrc[0].setlnum (CFMR, 0);
      d_rsrc[1].setlnum (AREG, AR_EC);
      d_rsrc[2].setlnum (AREG, AR_LC);
      d_immv[0] = get_i25b    (d_inst);
      d_phint   = get_phint   (d_inst); 
      d_bhint   = get_bhint_2 (d_inst); 
      d_chint   = get_chint   (d_inst); 
      d_brch    = true;
      d_group   = "B02";
      d_valid   = true;
      break;

      // B03 instruction group
    case B_CALL_IP:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_immv[0] = get_i25b    (d_inst);
      d_rdst[0].setlnum (BREG, get_dst_br (d_inst));
      d_rdst[1].setlnum (IPRG, 0);
      d_rdst[2].setlnum (AREG, AR_PFS);
      d_rdst[3].setlnum (CFMR, 0);
      d_rdst[4].setlnum (AREG, AR_BSP);
      d_rsrc[0].setlnum (CFMR, 0);
      d_rsrc[1].setlnum (AREG, AR_EC);
      d_rsrc[2].setlnum (AREG, AR_BSP);
      d_phint = get_phint   (d_inst); 
      d_bhint = get_bhint_2 (d_inst); 
      d_chint = get_chint   (d_inst); 
      d_brch  = true;
      d_group = "B03";
      d_valid = true;
      break;

      // B04 instruction group
    case B_COND:
    case B_IA:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (IPRG, 0);
      d_rsrc[0].setlnum (BREG, get_src_br (d_inst));
      d_rrpm[0].setmap  (d_rsrc[0], d_rdst[0]);
      d_phint = get_phint   (d_inst);
      d_bhint = get_bhint_2 (d_inst); 
      d_chint = get_chint   (d_inst); 
      d_brch  = true;
      d_group = "B04";
      d_valid = true;
      break;
    case B_RET:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (IPRG, 0);
      d_rdst[1].setlnum (CFMR, 0);
      d_rdst[2].setlnum (AREG, AR_EC);
      d_rdst[3].setlnum (AREG, AR_BSP);
      d_rsrc[0].setlnum (BREG, get_src_br (d_inst));
      d_rsrc[1].setlnum (AREG, AR_PFS);
      d_rsrc[2].setlnum (AREG, AR_BSP);
      d_rrpm[0].setmap  (d_rsrc[0], d_rdst[0]);
      d_rrpm[1].setmap  (d_rsrc[1], d_rdst[1], rpm_pfstocfm);
      d_phint = get_phint   (d_inst);
      d_bhint = get_bhint_2 (d_inst);
      d_chint = get_chint   (d_inst);
      d_brch  = true;
      d_group = "B04";
      d_valid = true;
      break;

      // B05 instruction group
    case B_CALL:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_rdst[0].setlnum (BREG, get_dst_br (d_inst));
      d_rdst[1].setlnum (IPRG, 0);
      d_rdst[2].setlnum (AREG, AR_PFS);
      d_rdst[3].setlnum (CFMR, 0);
      d_rdst[4].setlnum (AREG, AR_BSP);
      d_rsrc[0].setlnum (BREG, get_src_br (d_inst));
      d_rsrc[1].setlnum (CFMR, 0);
      d_rsrc[2].setlnum (AREG, AR_EC);
      d_rsrc[3].setlnum (AREG, AR_BSP);
      d_rrpm[0].setmap  (d_rsrc[0], d_rdst[1]);
      d_phint = get_phint   (d_inst);
      d_bhint = get_bhint_2 (d_inst); 
      d_chint = get_chint   (d_inst); 
      d_brch  = true;
      d_group = "B05";
      d_valid = true;
      break;
    
      // B06 instruction group
    case B_BRP_IP:
      d_rprd.setlnum         (PREG, get_pred (d_inst));
      d_rdst[0].setlnum      (IPRG, 0);
      d_immv[0] = get_i25b   (d_inst);
      d_immv[1] = get_tag13  (d_inst);
      d_bphint  = get_bphint (d_inst);
      d_ihint   = get_ihint  (d_inst); 
      d_group   = "B06";
      d_valid   = true;
      break;   
      
     // B07 instruction group
    case B_BRP:
    case B_BRP_RET:
      d_rprd.setlnum         (PREG, get_pred (d_inst));
      d_rdst[0].setlnum      (IPRG, 0);
      d_rsrc[0].setlnum      (BREG, get_src_br (d_inst));
      d_rrpm[0].setmap       (d_rsrc[0], d_rdst[0]);
      d_immv[1] = get_tag13  (d_inst);
      d_bphint  = get_bphint (d_inst);
      d_ihint   = get_ihint  (d_inst); 
      d_group   = "B07";
      d_valid   = true;
      break;   
  
      // B08 instruction group
    case B_COVER:
    case B_EPC:
      d_rprd.setlnum (PREG, 0);
      d_group = "B08";
      d_valid = true;
      break;
    case B_CLRRRB:
    case B_CLRRRB_PR:
      d_rprd.setlnum (PREG, 0);
      d_rdst[0].setlnum (CFMR, 0);
      d_rsrc[0].setlnum (CFMR, 0);
      d_group = "B08";
      d_valid = true;
      break;
    case B_RFI:
    case B_BSW_0:
    case B_BSW_1:
      d_rprd.setlnum (PREG, 0);
      d_rsrc[0].setlnum (PSRG, 0);
      d_group = "B08";
      d_valid = true;
      break;

      // B09 instruction group
    case B_BREAK:
    case B_NOP:
      d_rprd.setlnum (PREG, get_pred (d_inst));
      d_immv[0] = get_im21 (d_inst);
      d_group   = "B09";
      d_valid   = true;
      break;      
    default:
      break;
    }
  }

  // decode a B unit instruction
  
  void Instr::bdecode (void) {
    assert ((d_valid == false) && (d_bunit == BUNIT));
    // get instruction major opcode
    d_code = slot_getmopc (d_inst);
    switch (d_code) {
    case 0x00:
      d_opcd = get_opcode_group_0 (d_inst);
      break;
    case 0x01:
      d_opcd = get_opcode_group_1 (d_inst);
      break;
    case 0x02:
      d_opcd = get_opcode_group_2 (d_inst);
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
    bupdate ();
    // mark as B unit
    d_funit = BUNIT;
  }
}
