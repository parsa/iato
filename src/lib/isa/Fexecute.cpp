// ---------------------------------------------------------------------------
// - Fexecute.cpp                                                            -
// - iato:isa library - F execution unit class implementation                -
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
#include "Fpsr.hpp"
#include "Utils.hpp"
#include "Fexecute.hpp"
#include "Exception.hpp"
#include "Interrupt.hpp"

namespace iato {

  static inline bool fclass_ispos (const t_octa value) {
    return ((value & 0x0000000000000001ULL) == 0x0000000000000001ULL);
  }

  static inline bool fclass_isneg (const t_octa value) {
    return ((value & 0x0000000000000002ULL) == 0x0000000000000002ULL);
  }

  static inline bool fclass_iszero (const t_octa value) {
    return ((value & 0x0000000000000004ULL) == 0x0000000000000004ULL);
  }

  static inline bool fclass_isunorm (const t_octa value) {
    return ((value & 0x0000000000000008ULL) == 0x0000000000000008ULL);
  }

  static inline bool fclass_isnorm (const t_octa value) {
    return ((value & 0x0000000000000010ULL) == 0x0000000000000010ULL);
  }

  static inline bool fclass_isinf (const t_octa value) {
    return ((value & 0x0000000000000020ULL) == 0x0000000000000020ULL);
  }

  static inline bool fclass_issnan (const t_octa value) {
    return ((value & 0x0000000000000040ULL) == 0x0000000000000040ULL);
  }

  static inline bool fclass_isqnan (const t_octa value) {
    return ((value & 0x0000000000000080ULL) == 0x0000000000000080ULL);
  }

  static inline bool fclass_isnat (const t_octa value) {
    return ((value & 0x0000000000000100ULL) == 0x0000000000000100ULL);
  }

  static inline Fpsr::t_mfield tofpcomp (t_fpcomp value) {
    Fpsr::t_mfield result = Fpsr::SF0;
    switch (value) {
    case S0:
      result = Fpsr::SF0;
      break;
    case S1:
      result = Fpsr::SF1;
      break;
    case S2:
      result = Fpsr::SF2;
      break;
    case S3:
      result = Fpsr::SF3;
      break;
    default:
      assert (false);
      break;
    }
    return result;
  }

  // ------------------------------------------------------------------------
  // - F01 instruction group                                                 -
  // ------------------------------------------------------------------------

  // F_FMA
  static Result exec_fma (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f0 = oprd.getrval (0);
    t_real f1 = oprd.getrval (1);
    t_real f2 = oprd.getrval (2);
    bool natr = (f0.isnat () || f1.isnat () || f2.isnat ());
    // set to natval if nat bit is set
    t_real fr;
    if (natr == true) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute value
    if (oprd.getrid (0). getlnum () != 0) fr = (f1 * f2) + f0;
    else fr = f1 * f2;
    Fpsr fpsr = oprd.getoval (3);
    fpsr.convert (NONEPC, tofpcomp (inst.getfpcomp ()), fr);
    result.setrval (0, fr);
    return result;
  }

  // F_FMA_S
  static Result exec_fma_s (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f0 = oprd.getrval (0);
    t_real f1 = oprd.getrval (1);
    t_real f2 = oprd.getrval (2);
    bool natr = (f0.isnat () || f1.isnat () || f2.isnat ());
    // set to natval if nat bit is set
    t_real fr;
    if (natr == true) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute value
    fr = (f1 * f2) + f0;
    Fpsr fpsr = oprd.getoval (3);
    fpsr.convert (S, tofpcomp (inst.getfpcomp ()), fr);
    result.setrval (0, fr);
    return result;
  }

  // F_FMA_D
  static Result exec_fma_d (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f0 = oprd.getrval (0);
    t_real f1 = oprd.getrval (1);
    t_real f2 = oprd.getrval (2);
    bool natr = (f0.isnat () || f1.isnat () || f2.isnat ());
    // set to natval if nat bit is set
    t_real fr;
    if (natr == true) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute value
    fr = (f1 * f2) + f0;
    Fpsr fpsr = oprd.getoval (3);
    fpsr.convert (D, tofpcomp (inst.getfpcomp ()), fr);
    result.setrval (0, fr);
    return result;
  }

  // F_FNMA
  static Result exec_fnma (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f0 = oprd.getrval (0);
    t_real f1 = oprd.getrval (1);
    t_real f2 = oprd.getrval (2);
    bool natr = (f0.isnat () || f1.isnat () || f2.isnat ());
    // set to natval if nat bit is set
    t_real fr;
    if (natr == true) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute value
    fr = f0 - (f1 * f2);
    Fpsr fpsr = oprd.getoval (3);
    fpsr.convert (NONEPC, tofpcomp (inst.getfpcomp ()), fr);
    result.setrval (0, fr);
    return result;
  }

  // F_FNMA_S
  static Result exec_fnma_s (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f0 = oprd.getrval (0);
    t_real f1 = oprd.getrval (1);
    t_real f2 = oprd.getrval (2);
    bool natr = (f0.isnat () || f1.isnat () || f2.isnat ());
    // set to natval if nat bit is set
    t_real fr;
    if (natr == true) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute value
    fr = f0 - (f1 * f2);
    Fpsr fpsr = oprd.getoval (3);
    fpsr.convert (S, tofpcomp (inst.getfpcomp ()), fr);
    result.setrval (0, fr);
    return result;
  }

  // F_FNMA_D
  static Result exec_fnma_d (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f0 = oprd.getrval (0);
    t_real f1 = oprd.getrval (1);
    t_real f2 = oprd.getrval (2);
    bool natr = (f0.isnat () || f1.isnat () || f2.isnat ());
    // set to natval if nat bit is set
    t_real fr;
    if (natr == true) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute value
    fr = f0 - (f1 * f2);
    Fpsr fpsr = oprd.getoval (3);
    fpsr.convert (D, tofpcomp (inst.getfpcomp ()), fr);
    result.setrval (0, fr);
    return result;
  }

  // F_FPMA
  static Result exec_fpma (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f0 = oprd.getrval (0);
    t_real f1 = oprd.getrval (1);
    t_real f2 = oprd.getrval (2);
    bool natr = (f0.isnat () || f1.isnat () || f2.isnat ());
    // set to natval if nat bit is set
    t_real fr;
    if (natr == true) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute value
    union {
      t_octa d_sgfd;
      t_byte d_bval[8];
    } src0, src1, src2, res;
    src0.d_sgfd = lfixocta (f0.getsgfd ());
    src1.d_sgfd = lfixocta (f1.getsgfd ());
    src2.d_sgfd = lfixocta (f2.getsgfd ());
    t_real f01, f02, f11, f12, f21, f22, fr1, fr2;
    f01.singleld (src0.d_bval);
    f02.singleld (&src0.d_bval[4]);
    f11.singleld (src1.d_bval);
    f12.singleld (&src1.d_bval[4]);
    f21.singleld (src2.d_bval);
    f22.singleld (&src2.d_bval[4]);
    if (oprd.getrid (0). getlnum () != 0) {
      fr1 = (f11 * f21) + f01;
      fr2 = (f12 * f22) + f02;
    }
    else {
      fr1 = (f11 * f21);
      fr2 = (f12 * f22);
    }
    Fpsr fpsr = oprd.getoval (3);
    fpsr.pconvert (tofpcomp (inst.getfpcomp ()), fr1, fr2);
    fr1.singlest (res.d_bval);
    fr2.singlest (&res.d_bval[4]);
    fr.integerld (res.d_bval);
    result.setrval (0, fr);
    return result;
  }

