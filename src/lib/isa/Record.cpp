// ---------------------------------------------------------------------------
// - Record.cpp                                                              -
// - iato:isa library - information record class implementation              -
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

#include "Bits.hpp"
#include "Utils.hpp"
#include "Record.hpp"
#include "Record.hxx"
#include "Exception.hpp"

namespace iato {
  using namespace std;
 
  // this procedure encode a record type to a byte code
  static inline t_byte encode_type (Record::t_rctp type) {
    t_byte result = type; assert (result != 0x00);
    return result;
  }

  // this procedure decode a record type as a byte
  static inline Record::t_rctp decode_type (const t_byte code) {
    if ((code > Record::IGNORE) && (code < Record::TYPMAX))
      return (Record::t_rctp) code;
    throw Exception ("record-error", "invalid record type to decode");
  }

  // this procedure encode a register to a byte code
  static inline t_byte encode_lreg (t_lreg lreg) {
    return (t_byte) lreg;
  }

  // this procedure decode a register code to a register
  static inline t_lreg decode_lreg (const t_byte code) {
    if ((code > LREG_NONE) && (code < LREG_RMAX)) return (t_lreg) code;
    throw Exception ("record-error", "invalid register code to decode");
  }

  // this procedure returns a string representation of a register type
  static string lreg_to_str (t_lreg lreg) {
    string result;
    switch (lreg) {
    case GREG:
      result = "gr";
      break;
    case NREG:   
      result = "nr";
      break;
    case FREG:   
      result = "fr";
      break;
    case PREG:   
    case PRRG:
      result = "pr";
      break;
    case BREG:   
      result = "br";
      break;
    case AREG:   
      result = "ar";
      break;
    case IPRG:
      result = "ip";
      break;
    case CFMR:
      result = "cfm";
      break;  
    case UMRG:   
      result = "psr.um";
      break;
    case PROT:
      result = "pr.rot";
      break;
    case PSRG:
      result = "psr";
      break;
    case CREG:
      result = "cr";
      break;
    }
    return result;
  }

  // this procedure returns a string representation of a register by
  // type, logical number and physical number
  static string reg_to_str (t_lreg lreg, const long lnum, const long pnum) {
    ostringstream os;
    os << lreg_to_str (lreg);
    switch (lreg) {
    case GREG:
    case NREG:
    case FREG:
    case PREG:
    case BREG:
    case AREG:
      os << '[' << setw (3) << setfill ('0') << lnum << ']' << dec;
      os << '[' << setw (3) << setfill ('0') << pnum << ']' << dec;
      break;
    default:
      break;
    }
    return os.str ();
  }
  
  // this procedure encode a unit to a unit code
  static inline t_byte encode_unit (t_unit unit) {
    return (t_byte) unit;
  }

  // this procedure decode a unit code to a unit
  static inline t_unit decode_unit (const t_byte code) {
    if ((code > UNIT_NONE) && (code < UNIT_UMAX)) return (t_unit) code;
    throw Exception ("record-error", "invalid unit code to decode");
  }

  // this procedure map a unit code to a character
  static t_byte unit_to_char (const t_byte code) {
    t_unit unit = decode_unit (code);
    t_byte result = 'U';
    switch (unit) {
    case AUNIT:
      result = 'A';
      break;
    case MUNIT:
      result = 'M';
      break;
    case IUNIT:
      result = 'I';
      break;
    case FUNIT:
      result = 'F';
      break;
    case BUNIT:
      result = 'B';
      break;
    case XUNIT:
      result = 'X';
      break;
    }
    return result;
  }

  // transform a char to is lower form
  static char tolower (const char c) {
    char result = c;
    if (c >= 'A' && c <= 'Z') result = c - 'A' + 'a';
    return result;
  }

  // map a string to record type
  Record::t_rctp Record::totype (const string& s) {
    // convert to lower case
    string us = s;
    transform (us.begin(), us.end(), us.begin (), tolower);
    // and now check it
    if (us == "ignore") return IGNORE;
    if (us == "halted") return HALTED;
    if (us == "rdtack") return RDTACK;
    if (us == "bundle") return BUNDLE;
    if (us == "rinstr") return RINSTR;
    if (us == "regupd") return REGUPD;
    if (us == "regchk") return REGCHK;
    if (us == "ralloc") return RALLOC;
    if (us == "rgread") return RGREAD;
    if (us == "smemrd") return SMEMRD;
    if (us == "smemwr") return SMEMWR;
    if (us == "altupd") return ALTUPD;
    string msg = "invalid type record name " + s;
    throw Exception ("convert error", msg);
  }

