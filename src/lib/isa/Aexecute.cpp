// ---------------------------------------------------------------------------
// - Aexecute.cpp                                                            -
// - iato:isa library - A execution unit class implementation                -
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
#include "Aexecute.hpp"
#include "Uexecute.hxx"
#include "Interrupt.hpp"

namespace iato {

  // ------------------------------------------------------------------------
  // - A01 instruction group                                                -
  // ------------------------------------------------------------------------

  // A_ADD instruction
  static Result exec_add (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    t_octa rval = src1 + src2;
    // set result
    Result result = inst.getresl ();
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // A_ADD_1 instruction
  static Result exec_add_1 (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    t_octa rval = src1 + src2 + 1;
    // set result
    Result result = inst.getresl ();
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // A_SUB instruction
  static Result exec_sub (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    t_octa rval = src1 - src2;
    // set result
    Result result = inst.getresl ();
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // A_SUB instruction
  static Result exec_sub_1 (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    t_octa rval = src1 - src2 - 1;
    // set result
    Result result = inst.getresl ();
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // A_ADDP4 instruction
  static Result exec_addp4 (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    t_octa rval = (src1 + src2) & MASK_4;
    t_octa temp = (src2 & 0x00000000C0000000LL) << 31;
    rval = rval | temp;
    // set result
    Result result = inst.getresl ();
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // A_AND instruction
  static Result exec_and (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    t_octa rval = src1 & src2;
    // set result
    Result result = inst.getresl ();
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // A_ANDCM instruction
  static Result exec_andcm (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    t_octa rval = src1 & (~src2);
    // set result
    Result result = inst.getresl ();
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // A_OR instruction
  static Result exec_or (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    t_octa rval = src1 | src2;
    // set result
    Result result = inst.getresl ();
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }
  
  // A_XOR instruction
  static Result exec_xor (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    t_octa rval = src1 ^ src2;
    // set result
    Result result = inst.getresl ();
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // ------------------------------------------------------------------------
  // - A02 instruction group                                                -
  // ------------------------------------------------------------------------

  // A_SHLADD instruction
  static Result exec_shladd (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    long   src3 = inst.getimmv (0);
    t_octa rval = (src1 << src3) + src2;
    // set result
    Result result = inst.getresl ();
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }
  
  // A_SHLADDP4 instruction
  static Result exec_shladdp4 (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    long   src3 = inst.getimmv (0);
    t_octa rval = ((src1 << src3) + src2) & MASK_4;
    t_octa temp = (src2 & 0x00000000C0000000LL) << 31;
    rval = rval | temp;
    // set result
    Result result = inst.getresl ();
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // ------------------------------------------------------------------------
  // - A03 instruction group                                                -
  // ------------------------------------------------------------------------

  // A_SUB_I instruction
  static Result exec_sub_i (const Instr& inst, const Operand& oprd){
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute result
    t_octa src1 = oprd.getoval (1);
    t_octa src2 = inst.getimmv (0);
    t_octa rval = src2 - src1;
    // set result
    Result result = inst.getresl ();
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }
  
  // A_AND_I:
  static Result exec_and_i (const Instr& inst, const Operand& oprd){
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute result
    t_octa src1 = oprd.getoval (1);
    t_octa src2 = inst.getimmv (0);
    t_octa rval = src1 & src2;
    // set result
    Result result = inst.getresl ();
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }
 
  // A_ANDCM_I:
  static Result exec_andcm_i (const Instr& inst, const Operand& oprd){
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute result
    t_octa src1 = oprd.getoval (1);
    t_octa src2 = inst.getimmv (0);
    t_octa rval = src2 & (~src1);
    // set result
    Result result = inst.getresl ();
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }
  
  // A_OR_I:
  static Result exec_or_i (const Instr& inst, const Operand& oprd){
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute result
    t_octa src1 = oprd.getoval (1);
    t_octa src2 = inst.getimmv (0);
    t_octa rval = src1 | src2;
    // set result
    Result result = inst.getresl ();
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // A_XOR_I:
  static Result exec_xor_i (const Instr& inst, const Operand& oprd){
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute result
    t_octa src1 = oprd.getoval (1);
    t_octa src2 = inst.getimmv (0);
    t_octa rval = src1 ^ src2;
    // set result
    Result result = inst.getresl ();
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // ------------------------------------------------------------------------
  // - A04 instruction group                                                -
  // ------------------------------------------------------------------------

  // A_ADDS_I instruction
  static Result exec_adds_i (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute result
    t_long immv = inst.getimmv (0);
    t_long data = oprd.getoval (1);
    t_octa rval = static_cast <t_octa> (immv + data);
    // set result
    Result result = inst.getresl ();
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // A_ADDP4_I instruction
  static Result exec_addp4_i (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute result
    t_octa src1 = oprd.getoval (1);
    t_octa src2 = inst.getimmv (0);
    t_octa rval = (src1 + src2) & MASK_4;
    t_octa temp = (src1 & 0x00000000C0000000LL) << 31;
    rval = rval | temp;
    // set result
    Result result = inst.getresl ();
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // ------------------------------------------------------------------------
  // - A05 instruction group                                                -
  // ------------------------------------------------------------------------

  // A_ADDL_I instruction
  static Result exec_addl_i (const Instr& inst, const Operand& oprd) {
     // compute nat bit
    bool natr = oprd.getbval (0);
    // compute result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = inst.getimmv (0);
    t_octa rval = src1 + src2;
    // set result
    Result result = inst.getresl ();
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // ------------------------------------------------------------------------
  // - A06 instruction group                                                -
  // ------------------------------------------------------------------------

  // A_CMP_LT_RR and A_CMP_LT_UNC_RR instructions
  static Result exec_cmp_lt_rr(const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // check for result clear with unc
    if ((inst.getiopc () == A_CMP_LT_UNC_RR) && (oprd.getbval (2) == false)) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = static_cast <t_long> (oprd.getoval (0));
    t_long src2 = static_cast <t_long> (oprd.getoval (1));
    if (src1 < src2) {
      result.setbval (0, true );
      result.setbval (1, false);
    } else {
      result.setbval (0, false);
      result.setbval (1, true );
    }
    return result;
  }

  // A_CMP_LTU_RR and A_CMP_LTU_UNC_RR instructions
  static Result exec_cmp_ltu_rr(const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // check for result clear with unc
    if ((inst.getiopc () == A_CMP_LTU_UNC_RR) && (oprd.getbval (2) == false)) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    if (src1 < src2) {
      result.setbval (0, true );
      result.setbval (1, false);
    } else {
      result.setbval (0, false);
      result.setbval (1, true );
    }
    return result;
  }

  // A_CMP_EQ_RR and A_CMP_EQ_UNC_RR instruction
  static Result exec_cmp_eq_rr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // check for result clear with unc
    if ((inst.getiopc () == A_CMP_EQ_UNC_RR) && (oprd.getbval (2) == false)) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    if (src1 == src2) {
      result.setbval (0, true );
      result.setbval (1, false);
    } else {
      result.setbval (0, false);
      result.setbval (1, true );
    }
    return result;
  }

  // A_CMP_EQ_AND_RR:
  static Result  exec_cmp_eq_and_rr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    if (src1 != src2) {
      result.setbval (0, false);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP_EQ_OR_RR:
  static Result  exec_cmp_eq_or_rr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    if (src1 == src2) {
      result.setbval (0, true);
      result.setbval (1, true);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP_EQ_OR_ANDCM_RR:
  static Result  exec_cmp_eq_or_andcm_rr (const Instr& inst, 
					  const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    if (src1 == src2) {
      result.setbval (0, true);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP_NE_AND_RR:
  static Result  exec_cmp_ne_and_rr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    if ((src1 != src2) == false) {
      result.setbval (0, false);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP_NE_OR_RR:
  static Result  exec_cmp_ne_or_rr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    if (src1 != src2) {
      result.setbval (0, true);
      result.setbval (1, true);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP_NE_OR_ANDCM_RR:
  static Result  exec_cmp_ne_or_andcm_rr (const Instr& inst, 
					  const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    if (src1 != src2) {
      result.setbval (0, true);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP4_LT_RR and A_CMP4_LT_UNC_RR instructions
  static Result exec_cmp4_lt_rr(const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // check for result clear with unc
    if ((inst.getiopc () == A_CMP4_LT_UNC_RR) && (oprd.getbval (2) == false)) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = oprd.getoval (0) & MASK_4;
    if ((src1 & SIGN_4) == SIGN_4) src1 |= ~MASK_4;
    t_long src2 = oprd.getoval (1) & MASK_4;
    if ((src2 & SIGN_4) == SIGN_4) src2 |= ~MASK_4;
    if (src1 < src2) {
      result.setbval (0, true );
      result.setbval (1, false);
    } else {
      result.setbval (0, false);
      result.setbval (1, true );
    }
    return result;
  }

  // A_CMP4_LTU_RR and A_CMP4_LTU_UNC_RR instructions
  static Result exec_cmp4_ltu_rr(const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // check for result clear with unc
    if ((inst.getiopc () == A_CMP4_LTU_UNC_RR) && 
	(oprd.getbval (2) == false)) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_quad src1 = oprd.getoval (0);
    t_quad src2 = oprd.getoval (1);
    if (src1 < src2) {
      result.setbval (0, true );
      result.setbval (1, false);
    } else {
      result.setbval (0, false);
      result.setbval (1, true );
    }
    return result;
  }

  // A_CMP4_EQ_RR and A_CMP4_EQ_UNC_RR instruction
  static Result exec_cmp4_eq_rr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // check for result clear with unc
    if ((inst.getiopc () == A_CMP4_EQ_UNC_RR) && (oprd.getbval (2) == false)) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_quad src1 = oprd.getoval (0);
    t_quad src2 = oprd.getoval (1);
    if (src1 == src2) {
      result.setbval (0, true );
      result.setbval (1, false);
    } else {
      result.setbval (0, false);
      result.setbval (1, true );
    }
    return result;
  }

  // A_CMP4_EQ_AND_RR:
  static Result  exec_cmp4_eq_and_rr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compute result
    t_quad src1 = oprd.getoval (0);
    t_quad src2 = oprd.getoval (1);
    // set result
    if (src1 != src2) {
      result.setbval (0, false);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP4_EQ_OR_RR:
  static Result  exec_cmp4_eq_or_rr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_quad src1 = oprd.getoval (0);
    t_quad src2 = oprd.getoval (1);
    if (src1 == src2) {
      result.setbval (0, true);
      result.setbval (1, true);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP4_EQ_OR_ANDCM_RR:
  static Result  exec_cmp4_eq_or_andcm_rr (const Instr& inst, 
					   const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_quad src1 = oprd.getoval (0);
    t_quad src2 = oprd.getoval (1);
    if (src1 == src2) {
      result.setbval (0, true);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP4_NE_AND_RR:
  static Result  exec_cmp4_ne_and_rr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_quad src1 = oprd.getoval (0);
    t_quad src2 = oprd.getoval (1);
    if ((src1 != src2) == false) {
      result.setbval (0, false);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP4_NE_OR_RR:
  static Result  exec_cmp4_ne_or_rr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_quad src1 = oprd.getoval (0);
    t_quad src2 = oprd.getoval (1);
    if (src1 != src2) {
      result.setbval (0, true);
      result.setbval (1, true);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP4_NE_OR_ANDCM_RR:
  static Result  exec_cmp4_ne_or_andcm_rr (const Instr& inst, 
					   const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_quad src1 = oprd.getoval (0);
    t_quad src2 = oprd.getoval (1);
    if (src1 != src2) {
      result.setbval (0, true);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // ------------------------------------------------------------------------
  // - A07 instruction group                                                -
  // ------------------------------------------------------------------------

  // A_CMP_GT_AND_ZR:
  static Result exec_cmp_gt_and_zr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = 0;
    t_long src2 = oprd.getoval (1);
    if ((src1 > src2) == false) {
      result.setbval (0, false);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP_GT_OR_ZR:
  static Result exec_cmp_gt_or_zr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and setresult
    t_long src1 = 0;
    t_long src2 = oprd.getoval (1);
    if (src1 > src2) {
      result.setbval (0, true);
      result.setbval (1, true);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP_GT_OR_ANDCM_ZR:
  static Result exec_cmp_gt_or_andcm_zr (const Instr& inst, 
					 const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = 0;
    t_long src2 = oprd.getoval (1);
    if (src1 > src2) {
      result.setbval (0, true);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }
  
  // A_CMP_LE_AND_ZR:
  static Result exec_cmp_le_and_zr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = 0;
    t_long src2 = oprd.getoval (1);
    if ((src1 <= src2) == false) {
      result.setbval (0, false);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP_LE_OR_ZR:
  static Result exec_cmp_le_or_zr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = 0;
    t_long src2 = oprd.getoval (1);
    if (src1 <= src2) {
      result.setbval (0, true);
      result.setbval (1, true);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP_LE_OR_ANDCM_ZR:
  static Result exec_cmp_le_or_andcm_zr (const Instr& inst, 
					 const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = 0;
    t_long src2 = oprd.getoval (1);
    if (src1 <= src2) {
      result.setbval (0, true);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }
    
  // A_CMP_GE_AND_ZR:
  static Result exec_cmp_ge_and_zr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = 0;
    t_long src2 = oprd.getoval (1);
    if ((src1 >= src2) == false) {
      result.setbval (0, false);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }
    
  // A_CMP_GE_OR_ZR:
  static Result exec_cmp_ge_or_zr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = 0;
    t_long src2 = oprd.getoval (1);
    if (src1 >= src2) {
      result.setbval (0, true);
      result.setbval (1, true);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }
    
  // A_CMP_GE_OR_ANDCM_ZR:
  static Result exec_cmp_ge_or_andcm_zr (const Instr& inst, 
					 const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = 0;
    t_long src2 = oprd.getoval (1);
    if (src1 >= src2) {
      result.setbval (0, true);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  } 
    
  // A_CMP_LT_AND_ZR:
  static Result exec_cmp_lt_and_zr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = 0;
    t_long src2 = oprd.getoval (1);
    if ((src1 < src2) == false) {
      result.setbval (0, false);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }
    
  // A_CMP_LT_OR_ZR:
  static Result exec_cmp_lt_or_zr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = 0;
    t_long src2 = oprd.getoval (1);
    if (src1 < src2) {
      result.setbval (0, true);
      result.setbval (1, true);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }
    
  // A_CMP_LT_OR_ANDCM_ZR:
  static Result exec_cmp_lt_or_andcm_zr (const Instr& inst, 
					 const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = 0;
    t_long src2 = oprd.getoval (1);
    if (src1 < src2) {
      result.setbval (0, true);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  } 


 // A_CMP4_GT_AND_ZR:
  static Result exec_cmp4_gt_and_zr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = 0;
    t_long src2 = oprd.getoval (1) & MASK_4;
    if ((src2 & SIGN_4) == SIGN_4) src2 |= ~MASK_4;
    if ((src1 > src2) == false) {
      result.setbval (0, false);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP4_GT_OR_ZR:
  static Result exec_cmp4_gt_or_zr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = 0;
    t_long src2 = oprd.getoval (1) & MASK_4;
    if ((src2 & SIGN_4) == SIGN_4) src2 |= ~MASK_4;
    if (src1 > src2) {
      result.setbval (0, true);
      result.setbval (1, true);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP4_GT_OR_ANDCM_ZR:
  static Result exec_cmp4_gt_or_andcm_zr (const Instr& inst, 
					  const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = 0;
    t_long src2 = oprd.getoval (1) & MASK_4;
    if ((src2 & SIGN_4) == SIGN_4) src2 |= ~MASK_4;
    if (src1 > src2) {
      result.setbval (0, true);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }
  
  // A_CMP4_LE_AND_ZR:
  static Result exec_cmp4_le_and_zr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = 0;
    t_long src2 = oprd.getoval (1) & MASK_4;
    if ((src2 & SIGN_4) == SIGN_4) src2 |= ~MASK_4;
    if ((src1 <= src2) == false) {
      result.setbval (0, false);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP4_LE_OR_ZR:
  static Result exec_cmp4_le_or_zr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = 0;
    t_long src2 = oprd.getoval (1) & MASK_4;
    if ((src2 & SIGN_4) == SIGN_4) src2 |= ~MASK_4;
    if (src1 <= src2) {
      result.setbval (0, true);
      result.setbval (1, true);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP4_LE_OR_ANDCM_ZR:
  static Result exec_cmp4_le_or_andcm_zr (const Instr& inst, 
					  const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = 0;
    t_long src2 = oprd.getoval (1) & MASK_4;
    if ((src2 & SIGN_4) == SIGN_4) src2 |= ~MASK_4;
    if (src1 <= src2) {
      result.setbval (0, true);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }
    
  // A_CMP4_GE_AND_ZR:
  static Result exec_cmp4_ge_and_zr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = 0;
    t_long src2 = oprd.getoval (1) & MASK_4;
    if ((src2 & SIGN_4) == SIGN_4) src2 |= ~MASK_4;
    if ((src1 >= src2) == false) {
      result.setbval (0, false);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }
    
  // A_CMP4_GE_OR_ZR:
  static Result exec_cmp4_ge_or_zr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = 0;
    t_long src2 = oprd.getoval (1) & MASK_4;
    if ((src2 & SIGN_4) == SIGN_4) src2 |= ~MASK_4;
    if (src1 >= src2) {
      result.setbval (0, true);
      result.setbval (1, true);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }
    
  // A_CMP4_GE_OR_ANDCM_ZR:
  static Result exec_cmp4_ge_or_andcm_zr (const Instr& inst, 
					  const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = 0;
    t_long src2 = oprd.getoval (1) & MASK_4;
    if ((src2 & SIGN_4) == SIGN_4) src2 |= ~MASK_4;
    if (src1 >= src2) {
      result.setbval (0, true);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  } 
    
  // A_CMP4_LT_AND_ZR:
  static Result exec_cmp4_lt_and_zr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = 0;
    t_long src2 = oprd.getoval (1) & MASK_4;
    if ((src2 & SIGN_4) == SIGN_4) src2 |= ~MASK_4;
    if ((src1 < src2) == false) {
      result.setbval (0, false);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }
    
  // A_CMP4_LT_OR_ZR:
  static Result exec_cmp4_lt_or_zr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = 0;
    t_long src2 = oprd.getoval (1) & MASK_4;
    if ((src2 & SIGN_4) == SIGN_4) src2 |= ~MASK_4;
    if (src1 < src2) {
      result.setbval (0, true);
      result.setbval (1, true);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }
    
  // A_CMP4_LT_OR_ANDCM_ZR:
  static Result exec_cmp4_lt_or_andcm_zr (const Instr& inst, 
					  const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = 0;
    t_long src2 = oprd.getoval (1) & MASK_4;
    if ((src2 & SIGN_4) == SIGN_4) src2 |= ~MASK_4;
    if (src1 < src2) {
      result.setbval (0, true);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  } 

  // ------------------------------------------------------------------------
  // - A08 instruction group                                                -
  // ------------------------------------------------------------------------
  
  // A_CMP_LT_IR and A_CMP_LT_UNC_IR instructions
  static Result exec_cmp_lt_ir(const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // check for result clear with unc
    if ((inst.getiopc () == A_CMP_LT_UNC_IR) && (oprd.getbval (2) == false)) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = inst.getimmv (0);
    t_long src2 = oprd.getoval (1);
    if (src1 < src2) {
      result.setbval (0, true );
      result.setbval (1, false);
    } else {
      result.setbval (0, false);
      result.setbval (1, true );
    }
    return result;
  }

  // A_CMP_LTU_IR and A_CMP_LTU_UNC_IR instructions
  static Result exec_cmp_ltu_ir(const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // check for result clear with unc
    if ((inst.getiopc () == A_CMP_LTU_UNC_IR) && (oprd.getbval (2) == false)) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_octa src1 = inst.getimmv (0);
    t_octa src2 = oprd.getoval (1);
    // set result
    if (src1 < src2) {
      result.setbval (0, true );
      result.setbval (1, false);
    } else {
      result.setbval (0, false);
      result.setbval (1, true );
    }
    return result;
  }

  // A_CMP_EQ_IR and A_CMP_EQ_UNC_IR instruction
  static Result exec_cmp_eq_ir (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // check for result clear with unc
    if ((inst.getiopc () == A_CMP_EQ_UNC_IR) && (oprd.getbval (2) == false)) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_octa src1 = inst.getimmv (0);
    t_octa src2 = oprd.getoval (1);
    if (src1 == src2) {
      result.setbval (0, true );
      result.setbval (1, false);
    } else {
      result.setbval (0, false);
      result.setbval (1, true );
    }
    return result;
  }

  // A_CMP_EQ_AND_IR:
  static Result  exec_cmp_eq_and_ir (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_octa src1 = inst.getimmv (0);
    t_octa src2 = oprd.getoval (1);
    if ((src1 == src2) == false) {
      result.setbval (0, false);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP_EQ_OR_IR:
  static Result  exec_cmp_eq_or_ir (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_octa src1 = inst.getimmv (0);
    t_octa src2 = oprd.getoval (1);
    if (src1 == src2) {
      result.setbval (0, true);
      result.setbval (1, true);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP_EQ_OR_ANDCM_IR:
  static Result  exec_cmp_eq_or_andcm_ir (const Instr& inst, 
					  const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_octa src1 = inst.getimmv (0);
    t_octa src2 = oprd.getoval (1);
    if (src1 == src2) {
      result.setbval (0, true);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP_NE_AND_IR:
  static Result  exec_cmp_ne_and_ir (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_octa src1 = inst.getimmv (0);
    t_octa src2 = oprd.getoval (1);
    if ((src1 != src2) == false) {
      result.setbval (0, false);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP_NE_OR_IR:
  static Result  exec_cmp_ne_or_ir (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_octa src1 = inst.getimmv (0);
    t_octa src2 = oprd.getoval (1);
    if (src1 != src2) {
      result.setbval (0, true);
      result.setbval (1, true);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP_NE_OR_ANDCM_IR:
  static Result  exec_cmp_ne_or_andcm_ir (const Instr& inst, 
					  const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_octa src1 = inst.getimmv (0);
    t_octa src2 = oprd.getoval (1);
    if (src1 != src2) {
      result.setbval (0, true);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP4_LT_IR and A_CMP4_LT_UNC_IR instructions
  static Result exec_cmp4_lt_ir (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // check for result clear with unc
    if ((inst.getiopc () == A_CMP4_LT_UNC_IR) && (oprd.getbval (2) == false)) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_long src1 = inst.getimmv (0);
    t_long src2 = oprd.getoval (1) & MASK_4;
    if ((src2 & SIGN_4) == SIGN_4) src2 |= ~MASK_4;
    if (src1 < src2) {
      result.setbval (0, true );
      result.setbval (1, false);
    } else {
      result.setbval (0, false);
      result.setbval (1, true );
    }
    return result;
  }

  // A_CMP4_LTU_IR and A_CMP4_LTU_UNC_IR instructions
  static Result exec_cmp4_ltu_ir(const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // check for result clear with unc
    if ((inst.getiopc () == A_CMP4_LTU_UNC_IR) && 
	(oprd.getbval (2) == false)) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_quad src1 = inst.getimmv (0);
    t_quad src2 = oprd.getoval (1);
    if (src1 < src2) {
      result.setbval (0, true );
      result.setbval (1, false);
    } else {
      result.setbval (0, false);
      result.setbval (1, true );
    }
    return result;
  }

  // A_CMP4_EQ_IR and A_CMP4_EQ_UNC_IR instruction
  static Result exec_cmp4_eq_ir (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // check for result clear with unc
    if ((inst.getiopc () == A_CMP4_EQ_UNC_IR) && (oprd.getbval (2) == false)) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_quad src1 = inst.getimmv (0);
    t_quad src2 = oprd.getoval (1);
    if (src1 == src2){
      result.setbval (0, true );
      result.setbval (1, false);
    } else {
      result.setbval (0, false);
      result.setbval (1, true );
    }
    return result;
  }

  // A_CMP4_EQ_AND_IR:
  static Result  exec_cmp4_eq_and_ir (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_quad src1 = inst.getimmv (0);
    t_quad src2 = oprd.getoval (1);
    if ((src1 == src2) == false) {
      result.setbval (0, false);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP4_EQ_OR_IR:
  static Result  exec_cmp4_eq_or_ir (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_quad src1 = inst.getimmv (0);
    t_quad src2 = oprd.getoval (1);
    if (src1 == src2) {
      result.setbval (0, true);
      result.setbval (1, true);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP4_EQ_OR_ANDCM_IR:
  static Result  exec_cmp4_eq_or_andcm_ir (const Instr& inst, const 
					   Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_quad src1 = inst.getimmv (0);
    t_quad src2 = oprd.getoval (1);
    if (src1 == src2) {
      result.setbval (0, true);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP4_NE_AND_IR:
  static Result  exec_cmp4_ne_and_ir (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_quad src1 = inst.getimmv (0);
    t_quad src2 = oprd.getoval (1);
    if ((src1 != src2) == false) {
      result.setbval (0, false);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP4_NE_OR_IR:
  static Result  exec_cmp4_ne_or_ir (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_quad src1 = inst.getimmv (0);
    t_quad src2 = oprd.getoval (1);
    if (src1 != src2) {
      result.setbval (0, true);
      result.setbval (1, true);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // A_CMP4_NE_OR_ANDCM_IR:
  static Result  exec_cmp4_ne_or_andcm_ir (const Instr& inst, 
					   const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // compute nat bit
    bool natr = oprd.getbval (1);
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compare and set result
    t_quad src1 = inst.getimmv (0);
    t_quad src2 = oprd.getoval (1);
    if (src1 != src2) {
      result.setbval (0, true);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // ------------------------------------------------------------------------
  // - A09 instruction group                                                -
  // ------------------------------------------------------------------------

  // A_PADD1 :
  static Result  exec_padd1 (const Instr& inst, 
			     const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_byte bval[8];
    } src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i = 0; i < 8; i++) 
      res.bval[i] = src1.bval[i] + src2.bval[i];
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, false);
    return result;
  }

  // A_PADD2:
  static Result exec_padd2 (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_word wval[4];
    } src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i = 0; i < 4; i++) 
      res.wval[i] = src1.wval[i] + src2.wval[i];
    // set result
    result.setoval(0, res.oval);
    result.setbval (0, false);
    return result;
  }

  // A_PADD4:
  static Result exec_padd4 (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_quad qval[2];
    } src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i = 0; i < 2; i++) 
      res.qval[i] = src1.qval[i] + src2.qval[i];
    // set result
    result.setoval(0, res.oval);
    result.setbval (0, false);
    return result;
  }

  // A_PADD1_SSS:
  static Result exec_padd1_sss (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_byte bval[8];
    } src1, src2, res;
    const t_byte bmin = 0x80;
    const t_byte bmax = 0x7F;
    const t_long lmin = sign_extend (bmin);
    const t_long lmax = sign_extend (bmax);
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i=0; i < 8; i++) {
      t_long tmpres = sign_extend (src2.bval[i]) + sign_extend (src1.bval[i]);
      if (tmpres > lmax) res.bval[i] = bmax;
      else if (tmpres < lmin) res.bval[i] = bmin;
      else res.bval[i] = tmpres;
    }
    // set result
    result.setoval(0, res.oval);
    result.setbval (0, false);
    return result;
  }

  // A_PADD2_SSS:
  static Result  exec_padd2_sss (const Instr& inst, 
				 const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    result.setbval (0, false);
    // compute result
    union{
      t_octa oval;
      t_word wval[4];
    } src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    const t_word wmin = 0x8000LL;
    const t_word wmax = 0x7FFFLL;
    const t_long lmin = sign_extend (wmin);
    const t_long lmax = sign_extend (wmax);
    for (long i=0; i < 4; i++){
      t_long tmpres = sign_extend (src2.wval[i]) + sign_extend (src1.wval[i]);
      if (tmpres > lmax) res.wval[i] = wmax;
      else if(tmpres < lmin) res.wval[i] = wmin;
      else res.wval[i] = tmpres;
    }
    // set result
    result.setbval (0, false);
    result.setoval (0, res.oval);
    return result;
  }

  // A_PADD1_UUU:
  static Result  exec_padd1_uuu (const Instr& inst, 
				 const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_byte bval[8];
    } src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i=0; i < 8; i++){
      t_octa tmpres =  src1.bval[i] + src2.bval[i];
      if (tmpres > (t_octa) BYTE_A) res.bval[i] = BYTE_A;
      else res.bval[i] = tmpres;
    }
    // set result
    result.setbval (0, false);
    result.setoval (0, res.oval);
    return result;
  }

  // A_PADD2_UUU:
  static Result  exec_padd2_uuu (const Instr& inst, 
			       const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_word wval[4];
    }src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    t_octa max = 0x000000000000FFFFLL;
    for (long i=0; i < 4; i++){
      t_octa tmpres  = src1.wval[i] + src2.wval[i];
      if (tmpres > max) res.wval[i] = max;
      else res.wval[i] = tmpres;
    }
    // set result
    result.setbval (0, false);
    result.setoval (0, res.oval);
    return result;
  }

  // A_PADD1_UUS:
  static Result  exec_padd1_uus (const Instr& inst, 
				 const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_byte bval[8];
    } src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i=0; i < 8; i++){
      t_long tmpres = src1.bval[i] + sign_extend (src2.bval[i]);
      if (tmpres > (t_long) BYTE_A) res.bval[i] = BYTE_A;
      else if (get_sign (tmpres) == true) res.bval[i] = BYTE_0;
      else res.bval[i] = tmpres;
    }
    // set result
    result.setbval (0, false);
    result.setoval (0, res.oval);
    return result;
  }

  // A_PADD2_UUS:
  static Result  exec_padd2_uus (const Instr& inst, 
				 const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_word wval[4];
    }src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i=0; i < 4; i++){
      t_long tmpres = src1.wval[i] + sign_extend (src2.wval[i]);
      if (tmpres > (t_long) WORD_A) res.wval[i] = WORD_A;
      else if(get_sign (tmpres) == true) res.wval[i] = WORD_0;
      else res.wval[i] = tmpres;
    }
    // set result
    result.setbval (0, false);
    result.setoval (0, res.oval);
    return result;
  }

  // A_PSUB1:
  static Result  exec_psub1 (const Instr& inst, 
			     const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_byte bval[8];
    }src1, src2, res;
    src1.oval =  oprd.getoval (0);
    src2.oval =  oprd.getoval (1);
    for (long i=0; i <8; i++) res.bval[i] = src1.bval[i] - src2.bval[i];
    // set result
    result.setbval (0, false);
    result.setoval (0, res.oval);
    return result;
  }

  // A_PSUB2:
  static Result  exec_psub2 (const Instr& inst, 
			     const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_word wval[4];
    }src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i=0; i < 4; i++) 
      res.wval[i] = src1.wval[i] - src2.wval[i];
    // set result
    result.setbval (0, false);
    result.setoval (0, res.oval); 
    return result;
  }

  // A_PSUB4:
  static Result  exec_psub4 (const Instr& inst, 
			     const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    result.setbval (0, false);
    // compute result
    union{
      t_octa oval;
      t_quad qval[2];
    } src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i = 0; i < 2; i++) 
      res.qval[i] = src1.qval[i] - src2.qval[i];
    // set result
    result.setbval (0, false);
    result.setoval (0, res.oval);
    return result;
  }

  // A_PSUB1_SSS:
  static Result  exec_psub1_sss (const Instr& inst, 
				 const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_byte bval[8];
    } src1, src2, res;
    const t_byte bmin = 0x80;
    const t_byte bmax = 0x7f;
    const t_long lmin = sign_extend (bmin);
    const t_long lmax = sign_extend (bmax);
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i = 0; i < 8; i++){
      t_long tmpres = sign_extend (src1.bval[i]) - sign_extend (src2.bval[i]);
      if (tmpres > lmax) res.bval[i] = bmax;
      else if(tmpres < lmin) res.bval[i] = bmin;
      else res.bval[i] = tmpres;
    }
    // set result
    result.setbval (0, false);
    result.setoval (0, res.oval);
    return result;
  }

  // A_PSUB2_SSS:
  static Result  exec_psub2_sss (const Instr& inst, 
				 const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
   union{
      t_octa oval;
      t_word wval[4];
    } src1, src2, res;
    const t_word wmin = 0x8000;
    const t_word wmax = 0x7FFF;
    const t_long lmin = sign_extend (wmin);
    const t_long lmax = sign_extend (wmax);
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i = 0; i < 4; i++){
      t_long tmpres = sign_extend (src1.wval[i]) - sign_extend (src2.wval[i]);
      if (tmpres > lmax) res.wval[i] = wmax;
      else if(tmpres < lmin) res.wval[i] = wmin;
      else res.wval[i] = tmpres;
    }
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, false);
    return result;
  }

  // A_PSUB1_UUU:
  static Result  exec_psub1_uuu (const Instr& inst, 
				 const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_byte bval[8];
    } src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i=0; i < 8; i++){
      t_long tmpres = src1.bval[i] - src2.bval[i];
      if (get_sign (tmpres) == true) res.bval[i] = BYTE_0;
      else res.bval[i] = tmpres;
    }
    // set result
    result.setbval (0, false);
    result.setoval (0, res.oval);
    return result;
  }

  // A_PSUB2_UUU:
  static Result  exec_psub2_uuu (const Instr& inst, 
				 const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_word wval[4];
    } src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i=0; i < 4; i++){
      t_long tmpres = src1.wval[i] - src2.wval[i];
      if (get_sign (tmpres) == true) res.wval[i] = WORD_0;
      else res.wval[i] = tmpres;
    }
    // set result
    result.setbval (0, false);
    result.setoval (0, res.oval);
    return result;
  }

  // A_PSUB1_UUS:
  static Result  exec_psub1_uus (const Instr& inst, 
				 const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_byte bval[8];
    } src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i=0; i < 8; i++){
      t_long tmpres = src1.bval[i] - sign_extend (src2.bval[i]);
      if (tmpres > (t_long) BYTE_A) res.bval[i] = BYTE_A;
      else if (get_sign (tmpres) == true) res.bval[i] = BYTE_0;
      else res.bval[i] = tmpres;
    }
    // set result
    result.setbval (0, false);
    result.setoval (0, res.oval);
    return result;
  }

  // A_PSUB2_UUS:
  static Result  exec_psub2_uus (const Instr& inst, 
				 const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_word wval[4];
    } src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i=0; i < 4; i++){
      t_long tmpres = src1.wval[i] - sign_extend (src2.wval[i]);
      if (tmpres > (t_long) WORD_A) res.wval[i] = WORD_A;
      else if(get_sign (tmpres) == true) res.wval[i] = WORD_0;
      else res.wval[i] = tmpres;
    }
    // set result
    result.setbval (0, false);
    result.setoval (0, res.oval);
    return result;
  }

  // A_PAVG1:
  static Result  exec_pavg1 (const Instr& inst, 
			     const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_byte bval[8];
    }src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i=0; i < 8; i++){
      t_word tmpres = src1.bval[i] + src2.bval[i];
      if ((tmpres & WORD_1) == WORD_1) res.bval[i] = (tmpres >> 1) | WORD_1;
      else res.bval[i] = tmpres >> 1;
    }
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, false);
    return result;
  }

  // A_PAVG2:
  static Result  exec_pavg2 (const Instr& inst, 
			     const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_word wval[4];
    } src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i = 0; i < 4; i++){
      t_quad tmpres = src1.wval[i] + src2.wval[i];
      if ((tmpres & QUAD_1) == QUAD_1) 
	res.wval[i] = (tmpres >> 1) | QUAD_1;
      else 
	res.wval[i] = tmpres >> 1;
    }
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, false);
    return result;
  }

  // A_PAVG1_RAZ:
  static Result  exec_pavg1_raz (const Instr& inst, 
				 const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_byte bval[8];
    } src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i=0; i < 8; i++){
      t_word tmpres = src1.bval[i] + src2.bval[i]  + 1;
      res.bval[i] = tmpres >> 1;
    }
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, false);
    return result;
  }

  // A_PAVG2_RAZ:
  static Result  exec_pavg2_raz (const Instr& inst, 
				 const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_word wval[4];
    } src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i=0; i < 4; i++){
      t_quad tmpres = src1.wval[i] + src2.wval[i] + 1;
      res.wval[i] = tmpres >> 1;
    }
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, false);
    return result;
  }

  // A_PAVGSUB1:
  static Result  exec_pavgsub1 (const Instr& inst, 
				const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_byte bval[8];
    } src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i=0; i < 8; i++){
      t_word tmpres = src1.bval[i] - src2.bval[i];
      if ((tmpres & WORD_1) == WORD_1) res.bval[i] = (tmpres >> 1) | WORD_1;
      else res.bval[i] = tmpres >> 1;
    }
    // set result
    result.setbval (0, false);
    result.setoval (0, res.oval);
    return result;
  }

  // A_PAVGSUB2:
  static Result  exec_pavgsub2 (const Instr& inst, 
				const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_word wval[4];
    } src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i=0; i < 4; i++){
      t_quad tmpres = src1.wval[i] - src2.wval[i];
      if ((tmpres & QUAD_1) == QUAD_1) res.wval[i] = (tmpres >> 1) | QUAD_1;
      else res.wval[i] = tmpres >> 1;
    }
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, false);
    return result;
  }

  // A_PCMP1_EQ:
  static Result  exec_pcmp1_eq (const Instr& inst, 
				const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_byte bval[8];
    } src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i=0; i < 8; i++)
      if (src1.bval[i] == src2.bval[i]) res.bval[i]=0xFF;
      else res.bval[i] = 0x00;
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, false);
    return result;
  }

  // A_PCMP2_EQ:
  static Result  exec_pcmp2_eq (const Instr& inst, 
				const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_word wval[4];
    } src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i=0; i < 4; i++)
      if (src1.wval[i] == src2.wval[i]) res.wval[i]=0xFFFF;
      else res.wval[i] = 0x0000;
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, false);
    return result;
  }

  // A_PCMP4_EQ:
  static Result  exec_pcmp4_eq (const Instr& inst, 
				const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_quad qval[2];
    } src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i=0; i < 2; i++)
      if (src1.qval[i] == src2.qval[i]) res.qval[i]=0xFFFFFFFF;
      else res.qval[i] = 0x00000000;
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, false);
    return result;
  }

  // A_PCMP1_GT:
  static Result  exec_pcmp1_gt (const Instr& inst, 
				const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_byte bval[8];
    } src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i=0; i < 8; i++)
      if (sign_extend (src1.bval[i]) > sign_extend (src2.bval[i]))
	res.bval[i]=0xFF; 
      else res.bval[i] = 0x00;
    // set result
    result.setbval (0, false);
    result.setoval (0, res.oval);
    return result;
  }

  // A_PCMP2_GT:
  static Result  exec_pcmp2_gt (const Instr& inst, 
				const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_word wval[4];
    } src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i=0; i < 4; i++)
      if (sign_extend (src1.wval[i]) > sign_extend (src2.wval[i]))
	res.wval[i]= WORD_A;
      else res.wval[i] = WORD_0;
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, false);
    return result;
  }

  // A_PCMP4_GT:
  static Result  exec_pcmp4_gt (const Instr& inst, 
				const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_quad qval[2];
    } src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i=0; i < 2; i++)
      if (sign_extend (src1.qval[i]) > sign_extend(src2.qval[i]))
	res.qval[i]= QUAD_A;
      else res.qval[i] = QUAD_0;
    // set result
    result.setbval (0, false);
    result.setoval (0, res.oval);
    return result;
  }

  // ------------------------------------------------------------------------
  // - A10 instruction group                                                -
  // ------------------------------------------------------------------------

  // A_PSHLADD2:
  static Result  exec_pshladd2 (const Instr& inst, 
				const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_word wval[4];
    } src1, src2, res;
    src1.oval  = oprd.getoval (0);
    src2.oval  = oprd.getoval (1);
    t_octa shc = inst.getimmv (0);
    const t_word wmin = 0x8000LL;
    const t_word wmax = 0x7FFFLL;
    const t_long lmin = sign_extend (wmin);
    const t_long lmax = sign_extend (wmax);
    for (long i = 0; i < 4; i++){
      t_long tmpres = sign_extend (src1.wval[i]) << shc;
      if (tmpres > lmax) res.wval[i] = wmax;
      else if (tmpres < lmin) res.wval[i] = wmin;
      else {
	tmpres += sign_extend (src2.wval[i]);
	if (tmpres > lmax) res.wval[i] = wmax;
	else if(tmpres < lmin) res.wval[i] = wmin;
	else res.wval[i] = tmpres;
      }
    }
    // set result
    result.setbval (0, false);
    result.setoval (0, res.oval);
    return result;
  }

  // A_PSHRADD2:
  static Result  exec_pshradd2 (const Instr& inst, 
				const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    if (natr == true) {
      result.setbval (0, true);
      return result;
    }
    // compute result
    union{
      t_octa oval;
      t_word wval[4];
    } src1, src2, res;
    src1.oval  = oprd.getoval (0);
    src2.oval  = oprd.getoval (1);
    t_octa shc = inst.getimmv (0);
    const t_word wmin = 0x8000LL;
    const t_word wmax = 0x7FFFLL;
    const t_long lmin = sign_extend (wmin);
    const t_long lmax = sign_extend (wmax);
    for (long i = 0; i < 4; i++){
      t_long tmpres = sign_extend (src1.wval[i]) >> shc;
      tmpres += sign_extend (src2.wval[i]);
      if (tmpres > lmax) res.wval[i] = wmax;
      else if(tmpres < lmin) res.wval[i] = wmin;
      else res.wval[i] = tmpres;
    }
    // set result
    result.setbval (0, false);
    result.setoval (0, res.oval);
    return result;
  }

  // execute a A unit instruction with operands

  Result Aexecute::exec (const Instr& inst, const Operand& oprd) const {
    Result result;
    if (inst.isvalid () == false) return result;
    // get the instruction opcode
    t_iopc iopc = inst.getiopc ();
    switch (iopc) {

      // A01 instruction group
    case A_ADD:
      result = exec_add (inst, oprd);
      break;
    case A_ADD_1:
      result = exec_add_1 (inst, oprd);
      break;
    case A_SUB:
      result = exec_sub (inst, oprd);
      break;
    case A_SUB_1:
      result = exec_sub_1 (inst, oprd);
      break;
    case A_ADDP4:
      result = exec_addp4 (inst, oprd);
      break;
    case A_AND:
      result = exec_and (inst, oprd);
      break;
    case A_ANDCM:
      result = exec_andcm (inst, oprd);
      break;
    case A_OR:
      result = exec_or (inst, oprd);
      break;
    case A_XOR:
      result = exec_xor (inst, oprd);
      break;

      // A02 instruction group
    case A_SHLADD:
       result = exec_shladd (inst, oprd);
       break;
    case A_SHLADDP4:
      result = exec_shladdp4 (inst, oprd);
      break;

      // A03 instruction group
    case A_SUB_I:
      result = exec_sub_i (inst, oprd);
      break;
    case A_AND_I:
      result = exec_and_i (inst, oprd);
      break;
    case A_ANDCM_I:
      result = exec_andcm_i (inst, oprd);
      break;
    case A_OR_I:
      result = exec_or_i (inst, oprd);
      break;
    case A_XOR_I:
      result = exec_xor_i (inst, oprd);
      break;

      // A04 instruction group
    case A_ADDS_I:
      result = exec_adds_i  (inst, oprd);
      break;
    case A_ADDP4_I:
      result = exec_addp4_i (inst, oprd);
      break;

      // A05 instruction group
    case A_ADDL_I:
      result = exec_addl_i (inst, oprd);
      break;

      // A06 instruction group
    case A_CMP_LT_RR:
    case A_CMP_LT_UNC_RR:
      result = exec_cmp_lt_rr (inst, oprd);
      break;
    case A_CMP_LTU_RR:
    case A_CMP_LTU_UNC_RR:
      result = exec_cmp_ltu_rr (inst, oprd);
      break;
    case A_CMP_EQ_RR:
    case A_CMP_EQ_UNC_RR:
      result = exec_cmp_eq_rr (inst, oprd);
      break;
    case A_CMP_EQ_AND_RR:
      result = exec_cmp_eq_and_rr (inst, oprd);
      break;
    case A_CMP_EQ_OR_RR:
      result = exec_cmp_eq_or_rr (inst, oprd);
      break;
    case A_CMP_EQ_OR_ANDCM_RR:
      result = exec_cmp_eq_or_andcm_rr (inst, oprd);
      break;
    case A_CMP_NE_AND_RR:
      result = exec_cmp_ne_and_rr (inst, oprd);
      break;
    case A_CMP_NE_OR_RR:
      result = exec_cmp_ne_or_rr (inst, oprd);
      break;
    case A_CMP_NE_OR_ANDCM_RR:
      result = exec_cmp_ne_or_andcm_rr (inst, oprd);
      break;
    case A_CMP4_LT_RR:
    case A_CMP4_LT_UNC_RR:
      result = exec_cmp4_lt_rr (inst, oprd);
      break;
    case A_CMP4_LTU_RR:
    case A_CMP4_LTU_UNC_RR:
      result = exec_cmp4_ltu_rr (inst, oprd);
      break;
    case A_CMP4_EQ_RR:
    case A_CMP4_EQ_UNC_RR:
      result = exec_cmp4_eq_rr (inst, oprd);
      break;
    case A_CMP4_EQ_AND_RR:
      result = exec_cmp4_eq_and_rr (inst, oprd);
      break;
    case A_CMP4_EQ_OR_RR:
      result = exec_cmp4_eq_or_rr (inst, oprd);
      break;
    case A_CMP4_EQ_OR_ANDCM_RR:
      result = exec_cmp4_eq_or_andcm_rr (inst, oprd);
      break;
    case A_CMP4_NE_AND_RR:
      result = exec_cmp4_ne_and_rr (inst, oprd);
      break;
    case A_CMP4_NE_OR_RR:
      result = exec_cmp4_ne_or_rr (inst, oprd);
      break;
    case A_CMP4_NE_OR_ANDCM_RR:
      result = exec_cmp4_ne_or_andcm_rr (inst, oprd);
      break;

      // A07 instruction group
    case A_CMP_GT_AND_ZR:
      result = exec_cmp_gt_and_zr (inst, oprd);
      break;
    case A_CMP_GT_OR_ZR:
      result = exec_cmp_gt_or_zr (inst, oprd);
      break;
    case A_CMP_GT_OR_ANDCM_ZR:
      result = exec_cmp_gt_or_andcm_zr (inst, oprd);
      break;
    case A_CMP_LE_AND_ZR:
      result = exec_cmp_le_and_zr (inst, oprd);
      break;
    case A_CMP_LE_OR_ZR:
      result = exec_cmp_le_or_zr (inst, oprd);
      break;
    case A_CMP_LE_OR_ANDCM_ZR:
      result = exec_cmp_le_or_andcm_zr (inst, oprd);
      break;
    case A_CMP_GE_AND_ZR:
      result = exec_cmp_ge_and_zr (inst, oprd);
      break;
    case A_CMP_GE_OR_ZR:
      result = exec_cmp_ge_or_zr (inst, oprd);
      break;
    case A_CMP_GE_OR_ANDCM_ZR:
      result = exec_cmp_ge_or_andcm_zr (inst, oprd);
      break;
    case A_CMP_LT_AND_ZR:
      result = exec_cmp_lt_and_zr (inst, oprd);
      break;
    case A_CMP_LT_OR_ZR:
      result = exec_cmp_lt_or_zr (inst, oprd);
      break;
    case A_CMP_LT_OR_ANDCM_ZR:
      result = exec_cmp_lt_or_andcm_zr (inst, oprd);
      break;
    case A_CMP4_GT_AND_ZR:
      result = exec_cmp4_gt_and_zr (inst, oprd);
      break;
    case A_CMP4_GT_OR_ZR:
      result = exec_cmp4_gt_or_zr (inst, oprd);
      break;
    case A_CMP4_GT_OR_ANDCM_ZR:
      result = exec_cmp4_gt_or_andcm_zr (inst, oprd);
      break;
    case A_CMP4_LE_AND_ZR:
      result = exec_cmp4_le_and_zr (inst, oprd);
      break;
    case A_CMP4_LE_OR_ZR:
      result = exec_cmp4_le_or_zr (inst, oprd);
      break;
    case A_CMP4_LE_OR_ANDCM_ZR:
      result = exec_cmp4_le_or_andcm_zr (inst, oprd);
      break;
    case A_CMP4_GE_AND_ZR:
      result = exec_cmp4_ge_and_zr (inst, oprd);
      break;
    case A_CMP4_GE_OR_ZR:
      result = exec_cmp4_ge_or_zr (inst, oprd);
      break;
    case A_CMP4_GE_OR_ANDCM_ZR:
      result = exec_cmp4_ge_or_andcm_zr (inst, oprd);
      break;
    case A_CMP4_LT_AND_ZR:
      result = exec_cmp4_lt_and_zr (inst, oprd);
      break;
    case A_CMP4_LT_OR_ZR:
      result = exec_cmp4_lt_or_zr (inst, oprd);
      break;
    case A_CMP4_LT_OR_ANDCM_ZR:
      result = exec_cmp4_lt_or_andcm_zr (inst, oprd);
      break;

      // A08 instruction
    case A_CMP_LT_IR:
    case A_CMP_LT_UNC_IR:
      result = exec_cmp_lt_ir (inst, oprd);
      break;
    case A_CMP_LTU_IR:
    case A_CMP_LTU_UNC_IR:
      result = exec_cmp_ltu_ir (inst, oprd);
      break;
    case A_CMP_EQ_IR:
    case A_CMP_EQ_UNC_IR:
      result = exec_cmp_eq_ir (inst, oprd);
      break;
    case A_CMP_EQ_AND_IR:
      result = exec_cmp_eq_and_ir (inst, oprd);
      break;
    case A_CMP_EQ_OR_IR:
      result = exec_cmp_eq_or_ir (inst, oprd);
      break;
    case A_CMP_EQ_OR_ANDCM_IR:
      result = exec_cmp_eq_or_andcm_ir (inst, oprd);
      break;
    case A_CMP_NE_AND_IR:
      result = exec_cmp_ne_and_ir (inst, oprd);
      break;
    case A_CMP_NE_OR_IR:
      result = exec_cmp_ne_or_ir (inst, oprd);
      break;
    case A_CMP_NE_OR_ANDCM_IR:
      result = exec_cmp_ne_or_andcm_ir (inst, oprd);
      break;
    case A_CMP4_LT_IR:
    case A_CMP4_LT_UNC_IR:
      result = exec_cmp4_lt_ir (inst, oprd);
      break;
    case A_CMP4_LTU_IR:
    case A_CMP4_LTU_UNC_IR:
      result = exec_cmp4_ltu_ir (inst, oprd);
      break;
    case A_CMP4_EQ_IR:
    case A_CMP4_EQ_UNC_IR:
      result = exec_cmp4_eq_ir (inst, oprd);
      break;
    case A_CMP4_EQ_AND_IR:
      result = exec_cmp4_eq_and_ir (inst, oprd);
      break;
    case A_CMP4_EQ_OR_IR:
      result = exec_cmp4_eq_or_ir (inst, oprd);
      break;
    case A_CMP4_EQ_OR_ANDCM_IR:
      result = exec_cmp4_eq_or_andcm_ir (inst, oprd);
      break;
    case A_CMP4_NE_AND_IR:
      result = exec_cmp4_ne_and_ir (inst, oprd);
      break;
    case A_CMP4_NE_OR_IR:
      result = exec_cmp4_ne_or_ir (inst, oprd);
      break;
    case A_CMP4_NE_OR_ANDCM_IR:
      result = exec_cmp4_ne_or_andcm_ir (inst, oprd);
      break;

      // A09 instruction group
    case A_PADD1:
      result = exec_padd1 (inst, oprd);
      break;
    case A_PADD2:
      result = exec_padd2 (inst, oprd);
      break;
    case A_PADD4:
      result = exec_padd4 (inst, oprd);
      break;
    case A_PADD1_SSS:
      result = exec_padd1_sss (inst, oprd);
      break;
    case A_PADD2_SSS:
      result = exec_padd2_sss (inst, oprd);
      break;
    case A_PADD1_UUU:
      result = exec_padd1_uuu (inst, oprd);
      break;
    case A_PADD2_UUU:
      result = exec_padd2_uuu (inst, oprd);
      break;
    case A_PADD1_UUS:
      result = exec_padd1_uus (inst, oprd);
      break;
    case A_PADD2_UUS:
      result = exec_padd2_uus (inst, oprd);
      break;
    case A_PSUB1:
      result = exec_psub1 (inst, oprd);
      break;
    case A_PSUB2:
      result = exec_psub2 (inst, oprd);
      break;
    case A_PSUB4:
      result = exec_psub4 (inst, oprd);
      break;
    case A_PSUB1_SSS:
      result = exec_psub1_sss (inst, oprd);
      break;
    case A_PSUB2_SSS:
      result = exec_psub2_sss (inst, oprd);
      break;
    case A_PSUB1_UUU:
      result = exec_psub1_uuu (inst, oprd);
      break;
    case A_PSUB2_UUU:
      result = exec_psub2_uuu (inst, oprd);
      break;
    case A_PSUB1_UUS:
      result = exec_psub1_uus (inst, oprd);
      break;
    case A_PSUB2_UUS:
      result = exec_psub2_uus (inst, oprd);
      break;
    case A_PAVG1:
      result = exec_pavg1 (inst, oprd);
      break;
    case A_PAVG2:
      result = exec_pavg2 (inst, oprd);
      break;
    case A_PAVG1_RAZ:
      result = exec_pavg1_raz (inst, oprd);
      break;
    case A_PAVG2_RAZ:
      result = exec_pavg2_raz (inst, oprd);
      break;
    case A_PAVGSUB1:
      result = exec_pavgsub1 (inst, oprd);
      break;
    case A_PAVGSUB2:
      result = exec_pavgsub2 (inst, oprd);
      break;
    case A_PCMP1_EQ:
      result = exec_pcmp1_eq (inst, oprd);
      break;
    case A_PCMP2_EQ:
      result = exec_pcmp2_eq (inst, oprd);
      break;
    case A_PCMP4_EQ:
      result = exec_pcmp4_eq (inst, oprd);
      break;
    case A_PCMP1_GT:
      result = exec_pcmp1_gt (inst, oprd);
      break;
    case A_PCMP2_GT:
      result = exec_pcmp2_gt (inst, oprd);
      break;
    case A_PCMP4_GT:
      result = exec_pcmp4_gt (inst, oprd);
      break;

      // A10 instructrion group
    case A_PSHLADD2:
      result = exec_pshladd2 (inst, oprd);
      break;
    case A_PSHRADD2:
      result = exec_pshradd2 (inst, oprd);
      break;
    default:
      break;
    }
    return result;
  }
}