  // F_FPNMA
  static Result exec_fpnma (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f0 = oprd.getrval (0);
    t_real f1 = oprd.getrval (1);
    t_real f2 = oprd.getrval (2);
    bool natr = (f0.isnat () || f1.isnat () || f2.isnat ());
    // set to natval if nat bit is set
    t_real fr;
    if (natr == true) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute value
    union {
      t_octa d_sgfd;
      t_byte d_bval[8];
    } src0, src1, src2, res;
    src0.d_sgfd = lfixocta (f0.getsgfd ());
    src1.d_sgfd = lfixocta (f1.getsgfd ());
    src2.d_sgfd = lfixocta (f2.getsgfd ());
    t_real f01, f02, f11, f12, f21, f22, fr1, fr2;
    f01.singleld (src0.d_bval);
    f02.singleld (&src0.d_bval[4]);
    f11.singleld (src1.d_bval);
    f12.singleld (&src1.d_bval[4]);
    f21.singleld (src2.d_bval);
    f22.singleld (&src2.d_bval[4]);
    if (oprd.getrid (0). getlnum () != 0) {
      fr1 = f01 - (f11 * f21);
      fr2 = f02 - (f12 * f22);
    }
    else {
      fr1 = -(f11 * f21);
      fr2 = -(f12 * f22);
    }
    Fpsr fpsr = oprd.getoval (3);
    fpsr.pconvert (tofpcomp (inst.getfpcomp ()), fr1, fr2);
    fr1.singlest (res.d_bval);
    fr2.singlest (&res.d_bval[4]);
    fr.integerld (res.d_bval);
    result.setrval (0, fr);
    return result;
  }

  // F_FMS
  static Result exec_fms (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f0 = oprd.getrval (0);
    t_real f1 = oprd.getrval (1);
    t_real f2 = oprd.getrval (2);
    bool natr = (f0.isnat () || f1.isnat () || f2.isnat ());
    // set to natval if nat bit is set
    t_real fr;
    if (natr == true) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute value
    if (oprd.getrid (0). getlnum () != 0) fr = (f1 * f2) - f0;
    else fr = f1 * f2;
    Fpsr fpsr = oprd.getoval (3);
    fpsr.convert (NONEPC, tofpcomp (inst.getfpcomp ()), fr);
    result.setrval (0, fr);
    return result;
  }

  // F_FMS_S
  static Result exec_fms_s (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f0 = oprd.getrval (0);
    t_real f1 = oprd.getrval (1);
    t_real f2 = oprd.getrval (2);
    bool natr = (f0.isnat () || f1.isnat () || f2.isnat ());
    // set to natval if nat bit is set
    t_real fr;
    if (natr == true) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute value
    fr = (f1 * f2) - f0;
    Fpsr fpsr = oprd.getoval (3);
    fpsr.convert (S, tofpcomp (inst.getfpcomp ()), fr);
    result.setrval (0, fr);
    return result;
  }

  // F_FMS_D
  static Result exec_fms_d (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f0 = oprd.getrval (0);
    t_real f1 = oprd.getrval (1);
    t_real f2 = oprd.getrval (2);
    bool natr = (f0.isnat () || f1.isnat () || f2.isnat ());
    // set to natval if nat bit is set
    t_real fr;
    if (natr == true) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute value
    fr = (f1 * f2) - f0;
    Fpsr fpsr = oprd.getoval (3);
    fpsr.convert (D, tofpcomp (inst.getfpcomp ()), fr);
    result.setrval (0, fr);
    return result;
  }


  // F_FPMS
  static Result exec_fpms (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f0 = oprd.getrval (0);
    t_real f1 = oprd.getrval (1);
    t_real f2 = oprd.getrval (2);
    bool natr = (f0.isnat () || f1.isnat () || f2.isnat ());
    // set to natval if nat bit is set
    t_real fr;
    if (natr == true) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute value
    union {
      t_octa d_sgfd;
      t_byte d_bval[8];
    } src0, src1, src2, res;
    src0.d_sgfd = lfixocta (f0.getsgfd ());
    src1.d_sgfd = lfixocta (f1.getsgfd ());
    src2.d_sgfd = lfixocta (f2.getsgfd ());
    t_real f01, f02, f11, f12, f21, f22, fr1, fr2;
    f01.singleld (src0.d_bval);
    f02.singleld (&src0.d_bval[4]);
    f11.singleld (src1.d_bval);
    f12.singleld (&src1.d_bval[4]);
    f21.singleld (src2.d_bval);
    f22.singleld (&src2.d_bval[4]);
    if (oprd.getrid (0). getlnum () != 0) {
      fr1 = (f11 * f21) - f01;
      fr2 = (f12 * f22) - f02;
    }
    else {
      fr1 = (f11 * f21);
      fr2 = (f12 * f22);
    }
    Fpsr fpsr = oprd.getoval (3);
    fpsr.pconvert (tofpcomp (inst.getfpcomp ()), fr1, fr2);
    fr1.singlest (res.d_bval);
    fr2.singlest (&res.d_bval[4]);
    fr.integerld (res.d_bval);
    result.setrval (0, fr);
    return result;
  }

  // ------------------------------------------------------------------------
  // - F02 instruction group                                                 -
  // ------------------------------------------------------------------------

  // F_XMA_L
  static Result exec_xma_l (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    t_real fr;
    // compute nat value
    t_real f0 = oprd.getrval (0);
    t_real f1 = oprd.getrval (1);
    t_real f2 = oprd.getrval (2);
    bool natr = (f0.isnat () || f1.isnat () || f2.isnat ());
    if (natr == true) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // prepare huge number
    t_huge h0 = f0.getsgfd ();
    t_huge h1 = f1.getsgfd ();
    t_huge h2 = f2.getsgfd ();
    // compute result as a real integer
    t_huge rh = (h1 * h2) + h0;
    fr.setinteger (rh.getlow ());
    // set result
    result.setrval (0, fr);
    return result;
  }

  // F_XMA_H
  static Result exec_xma_h (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    t_real fr;
    // compute nat value
    t_real f0 = oprd.getrval (0);
    t_real f1 = oprd.getrval (1);
    t_real f2 = oprd.getrval (2);
    bool natr = (f0.isnat () || f1.isnat () || f2.isnat ());
    if (natr == true) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // prepare huge number
    t_huge h0 = f0.getsgfd ();
    t_huge h1 = f1.getsgfd ();
    t_huge h2 = f2.getsgfd ();
    if ((h1.getlow () & SIGN_8) == SIGN_8) h1.sethigh (OCTA_A);
    if ((h2.getlow () & SIGN_8) == SIGN_8) h2.sethigh (OCTA_A);
    // compute result as a real integer
    t_huge rh = (h1 * h2) + h0;
    fr.setinteger (rh.gethigh ());
    // set result
    result.setrval (0, fr);
    return result;
  }

