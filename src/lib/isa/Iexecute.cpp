// ---------------------------------------------------------------------------
// - Iexecute.cpp                                                            -
// - iato:isa library - I execution unit class implementation                -
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
#include "Iexecute.hpp"
#include "Uexecute.hxx"
#include "Exception.hpp"
#include "Interrupt.hpp"

namespace iato {

  // return the absolute value of src
  static inline t_long abs (t_long src) {
    return (src < 0) ? -src : src;
  }

  // ------------------------------------------------------------------------
  // - X02 instruction group                                                 -
  // ------------------------------------------------------------------------

  // X_MOVL instruction
  static Result exec_movl (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    result.setbval (0, false);
    result.setoval (0, inst.getimmv (0));
    return result;
  }

  // ------------------------------------------------------------------------
  // - I01 instruction group                                                 -
  // ------------------------------------------------------------------------

  // I_PMPYSHR2
  Result exec_pmpyshr2 (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    union{
      t_octa oval;
      t_word wval[4];
    } src1, src2, res;
    t_octa cnt = inst.getimmv (0);
    if ((cnt != 0) && (cnt != 7) && (cnt != 15) && (cnt != 16)) 
      assert (false);
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i = 0; i < 4; i++)
      res.wval[i]=(sign_extend(src1.wval[i])*sign_extend(src2.wval[i]))>>cnt;
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, natr);
    return result;
  }

  // I_PMPYSHR2_U
  Result exec_pmpyshr2_u (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    union{
      t_octa oval;
      t_word wval[4];
    } src1, src2, res;
    t_octa cnt = inst.getimmv (0);
    if ((cnt != 0) && (cnt != 7) && (cnt != 15) && (cnt != 16)) 
      assert (false);
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i = 0; i < 4; i++)
      res.wval[i]=((t_octa) src1.wval[i] * (t_octa) src2.wval[i]) >> cnt;
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, natr);
    return result;
  }

  // ------------------------------------------------------------------------
  // - I02 instruction group                                                 -
  // ------------------------------------------------------------------------

  // I_PMPY2_R
  Result exec_pmpy2_r (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    union{
      t_octa oval;
      t_word wval[4];
      t_quad qval[2];
    } src1, src2, res;
    src1.oval = lfixocta (oprd.getoval (0));
    src2.oval = lfixocta (oprd.getoval (1));
    for (long i=0; i < 2; i++)
      res.qval[i] = lfixquad ( sign_extend (lfixword (src1.wval[2*i])) * 
			       sign_extend (lfixword (src2.wval[2*i])));
    // set result
    result.setoval (0, lfixocta (res.oval));
    result.setbval (0, natr);
    return result;
  }

  // I_PMPY2_L
  static Result exec_pmpy2_l (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    union{
      t_octa oval;
      t_word wval[4];
      t_quad qval[2];
    } src1, src2, res;
    src1.oval = lfixocta (oprd.getoval (0));
    src2.oval = lfixocta (oprd.getoval (1));
    for (long i=0; i < 2; i++)
      res.qval[i] = lfixquad ( sign_extend (lfixword (src1.wval[(2*i) + 1])) *
			       sign_extend (lfixword (src2.wval[(2*i) + 1])));
    // set result
    result.setoval (0, lfixocta (res.oval));
    result.setbval (0, natr);
    return result;
  }

  // I_MIX1_R
  static Result exec_mix1_r (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = (oprd.getoval (0) & 0x00FF00FF00FF00FFULL) << 8;
    t_octa src2 = (oprd.getoval (1) & 0x00FF00FF00FF00FFULL);
    t_octa rval = src1 | src2;
    // set result
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // I_MIX2_R
  static Result exec_mix2_r (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = (oprd.getoval (0) & 0x0000FFFF0000FFFFULL) << 16;
    t_octa src2 = (oprd.getoval (1) & 0x0000FFFF0000FFFFULL);
    t_octa rval = src1 | src2;
    // set result
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // I_MIX4_R
  static Result exec_mix4_r (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = (oprd.getoval (0) & 0x00000000FFFFFFFFULL) << 32;
    t_octa src2 = (oprd.getoval (1) & 0x00000000FFFFFFFFULL);
    t_octa rval = src1 | src2;
    // set result
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // I_MIX1_L
  static Result exec_mix1_l (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = (oprd.getoval (0) & 0xFF00FF00FF00FF00ULL);
    t_octa src2 = (oprd.getoval (1) & 0xFF00FF00FF00FF00ULL) >> 8;
    t_octa rval = src1 | src2;
    // set result
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // I_MIX2_L
  static Result exec_mix2_l (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = (oprd.getoval (0) & 0xFFFF0000FFFF0000ULL);
    t_octa src2 = (oprd.getoval (1) & 0xFFFF0000FFFF0000ULL) >> 16;
    t_octa rval = src1 | src2;
    // set result
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // I_MIX4_L
  static Result exec_mix4_l (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = (oprd.getoval (0) & 0xFFFFFFFF00000000ULL);
    t_octa src2 = (oprd.getoval (1) & 0xFFFFFFFF00000000ULL) >> 32;
    t_octa rval = src1 | src2;
    // set result
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // I_PACK2_USS
  static Result exec_pack2_uss (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    union {
      t_octa oval;
      t_byte bval[8];
    } src2, src3, res;
    src2.oval = lfixocta (oprd.getoval (0));
    src3.oval = lfixocta (oprd.getoval (1));
    t_byte bmin = BYTE_0;
    t_byte bmax = BYTE_A;
    for (long i = 0, j=0; i < 4; i ++, j +=2) {
      if (src2.bval[j+1] == BYTE_0) res.bval[i] = src2.bval[j];
      else if ((src2.bval[j+1] & 0x80) == 0x80) res.bval[i] = bmin;
      else res.bval[i] = bmax;
      if (src3.bval[j+1] == BYTE_0) res.bval[4+i] = src3.bval[j];
      else if ((src3.bval[j+1] & 0x80) == 0x80) res.bval[4+i] = bmin;
      else res.bval[4+i] = bmax;
    }
    // set result
    result.setoval (0, lfixocta (res.oval));
    result.setbval (0, natr);
    return result;
  }

  // I_PACK2_SSS
  static Result exec_pack2_sss (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    union {
      t_octa oval;
      t_byte bval[8];
    } src2, src3, res;
    src2.oval = lfixocta (oprd.getoval (0));
    src3.oval = lfixocta (oprd.getoval (1));
    t_byte bmin = 0x80;
    t_byte bmax = 0x7f;
    for (long i = 0, j=0; i < 4; i ++, j +=2) {
      // compute low result bits
      if (src2.bval[j+1] == BYTE_0) {
      	if (src2.bval[j] > bmax) res.bval[i] = bmax;
	else  res.bval[i] = src2.bval[j];
      }
      else if (src2.bval[j+1] == BYTE_A) {
      	if (src2.bval[j] > bmax) res.bval[i] = src2.bval[j];
	else  res.bval[i] = bmin;
      }
      else if ((src2.bval[j+1] & 0x80) == 0x80) res.bval[i] = bmin;
      else res.bval[i] = bmax;
      // compute high result bits
      if (src3.bval[j+1] == BYTE_0) {
      	if (src3.bval[j] > bmax) res.bval[4+i] = bmax;
	else  res.bval[4+i] = src3.bval[j];
      }
      else if (src3.bval[j+1] == BYTE_A) {
      	if (src3.bval[j] > bmax) res.bval[4+i] = src3.bval[j];
	else  res.bval[4+i] = bmin;
      }
      else if ((src3.bval[j+1] & 0x80) == 0x80) res.bval[4+i] = bmin;
      else res.bval[4+i] = bmax;
    }
    // set result
    result.setoval (0, lfixocta (res.oval));
    result.setbval (0, natr);
    return result;
  }

  // I_PACK4_SSS
  static Result exec_pack4_sss (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    union {
      t_octa oval;
      t_word wval[4];
      t_byte bval[8];
    } src2, src3, res;
    src2.oval = lfixocta (oprd.getoval (0));
    src3.oval = lfixocta (oprd.getoval (1));
    t_word wmin = 0x8000;
    t_word wmax = 0x7fff;
    for (long i = 0, j=0; i < 2; i ++, j +=2) {
      // compute low result bits
      if (src2.wval[j+1] == WORD_0) {
      	if (lfixword (src2.wval[j]) > wmax) res.bval[i] = lfixword (wmax);
	else  res.wval[i] = src2.wval[j];
      }
      else if (src2.wval[j+1] == WORD_A) {
      	if (lfixword (src2.wval[j]) > wmax) res.wval[i] = src2.wval[j];
	else  res.wval[i] = lfixword (wmin);
      }
      else if ((lfixword (src2.wval[j+1]) & 0x8000) == 0x8000)
	res.wval[i] = lfixword (wmin);
      else res.wval[i] = lfixword (wmax);
      // compute high result bits
      if (src3.wval[j+1] == WORD_0) {
      	if (lfixword (src3.wval[j]) > wmax) res.bval[2+i] = lfixword (wmax);
	else  res.wval[2+i] = src3.wval[j];
      }
      else if (src3.wval[j+1] == WORD_A) {
      	if (lfixword (src3.wval[j]) > wmax) res.wval[2+i] = src3.wval[j];
	else  res.wval[2+i] = lfixword (wmin);
      }
      else if ((lfixword (src3.wval[j+1]) & 0x8000) == 0x8000)
	res.wval[2+i] = lfixword (wmin);
      else res.wval[2+i] = lfixword (wmax);
      // set result
    }
    result.setoval (0, lfixocta(res.oval));
    result.setbval (0, natr);
    return result;
  }
  
  // I_UNPACK1_H
  static Result exec_unpack1_h (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    t_octa tmp14 =  src1 & 0xFF00000000000000LL;
    t_octa tmp13 = (src1 & 0x00FF000000000000LL) >> 8;
    t_octa tmp12 = (src1 & 0x0000FF0000000000LL) >> 16;
    t_octa tmp11 = (src1 & 0x000000FF00000000LL) >> 24;
    t_octa tmp24 = (src2 & 0xFF00000000000000LL) >> 8;
    t_octa tmp23 = (src2 & 0x00FF000000000000LL) >> 16;
    t_octa tmp22 = (src2 & 0x0000FF0000000000LL) >> 24;
    t_octa tmp21 = (src2 & 0x000000FF00000000LL) >> 32;
    t_octa rval = tmp14 | tmp13 | tmp12 | tmp11 | tmp24 | tmp23 |tmp22 | tmp21;
    // set result
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // I_UNPACK2_H
  static Result exec_unpack2_h (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    t_octa tmp12 =  src1 & 0xFFFF000000000000LL;
    t_octa tmp11 = (src1 & 0x0000FFFF00000000LL) >> 16;
    t_octa tmp22 = (src2 & 0xFFFF000000000000LL) >> 16;
    t_octa tmp21 = (src2 & 0x0000FFFF00000000LL) >> 32;
    t_octa rval = tmp12 | tmp11 | tmp22 | tmp21;
    // set result
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // I_UNPACK4_H
  static Result exec_unpack4_h (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 =  oprd.getoval (0) & 0xFFFFFFFF00000000LL;
    t_octa src2 = (oprd.getoval (1) & 0xFFFFFFFF00000000LL) >> 32;
    t_octa rval = src2 | src1;
    // set result
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // I_UNPACK1_L
  static Result exec_unpack1_l (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    t_octa tmp14 = (src1 & 0x00000000FF000000LL) << 32;
    t_octa tmp13 = (src1 & 0x0000000000FF0000LL) << 24;
    t_octa tmp12 = (src1 & 0x000000000000FF00LL) << 16;
    t_octa tmp11 = (src1 & 0x00000000000000FFLL) << 8;
    t_octa tmp24 = (src2 & 0x00000000FF000000LL) << 24;
    t_octa tmp23 = (src2 & 0x0000000000FF0000LL) << 16;
    t_octa tmp22 = (src2 & 0x000000000000FF00LL) << 8;
    t_octa tmp21 =  src2 & 0x00000000000000FFLL;
    t_octa rval = tmp14 | tmp13 | tmp12 | tmp11
      | tmp24 | tmp23 |tmp22 | tmp21;
    // set result
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // I_UNPACK2_L
  static Result exec_unpack2_l (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    t_octa tmp12 = (src1 & 0x00000000FFFF0000LL) << 32;
    t_octa tmp11 = (src1 & 0x000000000000FFFFLL) << 16;
    t_octa tmp22 = (src2 & 0x00000000FFFF0000LL) << 16;
    t_octa tmp21 =  src2 & 0x000000000000FFFFLL;
    t_octa rval = tmp12 | tmp11 | tmp22 | tmp21;
    // set result
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // I_UNPACK4_L
  static Result exec_unpack4_l (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = (oprd.getoval (0) & 0x00000000FFFFFFFFLL) << 32;
    t_octa src2 =  oprd.getoval (1) & 0x00000000FFFFFFFFLL;
    t_octa rval = src2 | src1;
    // set result
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // I_PMIN1_U:
  static Result exec_pmin1_u (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    union {
      t_octa oval;
      t_byte bval[8];
    }src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i = 0; i < 8; i++)
      res.bval[i] = (src1.bval[i] < src2.bval[i]) ? src1.bval[i] : src2.bval[i];
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, natr);
    return result;
  }

  // I_PMAX1_U:
  static Result exec_pmax1_u (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    union {
      t_octa oval;
      t_byte bval[8];
    }src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i = 0; i < 8; i++)
      res.bval[i] = (src1.bval[i] > src2.bval[i]) ? src1.bval[i] : src2.bval[i];
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, natr);
    return result;
  }

  // I_PMIN2:
  static Result exec_pmin2 (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    union{
      t_octa oval;
      t_word wval[4];
    }src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i = 0; i < 4; i++)
      res.wval[i] = (sign_extend (src1.wval[i]) < sign_extend (src2.wval[i])) 
	? src1.wval[i] : src2.wval[i];
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, natr);
    return result;
  }

  // I_PMAX2:
  static Result exec_pmax2 (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    union{
      t_octa oval;
      t_word wval[4];
    } src1, src2, res;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i = 0; i < 4; i++)
      res.wval[i] = (sign_extend (src1.wval[i]) > sign_extend (src2.wval[i])) 
	? src1.wval[i] : src2.wval[i];
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, natr);
    return result;
  }

  // I_PSAD1
  static Result exec_psad1 (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    union {
      t_octa oval;
      t_byte bval[8];
    }src1, src2;
    t_octa res = OCTA_0;
    src1.oval = oprd.getoval (0);
    src2.oval = oprd.getoval (1);
    for (long i =0; i < 8; i++) res += abs (src1.bval[i] - src2.bval[i]);
    // set result
    result.setoval (0, res);
    result.setbval (0, natr);
    return result;
  }

  // ------------------------------------------------------------------------
  // - I03 instruction group                                                 -
  // ------------------------------------------------------------------------

  // I_MUX1
  static Result exec_mux1 (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = oprd.getbval (0);
    result.setbval (0, natr);
    // get source value and fix it
    union {
      t_byte bval[8];
      t_octa oval;
    } src, dst;
    src.oval = lfixocta (oprd.getoval (0));
    // compute result based on operation
    t_octa oper = inst.getimmv (0);
    // check for broadcast
    if (oper == 0x0000000000000000ULL) {
      for (long i = 0; i < 8; i++) dst.bval[i] = src.bval[0];
    }
    // check for mix
    if (oper == 0x0000000000000008ULL) {
      dst.bval[0] = src.bval[0];
      dst.bval[1] = src.bval[4];
      dst.bval[2] = src.bval[2];
      dst.bval[3] = src.bval[6];
      dst.bval[4] = src.bval[1];
      dst.bval[5] = src.bval[5];
      dst.bval[6] = src.bval[3];
      dst.bval[7] = src.bval[7];
    }
    // check for shuff
    if (oper == 0x0000000000000009ULL) {
      dst.bval[0] = src.bval[0];
      dst.bval[1] = src.bval[4];
      dst.bval[2] = src.bval[1];
      dst.bval[3] = src.bval[5];
      dst.bval[4] = src.bval[2];
      dst.bval[5] = src.bval[6];
      dst.bval[6] = src.bval[3];
      dst.bval[7] = src.bval[7];
    }
    // check for alt
    if (oper == 0x000000000000000AULL) {
      dst.bval[0] = src.bval[0];
      dst.bval[1] = src.bval[2];
      dst.bval[2] = src.bval[4];
      dst.bval[3] = src.bval[6];
      dst.bval[4] = src.bval[1];
      dst.bval[5] = src.bval[3];
      dst.bval[6] = src.bval[5];
      dst.bval[7] = src.bval[7];
    }
    // check for rev
    if (oper == 0x000000000000000BULL) {
      dst.bval[0] = src.bval[7];
      dst.bval[1] = src.bval[6];
      dst.bval[2] = src.bval[5];
      dst.bval[3] = src.bval[4];
      dst.bval[4] = src.bval[3];
      dst.bval[5] = src.bval[2];
      dst.bval[6] = src.bval[1];
      dst.bval[7] = src.bval[0];
    }
    t_octa data = lfixocta (dst.oval);
    result.setoval (0, data);
    return result;
  }

  // ------------------------------------------------------------------------
  // - I04 instruction group                                                 -
  // ------------------------------------------------------------------------

  // I_MUX2
  Result exec_mux2 (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat = oprd.getbval (0);
    // compute result
    union{
      t_octa oval;
      t_word wval[4];
    } src, res;
    t_byte immv = inst.getimmv (0);
    t_byte pos1 = immv & 0x03;
    t_byte pos2 = (immv & 0x0c) >> 2;
    t_byte pos3 = (immv & 0x30) >> 4;
    t_byte pos4 = (immv & 0xc0) >> 6;
    src.oval = lfixocta (oprd.getoval (0));
    res.wval[0] = src.wval[pos1];
    res.wval[1] = src.wval[pos2];
    res.wval[2] = src.wval[pos3];
    res.wval[3] = src.wval[pos4];
    //set result
    result.setoval (0, lfixocta (res.oval));
    result.setbval (0, nat);
    return result;
  }

  // ------------------------------------------------------------------------
  // - I05 instruction group                                                 -
  // ------------------------------------------------------------------------

  // I_PSHR2
  static Result exec_pshr2 (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = oprd.getoval (0);
    union{
      t_octa oval;
      t_word wval[4];
    }src2, res;
    src2.oval = oprd.getoval (1);
    if (src1 > 15) src1 = 16;
    for (long i = 0; i < 4; i++) {
      t_octa tmpsrc2 = sign_extend (src2.wval[i]);
      res.wval[i] = tmpsrc2 >> src1;
    }
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, natr);
    return result;
  }

  // I_PSHR4
  static Result exec_pshr4 (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = oprd.getoval (0);
    union{
      t_octa oval;;
      t_quad qval[2];
    }src2, res;
    src2.oval = oprd.getoval (1);
    if (src1 > 32) src1 = 32;
    for (long i = 0; i < 2; i++) {
      t_octa tmpsrc2 = sign_extend (src2.qval[i]);
      res.qval[i] = tmpsrc2 >> src1;
    }
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, natr);
    return result;
 }

  // I_SHR
  static Result exec_shr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = oprd.getoval (0);
    t_octa rval = oprd.getoval (1);
    if (src1 > 63)
      rval = (get_sign (rval) == true) ? OCTA_A : OCTA_0;
    else rval = (get_sign (rval) == true) ?
	   rval >> src1 | (MASK_8 << (64 - src1)) : rval >> src1;
    // set result
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
 }

  // I_PSHR2_U
  static Result exec_pshr2_u (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = oprd.getoval (0);
    union{
      t_octa oval;
      t_word wval[4];
    } src2, res;
    src2.oval = oprd.getoval (1);
    if (src1 > 15) res.oval = OCTA_0;
    else for (long i = 0; i < 4; i++) res.wval[i] = src2.wval[i] >> src1;
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, natr);
    return result;
  }

  // I_PSHR4_U
  static Result exec_pshr4_u (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = oprd.getoval (0);
    union{
      t_octa oval;
      t_quad qval[2];
    } src2, res;
    src2.oval = oprd.getoval (1);
    if (src1 > 31) res.oval = OCTA_0;
    else for (long i = 0; i < 2; i++) res.qval[i] = src2.qval[i] >> src1;
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, natr);
    return result;
  }

  // I_SHR_U
  static Result exec_shr_u (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src1 = oprd.getoval (0);
    t_octa rval = oprd.getoval (1);
    if (src1 > 63) rval = OCTA_0;
    else rval = rval >> src1;
    // set result
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // ------------------------------------------------------------------------
  // - I06 instruction group                                                 -
  // ------------------------------------------------------------------------

  // I_PSHR2_I
  static Result exec_pshr2_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (1);
    // compute result
    t_octa shc = inst.getimmv (0);
    union{
      t_octa oval;
      t_word wval[4];
    }src2, res;
    src2.oval = oprd.getoval (1);
    if (shc > 15) shc = 16;
    for (long i = 0; i < 4; i++) res.wval[i] = sign_extend (src2.wval[i]) >> shc;
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, nat1);
    return result;
  }

  // I_PSHR4_I
  static Result exec_pshr4_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (1);
    // compute result
    t_octa shc = inst.getimmv (0);
    union{
      t_octa oval;
      t_quad qval[2];
    }src2, res;
    src2.oval = oprd.getoval (1);
    if (shc > 32) shc = 32;
    for (long i = 0; i < 2; i++) res.qval[i] = sign_extend (src2.qval[i]) >> shc;
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, nat1);
    return result;
 }

  // I_PSHR2_U_I
  static Result exec_pshr2_u_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (1);
    // compute result
    t_octa shc = inst.getimmv (0);
    union{
      t_octa oval;
      t_word wval[4];
    } src2, res;
    src2.oval = oprd.getoval (1);
    if (shc > 15) res.oval = OCTA_0;
    else for (long i = 0; i < 4; i++) res.wval[i] = src2.wval[i] >> shc;
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, nat1);
    return result;
  }

  // I_PSHR4_U_I
  static Result exec_pshr4_u_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (1);
    // compute result
    t_octa shc = inst.getimmv (0);
    union{
      t_octa oval;
      t_quad qval[2];
    } src2, res;
    src2.oval = oprd.getoval (1);
    if (shc > 31) res.oval = OCTA_0;
    else for (long i = 0; i < 2; i++) res.qval[i] = src2.qval[i] >> shc;
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, nat1);
    return result;
  }

  // ------------------------------------------------------------------------
  // - I07 instruction group                                                 -
  // ------------------------------------------------------------------------

  // I_PSHL2
  static Result exec_pshl2 (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src2 = oprd.getoval (1);
    union{
      t_octa oval;
      t_word wval[4];
    } src1, res;
    src1.oval = oprd.getoval (0);
    for (long i = 0; i < 4; i++) {
      if (src2 > 15) res.wval[i] = WORD_0;
      else res.wval[i] = src1.wval[i] << src2;
    }
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, natr);
    return result;
  }

  // I_PSHL4
  static Result exec_pshl4 (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa src2 = oprd.getoval (1);
    union{
      t_octa oval;
      t_quad qval[2];
    } src1, res;
    src1.oval = oprd.getoval (0);
    for (long i = 0; i < 2; i++) {
      if (src2 > 31) res.qval[i] = QUAD_0;
      else res.qval[i] = src1.qval[i] << src2;
    }
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, natr);
    return result;
  }

  // I_SHL
  static Result exec_shl (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 | nat2;
    // compute result
    t_octa rval = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    if (src2 > 63) rval = OCTA_0;
    else rval = rval << src2;
    // set result
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // ------------------------------------------------------------------------
  // - I08 instruction group                                                 -
  // ------------------------------------------------------------------------

  // I_PSHL2_I
  static Result exec_pshl2_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    // compute result
    t_octa shc = inst.getimmv (0);
    union{
      t_octa oval;
      t_word wval[4];
    } src1, res;
    src1.oval = oprd.getoval (0);
    if (shc > 15) res.oval = OCTA_0;
    else for (long i = 0; i < 4; i++)
      res.wval[i] = src1.wval[i] << shc;
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, nat1);
    return result;
  }

  // I_PSHL4_I
  static Result exec_pshl4_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    // compute result
    t_octa shc = inst.getimmv (0);
    union{
      t_octa oval;
      t_quad qval[2];
    } src1, res;
    src1.oval = oprd.getoval (0);
    if (shc > 31) res.oval = OCTA_0;
    else for (long i = 0; i < 2; i++)
      res.qval[i] = src1.qval[i] << shc;
    // set result
    result.setoval (0, res.oval);
    result.setbval (0, nat1);
    return result;
  }

  // ------------------------------------------------------------------------
  // - I09 instruction group                                                 -
  // ------------------------------------------------------------------------

  // I_POPCNT
  static Result exec_popcnt (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute result
    t_octa src = oprd.getoval (1);
    t_byte res = 0;
    t_octa mask = OCTA_1;
    for (long i = 0; i < 64; i++){
      if ((src & mask) == mask)
	res ++;
      mask = mask << 1;
    }
    result.setoval (0, res);
    result.setbval (0, natr);
    return result;
  }

  // ------------------------------------------------------------------------
  // - I10 instruction group                                                 -
  // ------------------------------------------------------------------------

  // I_SHRP
  static Result exec_shrp (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool nat1 = oprd.getbval (0);
    bool nat2 = oprd.getbval (1);
    bool natr = nat1 || nat2;
    // compute result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = oprd.getoval (1);
    t_octa shc  = inst.getimmv (0);
    t_huge value (src1, src2);
    value = value >> shc;
    t_octa res = value.getlow ();
    result.setoval (0, res);
    result.setbval (0, natr);
    return result;
  }

  // ------------------------------------------------------------------------
  // - I11 instruction group                                                -
  // ------------------------------------------------------------------------

  // I_EXTR:
  static Result exec_extr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute result
    t_octa src  = oprd.getoval (1);
    long   pos  = inst.getimmv (0);
    long   len  = inst.getimmv (1);
    t_octa rval = OCTA_0;
    for (int i = 0; i < 64; i++)
      if ((i < len) && (pos + i < 64))
	rval = bsetocta (rval, i, bsetget (src, pos + i));
      else {
	if (bsetget (rval, i-1) == true)
	  rval |= OCTA_A << i;
	break;
      }
    // set result
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // I_EXTR_U
  static Result exec_extr_u (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute result
    t_octa src = oprd.getoval (1);
    long   pos = inst.getimmv (0);
    long   len = inst.getimmv (1);
    t_octa rval = OCTA_0;
    for (int i = 0; ((i < len) && (pos + i < 64)); i++)
      rval = bsetocta (rval, i, bsetget (src, pos + i));
    // set result
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // ------------------------------------------------------------------------
  // - I12 instruction group                                                -
  // ------------------------------------------------------------------------

  // I_DEP_Z
  static Result exec_dep_z (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = oprd.getbval (0);
    // compute result
    t_octa src  = oprd.getoval (0);
    long  pos   = inst.getimmv (0);
    long  len   = inst.getimmv (1);
    t_octa rval = OCTA_0;
    for (int i = 0; ((i < len) && (pos + i < 64)); i++)
      rval = bsetocta (rval, pos + i, bsetget (src, i));
    // set result
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // ------------------------------------------------------------------------
  // - I13 instruction group                                                -
  // ------------------------------------------------------------------------

  // I_DEP_Z_I
  static Result exec_dep_z_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa src  = inst.getimmv (0);
    long   pos  = inst.getimmv (1);
    long   len  = inst.getimmv (2);
    t_octa rval = OCTA_0;
    for (int i = 0; ((i < len) && (pos + i < 64)); i++)
      rval = bsetocta (rval, pos + i, bsetget (src, i));
    // set result
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }
  // ------------------------------------------------------------------------
  // - I14 instruction group                                                -
  // ------------------------------------------------------------------------

  // I_DEP_I
  static Result exec_dep_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute result
    t_octa rval = oprd.getoval (1);
    long   pos  = inst.getimmv (0);
    long   len  = inst.getimmv (1);
    t_long imm  = inst.getimmv (2);
    t_octa src = OCTA_0;
    if (imm == -1) src = OCTA_A;
    for (int i = 0; ((i < len) && (pos + i < 64)); i++)
      rval = bsetocta (rval, pos + i, bsetget (src, i));
    // set result
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // ------------------------------------------------------------------------
  // - I15 instruction group                                                -
  // ------------------------------------------------------------------------

  // I_DEP
  static Result exec_dep (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute result
    t_octa  src = oprd.getoval (0);
    t_octa rval = oprd.getoval (1); 
    long    pos = inst.getimmv (1);
    long    len = inst.getimmv (0);
    for (int i = 0; ((i < len) && (pos + i < 64)); i++)
      rval = bsetocta (rval, pos + i, bsetget (src, i));
    // set result
    result.setoval (0, rval);
    result.setbval (0, natr);
    return result;
  }

  // ------------------------------------------------------------------------
  // - I16 instruction group                                                -
  // ------------------------------------------------------------------------

  // I_TBIT_Z and I_TBIT_Z_UNC
  static Result exec_tbit_z (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for interuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
       throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // check for result clear with unc
    if ((inst.getiopc () == I_TBIT_Z_UNC) && (oprd.getbval (2) == false)) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compute result bit value
    bool   natr = oprd.getbval (1);
    t_octa oval = oprd.getoval (1);
    long   bpos = inst.getimmv (0);
    bool   trel = bsetget (oval, bpos);
    // set result from trel or nat bit
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
    } else {
      result.setbval (0, !trel);
      result.setbval (1,  trel);
    }
    return result;
  }

  // I_TBIT_Z_AND
  static Result exec_tbit_z_and (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for interuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
      // compute result bit value
    bool   natr = oprd.getbval (1);
    t_octa oval = oprd.getoval (1);
    long   bpos = inst.getimmv (0);
    bool   trel = bsetget (oval, bpos);
    // set result from trel or nat bit
    if ((natr == true) | (trel == true)) {
      result.setbval (0, false);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // I_TBIT_NZ_AND
  static Result exec_tbit_nz_and (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for interuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
      // compute result bit value
    bool   natr = oprd.getbval (1);
    t_octa oval = oprd.getoval (1);
    long   bpos = inst.getimmv (0);
    bool   trel = bsetget (oval, bpos);
    // set result from trel or nat bit
    if ((natr == true) | (trel == false)) {
      result.setbval (0, false);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // I_TBIT_Z_OR
  static Result exec_tbit_z_or (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for interuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
      // compute result bit value
    bool   natr = oprd.getbval (1);
    t_octa oval = oprd.getoval (1);
    long   bpos = inst.getimmv (0);
    bool   trel = bsetget (oval, bpos);
    // set result from trel or nat bit
    if ((natr == false) & (trel == false)) {
      result.setbval (0, true);
      result.setbval (1, true);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // I_TBIT_NZ_OR
  static Result exec_tbit_nz_or (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for interuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
      // compute result bit value
    bool   natr = oprd.getbval (1);
    t_octa oval = oprd.getoval (1);
    long   bpos = inst.getimmv (0);
    bool   trel = bsetget (oval, bpos);
    // set result from trel or nat bit
    if ((natr == false) & (trel == true)) {
      result.setbval (0, true);
      result.setbval (1, true);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // I_TBIT_Z_OR_ANDCM
  static Result exec_tbit_z_or_andcm (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for interuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
      // compute result bit value
    bool   natr = oprd.getbval (1);
    t_octa oval = oprd.getoval (1);
    long   bpos = inst.getimmv (0);
    bool   trel = bsetget (oval, bpos);
    // set result from trel or nat bit
    if ((natr == false) & (trel == false)) {
      result.setbval (0, true);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // I_TBIT_NZ_OR_ANDCM
  static Result exec_tbit_nz_or_andcm (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for interuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
      // compute result bit value
    bool   natr = oprd.getbval (1);
    t_octa oval = oprd.getoval (1);
    long   bpos = inst.getimmv (0);
    bool   trel = bsetget (oval, bpos);
    // set result from trel or nat bit
    if ((natr == false) & (trel == true)) {
      result.setbval (0, true);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // ------------------------------------------------------------------------
  // - I17 instruction group                                                -
  // ------------------------------------------------------------------------

  // I_TNAT_Z and I_TNAT_Z_UNC
  static Result exec_tnat_z (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for interuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
       throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
    // check for result clear with unc
    if ((inst.getiopc () == I_TNAT_Z_UNC) && (oprd.getbval (2) == false)) {
      result.setbval (0, false);
      result.setbval (1, false);
      return result;
    }
    // compute result bit value
    bool   natr = oprd.getbval (1);
    // set result from nat bit
    result.setbval (0, !natr);
    result.setbval (1, natr);
    return result;
  }

  // I_TNAT_Z_AND
  static Result exec_tnat_z_and (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for interuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
      // compute result bit value
    bool   natr = oprd.getbval (1);
    // set result from nat bit
    if (natr == true) {
      result.setbval (0, false);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // I_TNAT_NZ_AND
  static Result exec_tnat_nz_and (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for interuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
      // compute result bit value
    bool   natr = oprd.getbval (1);
    // set result from trel or nat bit
    if (natr == false) {
      result.setbval (0, false);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // I_TNAT_Z_OR
  static Result exec_tnat_z_or (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for interuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
      // compute result bit value
    bool   natr = oprd.getbval (1);
    // set result from nat bit
    if (natr == false) {
      result.setbval (0, true);
      result.setbval (1, true);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // I_TNAT_NZ_OR
  static Result exec_tnat_nz_or (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for interuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
      // compute result bit value
    bool   natr = oprd.getbval (1);
    // set result from nat bit
    if (natr == true) {
      result.setbval (0, true);
      result.setbval (1, true);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // I_TNAT_Z_OR_ANDCM
  static Result exec_tnat_z_or_andcm (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for interuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
      // compute result bit value
    bool   natr = oprd.getbval (1);
    // set result from nat bit
    if (natr == false) {
      result.setbval (0, true);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // I_TNAT_NZ_OR_ANDCM
  static Result exec_tnat_nz_or_andcm (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for interuption
    if (result.getrid (0).getlnum () == result.getrid (1).getlnum ())
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst);
      // compute result bit value
    bool   natr = oprd.getbval (1);
    // set result from nat bit
    if (natr == true) {
      result.setbval (0, true);
      result.setbval (1, false);
    } else {
      result.setrrt  (0, false);
      result.setrrt  (1, false);
    }
    return result;
  }

  // ------------------------------------------------------------------------
  // - I20 instruction group                                                -
  // ------------------------------------------------------------------------

  // I_CHK_S_I
  static Result exec_chk_s_i (const Instr& inst, const Operand& oprd) {
    // get result
    Result result = inst.getresl ();
    // do nothing if the nat bit is not set
    bool natr = oprd.getbval (0);
    if (natr == false) {
      result.setvalid (true);
      return result;
    }
    // compute new ip with displacement
    t_long ip = inst.getiip  ();
    t_long rd = inst.getimmv (0);
    t_octa rp = ip + rd;
    // set result
    result.setoval (0, rp);
    return result;
  }

  // ------------------------------------------------------------------------
  // - I21 instruction group                                                -
  // ------------------------------------------------------------------------

  // I_MOV_TO_B
  static Result exec_mov_to_b (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = oprd.getbval (0);
    // if natr the unimplemented Register NaT Consumption fault
    assert (natr == false);
    // compute result
    t_octa rval = oprd.getoval (0);
    // set result
    result.setoval (0, rval);
    return result;
  }

  // I_MOV_RET_TO_B
  static Result exec_mov_ret_to_b(const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = oprd.getbval (0);
    // if natr the unimplemented Register NaT Consumption fault
    assert (natr == false);
    // compute result
    t_octa rval = oprd.getoval (0);
    // set result
    result.setoval (0, rval);
    return result;
  }

  // ------------------------------------------------------------------------
  // - I22 instruction group                                                -
  // ------------------------------------------------------------------------

  // I_MOV_FROM_B
  static Result exec_mov_from_b (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
     // compute result
    t_octa rval = oprd.getoval (0);
    // set result
    result.setoval (0, rval);
    return result;
  }

  // ------------------------------------------------------------------------
  // - I23 instruction group                                                -
  // ------------------------------------------------------------------------

  // I_MOV_TO_PR
  static Result exec_mov_to_pr(const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = oprd.getbval (0);
    // if natr the unimplemented Register NaT Consumption fault
    assert (natr == false);
    // compute result
    t_octa src1 = oprd.getoval (0);
    t_octa src2 = inst.getimmv (0);
    t_octa rval = oprd.getoval (1);
    rval &= ~src2;
    rval |= (src1 & src2);
    // set result
    result.setoval (0, rval);
    return result;
  }

  // ------------------------------------------------------------------------
  // - I24 instruction group                                                -
  // ------------------------------------------------------------------------
 
  // I_MOV_TO_PR_ROT
  static Result exec_mov_to_pr_rot (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute result
    t_octa rval = inst.getimmv (0);
    // set result
    result.setoval (0, rval);
    return result;
  }

  // ------------------------------------------------------------------------
  // - I25 instruction group                                                -
  // ------------------------------------------------------------------------

  // I_MOV_FROM_IP
  static Result exec_mov_from_ip (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa rval = inst.getiip ();
    // set result 
    result.setbval (0, natr);
    result.setoval (0, rval);
    return result;
  }

  // I_MOV_FROM_PR
  static Result exec_mov_from_pr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa rval = oprd.getoval (0);
    // set result
    result.setbval (0, natr);
    result.setoval (0, rval);
    return result;
  }

  // ------------------------------------------------------------------------
  // - I26 instruction group                                                -
  // ------------------------------------------------------------------------

  // I_MOV_TO_AR_R
  static Result exec_mov_to_ar_r (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for nat bit and throw a register nat consumption fault
    bool nat = oprd.getbval (0); 
    if (nat == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa rval = oprd.getoval (0);
    // set result 
    result.setoval (0, rval);
    return result;
  }

  // ------------------------------------------------------------------------
  // - I27 instruction group                                                -
  // ------------------------------------------------------------------------

  // I_MOV_TO_AR_I
  static Result exec_mov_to_ar_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute result
    t_octa rval = inst.getimmv (0);
    // set result 
    result.setoval (0, rval);
    return result;
  }

  // ------------------------------------------------------------------------
  // - I28 instruction group                                                -
  // ------------------------------------------------------------------------

  // I_MOV_FROM_AR:
  static Result exec_mov_from_ar (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute result
    t_octa rval = oprd.getoval (1);
    // set result
    result.setbval (0, false); 
    result.setoval (0, rval);
    return result;
  }

  // ------------------------------------------------------------------------
  // - I29 instruction group                                                -
  // ------------------------------------------------------------------------

  // I_ZXT1:
  static Result exec_zxt1 (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute mask and result
    t_octa rval = (oprd.getoval (1) & MASK_1);
    // set result 
    result.setbval (0, natr);
    result.setoval (0, rval);
    return result;
  }

  // I_ZXT2
  static Result exec_zxt2 (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute mask and result
    t_octa rval = (oprd.getoval (1) & MASK_2);
    // set result 
    result.setbval (0, natr);
    result.setoval (0, rval);
    return result;
  }

  // I_ZXT4
  static Result exec_zxt4 (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute mask and result
    t_octa rval = (oprd.getoval (1) & MASK_4);
    // set result 
    result.setbval (0, natr);
    result.setoval (0, rval);
    return result;
  }

  // I_SXT1:
  static Result exec_sxt1 (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute mask and result
    union {
      t_octa rval;
      t_byte bval[8];
    };
    rval = lfixocta (oprd.getoval (1));
    // set result 
    result.setbval (0, natr);
    result.setoval (0, sign_extend (bval[0]));
    return result;
  }

  // I_SXT2
  static Result exec_sxt2 (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute mask and result
    union {
      t_octa rval;
      t_word wval[4];
    };
    rval = lfixocta (oprd.getoval (1));
    // set result 
    result.setbval (0, natr);
    result.setoval (0, sign_extend (lfixword (wval[0])));
    return result;
  }

  // I_SXT4
  static Result exec_sxt4 (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute mask and result
    union {
      t_octa rval;
      t_quad qval[2];
    };
    rval = lfixocta (oprd.getoval (1));
    // set result 
    result.setbval (0, natr);
    result.setoval (0, sign_extend (lfixquad (qval[0])));
    return result;
  }

  // I_CZX1_L:
  static Result exec_czx1_l (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    t_octa rval;
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute result
    union{
      t_octa oval;
      t_byte bval[8];
    };
    oval = lfixocta (oprd.getoval (1));
    if      (bval[7] == BYTE_0) rval = 0;
    else if (bval[6] == BYTE_0) rval = 1;
    else if (bval[5] == BYTE_0) rval = 2;
    else if (bval[4] == BYTE_0) rval = 3;
    else if (bval[3] == BYTE_0) rval = 4;
    else if (bval[2] == BYTE_0) rval = 5;
    else if (bval[1] == BYTE_0) rval = 6;
    else if (bval[0] == BYTE_0) rval = 7;
    else rval = 8;
    // set result 
    result.setbval (0, natr);
    result.setoval (0, rval);
    return result;
  }

  // I_CZX2_L:
  static Result exec_czx2_l (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    t_octa rval;
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute result
    union{
      t_octa oval;
      t_word bval[4];
    };
    oval = lfixocta (oprd.getoval (1));
    if      (bval[3] == WORD_0) rval = 0;
    else if (bval[2] == WORD_0) rval = 1;
    else if (bval[1] == WORD_0) rval = 2;
    else if (bval[0] == WORD_0) rval = 3;
    else rval = 4;
    // set result 
    result.setbval (0, natr);
    result.setoval (0, rval);
    return result;
  }

  // I_CZX1_R:
  static Result exec_czx1_r (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    t_octa rval;
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute result
    union{
      t_octa oval;
      t_byte bval[8];
    };
    oval = lfixocta (oprd.getoval (1));
    if      (bval[0] == BYTE_0) rval = 0;
    else if (bval[1] == BYTE_0) rval = 1;
    else if (bval[2] == BYTE_0) rval = 2;
    else if (bval[3] == BYTE_0) rval = 3;
    else if (bval[4] == BYTE_0) rval = 4;
    else if (bval[5] == BYTE_0) rval = 5;
    else if (bval[6] == BYTE_0) rval = 6;
    else if (bval[7] == BYTE_0) rval = 7;
    else rval = 8;
    // set result 
    result.setbval (0, natr);
    result.setoval (0, rval);
    return result;
  }

  // I_CZX2_R:
  static Result exec_czx2_r (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    t_octa rval;
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute result
    union{
      t_octa oval;
      t_word bval[4];
    };
    oval = lfixocta (oprd.getoval (1));
    if      (bval[0] == WORD_0) rval = 0;
    else if (bval[1] == WORD_0) rval = 1;
    else if (bval[2] == WORD_0) rval = 2;
    else if (bval[3] == WORD_0) rval = 3;
    else rval = 4;
    // set result 
    result.setbval (0, natr);
    result.setoval (0, rval);
    return result;
  }

  // execute a I unit instruction with operands

  Result Iexecute::exec (const Instr& inst, const Operand& oprd) const {
    Result result;
    if (inst.isvalid () == false) return result;
    // check the A unit first
    result = Aexecute::exec (inst, oprd);
    if (result.isvalid () == true) return result;
    // execute I specific instruction
    t_iopc iopc = inst.getiopc ();
    switch (iopc) {

      // X02 instruction group
    case X_MOVL:
      result = exec_movl (inst, oprd);
      break;

      // I01 instruction group
    case I_PMPYSHR2:
      result = exec_pmpyshr2 (inst, oprd);
      break;
    case I_PMPYSHR2_U:
      result = exec_pmpyshr2_u (inst, oprd);
      break;

      // I02 instruction group
    case I_PMPY2_R:
      result = exec_pmpy2_r (inst, oprd);
      break;
    case I_PMPY2_L:
      result = exec_pmpy2_l (inst, oprd);
      break;
    case I_MIX1_R:
      result = exec_mix1_r (inst, oprd);
      break;
    case I_MIX2_R:
      result = exec_mix2_r (inst, oprd);
      break;
    case I_MIX4_R:
      result = exec_mix4_r (inst, oprd);
      break;  
    case I_MIX1_L:
      result = exec_mix1_l (inst, oprd);
      break;
    case I_MIX2_L:
      result = exec_mix2_l (inst, oprd);
      break;
    case I_MIX4_L:
      result = exec_mix4_l (inst, oprd);
      break;
    case I_PACK2_USS:
      result = exec_pack2_uss (inst, oprd);
      break;
    case I_PACK2_SSS:
      result = exec_pack2_sss (inst, oprd);
      break;
    case I_PACK4_SSS:
      result = exec_pack4_sss (inst, oprd);
      break;
    case I_UNPACK1_H:
      result = exec_unpack1_h (inst, oprd);
      break;
    case I_UNPACK2_H:
      result = exec_unpack2_h (inst, oprd);
      break;
    case I_UNPACK4_H:
      result = exec_unpack4_h (inst, oprd);
      break;
    case I_UNPACK1_L:
      result = exec_unpack1_l (inst, oprd);
      break;
    case I_UNPACK2_L:
      result = exec_unpack2_l (inst, oprd);
      break;
    case I_UNPACK4_L:
      result = exec_unpack4_l (inst, oprd);
      break;
    case I_PMIN1_U:
      result = exec_pmin1_u (inst, oprd);
      break;
    case I_PMAX1_U:
      result = exec_pmax1_u (inst, oprd);
      break;
    case I_PMIN2:
      result = exec_pmin2 (inst, oprd);
      break;
    case I_PMAX2:
      result = exec_pmax2 (inst, oprd);
      break;
    case I_PSAD1:
      result = exec_psad1 (inst, oprd);
      break;
 
      // I03 instruction group
    case I_MUX1:
      result = exec_mux1 (inst, oprd);
      break;

      // I04 instruction group
    case I_MUX2:
      result = exec_mux2 (inst, oprd);
      break;

      // I05 instruction group
    case I_PSHR2:
      result = exec_pshr2 (inst, oprd);
      break;
    case I_PSHR4:
      result = exec_pshr4 (inst, oprd);
      break;
    case I_SHR:
      result = exec_shr (inst, oprd);
      break;  
    case I_PSHR2_U:
      result = exec_pshr2_u (inst, oprd);
      break;
    case I_PSHR4_U:
      result = exec_pshr4_u (inst, oprd);
      break;
    case I_SHR_U:
      result = exec_shr_u (inst, oprd);
      break;

      // I06 instruction group
    case I_PSHR2_I:
      result = exec_pshr2_i (inst, oprd);
      break;
    case I_PSHR4_I:
      result = exec_pshr4_i (inst, oprd);
      break;
    case I_PSHR2_U_I:
      result = exec_pshr2_u_i (inst, oprd);
      break;
    case I_PSHR4_U_I:
      result = exec_pshr4_u_i (inst, oprd);
      break;

      // I07 instruction group
    case I_PSHL2:
      result = exec_pshl2 (inst, oprd);
      break;
    case I_PSHL4:
      result = exec_pshl4 (inst, oprd);
      break;
    case I_SHL:
      result = exec_shl (inst, oprd);
      break;  

      // I08 instruction group
    case I_PSHL2_I:
      result = exec_pshl2_i (inst, oprd);
      break;
    case I_PSHL4_I:
      result = exec_pshl4_i (inst, oprd);
      break;

      // I09 instruction group
    case I_POPCNT:
      result = exec_popcnt (inst, oprd);
      break;

     // I10 instruction group
    case I_SHRP:
      result = exec_shrp (inst, oprd);
      break;

      // I11 instruction group
    case I_EXTR:
      result = exec_extr (inst, oprd);
      break;
    case I_EXTR_U:
      result = exec_extr_u (inst, oprd);
      break;  

      // I12 instruction group
    case I_DEP_Z:
      result = exec_dep_z (inst, oprd);
      break;

      // I13 instruction group
    case I_DEP_Z_I:
      result = exec_dep_z_i (inst, oprd);
      break;
      
      // I14 instruction group
    case I_DEP_I:
      result = exec_dep_i (inst, oprd);
      break;

      // I15 instruction group
    case I_DEP:
      result = exec_dep (inst, oprd);
      break;

      // I16 instruction group
    case I_TBIT_Z:
    case I_TBIT_Z_UNC:
      result = exec_tbit_z (inst, oprd);
      break;
    case I_TBIT_Z_AND:
      result = exec_tbit_z_and (inst, oprd);
      break;
    case I_TBIT_NZ_AND:
      result = exec_tbit_nz_and (inst, oprd);
      break;
    case I_TBIT_Z_OR:
      result = exec_tbit_z_or (inst, oprd);
      break;
    case I_TBIT_NZ_OR:
      result = exec_tbit_nz_or (inst, oprd);
      break;
    case I_TBIT_Z_OR_ANDCM:
      result = exec_tbit_z_or_andcm (inst, oprd);
      break;
    case I_TBIT_NZ_OR_ANDCM:
      result = exec_tbit_nz_or_andcm (inst, oprd);
      break;

      // I17 instruction group
    case I_TNAT_Z:
    case I_TNAT_Z_UNC:
      result = exec_tnat_z (inst, oprd);
      break;
    case I_TNAT_Z_AND:
      result = exec_tnat_z_and (inst, oprd);
      break;
    case I_TNAT_NZ_AND:
      result = exec_tnat_nz_and (inst, oprd);
      break;
    case I_TNAT_Z_OR:
      result = exec_tnat_z_or (inst, oprd);
      break;
    case I_TNAT_NZ_OR:
      result = exec_tnat_nz_or (inst, oprd);
      break;
    case I_TNAT_Z_OR_ANDCM:
      result = exec_tnat_z_or_andcm (inst, oprd);
      break;
    case I_TNAT_NZ_OR_ANDCM:
      result = exec_tnat_nz_or_andcm (inst, oprd);
      break;

      // I19 instruction group
    case X_BREAK:
    case I_BREAK:
      throw Interrupt (FAULT_IT_INST_BREAK, inst);
      break;
    case X_NOP:
    case I_NOP:
      result.setvalid (true);
      break;

      // I20 instruction group
    case I_CHK_S_I:
      result = exec_chk_s_i (inst, oprd);
      break;

      // I21 instruction group
    case I_MOV_TO_B:
      result = exec_mov_to_b (inst, oprd);
      break;
    case I_MOV_RET_TO_B:
      result = exec_mov_ret_to_b (inst, oprd);
      break;
      
      // I22 instruction group
    case I_MOV_FROM_B:
      result = exec_mov_from_b (inst, oprd);
      break;

      // I23 instruction group
    case I_MOV_TO_PR:
      result = exec_mov_to_pr (inst, oprd);
      break;

      // I24 instruction group
    case I_MOV_TO_PR_ROT:
      result = exec_mov_to_pr_rot (inst, oprd);
      break;

      // I25 instruction group
    case I_MOV_FROM_IP:
      result = exec_mov_from_ip (inst, oprd);
      break;
    case I_MOV_FROM_PR:
      result = exec_mov_from_pr (inst, oprd);
      break;

      // I26 instruction group
    case I_MOV_TO_AR_R:
      result = exec_mov_to_ar_r (inst, oprd);
      break;

      // I27 instruction group
    case I_MOV_TO_AR_I:
      result = exec_mov_to_ar_i (inst, oprd);
      break;

      // I28 instruction group
    case I_MOV_FROM_AR:
      result = exec_mov_from_ar (inst, oprd);
      break;

      // I29 instruction group
    case  I_ZXT1:
      result = exec_zxt1 (inst, oprd);
      break;
    case  I_ZXT2:
      result = exec_zxt2 (inst, oprd);
      break;
    case  I_ZXT4:
      result = exec_zxt4 (inst, oprd);
      break;
    case  I_SXT1:
      result = exec_sxt1 (inst, oprd);
      break;
    case  I_SXT2:
      result = exec_sxt2 (inst, oprd);
      break;
    case  I_SXT4:
      result = exec_sxt4 (inst, oprd);
      break;
    case I_CZX1_L:
      result = exec_czx1_l (inst, oprd);
      break;
    case I_CZX2_L:
      result = exec_czx2_l (inst, oprd);
      break;
    case I_CZX1_R:
      result = exec_czx1_r (inst, oprd);
      break;
    case I_CZX2_R:
      result = exec_czx2_r (inst, oprd);
      break;
    default:
      break;
    }
    // check for valid result
    if (result.isvalid () == false) {
      string reason = "unimplemented instruction [I]";
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst, reason);
    }
    return result;
  }
}