  // create a default record

  Record::Record (void) {
    d_type = IGNORE;
  }
  
  // create an ignore record by name
  
  Record::Record (const string& rsrc) {
    d_rsrc = rsrc;
    d_type = IGNORE;
  }

  // create a record by source and bundle

  Record::Record (const string& rsrc, const Bundle& bndl) {
    d_rsrc = rsrc;
    d_type = IGNORE;
    setbndl (bndl);
  }

  // create a record by source and instruction

  Record::Record (const string& rsrc, const Instr& inst) {
    d_rsrc = rsrc;
    d_type = IGNORE;
    setinst (inst);
  }

  // create a record by source and instruction and execute flag

  Record::Record (const string& rsrc, const Instr& inst, const bool flag) {
    d_rsrc = rsrc;
    d_type = IGNORE;
    if (flag == true)
      setinst (inst);
    else
      setcanc (inst);
  }

  // create a record by source, cancel flag and target ip

  Record::Record (const string& rsrc, const Instr& inst, const bool flg,
		  const t_octa tg) {
    d_rsrc = rsrc;
    d_type = IGNORE;
    setbr (inst, flg, tg);
  }

  // create a record by source, result and index

  Record::Record (const string& rsrc, const Result& resl, const long index) {
    d_rsrc = rsrc;
    d_type = IGNORE;
    setresl (resl, index);
  }

 // create a record by source, operand and index

  Record::Record (const string& rsrc, const Operand& oprd, const long index) {
    d_rsrc = rsrc;
    d_type = IGNORE;
    setoprd (oprd, index);
  }

  // create a record by source, ip and allocated index

  Record::Record (const string& rsrc, const t_octa ip, const long ridx) {
    d_rsrc = rsrc;
    d_type = IGNORE;
    setrcda (ip, ridx);
  }

  // copy construct this record

  Record::Record (const Record& that) {
    d_rsrc = that.d_rsrc;
    d_type = that.d_type;
    switch (d_type) {
    case RDTACK:
      d_info.d_rdta.d_addr = that.d_info.d_rdta.d_addr;
      d_info.d_rdta.d_rlen = that.d_info.d_rdta.d_rlen;
      break;
    case BUNDLE:
      for (long i = 0; i < BN_BYSZ; i++) 
	d_info.d_bndl.d_data[i] = that.d_info.d_bndl.d_data[i];
      d_info.d_bndl.d_ip = that.d_info.d_bndl.d_ip;
      break;
    case RINSTR:
      d_info.d_inst.d_unit = that.d_info.d_inst.d_unit;
      d_info.d_inst.d_slot = that.d_info.d_inst.d_slot;
      d_info.d_inst.d_stop = that.d_info.d_inst.d_stop;
      d_info.d_inst.d_data = that.d_info.d_inst.d_data;
      d_info.d_inst.d_extd = that.d_info.d_inst.d_extd;
      d_info.d_inst.d_ip   = that.d_info.d_inst.d_ip;
      d_info.d_inst.d_tip  = that.d_info.d_inst.d_tip;
      d_info.d_inst.d_can  = that.d_info.d_inst.d_can;
      d_info.d_inst.d_br   = that.d_info.d_inst.d_br;
      break;
    case REGUPD:
    case RGREAD:
      d_info.d_rdat.d_lreg = that.d_info.d_rdat.d_lreg;
      d_info.d_rdat.d_lnum = that.d_info.d_rdat.d_lnum;
      d_info.d_rdat.d_pnum = that.d_info.d_rdat.d_pnum;
      d_info.d_rdat.d_bval = that.d_info.d_rdat.d_bval;
      d_info.d_rdat.d_oval = that.d_info.d_rdat.d_oval;
      for (long i = 0; i < t_real::TR_SFSZ; i++) {
	d_info.d_rdat.d_rval[i] = that.d_info.d_rdat.d_rval[i];
      }
      break;
    case REGCHK:
      d_info.d_rchk.d_lreg = that.d_info.d_rchk.d_lreg;
      d_info.d_rchk.d_pnum = that.d_info.d_rchk.d_pnum;
      d_info.d_rchk.d_bval = that.d_info.d_rchk.d_bval;
      d_info.d_rchk.d_oval = that.d_info.d_rchk.d_oval;
      for (long i = 0; i < t_real::TR_SFSZ; i++) {
	d_info.d_rchk.d_rval[i] = that.d_info.d_rchk.d_rval[i];
      }
      break;
    case RALLOC:
      d_info.d_rcda.d_ip   = that.d_info.d_rcda.d_ip;
      d_info.d_rcda.d_ridx = that.d_info.d_rcda.d_ridx;
      break;
    case SMEMRD:
    case SMEMWR:
      d_info.d_mema.d_addr = that.d_info.d_mema.d_addr;
      d_info.d_mema.d_oval = that.d_info.d_mema.d_oval;
      d_info.d_mema.d_size = that.d_info.d_mema.d_size;
      break;
    case ALTUPD:
      d_info.d_alat.d_tag  = that.d_info.d_alat.d_tag;
      d_info.d_alat.d_addr = that.d_info.d_alat.d_addr;
      d_info.d_alat.d_ldsz = that.d_info.d_alat.d_ldsz;
      d_info.d_alat.d_add  = that.d_info.d_alat.d_add;
    default:
      break;
    }
  }