  // F_XMA_HU
  static Result exec_xma_hu (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f0 = oprd.getrval (0);
    t_real f1 = oprd.getrval (1);
    t_real f2 = oprd.getrval (2);
    bool natr = (f0.isnat () || f1.isnat () || f2.isnat ());
    // set to natval if nat bit is set
    t_real fr;
    if (natr == true) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // prepare huge number
    t_huge h0 = f0.getsgfd ();
    t_huge h1 = f1.getsgfd ();
    t_huge h2 = f2.getsgfd ();
    // compute result as a real integer
    t_huge rh = (h1 * h2) + h0;
    fr.setinteger (rh.gethigh ());
    // set result
    result.setrval (0, fr);
    return result;
  }

  // ------------------------------------------------------------------------
  // - F03 instruction group                                                 -
  // ------------------------------------------------------------------------

  // F_FSELECT
  static Result exec_fselect (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    t_real fr;
    // compute nat value
    t_real f0 = oprd.getrval (0);
    t_real f1 = oprd.getrval (1);
    t_real f2 = oprd.getrval (2);
    bool natr = (f0.isnat () || f1.isnat () || f2.isnat ());
    if (natr == true) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute result
    t_octa sgfd0 = f0.getsgfd ();
    t_octa sgfd1 = f1.getsgfd ();
    t_octa sgfd2 = f2.getsgfd ();
    sgfd1 &= sgfd0;
    sgfd2 &= ~sgfd0;
    sgfd2 |= sgfd1;
    fr.setinteger (sgfd2);
    result.setrval (0, fr);
    return result;
  }

  // ------------------------------------------------------------------------
  // - F04 instruction group                                                 -
  // ------------------------------------------------------------------------

  // F_FCMP_EQ, F_FCMP_EQ_UNC
   static Result exec_fcmp_eq (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // check for result clear with unc
    if ((inst.getiopc () == F_FCMP_EQ_UNC) && (oprd.getbval (2) == false)) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compute result
    t_real f0 = oprd.getrval (0);
    t_real f1 = oprd.getrval (1);
    if ((f0.isnat () == true) || (f1.isnat () == true)) {
      result.setbval (0, false);
      result.setbval (1, false);
    } else if (f0 == f1) {
      result.setbval (0, true);
      result.setbval (1, false);
    } else {
      result.setbval (0, false);
      result.setbval (1, true);
    }
    return result;
   }

   // F_FCMP_LT, F_FCMP_LT_UNC
   static Result exec_fcmp_lt (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // check for result clear with unc
    if ((inst.getiopc () == F_FCMP_LT_UNC) && (oprd.getbval (2) == false)) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compute result
    t_real f0 = oprd.getrval (0);
    t_real f1 = oprd.getrval (1);
    if ((f0.isnat () == true) || (f1.isnat () == true)) {
      result.setbval (0, false);
      result.setbval (1, false);
    } else if (f0 < f1) {
      result.setbval (0, true);
      result.setbval (1, false);
    } else {
      result.setbval (0, false);
      result.setbval (1, true);
    }
    return result;
   }

   // F_FCMP_LE, F_FCMP_LE_UNC
   static Result exec_fcmp_le (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // check for result clear with unc
    if ((inst.getiopc () == F_FCMP_LE_UNC) && (oprd.getbval (2) == false)) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compute result
    t_real f0 = oprd.getrval (0);
    t_real f1 = oprd.getrval (1);
    if ((f0.isnat () == true) || (f1.isnat () == true)) {
      result.setbval (0, false);
      result.setbval (1, false);
    } else if (f0 <= f1) {
      result.setbval (0, true);
      result.setbval (1, false);
    } else {
      result.setbval (0, false);
      result.setbval (1, true);
    }
    return result;
   }

  // F_FCMP_UNORD, F_FCMP_UNORD_UNC
   static Result exec_fcmp_unord (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // check for result clear with unc
    if ((inst.getiopc () == F_FCMP_EQ_UNC) && (oprd.getbval (2) == false)) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compute result
    t_real f0 = oprd.getrval (0);
    t_real f1 = oprd.getrval (1);
    if ((f0.isnat () == true) || (f1.isnat () == true)){
      result.setbval (0, false);
      result.setbval (1, false);
    } else if (f0.isnan () || f1.isnan ()) {
      result.setbval (0, true);
      result.setbval (1, false);
    } else {
      result.setbval (0, false);
      result.setbval (1, true);
    }
    return result;
   }

  // ------------------------------------------------------------------------
  // - F05 instruction group                                                -
  // ------------------------------------------------------------------------

  // F_FCLASS_M & F_FCLASS_M_UNC
  static Result exec_fclass_m (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for intereuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // get operand
    t_octa immv = inst.getimmv (0);
    t_real f0 = oprd.getrval (0);
    // check for result clear with unc instruction or with not
    // tested nat val
    if (((inst.getiopc () == F_FCLASS_M_UNC) && (oprd.getbval (1) == false)) ||
	 ((fclass_isnat (immv) == false) &&  (f0.isnat () == true))) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compute result
    bool tmp_res = (((fclass_ispos (immv) == true) && 
		     (f0.getsign () == false)) || 
		    ((fclass_isneg (immv) == true) && 
		     (f0.getsign () == true)));
    tmp_res &= (((fclass_iszero  (immv) == true) && 
		 (f0 == 0.0L)) ||
		((fclass_isunorm (immv) == true) && 
		 (f0.isunorm () == true)) ||
		((fclass_isnorm  (immv) == true) && 
		 (f0.isnorm  () == true)) ||
		((fclass_isinf   (immv) == true) && 
		 (f0.isinf   () == true)));
    // instead of testing if we have silent nan we test if we have
    // a finit or infinite value that is a more large test.
    tmp_res &= (((fclass_isqnan  (immv) == true) && 
		 (f0.isnan   () == true)) ||
		((fclass_isnat   (immv) == true) && 
		 (f0.isnat   () == true)) ||
		((fclass_issnan  (immv) == true) && 
		 ((f0.isfinite() == false) || 
		  (f0.isinf () == false))));
    // set result
    result.setbval (0, tmp_res);
    result.setbval (1, !tmp_res);
    return result;
  }

  // ------------------------------------------------------------------------
  // - F06 instruction group                                                -
  // ------------------------------------------------------------------------

  // F_FRPCA
  static Result exec_frcpa (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check first predicate value
    bool pred = oprd.getbval (2);
    if (pred == false) {
      result.setrrt  (0, false);
      result.setbval (1, false);
      return result;
    }
    // compute nat value
    t_real f0 = oprd.getrval (0);
    t_real f1 = oprd.getrval (1);
    bool natr = (f0.isnat () || f1.isnat ());
    // set to natval if nat bit is set
    t_real fr;
    if (natr == true) {
      fr.setnat ();
      result.setrval (0, fr);
      result.setbval (1, false);
      return result;
    }
    // divide and check values
    fr = f0 / f1;
    if (fr.isnan () || fr.isinf ()) {
      result.setrval (0, fr);
      result.setbval (1, false);
      return result;
    }
    if (f0.isinf () && f1.isfinite ()) {
      bool sign = f0.getsign () ^ f1.getsign ();
      if (sign  == true) 
	fr.setninf ();
      else
	fr.setpinf ();
      result.setrval (0, fr);
      result.setbval (1, false);
      return result;
    }
    if (f0.isfinite () && f1.isinf ()) {
      bool sign = f0.getsign () ^ f1.getsign ();
      fr = 0.0;
      fr.setsign (sign);
      result.setrval (0, fr);
      result.setbval (1, false);
      return result;
    }
    if ((f0 == 0.0L) && f1.isfinite ()) {
      bool sign = f0.getsign () ^ f1.getsign ();
      fr = 0.0;
      fr.setsign (sign);
      result.setrval (0, fr);
      result.setbval (1, false);
      return result;
    }
    // compute reciprocal
    result.setrval (0, f1.rcpa ());
    result.setbval (1, true);
    return result;
  }

