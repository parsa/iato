// ---------------------------------------------------------------------------
// - Bexecute.cpp                                                            -
// - iato:isa library - B execution unit class implementation                -
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

#include "Ip.hpp"
#include "Cfm.hpp"
#include "Pfs.hpp"
#include "Utils.hpp"
#include "Bexecute.hpp"
#include "Exception.hpp"
#include "Interrupt.hpp"

namespace iato {

  /// the bsp mask
  const t_octa BSP_MASK = 0x00000000000001F8ULL;

  // ------------------------------------------------------------------------
  // - B01 instruction group                                                -
  // ------------------------------------------------------------------------
 
  // B_COND_IP instruction
  static Result exec_br_cond_ip (const Instr& inst, const Operand& oprd) {
    // get instruction ip
    t_long ip = inst.getiip  ();
    t_long rd = inst.getimmv (0);
    t_octa rp = ip + rd;
    // set result
    Result result = inst.getresl ();
    result.setoval (0, rp);
    return result;
  }

  // B_WTOP_IP instruction
  static Result exec_br_wtop_ip (const Instr& inst, const Operand& oprd) {
    // get instruction operand
    t_long ip     = inst.getiip  ();
    Cfm cfm       = oprd.getoval (0);
    t_octa ec     = oprd.getoval (1);
    Result result = inst.getresl ();
    // anyway set p63 to false
    result.setbval (3, false);
    // test predicate and set result
    if (oprd.getbval (2) == false) {
      if (ec > 1) {
	// in that case do branch
	ip += (t_long) inst.getimmv (0);
	result.setoval (0, ip);
	result.setoval (1, --ec);
	result.setoval (2, Result::RSE_LOP, cfm.rotate ());
      } else if (ec == 1) {
	// in that case do not branch
	ip += BN_BYSZ;
	result.setoval (0, ip);
	result.setoval (1, --ec);
	result.setoval (2, Result::RSE_LOP, cfm.rotate ());
      } else if (ec == 0) {
	// in that case do not branch
	ip += BN_BYSZ;
	result.setoval (0, ip);
	result.setoval (1, ec);
	result.setoval (2, Result::RSE_LOP, cfm.getcfm ());
      }
    } else {
      // in that case wtop branch
      ip += (t_long) inst.getimmv (0);
      result.setoval (0, ip);
      result.setoval (1, ec);
      result.setoval (2, Result::RSE_LOP, cfm.rotate ());
    }
    return result;
  }

  // B_WEXIT_IP instruction
  static Result exec_br_wexit_ip (const Instr& inst, const Operand& oprd) {
    // get instruction operand
    t_long ip     = inst.getiip  ();
    Cfm cfm       = oprd.getoval (0);
    t_octa ec     = oprd.getoval (1);
    Result result = inst.getresl ();
    // anyway set p63 to false
    result.setbval (3, false);
    // test predicate and set result
    if (oprd.getbval (2) == false) {
      if (ec > 1) {
	// in that case do not branch
	ip += BN_BYSZ;
	result.setoval (0, ip);
	result.setoval (1, --ec);
	result.setoval (2, Result::RSE_LOP, cfm.rotate ());
      } else if (ec == 1) {
	// in that case branch
	ip += (t_long) inst.getimmv (0);
	result.setoval (0, ip);
	result.setoval (1, --ec);
	result.setoval (2, Result::RSE_LOP, cfm.rotate ());
      } else if (ec == 0) {
	// in that case branch
	ip += (t_long) inst.getimmv (0);
	result.setoval (0, ip);
	result.setoval (1, ec);
	result.setoval (2, Result::RSE_LOP, cfm.getcfm ());
      }
    } else {
      // in that case do not branch
      ip += BN_BYSZ;
      result.setoval (0, ip);
      result.setoval (1, ec);
      result.setoval (2, Result::RSE_LOP, cfm.rotate ());
    }
    return result;
  }

  // ------------------------------------------------------------------------
  // - B02 instruction group                                                -
  // ------------------------------------------------------------------------
 
  // B_CLOOP_IP
  static Result exec_br_cloop_ip (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // get instruction ip and loop count
    t_long ip = inst.getiip  ();
    t_octa lc = oprd.getoval (0);
    // check for count - no branch if 0
    if (lc == OCTA_0) {
      t_octa nip = ip + BN_BYSZ;
      result.setoval (0, nip);
      result.setoval (1, lc);
    } else {
      t_long rd = inst.getimmv (0);
      t_octa rp = ip + rd;
      result.setoval (0, rp);
      result.setoval (1, --lc);
    }
    return result;
  }