  // assign a record to this one

  Record& Record::operator = (const Record& that) {
    d_rsrc = that.d_rsrc;
    d_type = that.d_type;
    switch (d_type) {
    case RDTACK:
      d_info.d_rdta.d_addr = that.d_info.d_rdta.d_addr;
      d_info.d_rdta.d_rlen = that.d_info.d_rdta.d_rlen;
      break;
    case BUNDLE:
      for (long i = 0; i < BN_BYSZ; i++) 
	d_info.d_bndl.d_data[i] = that.d_info.d_bndl.d_data[i];
      d_info.d_bndl.d_ip = that.d_info.d_bndl.d_ip;
      break;
    case RINSTR:
      d_info.d_inst.d_unit = that.d_info.d_inst.d_unit;
      d_info.d_inst.d_slot = that.d_info.d_inst.d_slot;
      d_info.d_inst.d_stop = that.d_info.d_inst.d_stop;
      d_info.d_inst.d_data = that.d_info.d_inst.d_data;
      d_info.d_inst.d_extd = that.d_info.d_inst.d_extd;
      d_info.d_inst.d_ip   = that.d_info.d_inst.d_ip;
      d_info.d_inst.d_tip  = that.d_info.d_inst.d_tip;
      d_info.d_inst.d_can  = that.d_info.d_inst.d_can;
      d_info.d_inst.d_br   = that.d_info.d_inst.d_br;
      break;
    case REGUPD:
    case RGREAD:
      d_info.d_rdat.d_lreg = that.d_info.d_rdat.d_lreg;
      d_info.d_rdat.d_lnum = that.d_info.d_rdat.d_lnum;
      d_info.d_rdat.d_pnum = that.d_info.d_rdat.d_pnum;
      d_info.d_rdat.d_bval = that.d_info.d_rdat.d_bval;
      d_info.d_rdat.d_oval = that.d_info.d_rdat.d_oval;
      for (long i = 0; i < t_real::TR_SFSZ; i++) {
	d_info.d_rdat.d_rval[i] = that.d_info.d_rdat.d_rval[i];
      }
      break;
    case REGCHK:
      d_info.d_rchk.d_lreg = that.d_info.d_rchk.d_lreg;
      d_info.d_rchk.d_pnum = that.d_info.d_rchk.d_pnum;
      d_info.d_rchk.d_bval = that.d_info.d_rchk.d_bval;
      d_info.d_rchk.d_oval = that.d_info.d_rchk.d_oval;
      for (long i = 0; i < t_real::TR_SFSZ; i++) {
	d_info.d_rchk.d_rval[i] = that.d_info.d_rchk.d_rval[i];
      }
      break;
    case RALLOC:
      d_info.d_rcda.d_ip   = that.d_info.d_rcda.d_ip;
      d_info.d_rcda.d_ridx = that.d_info.d_rcda.d_ridx;
      break;
    case SMEMRD:
    case SMEMWR:
      d_info.d_mema.d_addr = that.d_info.d_mema.d_addr;
      d_info.d_mema.d_oval = that.d_info.d_mema.d_oval;
      d_info.d_mema.d_size = that.d_info.d_mema.d_size;
      break;
    case ALTUPD:
      d_info.d_alat.d_tag  = that.d_info.d_alat.d_tag;
      d_info.d_alat.d_addr = that.d_info.d_alat.d_addr;
      d_info.d_alat.d_ldsz = that.d_info.d_alat.d_ldsz;
      d_info.d_alat.d_add  = that.d_info.d_alat.d_add;
    default:
      break;
    }
    return *this;
  }