  // ------------------------------------------------------------------------
  // - F07 instruction group                                                -
  // ------------------------------------------------------------------------

  // F_FRSQRTA
  static Result exec_frsqrta (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check first predicate value
    bool pred = oprd.getbval (2);
    if (pred == false) {
      result.setrrt  (0, false);
      result.setbval (1, false);
      return result;
    }
    // compute nat value
    t_real f1 = oprd.getrval (1);
    bool natr = f1.isnat ();
    // set to natval if nat bit is set - default real is nan
    t_real fr;
    if (natr == true) {
      fr.setnat ();
      result.setrval (0, fr);
      result.setbval (1, false);
      return result;
    }
    // compute result
    f1.normalize ();
    if (f1 == 0.0L)  {
      result.setrval (0, f1);
      result.setbval (1, false);
      return result;
    } else if (f1.isnan () == true) {
      result.setrval (0, f1);
      result.setbval (1, false);
      return result;
    } else if (f1.getsign () == true)  {
      result.setrval (0, fr);
      result.setbval (1, false);
      return result;
    } else if (f1.ispinf () == true) {
      result.setrval (0, f1);
      result.setbval (1, false);
      return result;
    }
    // compute reciprocal square root
    result.setrval (0, f1.rsqrt ());
    result.setbval (1, true);
    return result;
  }

  // F_FPRSQRTA
  static Result exec_fprsqrta (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check first predicate value
    bool pred = oprd.getbval (2);
    if (pred == false) {
      result.setrrt  (0, false);
      result.setbval (1, false);
      return result;
    }
    // compute nat value
    t_real f1 = oprd.getrval (1);
    bool natr = f1.isnat ();
    // set to natval if nat bit is set
    if (natr == true) {
      t_real fr;
      fr.setnat ();
      result.setrval (0, fr);
      result.setbval (1, false);
      return result;
    }
    // compute result
    union {
      t_octa d_sgfd;
      t_byte d_bval[8];
    } src, res;
    src.d_sgfd = lfixocta (f1.getsgfd ());
    t_real fhi, flo;
    flo.singleld (src.d_bval);
    fhi.singleld (&src.d_bval[4]);
    flo.normalize ();
    fhi.normalize ();
    t_real frlo;
    if (flo.getsign () == true) frlo.singlest (res.d_bval);
    else if (flo.isnan () == true) flo.singlest (res.d_bval);
    else if (flo == 0.0L) {
      frlo.setpinf ();
      frlo.singlest (res.d_bval);
    }
    else if (flo.ispinf () == true) {
      frlo = 0.0L;
      frlo.singlest (res.d_bval);
    }
    else {
      frlo = flo.rsqrt ();
      frlo.singlest (res.d_bval);
      natr = true;
    }
    t_real frhi;
    if (fhi.getsign () == true) {
      frhi.setnanindefinite ();
      frhi.singlest(&res.d_bval[4]);
      natr = false;
    }
    else if (fhi.isnan () == true) {
      fhi.singlest (&res.d_bval[4]);
      natr = false;
    }
    else if (fhi == 0.0L) {
      frhi.setpinf ();
      frhi.singlest (&res.d_bval[4]);
      natr = false;
    }
    else if (fhi.ispinf () == true) {
      frhi = 0.0L;
      frhi.singlest (&res.d_bval[4]);
      natr = false;
    }
    else {
      frhi = fhi.rsqrt ();
      frhi.singlest (&res.d_bval[4]);
    }
    // set result
    t_real fr;
    fr.setinteger (lfixocta (res.d_sgfd));
    result.setrval (0, fr);
    result.setbval (1, natr);
    return result;
  }

  // ------------------------------------------------------------------------
  // - F08 instruction group                                                -
  // ------------------------------------------------------------------------

  // F_FMIN
  static Result exec_fmin (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f2 = oprd.getrval (0);
    t_real f3 = oprd.getrval (1);
    // set to natval if nat bit is set
    t_real fr;
    if ((f2.isnat () == true) || (f3.isnat () == true)) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute result
    if ((f2.isnan () == true) || (f3.isnan () == true)) fr = f3;
    else if (f2 < f3) fr = f2;
    else fr = f3;
    Fpsr fpsr = oprd.getoval (2);
    fpsr.convert (NONEPC, tofpcomp (inst.getfpcomp ()), fr);
    result.setrval (0, fr);
    return result;
  }

  // F_FMAX
  static Result exec_fmax (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f2 = oprd.getrval (0);
    t_real f3 = oprd.getrval (1);
    // set to natval if nat bit is set
    t_real fr;
    if ((f2.isnat () == true) || (f3.isnat () == true)) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute result
    if ((f2.isnan () == true) || (f3.isnan () == true)) fr = f3;
    else if (f2 > f3) fr = f2;
    else fr = f3;
    Fpsr fpsr = oprd.getoval (2);
    fpsr.convert (NONEPC, tofpcomp (inst.getfpcomp ()), fr);
    result.setrval (0, fr);
    return result;
  }

  // F_FAMIN
  static Result exec_famin (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f2 = oprd.getrval (0);
    t_real f3 = oprd.getrval (1);
    // set to natval if nat bit is set
    t_real fr;
    if ((f2.isnat () == true) || (f3.isnat () == true)) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute result
    if ((f2.isnan () == true) || (f3.isnan () == true)) fr = f3;
    else if (f2.getabs () < f3.getabs ()) fr = f2;
    else fr = f3;
    Fpsr fpsr = oprd.getoval (2);
    fpsr.convert (NONEPC, tofpcomp (inst.getfpcomp ()), fr);
    result.setrval (0, fr);
    return result;
  }

  // F_FAMAX
  static Result exec_famax (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f2 = oprd.getrval (0);
    t_real f3 = oprd.getrval (1);
    // set to natval if nat bit is set
    t_real fr;
    if ((f2.isnat () == true) || (f3.isnat () == true)) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute result
    if ((f2.isnan () == true) || (f3.isnan () == true)) fr = f3;
    else if (f2.getabs () > f3.getabs ()) fr = f2;
    else fr = f3;
    Fpsr fpsr = oprd.getoval (2);
    fpsr.convert (NONEPC, tofpcomp (inst.getfpcomp ()), fr);
    result.setrval (0, fr);
    return result;
  }

  // F_FPMIN
  static Result exec_fpmin (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f2 = oprd.getrval (0);
    t_real f3 = oprd.getrval (1);
    // set to natval if nat bit is set
    t_real fr;
    if ((f2.isnat () == true) || (f3.isnat () == true)) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute value
    union {
      t_octa d_sgfd;
      t_byte d_bval[8];
    } src2, src3, res;
    src2.d_sgfd = lfixocta (f2.getsgfd ());
    src3.d_sgfd = lfixocta (f3.getsgfd ());
    t_real f21, f22, f31, f32, fr1, fr2;
    f21.singleld (src2.d_bval);
    f22.singleld (&src2.d_bval[4]);
    f31.singleld (src3.d_bval);
    f32.singleld (&src3.d_bval[4]);
    // compute result (low)
    if ((f21.isnan () == true) || (f31.isnan () == true)) fr1 = f31;
    else if (f21 < f31) fr1 = f21;
    else fr1 = f31;
    // compute result (high)
    if ((f22.isnan () == true) || (f32.isnan () == true)) fr2 = f32;
    else if (f22 < f32) fr2 = f22;
    else fr2 = f32;
    Fpsr fpsr = oprd.getoval (2);
    fpsr.pconvert (tofpcomp (inst.getfpcomp ()), fr1, fr2);
    fr1.singlest (res.d_bval);
    fr2.singlest (&res.d_bval[4]);
    fr.integerld (res.d_bval);
    result.setrval (0, fr);
    return result;
  }