  // B_CTOP_IP
  static Result exec_br_ctop_ip (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // get loop and epilog counts
    t_octa ec = oprd.getoval (1);
    t_octa lc = oprd.getoval (2);
    // compute loop taken flag
    bool taken = (lc != OCTA_0) || (ec > OCTA_1);
    // compute new ip
    t_long rd = inst.getimmv (0);
    t_octa ip = inst.getiip  ();
    t_octa np  = taken ? ip + rd : ip + BN_BYSZ;
    result.setoval (0, np);
    // prepare for cfm update
    Cfm cfm = oprd.getoval (0);
    // update loop count value
    if (lc != OCTA_0) {
      result.setoval (2, --lc);
      result.setoval (1, ec);
      result.setbval (4, true);
      result.setoval (3, Result::RSE_LOP, cfm.rotate ());
    } else if (ec != OCTA_0) {
      result.setoval (2, lc);
      result.setoval (1, --ec);
      result.setbval (4, false);
      result.setoval (3, Result::RSE_LOP, cfm.rotate ());
    } else {
      result.setoval (2, lc);
      result.setoval (1, ec);
      result.setbval (4, false);
      result.setoval (3, Result::RSE_LOP, cfm.getcfm ());
    }
    return result;
  }

  // B_CEXIT_IP
  static Result exec_br_cexit_ip (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // get loop and epilog counts
    t_octa ec = oprd.getoval (1);
    t_octa lc = oprd.getoval (2);
    // compute loop taken flag
    bool taken = !((lc != OCTA_0) || (ec > OCTA_1));
    // compute new ip
    t_long rd = inst.getimmv (0);
    t_octa ip = inst.getiip ();
    t_octa np  = taken ? ip + rd : ip + BN_BYSZ;
    result.setoval (0, np);
    // prepare for cfm update
    Cfm cfm = oprd.getoval (0);
    // update loop count value
    if (lc != OCTA_0) {
      result.setoval (2, --lc);
      result.setoval (1, ec);
      result.setbval (4, true);
      result.setoval (3, Result::RSE_LOP, cfm.rotate ());
    } else if (ec != OCTA_0) {
      result.setoval (2, lc);
      result.setoval (1, --ec);
      result.setbval (4, false);
      result.setoval (3, Result::RSE_LOP, cfm.rotate ());
    } else {
      result.setoval (2, lc);
      result.setoval (1, ec);
      result.setbval (4, false);
      result.setoval (3, Result::RSE_LOP, cfm.getcfm ());
    }
    return result;
  }

  // ------------------------------------------------------------------------
  // - B03 instruction group                                                -
  // ------------------------------------------------------------------------

  // B_CALL_IP
  static Result exec_br_call_ip (const Instr& inst, const Operand& oprd) {
    // compute ip and branch return
    t_long ip   = inst.getiip ();
    t_long rd   = inst.getimmv (0);
    t_octa nip  = ip + rd;
    t_octa rval = ip + BN_BYSZ;
    // compute cfm
    Cfm ocfm = oprd.getoval (0);
    Cfm ncfm = OCTA_0;
    ncfm.call (ocfm);
    // compute new pfs
    Pfs pfs;
    pfs.setfld (Pfs::PFM, oprd.getoval (0));
    pfs.setfld (Pfs::PEC, oprd.getoval (1));
    // compute new ar.bsp
    t_octa sol = ocfm.getfld (Cfm::SOL);
    t_octa bsp = oprd.getoval (2);
    bsp = ((bsp >> 3) + sol + ((((bsp & BSP_MASK) >> 3) + sol) / 63)) << 3;
    // set result
    Result result = inst.getresl ();
    result.setoval (0, rval);
    result.setoval (1, nip);
    result.setoval (2, pfs.getpfs ());
    result.setoval (3, Result::RSE_CAL, ncfm.getcfm ());
    result.setoval (4, bsp);
    return result;
  }

  // ------------------------------------------------------------------------
  // - B04 instruction group                                                -
  // ------------------------------------------------------------------------

  // B_COND
  static Result exec_br_cond (const Instr& inst, const Operand& oprd) {
    t_octa rp = oprd.getoval (0);
    // set result
    Result result = inst.getresl ();
    result.setoval (0, rp);
    return result;
  }

  // B_RET
  static Result exec_br_ret (const Instr& inst, const Operand& oprd) {
    Ip  ip  = oprd.getoval (0);
    Pfs pfs = oprd.getoval (1);
    Cfm cfm = pfs.getfld (Pfs::PFM);
    // compute ar.ec
    t_octa ec = pfs.getfld (Pfs::PEC);
    // compute ar.bsp
    t_octa sol = cfm.getfld (Cfm::SOL);
    t_octa bsp = oprd.getoval (2);
    bsp = ((bsp >> 3) - sol - ((62 - ((bsp & BSP_MASK) >> 3) + sol) / 63))<< 3;
    // set result
    Result result = inst.getresl ();
    result.setoval (0, ip.getip());
    result.setoval (1, Result::RSE_RET, cfm.getcfm ());
    result.setoval (2, ec);
    result.setoval (3, bsp);
    return result;
  }

  // ------------------------------------------------------------------------
  // - B05 instruction group                                                -
  // ------------------------------------------------------------------------