  // return true if a record is in order

  bool Record::operator < (const Record& rcd) const {
    // we only compare instrction record
    if ((d_type != RINSTR) || (rcd.d_type != RINSTR)) return false;
    // check for the address
    if (d_info.d_inst.d_ip < rcd.d_info.d_inst.d_ip) return true;
    if (d_info.d_inst.d_ip > rcd.d_info.d_inst.d_ip) return false;
    // both address are equal _ check the slot
    if (d_info.d_inst.d_slot < rcd.d_info.d_inst.d_slot) return true;
    return false;
  }

  // reset this record

  void Record::reset (void) {
    d_type = IGNORE;
  }

  // set the record source name

  void Record::setname (const string& rsrc) {
    d_rsrc = rsrc;
  }

  // return the record source name

  string Record::getname (void) const {
    return d_rsrc;
  }

  // set the record type

  void Record::settype (t_rctp type) {
    d_type = type;
  }

  // return the record type

  Record::t_rctp Record::gettype (void) const {
    return d_type;
  }

  // return true if the record is of type ignore

  bool Record::isignore (void) const {
    if (d_type == IGNORE) return true;
    return false;
  }

  // set a record read transaction acknowledge
  
  void Record::setrdta (const t_octa addr, const long rlen) {
    d_type               = RDTACK;
    d_info.d_rdta.d_addr = addr;
    d_info.d_rdta.d_rlen = rlen;
  }

  // set a record with a bundle

  void Record::setbndl (const Bundle& bndl) {
    // check for valid bundle
    if (bndl.isvalid () == false) return;
    // update record info
    d_type = BUNDLE;
    // update bundle info
    for (long i = 0; i<BN_BYSZ; i++) d_info.d_bndl.d_data[i] = bndl.get(i);
    d_info.d_bndl.d_ip = bndl.getbip ();
  }

  // set a record with an instruction

  void Record::setinst (const Instr& inst) {
    // check for valid instruction
    if (inst.isvalid () == false) return;
    // update record info
    d_type = RINSTR;
    // update instruction info
    d_info.d_inst.d_unit = encode_unit (inst.getbunit  ());
    d_info.d_inst.d_slot = inst.getslot ();
    d_info.d_inst.d_stop = inst.getstop ();
    d_info.d_inst.d_data = inst.getdata ();
    d_info.d_inst.d_extd = inst.getextd ();
    d_info.d_inst.d_ip   = inst.getiip  ();
    d_info.d_inst.d_tip  = OCTA_0;
    d_info.d_inst.d_can  = false;
    d_info.d_inst.d_br   = false;
  }

  // set a record with a cancel instruction

  void Record::setcanc (const Instr& inst) {
    // check for valid instruction
    if (inst.isvalid () == false) return;
    // update record info
    d_type = RINSTR;
    // update instruction info
    d_info.d_inst.d_unit = encode_unit (inst.getbunit  ());
    d_info.d_inst.d_slot = inst.getslot ();
    d_info.d_inst.d_stop = inst.getstop ();
    d_info.d_inst.d_data = inst.getdata ();
    d_info.d_inst.d_extd = inst.getextd ();
    d_info.d_inst.d_ip   = inst.getiip  ();
    d_info.d_inst.d_tip  = OCTA_0;
    d_info.d_inst.d_can  = true;
    d_info.d_inst.d_br   = false;
  }

  // set a record with a branch instruction

  void Record::setbr (const Instr& inst, const bool flg, const t_octa tg) {
    // check for valid instruction
    if (inst.isvalid () == false) return;
    // update record info
    d_type = RINSTR;
    // update instruction info
    d_info.d_inst.d_unit = encode_unit (inst.getbunit  ());
    d_info.d_inst.d_slot = inst.getslot ();
    d_info.d_inst.d_stop = inst.getstop ();
    d_info.d_inst.d_data = inst.getdata ();
    d_info.d_inst.d_extd = inst.getextd ();
    d_info.d_inst.d_ip   = inst.getiip  ();
    d_info.d_inst.d_tip  = tg;
    d_info.d_inst.d_can  = flg;
    d_info.d_inst.d_br   = true;
  }

  // set a record with checking data