  // F_FPMAX
  static Result exec_fpmax (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f2 = oprd.getrval (0);
    t_real f3 = oprd.getrval (1);
    // set to natval if nat bit is set
    t_real fr;
    if ((f2.isnat () == true) || (f3.isnat () == true)) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute value
    union {
      t_octa d_sgfd;
      t_byte d_bval[8];
    } src2, src3, res;
    src2.d_sgfd = lfixocta (f2.getsgfd ());
    src3.d_sgfd = lfixocta (f3.getsgfd ());
    t_real f21, f22, f31, f32, fr1, fr2;
    f21.singleld (src2.d_bval);
    f22.singleld (&src2.d_bval[4]);
    f31.singleld (src3.d_bval);
    f32.singleld (&src3.d_bval[4]);
    // compute result (low)
    if ((f21.isnan () == true) || (f31.isnan () == true)) fr1 = f31;
    else if (f21 > f31) fr1 = f21;
    else fr1 = f31;
    // compute result (high)
    if ((f22.isnan () == true) || (f32.isnan () == true)) fr2 = f32;
    else if (f22 > f32) fr2 = f22;
    else fr2 = f32;
    Fpsr fpsr = oprd.getoval (2);
    fpsr.pconvert (tofpcomp (inst.getfpcomp ()), fr1, fr2);
    fr1.singlest (res.d_bval);
    fr2.singlest (&res.d_bval[4]);
    fr.integerld (res.d_bval);
    result.setrval (0, fr);
    return result;
  }

  // F_FPAMIN
  static Result exec_fpamin (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f2 = oprd.getrval (0);
    t_real f3 = oprd.getrval (1);
    // set to natval if nat bit is set
    t_real fr;
    if ((f2.isnat () == true) || (f3.isnat () == true)) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute value
    union {
      t_octa d_sgfd;
      t_byte d_bval[8];
    } src2, src3, res;
    src2.d_sgfd = lfixocta (f2.getsgfd ());
    src3.d_sgfd = lfixocta (f3.getsgfd ());
    t_real f21, f22, f31, f32, fr1, fr2;
    f21.singleld (src2.d_bval);
    f22.singleld (&src2.d_bval[4]);
    f31.singleld (src3.d_bval);
    f32.singleld (&src3.d_bval[4]);
    // compute result (low)
    if ((f21.isnan () == true) || (f31.isnan () == true)) fr1 = f31;
    else if (f21.getabs () < f31.getabs ()) fr1 = f21;
    else fr1 = f31;
    // compute result (high)
    if ((f22.isnan () == true) || (f32.isnan () == true)) fr2 = f32;
    else if (f22.getabs () < f32.getabs ()) fr2 = f22;
    else fr2 = f32;
    Fpsr fpsr = oprd.getoval (2);
    fpsr.pconvert (tofpcomp (inst.getfpcomp ()), fr1, fr2);
    fr1.singlest (res.d_bval);
    fr2.singlest (&res.d_bval[4]);
    fr.integerld (res.d_bval);
    result.setrval (0, fr);
    return result;
  }

  // F_FPAMAX
  static Result exec_fpamax (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f2 = oprd.getrval (0);
    t_real f3 = oprd.getrval (1);
    // set to natval if nat bit is set
    t_real fr;
    if ((f2.isnat () == true) || (f3.isnat () == true)) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute value
    union {
      t_octa d_sgfd;
      t_byte d_bval[8];
    } src2, src3, res;
    src2.d_sgfd = lfixocta (f2.getsgfd ());
    src3.d_sgfd = lfixocta (f3.getsgfd ());
    t_real f21, f22, f31, f32, fr1, fr2;
    f21.singleld (src2.d_bval);
    f22.singleld (&src2.d_bval[4]);
    f31.singleld (src3.d_bval);
    f32.singleld (&src3.d_bval[4]);
    // compute result (low)
    if ((f21.isnan () == true) || (f31.isnan () == true)) fr1 = f31;
    else if (f21.getabs () > f31.getabs ()) fr1 = f21;
    else fr1 = f31;
    // compute result (high)
    if ((f22.isnan () == true) || (f32.isnan () == true)) fr2 = f32;
    else if (f22.getabs () > f32.getabs ()) fr2 = f22;
    else fr2 = f32;
    Fpsr fpsr = oprd.getoval (2);
    fpsr.pconvert (tofpcomp (inst.getfpcomp ()), fr1, fr2);
    fr1.singlest (res.d_bval);
    fr2.singlest (&res.d_bval[4]);
    fr.integerld (res.d_bval);
    result.setrval (0, fr);
    return result;
  }

  // F_FPCMP_EQ
  static Result exec_fpcmp_eq (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f2 = oprd.getrval (0);
    t_real f3 = oprd.getrval (1);
    // set to natval if nat bit is set
    t_real fr;
    if ((f2.isnat () == true) || (f3.isnat () == true)) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute value
    union {
      t_octa d_sgfd;
      t_byte d_bval[8];
      t_quad d_qval[2];
    } src2, src3, res;
    src2.d_sgfd = lfixocta (f2.getsgfd ());
    src3.d_sgfd = lfixocta (f3.getsgfd ());
    t_real f21, f22, f31, f32;
    f21.singleld (src2.d_bval);
    f22.singleld (&src2.d_bval[4]);
    f31.singleld (src3.d_bval);
    f32.singleld (&src3.d_bval[4]);
    // compute result (low)
    if (f21 == f31) res.d_qval[0] = QUAD_A;
    else res.d_qval[0] = QUAD_0;
    // compute result (high)
    if (f22 == f32) res.d_qval[1] = QUAD_A;
    else res.d_qval[1] = QUAD_0;
    fr.integerld (res.d_bval);
    result.setrval (0, fr);
    return result;
  }

  // F_FPCMP_LT
  static Result exec_fpcmp_lt (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f2 = oprd.getrval (0);
    t_real f3 = oprd.getrval (1);
    // set to natval if nat bit is set
    t_real fr;
    if ((f2.isnat () == true) || (f3.isnat () == true)) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute value
    union {
      t_octa d_sgfd;
      t_byte d_bval[8];
      t_quad d_qval[2];
    } src2, src3, res;
    src2.d_sgfd = lfixocta (f2.getsgfd ());
    src3.d_sgfd = lfixocta (f3.getsgfd ());
    t_real f21, f22, f31, f32;
    f21.singleld (src2.d_bval);
    f22.singleld (&src2.d_bval[4]);
    f31.singleld (src3.d_bval);
    f32.singleld (&src3.d_bval[4]);
    // compute result (low)
    if (f21 < f31) res.d_qval[0] = QUAD_A;
    else res.d_qval[0] = QUAD_0;
    // compute result (high)
    if (f22 < f32) res.d_qval[1] = QUAD_A;
    else res.d_qval[1] = QUAD_0;
    fr.integerld (res.d_bval);
    result.setrval (0, fr);
    return result;
  }

