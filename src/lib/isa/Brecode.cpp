// ---------------------------------------------------------------------------
// - Brecode.cpp                                                             -
// - iato:isa library - B unit instruction recode implementation             -
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

  // return a string representation of the branch extension
  static string to_brhint (t_phint ph, t_bhint bh, t_chint ch, bool ispseudo) {
    string result;
    if (!ispseudo)
      switch (bh) {
      case SPTK:
	result += ".sptk";
	break;
      case SPNT:
	result += ".spnt";
	break;
      case DPTK:
	result += ".dptk";
	break;
      case DPNT:
	result += ".dpnt";
	break;
      }
    if (ph == FEW)  result += ".few";
    if (ph == MANY) result += ".many";
    if (ch == NONE) result += "";
    if (ch == CLR)  result += ".clr";
    return result;
  }

  // return a string representation of the branch predict extension
  static string to_brphint (t_ihint ih, t_bphint iph) {
    string result;
    switch (iph) {
    case BPSPTK:
      result += ".sptk";
      break;
    case BPLOOP:
      result += ".loop";
      break;
    case BPDPTK:
      result += ".dptk";
      break;
    case BPEXIT:
      result += ".exit";
      break;
    }
    if (ih == INONE) result += "";
    if (ih == IIMP ) result += ".imp";
    return result;
  }

  // recode a B01 instruction
  string Instr::brecode_01 (void) const {
    string result = "";
    switch (d_opcd) {
    case B_COND_IP:
      result += to_pred (d_rprd.getlnum ());
      if (d_rprd.getlnum () == 0) result += "br";
      else result += "br.cond";
      break;
    case B_WEXIT_IP:
      result += to_pred (d_rsrc[2].getlnum ());
      result += "br.wexit";
      break;
    case B_WTOP_IP:
      result += to_pred (d_rsrc[2].getlnum ());
      result += "br.wtop";
      break;
    default:
      break;
    }
    if (d_rprd.getlnum () == 0)
      result += to_brhint (d_phint, d_bhint, d_chint, true);
    else
      result += to_brhint (d_phint, d_bhint, d_chint, false);
    result += ' ' + to_brtg (d_immv[0], d_iip);
    return result;
  }

  // recode a B02 instruction
  string Instr::brecode_02 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case B_CLOOP_IP:  result += "br.cloop";  break;
    case B_CEXIT_IP: result += "br.cexit"; break;
    case B_CTOP_IP:  result += "br.ctop";  break;
    default: 
      break;
    }
    result += to_brhint (d_phint, d_bhint, d_chint, false);
    result += ' ' + to_brtg (d_immv[0], d_iip);
    return result;
  }

  // recode a B03 instruction
  string Instr::brecode_03 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += "br.call";
    result += to_brhint (d_phint, d_bhint, d_chint, false);
    result += ' ' + to_breg (d_rdst[0].getlnum ())+ '=';
    result += to_brtg (d_immv[0], d_iip);
    return result;
  }

  // recode a B04 instruction
  string Instr::brecode_04 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case B_IA:
      result += "br.ia";
      break;
    case B_COND:
      if (d_rprd.getlnum () == 0) result += "br";
      else result += "br.cond";
      break;
    case B_RET:
      result += "br.ret";
      break;
    default:
      break;
    }
    if (d_rprd.getlnum () == 0)
      result += to_brhint (d_phint, d_bhint, d_chint, true);
    else
      result += to_brhint (d_phint, d_bhint, d_chint, false);
    result += ' ' + to_breg (d_rsrc[0].getlnum ());
    return result;
  }

  // recode a B05 instruction
  string Instr::brecode_05 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    result += "br.call";
    result += to_brhint (d_phint, d_bhint, d_chint, false);
    result += ' ' + to_breg (d_rdst[0].getlnum ())+ '=';
    result += to_breg (d_rsrc[0].getlnum ());
    return result;
  }

  // recode a B06 instruction
  string Instr::brecode_06 (void) const {
    string result = to_pred (d_rprd.getlnum ());
      switch (d_opcd) {
      case B_BRP_IP:
	 result += "brp";
	 result += to_brphint (d_ihint, d_bphint);
	 result += ' ' + to_brtg (d_immv[0], d_iip);
	 result += ',' + to_brtg (d_immv[1], d_iip);
	 break;
      default:
	break;
      }
      return result;
  }

 // recode a B07 instruction
  string Instr::brecode_07 (void) const {
    string result = to_pred (d_rprd.getlnum ());
      switch (d_opcd) {
      case B_BRP:
	result += "brp";
	break;
      case B_BRP_RET:
	result += "brp.ret";
	break;
      default:
	break;
      }
      result += to_brphint (d_ihint, d_bphint);
      result += ' ' + to_breg (d_rsrc[0].getlnum ());
      result += ',' + to_brtg (d_immv[1], d_iip);
      return result;
  }

  // recode a B08 instruction
  string Instr::brecode_08 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case B_COVER:     result += "cover"; break;
    case B_CLRRRB:    result += "clrrrb"; break;
    case B_CLRRRB_PR: result += "clrrrb.pr"; break;
    case B_EPC:       result += "epc"; break; 
    case B_RFI:       result += "rfi"; break;
    case B_BSW_0:     result += "bsw.0"; break;
    case B_BSW_1:     result += "bsw.1"; break;
    default: 
      break;
    }
    return result;
  }

  // recode a B09 instruction
  string Instr::brecode_09 (void) const {
    string result = to_pred (d_rprd.getlnum ());
    switch (d_opcd) {
    case B_BREAK: result += "break.b "; break;
    case B_NOP:   result += "nop.b "; break;
    default: 
      break;
    }
    // add immediate value
    result += to_immv (d_immv[0], false, true);
    return result;
  }

  // recode a B unit instruction

  string Instr::brecode (void) const {
    assert (d_valid == true);
    string result;
    // select instruction group
    switch (d_opcd) {
    case B_COND_IP:
    case B_WEXIT_IP:
    case B_WTOP_IP:
      result = brecode_01 ();
      break;
    case B_CLOOP_IP:
    case B_CEXIT_IP:
    case B_CTOP_IP:
      result = brecode_02 ();
      break;
    case B_CALL_IP:
      result = brecode_03();
      break;
    case B_COND:
    case B_IA:
    case B_RET:
      result = brecode_04();
      break;
    case B_CALL:
      result = brecode_05();
      break;
    case B_BRP_IP:
      result = brecode_06();
      break;
    case B_BRP:
    case B_BRP_RET:
      result = brecode_07();
      break;
    case B_COVER:
    case B_CLRRRB:
    case B_CLRRRB_PR:
    case B_EPC:
    case B_RFI:
    case B_BSW_0:
    case B_BSW_1:
      result = brecode_08 ();
      break;
    case B_BREAK:
    case B_NOP:
      result = brecode_09 ();
      break;
    default:
      break;
    }
    return result;
  }
}