  void Record::setrchk (const t_byte lreg, const t_byte pnum, 
			const t_octa data) {
    d_type = REGCHK;
    d_info.d_rchk.d_lreg = lreg;
    d_info.d_rchk.d_pnum = (t_sint) pnum;
    switch (decode_lreg (lreg)) {
    case GREG:
      d_info.d_rchk.d_bval = false;
      d_info.d_rchk.d_oval = data;
      break;
    case NREG:
    case PREG:
      d_info.d_rchk.d_bval = data == OCTA_0 ? false : true;
      break;
    case BREG:
    case AREG:
    case CREG:
      d_info.d_rchk.d_oval = data;
      break;
    default:
      throw Exception ("record-erro", "invalid register type to check");
      break;
    }
  }

  // set a record with checking data for floating point value

  void Record::setrchk (const t_byte lreg, const t_byte pnum, 
			const t_octa data1, const t_octa data2) {
    d_type = REGCHK;
    d_info.d_rchk.d_lreg = lreg;
    d_info.d_rchk.d_pnum = (t_sint) pnum;
    assert (lreg == FREG);
    union{
      t_octa oval;
      t_byte bval[8];
    } src1, src2;
    src1.oval = lfixocta (data1);
    src2.oval = lfixocta (data2);
    t_byte cval[16];
    for (long i = 0; i < 8; i++) {
      cval[8+i] = src1.bval[i];
      cval[i]   = src2.bval[i];
    }
    t_real rval;
    rval.fill  (cval);
    rval.spill (d_info.d_rchk.d_rval);
  }

  // set a record with a result by index

  void Record::setresl (const Result& resl, const long index) {
    // check for valid result
    if (resl.isvalid () == false) return;
    // get result type and return if not update
    Result::t_rop rop = resl.getrop (index);
    if ((rop == Result::ROP_NOP) || (rop == Result::REG_RRT)) return;
    // process a register update
    if ((rop == Result::REG_UPD) || (rop == Result::RSE_ALC) ||
	(rop == Result::RSE_CAL) || (rop == Result::RSE_RET) ||
	(rop == Result::RSE_LOP)) {
      d_type = REGUPD;
      // update result info
      Rid rid = resl.getrid (index);
      if (rid.isvalid () == false) return;
      t_lreg lreg = rid.gettype ();
      d_info.d_rdat.d_lreg = encode_lreg (lreg);
      d_info.d_rdat.d_lnum = rid.getlnum ();
      d_info.d_rdat.d_pnum = rid.getpnum ();
      switch (lreg) {
      case GREG:
	d_info.d_rdat.d_oval = resl.getoval (index);
	d_info.d_rdat.d_bval = resl.getbval (index);
	break;
      case NREG:
      case PREG:
	d_info.d_rdat.d_bval = resl.getbval (index);
	break;
      case FREG:
	{
	  t_real rval = resl.getrval(index);
	  rval.spill (d_info.d_rdat.d_rval);
	}
	break;
      default:
	d_info.d_rdat.d_oval = resl.getoval (index);
	break;
      }
    }
    if ((rop == Result::REG_LD1) || (rop == Result::REG_LD2) ||	
	(rop == Result::REG_LD4) || (rop == Result::REG_LD8)) assert (false);
    if (rop == Result::REG_ST1) {
      t_octa addr = resl.getaddr (index);
      t_octa oval = resl.getimmv (index);
      setwmem (addr, oval, 0x01);
    }
    if (rop == Result::REG_ST2) {
      t_octa addr = resl.getaddr (index);
      t_octa oval = resl.getimmv (index);
      setwmem (addr, oval, 0x02);
    }
    if (rop == Result::REG_ST4) {
      t_octa addr = resl.getaddr (index);
      t_octa oval = resl.getimmv (index);
      setwmem (addr, oval, 0x04);
    }
    if (rop == Result::REG_ST8) {
      t_octa addr = resl.getaddr (index);
      t_octa oval = resl.getimmv (index);
      setwmem (addr, oval, 0x08);
    }
  }

  // set a record with an operand by index

  void Record::setoprd (const Operand& oprd, const long index) {
    // update operand info
    Rid rid = oprd.getrid (index);
    // check if oprd rid is valid
    if (rid.isvalid () == false) return;
    d_type = RGREAD;
    t_lreg lreg = rid.gettype ();
    d_info.d_rdat.d_lreg = encode_lreg (lreg);
    d_info.d_rdat.d_lnum = rid.getlnum ();
    d_info.d_rdat.d_pnum = rid.getpnum ();
    switch (lreg) {
    case GREG:
      d_info.d_rdat.d_oval = oprd.getoval (index);
      d_info.d_rdat.d_bval = oprd.getbval (index);
      break;
    case NREG:
    case PREG:
      d_info.d_rdat.d_bval = oprd.getbval (index);
      break;
    case FREG:
      {
	t_real rval = oprd.getrval(index);
	rval.spill (d_info.d_rdat.d_rval);
      }
      break;
    default:
      d_info.d_rdat.d_oval = oprd.getoval (index);
      break;
    }
  }