  // F_FPCMP_LE
  static Result exec_fpcmp_le (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f2 = oprd.getrval (0);
    t_real f3 = oprd.getrval (1);
    // set to natval if nat bit is set
    t_real fr;
    if ((f2.isnat () == true) || (f3.isnat () == true)) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute value
    union {
      t_octa d_sgfd;
      t_byte d_bval[8];
      t_quad d_qval[2];
    } src2, src3, res;
    src2.d_sgfd = lfixocta (f2.getsgfd ());
    src3.d_sgfd = lfixocta (f3.getsgfd ());
    t_real f21, f22, f31, f32;
    f21.singleld (src2.d_bval);
    f22.singleld (&src2.d_bval[4]);
    f31.singleld (src3.d_bval);
    f32.singleld (&src3.d_bval[4]);
    // compute result (low)
    if (f21 <= f31) res.d_qval[0] = QUAD_A;
    else res.d_qval[0] = QUAD_0;
    // compute result (high)
    if (f22 <= f32) res.d_qval[1] = QUAD_A;
    else res.d_qval[1] = QUAD_0;
    fr.integerld (res.d_bval);
    result.setrval (0, fr);
    return result;
  }

  // F_FPCMP_UNORD
  static Result exec_fpcmp_unord (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f2 = oprd.getrval (0);
    t_real f3 = oprd.getrval (1);
    // set to natval if nat bit is set
    t_real fr;
    if ((f2.isnat () == true) || (f3.isnat () == true)) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute value
    union {
      t_octa d_sgfd;
      t_byte d_bval[8];
      t_quad d_qval[2];
    } src2, src3, res;
    src2.d_sgfd = lfixocta (f2.getsgfd ());
    src3.d_sgfd = lfixocta (f3.getsgfd ());
    t_real f21, f22, f31, f32;
    f21.singleld (src2.d_bval);
    f22.singleld (&src2.d_bval[4]);
    f31.singleld (src3.d_bval);
    f32.singleld (&src3.d_bval[4]);
    // compute result (low)
    if (f21.unordered (f31) == true) res.d_qval[0] = QUAD_A;
    else res.d_qval[0] = QUAD_0;
    // compute result (high)
    if (f22.unordered (f32) == true) res.d_qval[1] = QUAD_A;
    else res.d_qval[1] = QUAD_0;
    fr.integerld (res.d_bval);
    result.setrval (0, fr);
    return result;
  }

  // F_FPCMP_NEQ
  static Result exec_fpcmp_neq (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f2 = oprd.getrval (0);
    t_real f3 = oprd.getrval (1);
    // set to natval if nat bit is set
    t_real fr;
    if ((f2.isnat () == true) || (f3.isnat () == true)) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute value
    union {
      t_octa d_sgfd;
      t_byte d_bval[8];
      t_quad d_qval[2];
    } src2, src3, res;
    src2.d_sgfd = lfixocta (f2.getsgfd ());
    src3.d_sgfd = lfixocta (f3.getsgfd ());
    t_real f21, f22, f31, f32;
    f21.singleld (src2.d_bval);
    f22.singleld (&src2.d_bval[4]);
    f31.singleld (src3.d_bval);
    f32.singleld (&src3.d_bval[4]);
    // compute result (low)
    if (!(f21 == f31)) res.d_qval[0] = QUAD_A;
    else res.d_qval[0] = QUAD_0;
    // compute result (high)
    if (!(f22 == f32)) res.d_qval[1] = QUAD_A;
    else res.d_qval[1] = QUAD_0;
    fr.integerld (res.d_bval);
    result.setrval (0, fr);
    return result;
  }

  // F_FPCMP_NLT
  static Result exec_fpcmp_nlt (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f2 = oprd.getrval (0);
    t_real f3 = oprd.getrval (1);
    // set to natval if nat bit is set
    t_real fr;
    if ((f2.isnat () == true) || (f3.isnat () == true)) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute value
    union {
      t_octa d_sgfd;
      t_byte d_bval[8];
      t_quad d_qval[2];
    } src2, src3, res;
    src2.d_sgfd = lfixocta (f2.getsgfd ());
    src3.d_sgfd = lfixocta (f3.getsgfd ());
    t_real f21, f22, f31, f32;
    f21.singleld (src2.d_bval);
    f22.singleld (&src2.d_bval[4]);
    f31.singleld (src3.d_bval);
    f32.singleld (&src3.d_bval[4]);
    // compute result (low)
    if (!(f21 < f31)) res.d_qval[0] = QUAD_A;
    else res.d_qval[0] = QUAD_0;
    // compute result (high)
    if (!(f22 < f32)) res.d_qval[1] = QUAD_A;
    else res.d_qval[1] = QUAD_0;
    fr.integerld (res.d_bval);
    result.setrval (0, fr);
    return result;
  }

  // F_FPCMP_NLE
  static Result exec_fpcmp_nle (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f2 = oprd.getrval (0);
    t_real f3 = oprd.getrval (1);
    // set to natval if nat bit is set
    t_real fr;
    if ((f2.isnat () == true) || (f3.isnat () == true)) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute value
    union {
      t_octa d_sgfd;
      t_byte d_bval[8];
      t_quad d_qval[2];
    } src2, src3, res;
    src2.d_sgfd = lfixocta (f2.getsgfd ());
    src3.d_sgfd = lfixocta (f3.getsgfd ());
    t_real f21, f22, f31, f32;
    f21.singleld (src2.d_bval);
    f22.singleld (&src2.d_bval[4]);
    f31.singleld (src3.d_bval);
    f32.singleld (&src3.d_bval[4]);
    // compute result (low)
    if (!(f21 <= f31)) res.d_qval[0] = QUAD_A;
    else res.d_qval[0] = QUAD_0;
    // compute result (high)
    if (!(f22 <= f32)) res.d_qval[1] = QUAD_A;
    else res.d_qval[1] = QUAD_0;
    fr.integerld (res.d_bval);
    result.setrval (0, fr);
    return result;
  }

  // F_FPCMP_ORD
  static Result exec_fpcmp_ord (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f2 = oprd.getrval (0);
    t_real f3 = oprd.getrval (1);
    // set to natval if nat bit is set
    t_real fr;
    if ((f2.isnat () == true) || (f3.isnat () == true)) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute value
    union {
      t_octa d_sgfd;
      t_byte d_bval[8];
      t_quad d_qval[2];
    } src2, src3, res;
    src2.d_sgfd = lfixocta (f2.getsgfd ());
    src3.d_sgfd = lfixocta (f3.getsgfd ());
    t_real f21, f22, f31, f32;
    f21.singleld (src2.d_bval);
    f22.singleld (&src2.d_bval[4]);
    f31.singleld (src3.d_bval);
    f32.singleld (&src3.d_bval[4]);
    // compute result (low)
    if (!(f21.unordered (f31) == true)) res.d_qval[0] = QUAD_A;
    else res.d_qval[0] = QUAD_0;
    // compute result (high)
    if (!(f22.unordered (f32) == true)) res.d_qval[1] = QUAD_A;
    else res.d_qval[1] = QUAD_0;
    fr.integerld (res.d_bval);
    result.setrval (0, fr);
    return result;
  }

  // ------------------------------------------------------------------------
  // - F09 instruction group                                                -
  // ------------------------------------------------------------------------

