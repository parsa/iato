// ---------------------------------------------------------------------------
// - Instr.cpp                                                               -
// - iato:isa library - instruction decode/recode class implementation       -
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
#include "InstrCode.hxx"
#include "Interrupt.hpp"

namespace iato {

  // number of instruction group
  const long   INSTR_GROUP_MAX = 112;
  // instruction groups definition
  const string INSTR_GROUP[] = {
    "A01", "A02", "A03", "A04", "A05", "A06", "A07", "A08", "A09", "A10",
    "M01", "M02", "M03", "M04", "M05", "M06", "M07", "M08", "M09", "M10",
    "M11", "M12", "M13", "M14", "M15", "M16", "M17", "M18", "M19", "M20",
    "M21", "M22", "M23", "M24", "M25", "M26", "M27", "M28", "M29", "M30",
    "M31", "M32", "M33", "M34", "M35", "M36", "M37", "M38", "M39", "M40",
    "M41", "M42", "M43", "M44", "M45", "M46",
    "I01", "I02", "I03", "I04", "I05", "I06", "I07", "I08", "I09", "I10",
    "I11", "I12", "I13", "I14", "I15", "I16", "I17",        "I19", "I20",
    "I21", "I22", "I23", "I24", "I25", "I26", "I27", "I28",
    "F01", "F02", "F03", "F04", "F05", "F06", "F07", "F08", "F09", "F10",
    "F11", "F12", "F13", "F14", "F15",
    "B01", "B02", "B03", "B04", "B05", "B06", "B07", "B08", "B09",
    "X01", "X02", "X03", "X04"
  };

  // get the string representation of an instruction according to its opcode

  string Instr::tostr (const long opcd) {
    assert ((opcd >= 0) && (opcd < OPCODE_MAX)); 
    return INSTR_CODE[opcd];
  }

  // check if an instruction group is valid
  
  bool Instr::isvig (const string& s) {
    for (long i = 0; i < INSTR_GROUP_MAX; i++) {
      if (INSTR_GROUP[i] == s) return true;
    }
    return false;
  };

  // create a default (invalid) instruction

  Instr::Instr (void) {
    reset ();
  }

  // create an instruction by type/slot/stop and data

  Instr::Instr (t_unit unit, long slot, bool bstp, t_octa inst) {
    reset  ();
    decode (unit, slot, bstp, inst);
  }

  // create an instruction by type/slot/stop data and extension

  Instr::Instr (t_unit unit, long slot, bool bstp, t_octa inst, t_octa extd) {
    reset  ();
    decode (unit, slot, bstp, inst, extd);
  }

  // copy construct an instruction

  Instr::Instr (const Instr& that) {
    d_valid   = that.d_valid;
    d_iip     = that.d_iip;
    d_sip     = that.d_sip;
    d_sfl     = that.d_sfl;
    d_hist    = that.d_hist;
    d_phst    = that.d_phst;
    d_rprd    = that.d_rprd;
    d_bunit   = that.d_bunit;
    d_funit   = that.d_funit;
    d_slot    = that.d_slot;
    d_stop    = that.d_stop;
    d_brch    = that.d_brch;
    d_inst    = that.d_inst;
    d_extd    = that.d_extd;
    d_code    = that.d_code;
    d_opcd    = that.d_opcd;
    d_ildb    = that.d_ildb;
    d_istb    = that.d_istb;
    d_group   = that.d_group;
    d_mhint   = that.d_mhint;
    d_ihint   = that.d_ihint;
    d_lhint   = that.d_lhint;
    d_shint   = that.d_shint;
    d_phint   = that.d_phint;
    d_bhint   = that.d_bhint;
    d_chint   = that.d_chint;
    d_lfhint  = that.d_lfhint;
    d_fpcomp  = that.d_fpcomp;
    d_bphint  = that.d_bphint;
    for (long i = 0; i < IA_MSRC; i++) {
      d_immv[i] = that.d_immv[i];
      d_rsrc[i] = that.d_rsrc[i];
    }
    for (long i = 0; i < IA_MDST; i++) d_rdst[i] = that.d_rdst[i];
    for (long i = 0; i < IA_MRPM; i++) d_rrpm[i] = that.d_rrpm[i];
  }

  // assign an instruction to this one

  Instr& Instr::operator = (const Instr& that) {
    if (this == &that) return *this;
    d_valid   = that.d_valid;
    d_iip     = that.d_iip;
    d_sip     = that.d_sip;
    d_sfl     = that.d_sfl;
    d_hist    = that.d_hist;
    d_phst    = that.d_phst;
    d_rprd    = that.d_rprd;
    d_bunit   = that.d_bunit;
    d_funit   = that.d_funit;
    d_slot    = that.d_slot;
    d_stop    = that.d_stop;
    d_brch    = that.d_brch;
    d_inst    = that.d_inst;
    d_extd    = that.d_extd;
    d_code    = that.d_code;
    d_opcd    = that.d_opcd;
    d_ildb    = that.d_ildb;
    d_istb    = that.d_istb;
    d_group   = that.d_group;
    d_mhint   = that.d_mhint;
    d_ihint   = that.d_ihint;
    d_lhint   = that.d_lhint;
    d_shint   = that.d_shint;
    d_phint   = that.d_phint;
    d_bhint   = that.d_bhint;
    d_chint   = that.d_chint;
    d_lfhint  = that.d_lfhint;
    d_fpcomp  = that.d_fpcomp;
    d_bphint  = that.d_bphint;
    for (long i = 0; i < IA_MSRC; i++) {
      d_immv[i] = that.d_immv[i];
      d_rsrc[i] = that.d_rsrc[i];
    }
    for (long i = 0; i < IA_MDST; i++) d_rdst[i] = that.d_rdst[i];
    for (long i = 0; i < IA_MRPM; i++) d_rrpm[i] = that.d_rrpm[i];
    return *this;
  }