  // set a record with an ip and allocated index

  void Record::setrcda (const t_octa ip, const long ridx) {
    // check for valid index
    if (ridx == -1) return;
    // update record info
    d_type               = RALLOC;
    d_info.d_rcda.d_ip   = ip;
    d_info.d_rcda.d_ridx = ridx;
  }

  // set a record with a memory read access information

  void Record::setrmem (const t_octa addr, const t_octa oval , 
			const t_byte size) {
    // update record info
    d_type               = SMEMRD;
    d_info.d_mema.d_addr = addr;
    d_info.d_mema.d_oval = oval;
    d_info.d_mema.d_size = size;
  }

  // set a record with a memory write access information

  void Record::setwmem (const t_octa addr, const t_octa oval , 
			const t_byte size) {
    // update record info
    d_type               = SMEMWR;
    d_info.d_mema.d_addr = addr;
    d_info.d_mema.d_oval = oval;
    d_info.d_mema.d_size = size;
  }

  // set a record with an alat action
  
  void Record::setalat (const t_word tag, const t_octa addr, const t_byte size,
			const bool add) {
    d_type               = ALTUPD;
    d_info.d_alat.d_tag  = tag;
    d_info.d_alat.d_addr = addr;
    d_info.d_alat.d_ldsz = size;
    d_info.d_alat.d_add  = add;
  }

  // return a string representation of the record
  