  // F_FMERGE_NS
  static Result exec_fmerge_ns (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f2 = oprd.getrval (0);
    t_real f3 = oprd.getrval (1);
    // set to natval if nat bit is set
    t_real fr;
    if ((f2.isnat () == true) || (f3.isnat () == true)) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute result
    fr.setsgfd (f3.getsgfd ());
    fr.setexp  (f3.getexp  ());
    fr.setsign ((f2.getsign () == true) ? false : true);
    result.setrval (0, fr);
    return result;
  }

  // F_FMERGE_S
  static Result exec_fmerge_s (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f2 = oprd.getrval (0);
    t_real f3 = oprd.getrval (1);
    // set to natval if nat bit is set
    t_real fr;
    if ((f2.isnat () == true) || (f3.isnat () == true)) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute result
    fr.setsgfd (f3.getsgfd ());
    fr.setexp  (f3.getexp  ());
    fr.setsign ((f2.getsign () == false) ? false : true);
    result.setrval (0, fr);
    return result;
  }

  // F_FMERGE_SE
  static Result exec_fmerge_se (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f2 = oprd.getrval (0);
    t_real f3 = oprd.getrval (1);
    // set to natval if nat bit is set
    t_real fr;
    if ((f2.isnat () == true) || (f3.isnat () == true)) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute result
    fr.setsgfd (f3.getsgfd ());
    fr.setexp  (f2.getexp  ());
    fr.setsign ((f2.getsign () == false) ? false : true);
    result.setrval (0, fr);
    return result;
  }

  // F_FSXT_L
  static Result exec_fsxt_l (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f2 = oprd.getrval (0);
    t_real f3 = oprd.getrval (1);
    // set to natval if nat bit is set
    t_real fr;
    if ((f2.isnat () == true) || (f3.isnat () == true)) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute result
    t_octa sgfd = (f3.getsgfd () >> 32) & MASK_4;
    sgfd |= ((f2.getsgfd () & 0x8000000000000000ULL)== 0x8000000000000000ULL) ?
              0xFFFFFFFF00000000ULL : OCTA_0;
    fr.setinteger (sgfd);
    result.setrval (0, fr);
    return result;
  }

  // F_FSXT_R
  static Result exec_fsxt_r (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f2 = oprd.getrval (0);
    t_real f3 = oprd.getrval (1);
    // set to natval if nat bit is set
    t_real fr;
    if ((f2.isnat () == true) || (f3.isnat () == true)) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute result
    t_octa sgfd = f3.getsgfd () & MASK_4;
    sgfd |= ((f2.getsgfd () & 0x0000000080000000ULL)== 0x0000000080000000ULL) ?
            0xFFFFFFFF00000000ULL : OCTA_0;
    fr.setinteger (sgfd);
    result.setrval (0, fr);
    return result;
  }

  // ------------------------------------------------------------------------
  // - F10 instruction group                                                -
  // ------------------------------------------------------------------------

  // F_FCVT_FX
  static Result exec_fcvt_fx (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f0 = oprd.getrval (0);
    // set to natval if nat bit is set
    t_real fr;
    if (f0.isnat () == true) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // check for nan
    if (f0.isnan () == true) {
      fr.setinteger (0x8000000000000000ULL);
      result.setrval (0, fr);
      return result;
    }
    // convert to integer
    t_long lmax = 0x7FFFFFFFFFFFFFFFULL;
    t_long lmin = 0x8000000000000000ULL;
    long double fmax = (long double) lmax;
    long double fmin = (long double) lmin;
    if (((long double) f0 < fmin) || ((long double) f0 > fmax)) {
      fr.setinteger (0x8000000000000000ULL);
      result.setrval (0, fr);
      return result;
    }
    t_long rval = (t_long) ((long double) f0);
    if (f0.isint () == false) {
      Fpsr fpsr = oprd.getoval (1);
      t_fpcomp fpcomp = inst.getfpcomp ();
      t_byte rc  = BYTE_0;
      switch (fpcomp) {
      case S0:
	rc  = fpsr.getbsfld (Fpsr::SF0, Fpsr::RC);
	break;
      case S1:
	rc  = fpsr.getbsfld (Fpsr::SF1, Fpsr::RC);
      break;
      case S2:
	rc  = fpsr.getbsfld (Fpsr::SF2, Fpsr::RC);
	break;
      case S3:
	rc  = fpsr.getbsfld (Fpsr::SF3, Fpsr::RC);
	break;
      default:
	assert (false);
	break;
      }
      if ((rc == 0x01) && (f0.getsign () == true))  rval--;
      if ((rc == 0x02) && (f0.getsign () == false)) rval++;
    }
    fr.setinteger (rval);
    // set result
    result.setrval (0, fr);
    return result;
  }

  // F_FCVT_FX_TRUNC
  static Result exec_fcvt_fx_trunc (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f0 = oprd.getrval (0);
    // set to natval if nat bit is set
    t_real fr;
    if (f0.isnat () == true) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // check for nan
    if (f0.isnan () == true) {
      fr.setinteger (0x8000000000000000ULL);
      result.setrval (0, fr);
      return result;
    }
    // convert to integer
    t_long lmax = 0x7FFFFFFFFFFFFFFFULL;
    t_long lmin = 0x8000000000000000ULL;
    long double fmax = (long double) lmax;
    long double fmin = (long double) lmin;
    if (((long double) f0 < fmin) || ((long double) f0 > fmax)) {
      fr.setinteger (0x8000000000000000ULL);
      result.setrval (0, fr);
      return result;
    }
    t_long rval = (t_long) ((long double) f0);
    fr.setinteger (rval);
    // set result
    result.setrval (0, fr);
    return result;
  }

  // F_FCVT_FXU
  static Result exec_fcvt_fxu (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f0 = oprd.getrval (0);
    // set to natval if nat bit is set
    t_real fr;
    if (f0.isnat () == true) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // check for nan
    if (f0.isnan () == true) {
      fr.setinteger (0x8000000000000000ULL);
      result.setrval (0, fr);
      return result;
    }
    // convert to integer
    t_octa lmax = 0xFFFFFFFFFFFFFFFFULL;
    long double fmax = (long double) lmax;
    if (((long double) f0 < 0.0L) || ((long double) f0 > fmax)) {
      fr.setinteger (0x8000000000000000ULL);
      result.setrval (0, fr);
      return result;
    }
    t_octa rval = (t_octa) ((long double) f0);
    if (f0.isint () == false) {
      Fpsr fpsr = oprd.getoval (1);
      t_fpcomp fpcomp = inst.getfpcomp ();
      t_byte rc  = BYTE_0;
      switch (fpcomp) {
      case S0:
	rc  = fpsr.getbsfld (Fpsr::SF0, Fpsr::RC);
	break;
      case S1:
	rc  = fpsr.getbsfld (Fpsr::SF1, Fpsr::RC);
      break;
      case S2:
	rc  = fpsr.getbsfld (Fpsr::SF2, Fpsr::RC);
	break;
      case S3:
	rc  = fpsr.getbsfld (Fpsr::SF3, Fpsr::RC);
	break;
      default:
	assert (false);
	break;
      }
      if (rc == 0x02) rval++;
    }
    fr.setinteger (rval);
    // set result
    result.setrval (0, fr);
    return result;
  }