  // reset this instruction

  void Instr::reset (void) {
    d_valid   = false;
    d_iip     = OCTA_0;
    d_sip     = OCTA_0;
    d_sfl     = false;
    d_hist    = OCTA_0;
    d_phst    = OCTA_0;
    d_bunit   = AUNIT;
    d_funit   = AUNIT;
    d_slot    = 0;
    d_stop    = false;
    d_brch    = false;
    d_inst    = OCTA_0;
    d_extd    = OCTA_0;
    d_code    = BYTE_0;
    d_opcd    = U_INVALID;
    d_ildb    = false;
    d_istb    = false;
    d_group   = "";
    d_mhint   = MNONE;
    d_ihint   = INONE;
    d_lhint   = LDNONE;
    d_shint   = STNONE;
    d_phint   = FEW;
    d_bhint   = SPTK;
    d_chint   = NONE;
    d_lfhint  = LFNONE;
    d_bphint  = BPSPTK;
    d_fpcomp  = S0;
    d_rprd.reset  ();
    for (long i = 0; i < IA_MSRC; i++) {
      d_immv[i] = 0;
      d_rsrc[i].reset ();
    }
    for (long i = 0; i < IA_MDST; i++) d_rdst[i].reset ();
    for (long i = 0; i < IA_MRPM; i++) d_rrpm[i].reset ();
  }

  
  // return the instruction valid bit

  bool Instr::isvalid (void) const {
    return d_valid;
  }

  // set the instruction ip

  void Instr::setiip (const t_octa ip) {
    d_iip = ip;
    d_sip = ip + BN_BYSZ;
  }

  // return the instruction ip
    
  t_octa Instr::getiip (void) const {
    return d_iip;
  }

  // set the instruction speculative ip

  void Instr::setsip (const t_octa sip) {
    d_sip = sip;
    d_sfl = true;
  }

  // return the instruction speculative ip

  t_octa Instr::getsip (void) const {
    return d_sip;
  }

  // return the instruction speculative flag

  bool Instr::getsfl (void) const {
    return d_sfl;
  }

  // set the branch history

  void Instr::sethist (const t_octa hist) {
    d_hist = hist;
  }
  
  // return the branch history

  t_octa Instr::gethist (void) const {
    return d_hist;
  }

  // set the predicate history
  
  void Instr::setphst (const t_octa phst) {
    d_phst = phst;
  }
  
  // return the predicate history
  
  t_octa Instr::getphst (void) const {
    return d_phst;
  }
  
  // return the instruction slot
    
  long Instr::getslot (void) const {
    return d_slot;
  }

  // return the instruction stop bit
  
  bool Instr::getstop (void) const {
    return d_stop;
  }

  // return the instruction group
  
  string Instr::getgroup (void) const {
    return d_group;
  }

  // return the instruction data

  t_octa Instr::getdata (void) const {
    return d_inst;
  }

  // return the instruction extension data

  t_octa Instr::getextd (void) const {
    return d_extd;
  }
    
  // return the instruction opcode
  
  t_iopc Instr::getiopc (void) const {
    return d_opcd;
  }

  // return the instruction bundle unit

  t_unit Instr::getbunit (void) const {
    return d_bunit;
  }

  // return the instruction functional unit
  
  t_unit Instr::getfunit (void) const {
    return d_funit;
  }

  // return the instruction slot unit

  t_unit Instr::getsunit (void) const {
    assert (d_valid == true);
    if (d_bunit == XUNIT) return d_funit;
    return d_bunit;
  }
  
  // return true if the instruction is a load
  
  bool Instr::getldb (void) const {
    return d_ildb;
  }

  // return true if the instruction is a store

  bool Instr::getstb (void) const {
    return d_istb;
  }

  // return true if the instruction is a branch
  
  bool Instr::isbr (void) const {
    return d_brch;
  }

  // return true if the instruction is a nop

  bool Instr::isnop (void) const {
    if (d_valid == true) {
      if (d_opcd == M_NOP) return true;
      if (d_opcd == I_NOP) return true;
      if (d_opcd == F_NOP) return true;
      if (d_opcd == B_NOP) return true;
      if (d_opcd == X_NOP) return true;
    }
    return false;
  }

  // return the instruction floating point status completer
  
  t_fpcomp Instr::getfpcomp (void) const {
    return d_fpcomp;
  }