  string Record::repr (void) const {
    ostringstream os;
    // format ignore for info
    if (d_type == IGNORE) {
      os << d_rsrc << ":IGNORE";
    }
    // format resource halted
    if (d_type == HALTED) {
      os << d_rsrc << ":HALTED";
    }
    // format read transaction information
    if (d_type == RDTACK) {
      os << d_rsrc << ":RDTACK ";
      os << "@0x" << hex << setw (16) << setfill ('0') << d_info.d_rdta.d_addr;
      os << dec << " [" << d_info.d_rdta.d_rlen << ']';
    }
    // format bundle information
    if (d_type == BUNDLE) {
      t_octa ip = d_info.d_bndl.d_ip;
      os << d_rsrc << ":BUNDLE ";
      os << "@0x" << hex << setw (16) << setfill ('0') << ip << dec;
      for (long i = 0; i < BN_BYSZ; i++) {
	os << ' ' << hex << setw (2) << setfill ('0');
	os << (long) d_info.d_bndl.d_data[i] << dec;
      }
    }
    // format instruction information
    if (d_type == RINSTR) {
      t_octa ip = d_info.d_inst.d_ip;
      switch (d_type) {
      case RINSTR:
	os << d_rsrc << ":RINSTR ";
	break;
      default:
	assert (false);
	break;
      }
      t_unit unit = decode_unit (d_info.d_inst.d_unit);
      long   slot = d_info.d_inst.d_slot;
      bool   stop = d_info.d_inst.d_stop;
      t_octa data = d_info.d_inst.d_data;
      t_octa extd = d_info.d_inst.d_extd;
      Instr inst (unit, slot, stop, data, extd);
      inst.setiip (d_info.d_inst.d_ip);
      os << "@0x" << hex << setw (16) << setfill ('0') << ip;
      os << " ["  << inst.getgroup () << ']';
      os << " ["  << unit_to_char (d_info.d_inst.d_unit) << ']';
      if (d_info.d_inst.d_br == true) os << " @" << hex << d_info.d_inst.d_tip;
      if (d_info.d_inst.d_can == true) 
	os << " [F]";
      else 
	os << " [T]";
      if (unit == XUNIT) os << ' ' << trim (inst.recode ());
      else os << ' ' << trim (inst.recode ());
    }
    // format register read and update
    if ((d_type == RGREAD) || (d_type == REGUPD)) {
      t_lreg lreg = decode_lreg (d_info.d_rdat.d_lreg);
      long   lnum = d_info.d_rdat.d_lnum;
      long   pnum = d_info.d_rdat.d_pnum;
      t_octa oval = d_info.d_rdat.d_oval;
      t_real rval = 0.0L;
      if (d_type == RGREAD) os << d_rsrc << ":RGREAD ";
      if (d_type == REGUPD) os << d_rsrc << ":REGUPD ";
      os << reg_to_str (lreg, lnum, pnum) << ' ';
      switch (lreg) {
      case GREG:
	os << "\t0x";
	os << hex << setw (16) << setfill ('0') << oval << dec;
	if (d_info.d_rdat.d_bval == true)
	  os << "[T]";
	else
	  os << "[F]";
	break;
      case NREG:
      case PREG:
	if (d_info.d_rdat.d_bval == true) 
	  os << '\t' << "T";
	else
	  os << '\t' << "F";
	break;
      case FREG:
	rval.fill (d_info.d_rdat.d_rval);
	os << '\t' << rval << "\t(raw " << rval.repr () << ')';
	break;
      case BREG:
      case AREG:
	os << "\t0x";
	os << hex << setw (16) << setfill ('0') << oval << dec;
	break;
      case CFMR:
	os << "\t\t0x";
	os << hex << setw (16) << setfill ('0') << oval << dec;
	break;
      default:
	os << "\t\t\t0x";
	os << hex << setw (16) << setfill ('0') << oval << dec;
	break;
      }
    }
    // format register checking
    if (d_type == REGCHK) {
      t_lreg lreg = decode_lreg (d_info.d_rchk.d_lreg);
      long   pnum = d_info.d_rchk.d_pnum;
      t_octa oval = d_info.d_rchk.d_oval;
      t_real rval = 0.0L;
      os << d_rsrc << ":REGCHK ";
      os << reg_to_str (lreg, pnum, pnum) << ' ';
      switch (lreg) {
      case GREG:
	os << "\t0x";
	os << hex << setw (16) << setfill ('0') << oval << dec;
	if (d_info.d_rchk.d_bval == true)
	  os << "[T]";
	else
	  os << "[F]";
	break;
      case NREG:
      case PREG:
	if (d_info.d_rchk.d_bval == true) 
	  os << '\t' << "T";
	else
	  os << '\t' << "F";
	break;
      case FREG:
	rval.fill (d_info.d_rchk.d_rval);
	os << '\t' << rval;
	break;
      case BREG:
      case AREG:
	os << "\t0x";
	os << hex << setw (16) << setfill ('0') << oval << dec;
	break;
      default:
	os << "\t\t\t0x";
	os << hex << setw (16) << setfill ('0') << oval << dec;
	break;
      }
    }
    // format record allocation
    if (d_type == RALLOC) {
      t_octa ip = d_info.d_rcda.d_ip;
      os << d_rsrc << ":RALLOC ";
      os << "@0x" << hex << setw (16) << setfill ('0') << ip << dec;
      os << " [" << d_info.d_rcda.d_ridx << ']';
    }
    // format memory read access
    if ((d_type == SMEMRD) || (d_type == SMEMWR)) {
      t_octa addr = d_info.d_mema.d_addr;
      t_octa oval = d_info.d_mema.d_oval;
      t_byte size = d_info.d_mema.d_size;
      if (d_type == SMEMRD) os << d_rsrc << ":SMEMRD ";
      if (d_type == SMEMWR) os << d_rsrc << ":SMEMWR ";
      os << "@0x" << hex << setw (16) << setfill ('0') << addr << dec;
      switch (size) {
      case 0x01:
	os << " 0x";
	os << hex << setw (2)  << setfill ('0') << oval << dec;
	os << " [1]";
	break;
      case 0x02:
	os << " 0x";
	os << hex << setw (4)  << setfill ('0') << oval << dec;
	os << " [2]";
	break;
      case 0x04:
	os << " 0x";
	os << hex << setw (8)  << setfill ('0') << oval << dec;
	os << " [4]";
	break;
      case 0x08:
	os << " 0x";
	os << hex << setw (16) << setfill ('0') << oval << dec;
	os << " [8]";
	break;
      default:
	assert (false);
	break;
      }
    }
    if (d_type == ALTUPD) {
      if (d_info.d_alat.d_add == true)
	os << d_rsrc << ":ALATAD ";
      else 
	os << d_rsrc << ":ALATRM ";
      t_octa addr = d_info.d_alat.d_addr;
      t_word tag  = d_info.d_alat.d_tag;
      t_byte ldsz = d_info.d_alat.d_ldsz;
      os << "@0x" << hex << setw (16) << setfill ('0') << addr << dec;
      os << " ["  << hex << setw (4)  << setfill ('0') << tag  << dec << ']';
      os << " ("  << (long) ldsz << ')';
    }
    return os.str ();
  }