  // F_FCVT_FXU_TRUNC
  static Result exec_fcvt_fxu_trunc (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f0 = oprd.getrval (0);
    // set to natval if nat bit is set
    t_real fr;
    if (f0.isnat () == true) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // check for nan
    if (f0.isnan () == true) {
      fr.setinteger (0x8000000000000000ULL);
      result.setrval (0, fr);
      return result;
    }
    // convert to integer
    t_octa lmax = 0xFFFFFFFFFFFFFFFFULL;
    long double fmax = (long double) lmax;
    if (((long double) f0 < 0.0L) || ((long double) f0 > fmax)) {
      fr.setinteger (0x8000000000000000ULL);
      result.setrval (0, fr);
      return result;
    }
    t_octa rval = (t_octa) ((long double) f0);
    fr.setinteger (rval);
    // set result
    result.setrval (0, fr);
    return result;
  }

  // ------------------------------------------------------------------------
  // - F11 instruction group                                                -
  // ------------------------------------------------------------------------

  // F_FCVT_XF
  static Result exec_fcvt_xf (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat value
    t_real f0 = oprd.getrval (0);
    // set to natval if nat bit is set
    t_real fr;
    if (f0.isnat () == true) {
      fr.setnat ();
      result.setrval (0, fr);
      return result;
    }
    // compute result
    t_octa sgfd = f0.getsgfd ();
    if ((sgfd & SIGN_8) == SIGN_8) {
      fr.setinteger (~sgfd + 1);
      fr.setsign    (true);
    } else fr.setinteger (sgfd);
    fr.normalize ();
    result.setrval (0, fr);
    return result;
    }

  // execute a F unit instruction with operands

  Result Fexecute::exec (const Instr& inst, const Operand& oprd) const {
    Result result;
    if (inst.isvalid () == false) return result;
    // execute F specific instruction
    t_iopc iopc = inst.getiopc ();
    switch (iopc) {

      // F01 instruction group
    case F_FMA:
      result = exec_fma    (inst, oprd);
      break;
    case F_FMA_S:
      result = exec_fma_s  (inst, oprd);
      break;
    case F_FMA_D:
      result = exec_fma_d  (inst, oprd);
      break;
    case F_FNMA:
      result = exec_fnma   (inst, oprd);
      break;
    case F_FNMA_S:
      result = exec_fnma_s (inst, oprd);
      break;
    case F_FNMA_D:
      result = exec_fnma_d (inst, oprd);
      break;
    case F_FPMA:
      result = exec_fpma   (inst, oprd);
      break;
    case F_FPNMA:
      result = exec_fpnma  (inst, oprd);
      break;
    case F_FMS:
      result = exec_fms    (inst, oprd);
      break;
    case F_FMS_S:
      result = exec_fms_s  (inst, oprd);
      break;
    case F_FMS_D:
      result = exec_fms_d  (inst, oprd);
      break;
    case F_FPMS:
      result = exec_fpms   (inst, oprd);
      break;

      // F02 instruction group
    case F_XMA_L:
      result = exec_xma_l  (inst, oprd);
      break;
    case F_XMA_H:
      result = exec_xma_h  (inst, oprd);
      break;
    case F_XMA_HU:
      result = exec_xma_hu (inst, oprd);
      break;

      // F03 instruction group
    case F_FSELECT:
      result = exec_fselect (inst, oprd);
      break;

      // F04 instruction group
    case F_FCMP_EQ:
    case F_FCMP_EQ_UNC:
      result = exec_fcmp_eq    (inst, oprd);
      break;
    case F_FCMP_LT:
    case F_FCMP_LT_UNC:
      result = exec_fcmp_lt    (inst, oprd);
      break;
    case F_FCMP_LE:
    case F_FCMP_LE_UNC:
      result = exec_fcmp_le    (inst, oprd);
      break;
    case F_FCMP_UNORD:
    case F_FCMP_UNORD_UNC:
      result = exec_fcmp_unord (inst, oprd);
      break;

      // F05 instruction group
    case F_FCLASS_M:
    case F_FCLASS_M_UNC:
      result = exec_fclass_m (inst, oprd);
      break;

      // F06 instruction group
    case F_FRCPA:
      result = exec_frcpa (inst, oprd);
      break;

      // F07 instruction group
    case F_FRSQRTA:
      result = exec_frsqrta (inst, oprd);
      break;
    case F_FPRSQRTA:
      result = exec_fprsqrta (inst, oprd);
      break;

      // F08  instruction group
    case F_FMIN:
      result = exec_fmin (inst, oprd);
      break;
    case F_FMAX:
      result = exec_fmax (inst, oprd);
      break;
    case F_FAMIN:
      result = exec_famin (inst, oprd);
      break;
    case F_FAMAX:
      result = exec_famax (inst, oprd);
      break;
    case F_FPMIN:
      result = exec_fpmin (inst, oprd);
      break;
    case F_FPMAX:
      result = exec_fpmax (inst, oprd);
      break;
    case F_FPAMIN:
      result = exec_fpamin (inst, oprd);
      break;
    case F_FPAMAX:
      result = exec_fpamax (inst, oprd);
      break;
    case F_FPCMP_EQ:
      result = exec_fpcmp_eq    (inst, oprd);
      break;
    case F_FPCMP_LT:
      result = exec_fpcmp_lt    (inst, oprd);
      break;
    case F_FPCMP_LE:
      result = exec_fpcmp_le    (inst, oprd);
      break;
    case F_FPCMP_UNORD:
      result = exec_fpcmp_unord (inst, oprd);
      break;
    case F_FPCMP_NEQ:
      result = exec_fpcmp_neq   (inst, oprd);
      break;
    case F_FPCMP_NLT:
      result = exec_fpcmp_nlt   (inst, oprd);
      break;
    case F_FPCMP_NLE:
      result = exec_fpcmp_nle   (inst, oprd);
      break;
    case F_FPCMP_ORD:
      result = exec_fpcmp_ord   (inst, oprd);
      break;

      // F09 instruction group
    case F_FMERGE_NS:
      result = exec_fmerge_ns (inst, oprd);
      break;
    case F_FMERGE_S:
      result = exec_fmerge_s  (inst, oprd);
      break;
    case F_FMERGE_SE:
      result = exec_fmerge_se (inst, oprd);
      break;
    case F_FSXT_L:
      result = exec_fsxt_l    (inst, oprd);
      break;
    case F_FSXT_R:
      result = exec_fsxt_r    (inst, oprd);
      break;

      // F10 instruction group
    case F_FCVT_FX:
      result = exec_fcvt_fx        (inst, oprd);
      break;
    case F_FCVT_FX_TRUNC:
      result = exec_fcvt_fx_trunc  (inst, oprd);
      break;
    case F_FCVT_FXU:
      result = exec_fcvt_fxu       (inst, oprd);
      break;
    case F_FCVT_FXU_TRUNC:
      result = exec_fcvt_fxu_trunc (inst, oprd);
      break;

      // F11 instruction group
    case F_FCVT_XF:
      result = exec_fcvt_xf (inst, oprd);
      break;

      // F15 instruction group
    case F_NOP:
      result.setvalid (true);
      break;
    case F_BREAK:
      throw Interrupt (FAULT_IT_INST_BREAK, inst);
      break;
      
    default:
      break;
    }
    // check for valid result
    if (result.isvalid () == false) {
      string reason = "unimplemented instruction [F]";
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst, reason);
    }
    return result;
  }
}