  // B_CALL
  static Result exec_br_call (const Instr& inst, const Operand& oprd) { 
    // compute ip and branch return
    t_octa nip   = oprd.getoval (0);
    t_octa rval  = inst.getiip () + BN_BYSZ;
    // compute new cfm
    Cfm ocfm = oprd.getoval (1);
    Cfm ncfm = OCTA_0;
    ncfm.call (ocfm);
    // compute new pfs
    Pfs pfs;
    pfs.setfld (Pfs::PFM, oprd.getoval (1));
    pfs.setfld (Pfs::PEC, oprd.getoval (2));
    // compute ar.bsp
    t_octa sol = ocfm.getfld (Cfm::SOL);
    t_octa bsp = oprd.getoval (3);
    bsp = ((bsp >> 3) + sol + ((((bsp & BSP_MASK) >> 3) + sol) / 63)) << 3;
    // set result
    Result result = inst.getresl ();
    result.setoval (0, rval);
    result.setoval (1, nip);
    result.setoval (2, pfs.getpfs ());
    result.setoval (3, Result::RSE_CAL, ncfm.getcfm ());
    result.setoval (4, bsp);
    return result;
  }

  // ------------------------------------------------------------------------
  // - B06 instruction group                                                -
  // ------------------------------------------------------------------------

  static Result exec_brp_ip (const Instr& inst, const Operand& oprd) {
    Result result = true;
    return result;
  }

  // ------------------------------------------------------------------------
  // - B07 instruction group                                                -
  // ------------------------------------------------------------------------

  static Result exec_brp (const Instr& inst, const Operand& oprd) {
    Result result = true;
    return result;
  }

  static Result exec_brp_ret (const Instr& inst, const Operand& oprd) {
    Result result = true;
    return result;
  }

  // ------------------------------------------------------------------------
  // - B08 instruction group                                                -
  // ------------------------------------------------------------------------

  // B_CLRRRB
  static Result exec_clrrrb (const Instr& inst, const Operand& oprd) {
    Cfm cfm = oprd.getoval (0);
    cfm.setrrb (Cfm::RGR, 0);
    cfm.setrrb (Cfm::RFR, 0);
    cfm.setrrb (Cfm::RPR, 0);
    Result result = inst.getresl ();
    result.setoval (0, Result::RSE_RRB, cfm.getcfm ());
    return result;
  }

  // B_CLRRRB_PR
  static Result exec_clrrrb_pr (const Instr& inst, const Operand& oprd) {
    Cfm cfm = oprd.getoval (0);
    cfm.setrrb (Cfm::RPR, 0);
    Result result = inst.getresl ();
    result.setoval (0, Result::RSE_RRB, cfm.getcfm ());
    return result;
  }

  // execute a B unit instruction with operands

  Result Bexecute::exec (const Instr& inst, const Operand& oprd) const {
    Result result;
    if (inst.isvalid () == false) return result;
    // execute B specific instruction
    t_iopc iopc = inst.getiopc ();
    switch (iopc) {

      // B01 instruction group
    case B_COND_IP:
      result = exec_br_cond_ip (inst, oprd);
      break;
    case B_WTOP_IP:
      result = exec_br_wtop_ip (inst, oprd);
      break;
    case B_WEXIT_IP:
      result = exec_br_wexit_ip (inst, oprd);
      break;

      // B02 instruction group
    case B_CLOOP_IP:
      result = exec_br_cloop_ip (inst, oprd);
      break;
    case B_CTOP_IP:
      result = exec_br_ctop_ip (inst, oprd);
      break;
    case B_CEXIT_IP:
      result = exec_br_cexit_ip (inst, oprd);
      break;

      // B03 instruction group
    case B_CALL_IP:
      result = exec_br_call_ip (inst, oprd);
      break;

      // B04 instruction group
    case B_COND:
      result = exec_br_cond (inst, oprd);
      break;
    case B_RET:
      result = exec_br_ret (inst, oprd);
      break;

      // B05 instruction group
    case B_CALL:
      result = exec_br_call (inst, oprd);
      break;

      // B06 instruction group
    case B_BRP_IP:
      result = exec_brp_ip (inst, oprd);
      break;

      // B07 instruction group
    case B_BRP:
      result = exec_brp (inst, oprd);
      break;
    case B_BRP_RET:
      result = exec_brp_ret (inst, oprd);
      break;

      // B08 instruction group
    case B_CLRRRB:
      result = exec_clrrrb (inst, oprd);
      break;
    case B_CLRRRB_PR:
      result = exec_clrrrb_pr (inst, oprd);
      break;

      // B09 instruction group
    case B_BREAK:
      throw Interrupt (FAULT_IT_INST_BREAK, inst);
      break;
    case B_NOP:
      result.setvalid (true);
      break;

    default:
      break;
    }
    // check for valid result
    if (result.isvalid () == false) {
      string reason = "unimplemented instruction [B]";
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst, reason);
    }
    return result;
  }
}