  // print the record information

  void Record::print (void) const {
    // do nothing with ignore
    if (d_type == IGNORE) return;
    // print formatted info
    cout << repr () << endl;
  }

  // return the record check register type

  t_lreg Record::chklreg (void) const {
    if (d_type != REGCHK) 
      throw Exception ("access-error", "invalid record check type");
    return decode_lreg (d_info.d_rchk.d_lreg);
  }

  // return the record check register number

  long Record::chkpnum (void) const {
    if (d_type != REGCHK) 
      throw Exception ("access-error", "invalid record check type");
    return d_info.d_rchk.d_pnum;
  }

  // return the record check boolean value

  bool Record::chkbval (void) const {
    if (d_type != REGCHK) 
      throw Exception ("access-error", "invalid record check type");
    return d_info.d_rchk.d_bval;
  }

  // return the record check octa value

  t_octa Record::chkoval (void) const {
    if (d_type != REGCHK) 
      throw Exception ("access-error", "invalid record check type");
    return d_info.d_rchk.d_oval;
  }

  // return the record check real value

  t_real Record::chkrval (void) const {
    if (d_type != REGCHK) 
      throw Exception ("access-error", "invalid record check type");
    t_real result;
    result.fill (d_info.d_rchk.d_rval);
    return result;
  }

  // write a record into a file

  void Record::rcdwr (int fd) const {
    // check valid descriptor
    if ((fd < 0) || (d_type == IGNORE)) return;
    // write the record type
    t_byte rt = encode_type (d_type);
    if (write (fd, &rt, 1) != 1)
      throw Exception ("record-error", "cannot write record to file");
    // write the record source
    const char* rsrc = d_rsrc.c_str ();
    long len = d_rsrc.length () + 1;
    if (write (fd, rsrc, len) != len)
      throw Exception ("record-error", "cannot write record to file");
    // write the record info
    if (write (fd, &d_info, sizeof (d_info)) != sizeof (d_info))
      throw Exception ("record-error", "cannot write record to file");
  }

  // read a record from a file

  void Record::rcdrd (int fd) {
    // check valid descriptor
    if (fd < 0) return;
    // read the record type
    t_byte rt = BYTE_0;
    if (readn (fd, &rt, 1) != 1)
      throw Exception ("record-error", "cannot read record from file");
    d_type = decode_type (rt);
    // read the record source name
    d_rsrc = "";
    while (readn (fd, &rt, 1) == 1) {
      if (rt == BYTE_0) break;
      d_rsrc.push_back (rt);
    }
    // read the record info
    long status = readn (fd, &d_info, sizeof (d_info));
    if (status != sizeof (d_info)) {
      throw Exception ("record-error", "cannot read record from file");
    }
  }

  // get an instruction from an instruction record

  Instr Record::getinst (void) const {
    assert (d_type == RINSTR);
    t_unit unit = decode_unit (d_info.d_inst.d_unit);
    long   slot = d_info.d_inst.d_slot;
    bool   stop = d_info.d_inst.d_stop;
    t_octa data = d_info.d_inst.d_data;
    t_octa extd = d_info.d_inst.d_extd;
    if (unit == XUNIT) {
      Instr inst (unit, slot, stop, data, extd);
      inst.setiip (d_info.d_inst.d_ip);
      return inst;
    }
    Instr inst (unit, slot, stop, data);
    inst.setiip (d_info.d_inst.d_ip);
    return inst;
  }

  // return true if the instruction is canceled

  bool Record::iscancel (void) const {
    if (d_type != RINSTR) return false;
    return (d_info.d_inst.d_can == true);
  }

  // return true if the instruction is a branch

  bool Record::istip (void) const {
    if (d_type != RINSTR) return false;
    return (d_info.d_inst.d_br == true);
  }

  // get a bundle

  Bundle Record::getbnd (void) const {
    assert (d_type == BUNDLE);
      t_octa ip = d_info.d_bndl.d_ip;
      Bundle bnd;
      bnd.setbip (ip);
      for (long i = 0; i < BN_BYSZ; i++) bnd.push (d_info.d_bndl.d_data[i]);
	return bnd;
  } 

  // @return the target ip

  t_octa Record::gettip (void) const {
    if (istip () == false) return OCTA_0;
    return d_info.d_inst.d_tip;
  }
}