  // return the instruction immediate value
    
  t_octa Instr::getimmv (const long index) const {
    assert ((index >= 0) && (index < IA_MSRC));
    return d_immv[index];
  }
  
  // return true if the predicate matches the rid
  
  bool Instr::ispnum (const Rid& rid) const {
    return d_rprd.isequal (rid);
  }

  // return the predicate register number

  Rid Instr::getpnum (void) const {
    return d_rprd;
  }

  // set the predicate register number

  void Instr::setpnum (const Rid& pnum) {
    d_rprd = pnum;
  }

  // return true if one source matches the rid
  
  bool Instr::issnum (const Rid& rid) const {
    for (long i = 0; i < IA_MSRC; i++) {
      if (d_rsrc[i].isequal (rid) == true) return true;
    }
    return false;
  }

  // get the source register number by index

  Rid Instr::getsnum (const long index) const {
    assert ((index >= 0) && (index < IA_MSRC));
    return d_rsrc[index];
  }

  // set the source register number

  void Instr::setsnum (const long index, const Rid& snum) {
    assert ((index >= 0) && (index < IA_MSRC));
    d_rsrc[index] = snum;
  }

  // return true if one destination matches the rid
  
  bool Instr::isdnum (const Rid& rid) const {
    for (long i = 0; i < IA_MDST; i++) {
      if (d_rdst[i].isequal (rid) == true) return true;
    }
    return false;
  }

  // get the destination register number by index

  Rid Instr::getdnum (const long index) const {
    assert ((index >= 0) && (index < IA_MDST));
    return d_rdst[index];
  }

  // set the destination register number

  void Instr::setdnum (const long index, const Rid& dnum) {
    assert ((index >= 0) && (index < IA_MDST));
    d_rdst[index] = dnum;
  }

  // return the rid pair map by index

  Rpm Instr::getrrpm (const long index) const {
    assert ((index >= 0) && (index < IA_MRPM));
    return d_rrpm[index];
  }

  // return the instruction operand

  Operand Instr::getoper (void) const {
    Operand result;
    for (long i = 0; i < IA_MSRC; i++) result.setrid (i, d_rsrc[i]);
    return result;
  }

  // return the instruction result

  Result Instr::getresl (void) const {
    Result result;
    for (long i = 0; i < IA_MDST; i++) result.setrid (i, d_rdst[i]);
    return result;
  }

  // return true if the instruction is predicated

  bool Instr::ispred (void) const {
    if ((d_valid == false) || (d_rprd.isvalid () == false)) return false;
    if (d_rprd.getlnum () == 0) return false;
    return true;
  }

  // return true if the instruction generates predicates

  bool Instr::ispgen (void) const {
    if (d_valid == false) return false;
    for (long i = 0; i < IA_MDST; i++) {
      if (d_rdst[i].ispred () == true) return true;
    }
    return false;
  }

  // set and decode this instruction

  void Instr::decode (t_unit unit, long slot, bool bstp, t_octa inst) {
    using namespace std;
    assert (d_valid == false);
    // save instruction info
    d_bunit = unit;
    d_funit = unit;
    d_slot  = slot;
    d_stop  = bstp;
    d_inst  = inst;
    // decode from type - I/M are first checked as A unit
    switch (d_bunit) {
    case MUNIT:
      adecode ();
      if (d_valid == true) break;
      mdecode ();
      break;
    case IUNIT:
      adecode ();
      if (d_valid == true) break;
      idecode ();
      break;
    case FUNIT:
      fdecode ();
      break;
    case BUNIT:
      bdecode ();
      break;
    case XUNIT:
      xdecode ();
      break;
    default:
      assert (false);
      break;
    }
    // check for decoding
    if (d_valid == false) {
      ostringstream os;
      os << "unimplemented at 0x";
      os << hex << setw (16) << setfill ('0') << d_iip << dec;
      os << " slot "   << d_slot;
      os << " opcode 0x" << hex << setw (6) << setfill ('0') << d_inst;
      throw Interrupt (FAULT_IT_OPER_LEGAL, os.str (), d_iip, d_slot);
    }
  }

  // set and decode this instruction with extended data

  void Instr::decode (t_unit unit, long slot, bool bstp, t_octa inst, 
		      t_octa extd) {
    using namespace std;
    assert (d_valid == false);
    d_extd = extd;
    decode (unit, slot, bstp, inst);
  }

  // return a string representation of the instruction

  string Instr::recode (void) const {
    assert (d_valid == true);
    string result;
    switch (d_bunit) {
    case MUNIT:
      result = arecode ();
      if (result.empty () == false) break;
      result = mrecode ();
      break;
    case IUNIT:
      result = arecode ();
      if (result.empty () == false) break;
      result = irecode ();
      break;
    case FUNIT:
      result = frecode ();
      break;
    case BUNIT:
      result = brecode ();
      break;
    case XUNIT:
      result = xrecode ();
      break;
    default:
      assert (false);
      break;
    }
    if ((result.empty () == false) && (d_stop == true)) result += ";;";
    return result;
  }
}
