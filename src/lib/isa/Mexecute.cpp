// ---------------------------------------------------------------------------
// - Mexecute.cpp                                                            -
// - iato:isa library - M execution unit class implementation                -
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
#include "Psr.hpp"
#include "Bits.hpp"
#include "Utils.hpp"
#include "Mexecute.hpp"
#include "Interrupt.hpp"

namespace iato {

  // ------------------------------------------------------------------------
  // - M01 instruction group                                                -
  // ------------------------------------------------------------------------

  // M_LD1
  static Result exec_ld1 (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    // set result 
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD1, addr);
    return result;
  }
  
  // M_LD2
  static Result exec_ld2 (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    // set result 
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD2, addr);
    return result;
  }
     
  // M_LD4
  static Result exec_ld4 (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    // set result
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD4, addr);
    return result;
  }
     
  // M_LD8
  static Result exec_ld8 (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    // set result 
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD8, addr);
    return result;
  }

  // M_LD1_S
  static Result exec_ld1_s (const Instr& inst, const Operand& oprd) {
    // build result
    Result result = inst.getresl ();
    t_octa addr = oprd.getoval (1);
    // if nat bit is true the target register has its nat bit set
    if (oprd.getbval (1) == true) {
      result.setbval (0, true);
      return result;
    }
    // set result 
    result.setbval (0, false);
    result.setaddr (0, Result::REG_LD1, addr);
    result.setspec (0, true);
    return result;
  }

  // M_LD2_S
  static Result exec_ld2_s (const Instr& inst, const Operand& oprd) {
    // build result
    Result result = inst.getresl ();
    t_octa addr = oprd.getoval (1);
    // if nat bit is true the target register has its nat bit set
    if (oprd.getbval (1) == true) {
      result.setbval (0, true);
      return result;
    }
    // set result 
    result.setbval (0, false);
    result.setaddr (0, Result::REG_LD2, addr);
    result.setspec (0, true);
    return result;
  }

  // M_LD4_S
  static Result exec_ld4_s (const Instr& inst, const Operand& oprd) {
    // build result
    Result result = inst.getresl ();
    t_octa addr = oprd.getoval (1);
    // if nat bit is true the target register has its nat bit set
    if (oprd.getbval (1) == true) {
      result.setbval (0, true);
      return result;
    }
    // set result
    result.setbval (0, false);
    result.setaddr (0, Result::REG_LD4, addr);
    result.setspec (0, true);
    return result;
  }

  // M_LD8_S
  static Result exec_ld8_s (const Instr& inst, const Operand& oprd) {
    // build result
    Result result = inst.getresl ();
    t_octa addr   = oprd.getoval (1);
    // if nat bit is true the target register has its nat bit set
    if (oprd.getbval (1) == true) {
      result.setbval (0, true);
      return result;
    }
    // set result 
    result.setbval (0, false);
    result.setaddr (0, Result::REG_LD8, addr);
    result.setspec (0, true);
    return result;
  }

  // M_LD1_SA
  static Result exec_ld1_sa (const Instr& inst, const Operand& oprd) {
    // build result
    Result result = inst.getresl ();
    t_octa addr = oprd.getoval (1);
    // if nat bit is true the target register has its nat bit set
    if (oprd.getbval (1) == true) {
      result.setbval (0, true);
      return result;
    }
    // set result 
    result.setbval (0, false);
    result.setaddr (0, Result::REG_LD1, addr);
    result.setaset (0, true);
    result.setspec (0, true);
    return result;
  }

  // M_LD2_SA
  static Result exec_ld2_sa (const Instr& inst, const Operand& oprd) {
    // build result
    Result result = inst.getresl ();
    t_octa addr = oprd.getoval (1);
    // if nat bit is true the target register has its nat bit set
    if (oprd.getbval (1) == true) {
      result.setbval (0, true);
      return result;
    }
    // set result 
    result.setbval (0, false);
    result.setaddr (0, Result::REG_LD2, addr);
    result.setaset (0, true);
    result.setspec (0, true);
    return result;
  }

  // M_LD4_SA
  static Result exec_ld4_sa (const Instr& inst, const Operand& oprd) {
    // build result
    Result result = inst.getresl ();
    t_octa addr = oprd.getoval (1);
    // if nat bit is true the target register has its nat bit set
    if (oprd.getbval (1) == true) {
      result.setbval (0, true);
      return result;
    }
    // set result 
    result.setbval (0, false);
    result.setaddr (0, Result::REG_LD4, addr);
    result.setaset (0, true);
    result.setspec (0, true);
    return result;
  }

  // M_LD8_SA
  static Result exec_ld8_sa (const Instr& inst, const Operand& oprd) {
    // build result
    Result result = inst.getresl ();
    t_octa addr = oprd.getoval (1);
    // if nat bit is true the target register has its nat bit set
    if (oprd.getbval (1) == true) {
      result.setbval (0, true);
      return result;
    }
    // set result 
    result.setbval (0, false);
    result.setaddr (0, Result::REG_LD8, addr);
    result.setaset (0, true);
    result.setspec (0, true);
    return result;
  }
  
  // M_LD1_A
  static Result exec_ld1_a (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    // set result 
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD1, addr);
    result.setaset (0, true);
    return result;
  }
  
  // M_LD2_A
  static Result exec_ld2_a (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    // set result 
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD2, addr);
    result.setaset (0, true);
    return result;
  }
     
  // M_LD4_A
  static Result exec_ld4_a (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    // set result
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD4, addr);
    result.setaset (0, true);
    return result;
  }
     
  // M_LD8_A
  static Result exec_ld8_a (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    // set result 
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD8, addr);
    result.setaset (0, true);
    return result;
  }
  
  // M_LD1_C_CLR
  static Result exec_ld1_c_clr (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    // set result 
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD1, addr);
    result.setachk (0, true);
    result.setaclr (0, true);
    return result;
  }
  
  // M_LD2_C_CLR
  static Result exec_ld2_c_clr (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    // set result 
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD2, addr);
    result.setachk (0, true);
    result.setaclr (0, true);
    return result;
  }
     
  // M_LD4_C_CRL
  static Result exec_ld4_c_clr (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    // set result
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD4, addr);
    result.setachk (0, true);
    result.setaclr (0, true);
    return result;
  }
     
  // M_LD8_C_CLR
  static Result exec_ld8_c_clr (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    // set result 
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD8, addr);
    result.setachk (0, true);
    result.setaclr (0, true);
    return result;
  }
    
  // M_LD1_C_NC
  static Result exec_ld1_c_nc (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    // set result 
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD1, addr);
    result.setachk (0, true);
    return result;
  }
  
  // M_LD2_C_NC
  static Result exec_ld2_c_nc (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    // set result 
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD2, addr);
    result.setachk (0, true);
    return result;
  }
     
  // M_LD4_C_NC
  static Result exec_ld4_c_nc (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    // set result
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD4, addr);
    result.setachk (0, true);
    return result;
  }
     
  // M_LD8_C_NC
  static Result exec_ld8_c_nc (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    // set result 
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD8, addr);
    result.setachk (0, true);
    return result;
  }
  
  // M_LD8_FILL
  static Result exec_ld8_fill (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute result
    t_octa addr = oprd.getoval (1);
    // compute nat bit from unat, position is in addr at 8:3
    t_byte bpos = ((t_byte) (addr >> 3)) & 0x3F;
    bool   natr  = bsetget (oprd.getoval (2), bpos);
    // set result 
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD8, addr);
    return result;
  }

  // ------------------------------------------------------------------------
  // - M02 instruction group                                                -
  // ------------------------------------------------------------------------

  // M_LD1_UPD_R
  static Result exec_ld1_upd_r (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // if one nat bit is true, there is a register nat consumption fault
    if ((nat0 | nat1) == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute result
    bool   natr = false;
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result 
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD1, addr);
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LD2_UPD_R
  static Result exec_ld2_upd_r (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // if one nat bit is true, there is a register nat consumption fault
    if ((nat0 | nat1) == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute result
    bool   natr = false;
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result 
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD2, addr);
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LD4_UPD_R
  static Result exec_ld4_upd_r (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // if one nat bit is true, there is a register nat consumption fault
    if ((nat0 | nat1) == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute result
    bool   natr = false;
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result 
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD4, addr);
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LD8_UPD_R
  static Result exec_ld8_upd_r (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // if one nat bit is true, there is a register nat consumption fault
    if ((nat0 | nat1) == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute result
    bool   natr = false;
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result 
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD8, addr);
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LD1_S_UPD_R
  static Result exec_ld1_s_upd_r (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // build result
    Result result = inst.getresl ();
    // compute result
    bool   natr = nat0 | nat1;
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result 
    result.setbval (0, natr);
    if (natr == false) {
      result.setaddr (0, Result::REG_LD1, addr);
      result.setspec (0, true);
    }
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LD2_S_UPD_R
  static Result exec_ld2_s_upd_r (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // build result
    Result result = inst.getresl ();
    // compute result
    bool   natr = nat0 | nat1;
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result 
    result.setbval (0, natr);
    if (natr == false) {
      result.setaddr (0, Result::REG_LD2, addr);
      result.setspec (0, true);
    }
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LD4_S_UPD_R
  static Result exec_ld4_s_upd_r (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // build result
    Result result = inst.getresl ();
    // compute result
    bool   natr = nat0 | nat1;
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result 
    result.setbval (0, natr);
    if (natr == false) {
      result.setaddr (0, Result::REG_LD4, addr);
      result.setspec (0, true);
    }
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LD8_S_UPD_R
  static Result exec_ld8_s_upd_r (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // build result
    Result result = inst.getresl ();
    // compute result
    bool   natr = nat0 | nat1;
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result 
    result.setbval (0, natr);
    if (natr == false) {
      result.setaddr (0, Result::REG_LD8, addr);
      result.setspec (0, true);
    }
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LD1_A_UPD_R
  static Result exec_ld1_a_upd_r (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // if one nat bit is true, there is a register nat consumption fault
    if ((nat0 | nat1) == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute result
    bool   natr = false;
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result 
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD1, addr);
    result.setaset (0, true);
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LD2_A_UPD_R
  static Result exec_ld2_a_upd_r (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // if one nat bit is true, there is a register nat consumption fault
    if ((nat0 | nat1) == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute result
    bool   natr = false;
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result 
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD2, addr);
    result.setaset (0, true);
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LD4_A_UPD_R
  static Result exec_ld4_a_upd_r (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // if one nat bit is true, there is a register nat consumption fault
    if ((nat0 | nat1) == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute result
    bool   natr = false;
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result 
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD4, addr);
    result.setaset (0, true);
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LD8_A_UPD_R
  static Result exec_ld8_a_upd_r (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // if one nat bit is true, there is a register nat consumption fault
    if ((nat0 | nat1) == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute result
    bool   natr = false;
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result 
    result.setbval (0, natr);
    result.setaddr (0, Result::REG_LD8, addr);
    result.setaset (0, true);
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LD1_SA_UPD_R
  static Result exec_ld1_sa_upd_r (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // build result
    Result result = inst.getresl ();
    // compute result
    bool   natr = nat0 | nat1;
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result 
    result.setbval (0, natr);
    if (natr == false) {
      result.setaddr (0, Result::REG_LD1, addr);
      result.setaset (0, true);
      result.setspec (0, true);
    }
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LD2_SA_UPD_R
  static Result exec_ld2_sa_upd_r (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // build result
    Result result = inst.getresl ();
    // compute result
    bool   natr = nat0 | nat1;
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result 
    result.setbval (0, natr);
    if (natr == false) {
      result.setaddr (0, Result::REG_LD2, addr);
      result.setaset (0, true);
      result.setspec (0, true);
    }
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LD4_SA_UPD_R
  static Result exec_ld4_sa_upd_r (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // build result
    Result result = inst.getresl ();
    // compute result
    bool   natr = nat0 | nat1;
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result 
    result.setbval (0, natr);
    if (natr == false) {
      result.setaddr (0, Result::REG_LD4, addr);
      result.setaset (0, true);
      result.setspec (0, true);
    }
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LD8_SA_UPD_R
  static Result exec_ld8_sa_upd_r (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // build result
    Result result = inst.getresl ();
    // compute result
    bool   natr = nat0 | nat1;
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result 
    result.setbval (0, natr);
    if (natr == false) {
      result.setaddr (0, Result::REG_LD8, addr);
      result.setaset (0, true);
      result.setspec (0, true);
    }
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }
  
  // M_LD8_FILL_UPD_R
  static Result exec_ld8_fill_upd_r (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // compute nat bit from unat, position is in addr at 8:3
    t_byte bpos  = ((t_byte) (addr >> 3)) & 0x3F;
    bool   natr1 = bsetget (oprd.getoval (2), bpos);
    bool   natr2 = oprd.getbval (0);
    // set result 
    result.setbval (0, natr1);
    result.setbval (1, natr2);
    result.setaddr (0, Result::REG_LD8, addr);
    result.setoval (1, rval);
    return result;
  }

  // ------------------------------------------------------------------------
  // - M03 instruction group                                                -
  // ------------------------------------------------------------------------

  // M_LD1_UPD_I
  static Result exec_ld1_upd_i (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0); 
    // set result 
    result.setbval (0, natr);
    result.setbval (1, natr);
    result.setaddr (0, Result::REG_LD1, addr);
    result.setoval (1, rval);
    return result;
  }
  
  // M_LD2_UPD_I
  static Result exec_ld2_upd_i (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0); 
    // set result 
    result.setbval (0, natr);
    result.setbval (1, natr);
    result.setaddr (0, Result::REG_LD2, addr);
    result.setoval (1, rval);
    return result;
  }
     
  // M_LD4_UPD_I
  static Result exec_ld4_upd_i (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0); 
    // set result 
    result.setbval (0, natr);
    result.setbval (1, natr);
    result.setaddr (0, Result::REG_LD4, addr);
    result.setoval (1, rval);
    return result;
  }
     
  // M_LD8_UPD_I
  static Result exec_ld8_upd_i (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0); 
    // set result 
    result.setbval (0, natr);
    result.setbval (1, natr);
    result.setaddr (0, Result::REG_LD8, addr);
    result.setoval (1, rval);
    return result;
  }

  // M_LD1_S_UPD_I
  static Result exec_ld1_s_upd_i (const Instr& inst, const Operand& oprd) {
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0); 
    // set result, this dependes on the nat bit
    result.setbval (0, natr);
    result.setbval (1, natr);
    result.setoval (1, rval);
    if (natr == false) {
      result.setaddr (0, Result::REG_LD1, addr);
      result.setspec (0, true);
    }
    return result;
  }

  // M_LD2_S_UPD_I
  static Result exec_ld2_s_upd_i (const Instr& inst, const Operand& oprd) {
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0); 
    // set result, this dependes on the nat bit
    result.setbval (0, natr);
    result.setbval (1, natr);
    result.setoval (1, rval);
    if (natr == false) {
      result.setaddr (0, Result::REG_LD2, addr);
      result.setspec (0, true);
    }
    return result;
  }

  // M_LD4_S_UPD_I
  static Result exec_ld4_s_upd_i (const Instr& inst, const Operand& oprd) {
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0); 
    // set result, this dependes on the nat bit
    result.setbval (0, natr);
    result.setbval (1, natr);
    result.setoval (1, rval);
    if (natr == false) {
      result.setaddr (0, Result::REG_LD4, addr);
      result.setspec (0, true);
    }
    return result;
  }

  // M_LD8_S_UPD_I
  static Result exec_ld8_s_upd_i (const Instr& inst, const Operand& oprd) {
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0);
    // set result, this dependes on the nat bit
    result.setbval (0, natr);
    result.setbval (1, natr);
    result.setoval (1, rval);
    if (natr == false) {
      result.setaddr (0, Result::REG_LD8, addr);
      result.setspec (0, true);
    }
    return result;
  }

  // M_LD1_A_UPD_I
  static Result exec_ld1_a_upd_i (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0); 
    // set result 
    result.setbval (0, natr);
    result.setbval (1, natr);
    result.setaddr (0, Result::REG_LD1, addr);
    result.setoval (1, rval);
    result.setaset (0, true);
    return result;
  }
  
  // M_LD2_A_UPD_I
  static Result exec_ld2_a_upd_i (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0); 
    // set result 
    result.setbval (0, natr);
    result.setbval (1, natr);
    result.setaddr (0, Result::REG_LD2, addr);
    result.setoval (1, rval);
    result.setaset (0, true);
    return result;
  }
     
  // M_LD4_A_UPD_I
  static Result exec_ld4_a_upd_i (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0); 
    // set result 
    result.setbval (0, natr);
    result.setbval (1, natr);
    result.setaddr (0, Result::REG_LD4, addr);
    result.setoval (1, rval);
    result.setaset (0, true);
    return result;
  }
     
  // M_LD8_A_UPD_I
  static Result exec_ld8_a_upd_i (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0); 
    // set result 
    result.setbval (0, natr);
    result.setbval (1, natr);
    result.setaddr (0, Result::REG_LD8, addr);
    result.setoval (1, rval);
    result.setaset (0, true);
    return result;
  }

  // M_LD1_SA_UPD_I
  static Result exec_ld1_sa_upd_i (const Instr& inst, const Operand& oprd) {
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0); 
    // set result, this dependes on the nat bit
    result.setbval (0, natr);
    result.setbval (1, natr);
    result.setoval (1, rval);
    if (natr == false) {
      result.setaddr (0, Result::REG_LD1, addr);
      result.setaset (0, true);
      result.setspec (0, true);
    }
    return result;
  }

  // M_LD2_SA_UPD_I
  static Result exec_ld2_sa_upd_i (const Instr& inst, const Operand& oprd) {
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0); 
    // set result, this dependes on the nat bit
    result.setbval (0, natr);
    result.setbval (1, natr);
    result.setoval (1, rval);
    if (natr == false) {
      result.setaddr (0, Result::REG_LD2, addr);
      result.setaset (0, true);
      result.setspec (0, true);
    }
    return result;
  }

  // M_LD4_SA_UPD_I
  static Result exec_ld4_sa_upd_i (const Instr& inst, const Operand& oprd) {
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0); 
    // set result, this dependes on the nat bit
    result.setbval (0, natr);
    result.setbval (1, natr);
    result.setoval (1, rval);
    if (natr == false) {
      result.setaddr (0, Result::REG_LD4, addr);
      result.setaset (0, true);
      result.setspec (0, true);
    }
    return result;
  }

  // M_LD8_SA_UPD_I
  static Result exec_ld8_sa_upd_i (const Instr& inst, const Operand& oprd) {
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = oprd.getbval (1);
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0);
    // set result, this dependes on the nat bit
    result.setbval (0, natr);
    result.setbval (1, natr);
    result.setoval (1, rval);
    if (natr == false) {
      result.setaddr (0, Result::REG_LD8, addr);
      result.setaset (0, true);
      result.setspec (0, true);
    }
    return result;
  }

  // M_LD1_C_CLR_UPD_I
  static Result exec_ld1_c_clr_upd_i (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0); 
    // set result 
    result.setbval (0, natr);
    result.setbval (1, natr);
    result.setaddr (0, Result::REG_LD1, addr);
    result.setoval (1, rval);
    result.setachk (0, true);
    result.setaclr (0, true);
    return result;
  }
  
  // M_LD2_C_CLR_UPD_I
  static Result exec_ld2_c_clr_upd_i (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0); 
    // set result 
    result.setbval (0, natr);
    result.setbval (1, natr);
    result.setaddr (0, Result::REG_LD2, addr);
    result.setoval (1, rval);
    result.setachk (0, true);
    result.setaclr (0, true);
    return result;
  }
     
  // M_LD4_C_CLR_UPD_I
  static Result exec_ld4_c_clr_upd_i (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0); 
    // set result 
    result.setbval (0, natr);
    result.setbval (1, natr);
    result.setaddr (0, Result::REG_LD4, addr);
    result.setoval (1, rval);
    result.setachk (0, true);
    result.setaclr (0, true);
    return result;
  }
     
  // M_LD8_C_CLR_UPD_I
  static Result exec_ld8_c_clr_upd_i (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute nat bit
    bool natr = false;
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0); 
    // set result 
    result.setbval (0, natr);
    result.setbval (1, natr);
    result.setaddr (0, Result::REG_LD8, addr);
    result.setoval (1, rval);
    result.setachk (0, true);
    result.setaclr (0, true);
    return result;
  }

  // M_LD8_FILL_UPD_I
  static Result exec_ld8_fill_upd_i (const Instr& inst, const Operand& oprd) {
    // if nat bit is true, there is a register nat consumption fault
    if (oprd.getbval (1) == true) 
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // build result
    Result result = inst.getresl ();
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0);
    // compute nat bit from unat, position is in addr at 8:3
    t_byte bpos  = ((t_byte) (addr >> 3)) & 0x3F;
    bool   natr1 = bsetget (oprd.getoval (2), bpos);
    bool   natr2 = false;
    // set result 
    result.setbval (0, natr1);
    result.setbval (1, natr2);
    result.setaddr (0, Result::REG_LD8, addr);
    result.setoval (1, rval);
    return result;
  }

  // ------------------------------------------------------------------------
  // - M04 instruction group                                                -
  // ------------------------------------------------------------------------
               
  // M_ST1
  static Result exec_st1 (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bits
    bool nat1 = oprd.getbval (1);
    bool nat2 = oprd.getbval (0);
    bool natr = nat1 | nat2;
    // if one nat bit is true, there is a register nat consumption fault
    if (natr == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = oprd.getoval (0);
    // set result 
    result.setaddr (0, Result::REG_ST1, addr);
    result.setimmv (0, rval);
    return result;
  }
  
  // M_ST2
  static Result exec_st2(const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();;
    // compute nat bits
    bool nat1 = oprd.getbval (1);
    bool nat2 = oprd.getbval (0);
    bool natr = nat1 | nat2;
    // if one nat bit is true, there is a register nat consumption fault
    if (natr == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = oprd.getoval (0);
    // set result 
    result.setaddr (0, Result::REG_ST2, addr);
    result.setimmv (0, rval);
    return result;
  }
     
  // M_ST4
  static Result exec_st4(const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bits
    bool nat1 = oprd.getbval (1);
    bool nat2 = oprd.getbval (0);
    bool natr = nat1 | nat2;
    // if one nat bit is true, there is a register nat consumption fault
    if (natr == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = oprd.getoval (0);
    // set result 
    result.setaddr (0, Result::REG_ST4, addr);
    result.setimmv (0, rval);
    return result;
  }
     
  // M_ST8
  static Result exec_st8 (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();;
    // compute nat bits
    bool nat1 = oprd.getbval (1);
    bool nat2 = oprd.getbval (0);
    bool natr = nat1 | nat2;
    // if one nat bit is true, there is a register nat consumption fault
    if (natr == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = oprd.getoval (0);
    // set result 
    result.setaddr (0, Result::REG_ST8, addr);
    result.setimmv (0, rval);
    return result;
  }

  // M_ST8_SPILL
  static Result exec_st8_spill (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();;
    // compute nat bits
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat1 == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = oprd.getoval (0);
    // compute unat bit value is in addr at 8:3
    t_byte bpos = ((t_byte) (addr >> 3)) & 0x3F;
    t_octa unat = bsetocta (oprd.getoval (2), bpos, nat0);
    // set result 
    result.setaddr (0, Result::REG_ST8, addr);
    result.setimmv (0, rval);
    result.setoval (2, unat);
    return result;
  }

  // ------------------------------------------------------------------------
  // - M05 instruction group                                                -
  // ------------------------------------------------------------------------

  // M_ST1_UPD_I
  static Result exec_st1_upd_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bits
    bool nat1 = oprd.getbval (1);
    bool nat2 = oprd.getbval (0);
    bool natr = nat1 | nat2;
    // if one nat bit is true, there is a register nat consumption fault
    if (natr == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa sval = oprd.getoval (0);
    t_octa rval = addr + inst.getimmv (0);
    // set result
    result.setaddr (0, Result::REG_ST1, addr);
    result.setimmv (0, sval);
    result.setoval (1, rval);
    result.setbval (1, nat1);
    return result;
  }

  // M_ST2_UPD_I
  static Result exec_st2_upd_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bits
    bool nat1 = oprd.getbval (1);
    bool nat2 = oprd.getbval (0);
    bool natr = nat1 | nat2;
    // if one nat bit is true, there is a register nat consumption fault
    if (natr == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa sval = oprd.getoval (0);
    t_octa rval = addr + inst.getimmv (0);
    // set result
    result.setaddr (0, Result::REG_ST2, addr);
    result.setimmv (0, sval);
    result.setoval (1, rval);
    result.setbval (1, nat1);
    return result;
  }

  // M_ST4_UPD_I
  static Result exec_st4_upd_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bits
    bool nat1 = oprd.getbval (1);
    bool nat2 = oprd.getbval (0);
    bool natr = nat1 | nat2;
    // if one nat bit is true, there is a register nat consumption fault
    if (natr == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa sval = oprd.getoval (0);
    t_octa rval = addr + inst.getimmv (0);
    // set result
    result.setaddr (0, Result::REG_ST4, addr);
    result.setimmv (0, sval);
    result.setoval (1, rval);
    result.setbval (1, nat1);
    return result;
  }

  // M_ST8_UPD_I
  static Result exec_st8_upd_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bits
    bool nat1 = oprd.getbval (1);
    bool nat2 = oprd.getbval (0);
    bool natr = nat1 | nat2;
    // if one nat bit is true, there is a register nat consumption fault
    if (natr == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa sval = oprd.getoval (0);
    t_octa rval = addr + inst.getimmv (0);
    // set result
    result.setaddr (0, Result::REG_ST8, addr);
    result.setimmv (0, sval);
    result.setoval (1, rval);
    result.setbval (1, nat1);
    return result;
  }

  // M_ST8_SPILL_UPD_I
  static Result exec_st8_spill_upd_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute nat bits
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat1 == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa sval = oprd.getoval (0);
    t_octa rval = addr + inst.getimmv (0);
    // compute unat bit value is in addr at 8:3
    t_byte bpos = ((t_byte) (addr >> 3)) & 0x3F;
    t_octa unat = bsetocta (oprd.getoval (2), bpos, nat0);
    // set result
    result.setaddr (0, Result::REG_ST8, addr);
    result.setimmv (0, sval);
    result.setoval (1, rval);
    result.setbval (1, nat0);
    result.setoval (2, unat);
    return result;
  }

  // ------------------------------------------------------------------------
  // - M06 instruction group                                                -
  // ------------------------------------------------------------------------

  // M_LDFS
  static Result exec_ldfs (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDS, addr);
    return result;
  }

  // M_LDFD
  static Result exec_ldfd (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDD, addr);
    return result;
  }

  // M_LDF8
  static Result exec_ldf8 (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDI, addr);
    return result;
  }

  // M_LDFE
  static Result exec_ldfe (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDE, addr);
    return result;
  }

  // M_LDFS_S
  static Result exec_ldfs_s (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true,  the target register is set to a nat value
    if (nat == true) {
      t_real rval; rval.setnat ();
      result.setrval (0, rval);
      return result;
    }
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDS, addr);
    result.setspec (0, true);
    return result;
  }

  // M_LDFD_S
  static Result exec_ldfd_s (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true,  the target register is set to a nat value
    if (nat == true) {
      t_real rval; rval.setnat ();
      result.setrval (0, rval);
      return result;
    }
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDD, addr);
    return result;
  }

  // M_LDF8_S
  static Result exec_ldf8_s (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true,  the target register is set to a nat value
    if (nat == true) {
      t_real rval; rval.setnat ();
      result.setrval (0, rval);
      return result;
    }
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDI, addr);
    return result;
  }

  // M_LDFE_S
  static Result exec_ldfe_s (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true,  the target register is set to a nat value
    if (nat == true) {
      t_real rval; rval.setnat ();
      result.setrval (0, rval);
      return result;
    }
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDE, addr);
    return result;
  }

  // M_LDFS_A
  static Result exec_ldfs_a (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true)  throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDS, addr);
    result.setaset (0, true);
    return result;
  }

  // M_LDFD_A
  static Result exec_ldfd_a (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true)  throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDD, addr);
    result.setaset (0, true);
    return result;
  }

  // M_LDF8_A
  static Result exec_ldf8_a (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true)  throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDI, addr);
    result.setaset (0, true);
    return result;
  }

  // M_LDFE_A
  static Result exec_ldfe_a (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true)  throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDE, addr);
    result.setaset (0, true);
    return result;
  }

  // M_LDFS_SA
  static Result exec_ldfs_sa (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true,  the target register is set to a nat value
    if (nat == true) {
      t_real rval; rval.setnat ();
      result.setrval (0, rval);
      return result;
    }
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDS, addr);
    result.setaset (0, true);
    result.setspec (0, true);
    return result;
  }

  // M_LDFD_SA
  static Result exec_ldfd_sa (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true,  the target register is set to a nat value
    if (nat == true) {
      t_real rval; rval.setnat ();
      result.setrval (0, rval);
      return result;
    }
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDD, addr);
    result.setaset (0, true);
    result.setspec (0, true);
    return result;
  }

  // M_LDF8_SA
  static Result exec_ldf8_sa (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true,  the target register is set to a nat value
    if (nat == true) {
      t_real rval; rval.setnat ();
      result.setrval (0, rval);
      return result;
    }
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDI, addr);
    result.setaset (0, true);
    result.setspec (0, true);
    return result;
  }

  // M_LDFE_SA
  static Result exec_ldfe_sa (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true,  the target register is set to a nat value
    if (nat == true) {
      t_real rval; rval.setnat ();
      result.setrval (0, rval);
      return result;
    }
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDE, addr);
    result.setaset (0, true);
    result.setspec (0, true);
    return result;
  }

  // M_LDF_FILL
  static Result exec_ldf_fill (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDF, addr);
    return result;
  }

  // M_LDFS_C_CLR
  static Result exec_ldfs_c_clr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true)  throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDS, addr);
    result.setachk (0, true);
    result.setaclr (0, true);
    return result;
  }

  // M_LDFD_C_CLR
  static Result exec_ldfd_c_clr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true)  throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDD, addr);
    result.setachk (0, true);
    result.setaclr (0, true);
    return result;
  }

  // M_LDF8_C_CLR
  static Result exec_ldf8_c_clr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true)  throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDI, addr);
    result.setachk (0, true);
    result.setaclr (0, true);
    return result;
  }

  // M_LDFE_C_CLR
  static Result exec_ldfe_c_clr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true)  throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDE, addr);
    result.setachk (0, true);
    result.setaclr (0, true);
    return result;
  }

  // M_LDFS_C_NC
  static Result exec_ldfs_c_nc (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true)  throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDS, addr);
    result.setachk (0, true);
    return result;
  }

  // M_LDFD_C_NC
  static Result exec_ldfd_c_nc (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true)  throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDD, addr);
    result.setachk (0, true);
    return result;
  }

  // M_LDF8_C_NC
  static Result exec_ldf8_c_nc (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true)  throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDI, addr);
    result.setachk (0, true);
    return result;
  }

  // M_LDFE_C_NC
  static Result exec_ldfe_c_nc (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true)  throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_LDE, addr);
    result.setachk (0, true);
    return result;
  }

  // ------------------------------------------------------------------------
  // - M07 instruction group                                                -
  // ------------------------------------------------------------------------

  // M_LDFS_UPD_R
  static Result exec_ldfs_upd_r (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    bool   natr = oprd.getbval (0);
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result
    result.setaddr (0, Result::REG_LDS, addr);
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LDFD_UPD_R
  static Result exec_ldfd_upd_r (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat  == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    bool   natr = oprd.getbval (0);
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result
    result.setaddr (0, Result::REG_LDD, addr);
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LDF8_UPD_R
  static Result exec_ldf8_upd_r (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    bool   natr = oprd.getbval (0);
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result
    result.setaddr (0, Result::REG_LDI, addr);
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LDFE_UPD_R
  static Result exec_ldfe_upd_r (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    bool   natr = oprd.getbval (0);
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result
    result.setaddr (0, Result::REG_LDE, addr);
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LDFS_S_UPD_R
  static Result exec_ldfs_s_upd_r (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // build result
    Result result = inst.getresl ();
    // compute result
    bool   natr = nat0 | nat1;
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result
    if (natr == false) {
      result.setaddr (0, Result::REG_LDS, addr);
      result.setspec (0, true);
    } else {
      t_real rval; rval.setnat ();
      result.setrval (0, rval);
    }
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LDFD_S_UPD_R
  static Result exec_ldfd_s_upd_r (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // build result
    Result result = inst.getresl ();
    // compute result
    bool   natr = nat0 | nat1;
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result
    if (natr == false) {
      result.setaddr (0, Result::REG_LDD, addr);
      result.setspec (0, true);
    } else {
      t_real rval; rval.setnat ();
      result.setrval (0, rval);
    }
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LDF8_S_UPD_R
  static Result exec_ldf8_s_upd_r (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // build result
    Result result = inst.getresl ();
    // compute result
    bool   natr = nat0 | nat1;
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result
    if (natr == false) {
      result.setaddr (0, Result::REG_LD8, addr);
      result.setspec (0, true);
    } else {
      t_real rval; rval.setnat ();
      result.setrval (0, rval);
    }
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LDFE_S_UPD_R
  static Result exec_ldfe_s_upd_r (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // build result
    Result result = inst.getresl ();
    // compute result
    bool   natr = nat0 | nat1;
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result
    if (natr == false) {
      result.setaddr (0, Result::REG_LDE, addr);
      result.setspec (0, true);
    } else {
      t_real rval; rval.setnat ();
      result.setrval (0, rval);
    }
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LDFS_A_UPD_R
  static Result exec_ldfs_a_upd_r (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    bool   natr = oprd.getbval (0);
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result
    result.setaddr (0, Result::REG_LDS, addr);
    result.setaset (0, true);
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LDFD_A_UPD_R
  static Result exec_ldfd_a_upd_r (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    bool   natr = oprd.getbval (0);
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result
    result.setaddr (0, Result::REG_LDD, addr);
    result.setaset (0, true);
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LDF8_A_UPD_R
  static Result exec_ldf8_a_upd_r (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    bool   natr = oprd.getbval (0);
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result
    result.setaddr (0, Result::REG_LD8, addr);
    result.setaset (0, true);
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LDFE_A_UPD_R
  static Result exec_ldfe_a_upd_r (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    bool   natr = oprd.getbval (0);
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result
    result.setaddr (0, Result::REG_LDE, addr);
    result.setaset (0, true);
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LDFS_SA_UPD_R
  static Result exec_ldfs_sa_upd_r (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // build result
    Result result = inst.getresl ();
    // compute result
    bool   natr = nat0 | nat1;
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result
    if (natr == false) {
      result.setaddr (0, Result::REG_LDS, addr);
      result.setaset (0, true);
      result.setspec (0, true);
    } else {
      t_real rval; rval.setnat ();
      result.setrval (0, rval);
    }
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LDFD_SA_UPD_R
  static Result exec_ldfd_sa_upd_r (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // build result
    Result result = inst.getresl ();
    // compute result
    bool   natr = nat0 | nat1;
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result
    if (natr == false) {
      result.setaddr (0, Result::REG_LDD, addr);
      result.setaset (0, true);
      result.setspec (0, true);
    } else {
      t_real rval; rval.setnat ();
      result.setrval (0, rval);
    }
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LDF8_SA_UPD_R
  static Result exec_ldf8_sa_upd_r (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // build result
    Result result = inst.getresl ();
    // compute result
    bool   natr = nat0 | nat1;
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result
    if (natr == false) {
      result.setaddr (0, Result::REG_LD8, addr);
      result.setaset (0, true);
      result.setspec (0, true);
    } else {
      t_real rval; rval.setnat ();
      result.setrval (0, rval);
    }
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LDFE_SA_UPD_R
  static Result exec_ldfe_sa_upd_r (const Instr& inst, const Operand& oprd) {
    // compute nat bit
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // build result
    Result result = inst.getresl ();
    // compute result
    bool   natr = nat0 | nat1;
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result
    if (natr == false) {
      result.setaddr (0, Result::REG_LDE, addr);
      result.setaset (0, true);
      result.setspec (0, true);
    } else {
      t_real rval; rval.setnat ();
      result.setrval (0, rval);
    }
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // M_LDF_FILL_UPD_R
  static Result exec_ldf_fill_upd_r (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool   nat    = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    bool   natr = oprd.getbval (0);
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + oprd.getoval (0); 
    // set result
    result.setaddr (0, Result::REG_LDF, addr);
    result.setbval (1, natr);
    result.setoval (1, rval);
    return result;
  }

  // ------------------------------------------------------------------------
  // - M08 instruction group                                                -
  // ------------------------------------------------------------------------

  // M_LDFS_UPD_i
  static Result exec_ldfs_upd_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool nat1 = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat1 == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0); 
    // set result
    result.setaddr (0, Result::REG_LDS, addr);
    result.setbval (1, false);
    result.setoval (1, rval);
    return result;
  }

  // M_LDFD_UPD_i
  static Result exec_ldfd_upd_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool nat1 = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat1 == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0); 
    // set result
    result.setaddr (0, Result::REG_LDD, addr);
    result.setbval (1, false);
    result.setoval (1, rval);
    return result;
  }

  // M_LDF8_UPD_i
  static Result exec_ldf8_upd_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool nat1 = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat1 == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0); 
    // set result
    result.setaddr (0, Result::REG_LDI, addr);
    result.setbval (1, false);
    result.setoval (1, rval);
    return result;
  }

  // M_LDFE_UPD_i
  static Result exec_ldfe_upd_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool nat1 = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat1 == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0); 
    // set result
    result.setaddr (0, Result::REG_LDE, addr);
    result.setbval (1, false);
    result.setoval (1, rval);
    return result;
  }

  // M_LDF_FILL_UPD_i
  static Result exec_ldf_fill_upd_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool nat1 = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat1 == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    t_octa rval = addr + inst.getimmv (0); 
    // set result
    result.setaddr (0, Result::REG_LDF, addr);
    result.setbval (1, false);
    result.setoval (1, rval);
    return result;
  }

  // ------------------------------------------------------------------------
  // - M09 instruction group                                                -
  // ------------------------------------------------------------------------

  // M_STFS
  static Result exec_stfs (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool nat1 = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat1 == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_real rval = oprd.getrval (0);
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_STS, addr);
    result.setrimv (0, rval);
    return result;
  }

  // M_STFD
  static Result exec_stfd (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool nat1 = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat1 == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_real rval = oprd.getrval (0);
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_STD, addr);
    result.setrimv (0, rval);
    return result;
  }
  
  // M_STF8
  static Result exec_stf8 (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool nat1 = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat1 == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_real rval = oprd.getrval (0);
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_STI, addr);
    result.setrimv (0, rval);
    return result;
  }

  // M_STFE
  static Result exec_stfe (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool nat1 = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat1 == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_real rval = oprd.getrval (0);
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_STE, addr);
    result.setrimv (0, rval);
    return result;
  }

  // M_STF_SPILL
  static Result exec_stf_spill (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool nat1 = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat1 == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_real rval = oprd.getrval (0);
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::REG_STF, addr);
    result.setrimv (0, rval);
    return result;
  }

  // ------------------------------------------------------------------------
  // - M10 instruction group                                                 -
  // ------------------------------------------------------------------------

  // M_STFS_UPD_I
  static Result exec_stfs_upd_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool nat1 = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat1 == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_real rval = oprd.getrval (0);
    t_octa addr = oprd.getoval (1);
    t_octa nadr = addr + inst.getimmv (0);
    result.setoval (1, nadr);
    result.setbval (1, false);
    result.setaddr (0, Result::REG_STS, addr);
    result.setrimv (0, rval);
    return result;
  }

  // M_STFD_UPD_I
  static Result exec_stfd_upd_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool nat1 = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat1 == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_real rval = oprd.getrval (0);
    t_octa addr = oprd.getoval (1);
    t_octa nadr = addr + inst.getimmv (0);
    result.setoval (1, nadr);
    result.setbval (1, false);
    result.setaddr (0, Result::REG_STD, addr);
    result.setrimv (0, rval);
    return result;
  }
  
  // M_STF8_UPD_I
  static Result exec_stf8_upd_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool nat1 = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat1 == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_real rval = oprd.getrval (0);
    t_octa addr = oprd.getoval (1);
    t_octa nadr = addr + inst.getimmv (0);
    result.setoval (1, nadr);
    result.setbval (1, false);
    result.setaddr (0, Result::REG_STI, addr);
    result.setrimv (0, rval);
    return result;
  }

  // M_STFE_UPD_I
  static Result exec_stfe_upd_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool nat1 = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat1 == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_real rval = oprd.getrval (0);
    t_octa addr = oprd.getoval (1);
    t_octa nadr = addr + inst.getimmv (0);
    result.setoval (1, nadr);
    result.setbval (1, false);
    result.setaddr (0, Result::REG_STE, addr);
    result.setrimv (0, rval);
    return result;
  }

  // M_STF_SPILL_UPD_I
  static Result exec_stf_spill_upd_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool nat1 = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat1 == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_real rval = oprd.getrval (0);
    t_octa addr = oprd.getoval (1);
    t_octa nadr = addr + inst.getimmv (0);
    result.setoval (1, nadr);
    result.setbval (1, false);
    result.setaddr (0, Result::REG_STF, addr);
    result.setrimv (0, rval);
    return result;
  }

  // ------------------------------------------------------------------------
  // - M11 instruction group                                                 -
  // ------------------------------------------------------------------------

  // M_LDFPS
  static Result exec_ldfps (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool nat1 = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat1 == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::RPL_LDS, addr);
    result.setaddr (1, Result::RPH_LDS, addr + 4);
    return result;
  }

  // M_LDFPD
  static Result exec_ldfpd (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool nat1 = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat1 == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::RPL_LDD, addr);
    result.setaddr (1, Result::RPH_LDD, addr + 8);
    return result;
  }

  // M_LDFP8
  static Result exec_ldfp8 (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool nat1 = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat1 == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::RPL_LDI, addr);
    result.setaddr (1, Result::RPH_LDI, addr + 8);
    return result;
  }

  // ------------------------------------------------------------------------
  // - M12 instruction group                                                 -
  // ------------------------------------------------------------------------

  // M_LDFPS_UPD
  static Result exec_ldfps_upd (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool nat1 = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat1 == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::RPL_LDS, addr);
    result.setaddr (1, Result::RPH_LDS, addr + 4);
    result.setoval (2, addr + 8);
    result.setbval (2, false);
    return result;
  }

  // M_LDFPD_UPD
  static Result exec_ldfpd_upd (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool nat1 = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat1 == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::RPL_LDD, addr);
    result.setaddr (1, Result::RPH_LDD, addr + 8);
    result.setoval (2, addr + 16);
    result.setbval (2, false);
    return result;
  }

  // M_LDFP8_UPD
  static Result exec_ldfp8_upd (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool nat1 = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if (nat1 == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa addr = oprd.getoval (1);
    result.setaddr (0, Result::RPL_LDI, addr);
    result.setaddr (1, Result::RPH_LDI, addr + 8);
    result.setoval (2, addr + 16);
    result.setbval (2, false);
    return result;
  }

  // ------------------------------------------------------------------------
  // - M13 instruction group                                                -
  // ------------------------------------------------------------------------

  static Result exec_lfetch (const Instr& inst, const Operand& oprd) {
    Result result;
    result.setvalid (true);
    return result;
  }

  // ------------------------------------------------------------------------
  // - M14 instruction group                                                -
  // ------------------------------------------------------------------------

  static Result exec_lfetch_upd_r (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool nat = oprd.getbval (1) || oprd.getbval (0);
    result.setbval (0, nat);
    if (nat == true) return result;
    // compute result
    t_octa oval = oprd.getoval (1) + oprd.getoval (0);
    result.setoval (0, oval);
    return result;
  }

  // ------------------------------------------------------------------------
  // - M15 instruction group                                                -
  // ------------------------------------------------------------------------

  static Result exec_lfetch_upd_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool nat = oprd.getbval (1);
    result.setbval (0, nat);
    if (nat == true) return result;
    // compute result
    t_octa oval = oprd.getoval (1) + inst.getimmv (0);
    result.setoval (0, oval);
    return result;
  }

  // ------------------------------------------------------------------------
  // - M16 instruction group                                                -
  // ------------------------------------------------------------------------

  static Result exec_cmpxchg8 (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    bool nat0 = oprd.getbval (0);
    bool nat1 = oprd.getbval (1);
    // if address nat bit is true, there is a register nat consumption fault
    if ((nat0 == true) || (nat1 == true))
      throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa cval = oprd.getoval (2);
    t_octa addr = oprd.getoval (1);
    t_octa wval = oprd.getoval (0);
    // set result
    result.setaddr (1, Result::REG_CX8, addr);
    result.setimmv (1, wval);
    result.setbval (0, false);
    result.setoval (0, cval);
    return result;
  }


  // ------------------------------------------------------------------------
  // - M18 instruction group                                                -
  // ------------------------------------------------------------------------

  // M_SETF_SIG
  static Result exec_setf_sig (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for nat bit
    bool   natr = oprd.getbval (0);
    t_octa oval = oprd.getoval (0);
    // prepare real result - by default it is a nan val
    t_real rval;
    if (natr == true) {
      rval.setnat ();
    } else {
      rval.setinteger (oval);
    }
    result.setrval (0, rval);
    return result;
  }

  // M_SETF_S
  static Result exec_setf_s (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for nat bit
    bool   natr = oprd.getbval (0);
    union{
      t_octa oval;
      t_byte bval[8];
    };
    oval = lfixocta (oprd.getoval (0));
    // prepare real result - by default it is a nan val
    t_real rval;
    if (natr == true) {
      rval.setnat ();
    } else {
      rval.singleld (bval);
    }
    result.setrval (0, rval);
    return result;
  }

  // M_SETF_D
  static Result exec_setf_d (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for nat bit
    bool   natr = oprd.getbval (0);
    union{
      t_octa oval;
      t_byte bval[8];
    };
    oval = lfixocta (oprd.getoval (0));
    // prepare real result - by default it is a nan val
    t_real rval;
    if (natr == true) {
      rval.setnat ();
    } else {
      rval.doubleld (bval);
    }
    result.setrval (0, rval);
    return result;
  }

  // M_SETF_EXP
  static Result exec_setf_exp (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for nat bit
    bool   natr = oprd.getbval (0);
    t_octa oval = oprd.getoval (0);
    t_quad qval = oval & 0x0001FFFF;
    t_octa mask = 0x0000000000020000LL;
    t_octa sgfd = 0x8000000000000000LL;
    // prepare real result - by default it is a nan val
    t_real rval;
    if (natr == true) {
      rval.setnat ();
    } else {
      rval.setexp  (qval);
      rval.setsgfd (sgfd);
      rval.setsign ((oval & mask) == mask);
    }
    result.setrval (0, rval);
    return result;
  }

  // ------------------------------------------------------------------------
  // - M19 instruction group                                                -
  // ------------------------------------------------------------------------

  // M_GETF_SIG
  static Result exec_getf_sig (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for nat bit
    t_real rval = oprd.getrval (0);
    bool   natr = rval.isnat ();
    // prepare result
    t_octa oval = rval.getsgfd ();
    result.setoval (0, oval);
    result.setbval (0, natr);
    return result;
  }

  // M_GETF_EXP
  static Result exec_getf_exp (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for nat bit
    t_real rval = oprd.getrval (0);
    bool   natr = rval.isnat ();
    // prepare result
    t_octa mask = 0x0000000000020000;
    t_octa oval = rval.getexp ();
    if (rval.getsign () == true) oval |= mask;
    result.setoval (0, oval);
    result.setbval (0, natr);
    return result;
  }

  // M_GETF_S
  static Result exec_getf_s (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for nat bit
    t_real rval = oprd.getrval (0);
    bool   natr = rval.isnat ();
    // prepare result
    union {
      t_octa oval;
      t_byte bval[8];
    }; 
    oval = OCTA_0;
    rval.singlest (bval);
    oval = lfixocta (oval);
    result.setoval (0, oval);
    result.setbval (0, natr);
    return result;
  }

  // M_GETF_D
  static Result exec_getf_d (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for nat bit
    t_real rval = oprd.getrval (0);
    bool   natr = rval.isnat ();
    // prepare result
    union {
      t_octa oval;
      t_byte bval[8];
    }; 
    rval.doublest (bval);
    oval = lfixocta (oval);
    result.setoval (0, oval);
    result.setbval (0, natr);
    return result;
  }

  // ------------------------------------------------------------------------
  // - M20 instruction group                                                -
  // ------------------------------------------------------------------------

  // CHK_S_M
  static Result exec_chk_s_m (const Instr& inst, const Operand& oprd) {
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
  // - M21 instruction group                                                -
  // ------------------------------------------------------------------------

  // CHK_S
  static Result exec_chk_s (const Instr& inst, const Operand& oprd) {
    // check for nat value
    t_real rval = oprd.getrval (0);
    bool   natr = rval.isnat ();
    // get result
    Result result = inst.getresl ();
    // do nothing if the nat bit is not set
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
  // - M22 instruction group                                                -
  // ------------------------------------------------------------------------

  // CHK_A_CLR_I
  static Result exec_chk_a_clr_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute new ip with displacement
    t_long ip = inst.getiip  ();
    t_long rd = inst.getimmv (0);
    t_octa rp = ip + rd;
    // set result
    result.setoval (1, Result::REG_CHK, 0);
    result.setaclr (1, true);
    result.setoval (0, rp);
    return result;
  }

  // CHK_A_NC_I
  static Result exec_chk_a_nc_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute new ip with displacement
    t_long ip = inst.getiip  ();
    t_long rd = inst.getimmv (0);
    t_octa rp = ip + rd;
    // set result
    result.setoval (1, Result::REG_CHK, 0);
    result.setaclr (1, false);
    result.setoval (0, rp);
    return result;
  }

  // ------------------------------------------------------------------------
  // - M23 instruction group                                                -
  // ------------------------------------------------------------------------

  // CHK_A_CLR_F
  static Result exec_chk_a_clr_f (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute new ip with displacement
    t_long ip = inst.getiip  ();
    t_long rd = inst.getimmv (0);
    t_octa rp = ip + rd;
    // set result
    result.setoval (1, Result::REG_CHK, 0);
    result.setaclr (1, true);
    result.setoval (0, rp);
    return result;
  }

  // CHK_A_NC_F
  static Result exec_chk_a_nc_f (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // compute new ip with displacement
    t_long ip = inst.getiip  ();
    t_long rd = inst.getimmv (0);
    t_octa rp = ip + rd;
    // set result
    result.setoval (1, Result::REG_CHK, 0);
    result.setaclr (1, false);
    result.setoval (0, rp);
    return result;
  }

  // ------------------------------------------------------------------------
  // - M24 instruction group                                                -
  // ------------------------------------------------------------------------

  // M_INVALA
  static Result exec_invala (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    result.setoval (0, Result::ALT_INV, 0);
    return result;
  }

  // ------------------------------------------------------------------------
  // - M29 instruction group                                                -
  // ------------------------------------------------------------------------

  // MOV_TO_AR_R
  static Result exec_mov_to_ar_r (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for nat bit and throw a register nat consumption fault
    bool nat = oprd.getbval (0);
    if (nat == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // compute result
    t_octa rval = oprd.getoval (0);
    result.setoval (0, rval);
    return result;
  }

  // ------------------------------------------------------------------------
  // - M30 instruction group                                                -
  // ------------------------------------------------------------------------

  // MOV_TO_AR_I
  static Result exec_mov_to_ar_i (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    t_octa rval = inst.getimmv (0);
    result.setoval (0, rval);
    return result;
  }

  // ------------------------------------------------------------------------
  // - M31 instruction group                                                -
  // ------------------------------------------------------------------------

  // MOV_FROM_AR
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
  // - M32 instruction group                                                -
  // ------------------------------------------------------------------------

  // MOV_TO_CR
  static Result exec_mov_to_cr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // check for nat bit and throw a register nat consumption fault
    bool nat = oprd.getbval (0);
    if (nat == true) throw Interrupt (FAULT_IT_RNAT_CONS, inst);
    // process result
    t_octa rval = oprd.getoval (0);
    result.setoval (0, rval);
    return result;
  }

  // ------------------------------------------------------------------------
  // - M33 instruction group                                                -
  // ------------------------------------------------------------------------

  // MOV_FROM_CR
  static Result exec_mov_from_cr (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // process result
    t_octa rval = oprd.getoval (1);
    result.setoval (0, rval);
    result.setbval (0, false);
    return result;
  }

  // ------------------------------------------------------------------------
  // - M34 instruction group                                                -
  // ------------------------------------------------------------------------

  // ALLOC
  static Result exec_alloc (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ();
    // get the alloc values
    t_byte sof = inst.getimmv(0);
    t_byte sol = inst.getimmv(1);
    t_byte sor = inst.getimmv(2);
    // get the cfm and update with the alloc value
    Cfm cfm = oprd.getoval (1);
    cfm.alloc (sof, sol, sor);
    // get the register value to save
    t_octa rval = oprd.getoval (0);
    // update result
    result.setbval (0, false);
    result.setoval (0, rval);
    result.setoval (1, Result::RSE_ALC, cfm.getcfm ());
    return result;
  }

  // ------------------------------------------------------------------------
  // - M44 instruction group                                                -
  // ------------------------------------------------------------------------

  // SUM
  static Result exec_sum (const Instr& inst, const Operand& oprd) {
    Result result = inst.getresl ( );
    t_octa value  = inst.getimmv (0);
    Psr    psr    = oprd.getoval (0);
    // check reserved field
    if (psr.isrvfd (Psr::UMB, value) == true)
      throw Interrupt (FAULT_IT_RESV_RGFD, inst);
    // compute result
    t_octa rval = psr.getpsr ();
    if  (bsetget (value, 1) == true)
      bsetocta (rval, 1, true);
    if ((bsetget (value, 2) == true) && (psr.getfld (Psr::SP)== false))
      bsetocta (rval, 2, true);
    if  (bsetget (value, 3) == true)
      bsetocta (rval, 3, true);
    if  (bsetget (value, 4) == true)
      bsetocta (rval, 4, true);
    if  (bsetget (value, 5) == true)
      bsetocta (rval, 5, true);
    // update result
    result.setoval (0, rval);
    return result;
  }

  // execute a M unit instruction with operands

  Result Mexecute::exec (const Instr& inst, const Operand& oprd) const {
    Result result;
    if (inst.isvalid () == false) return result;
    // check the A unit first
    result = Aexecute::exec (inst, oprd);
    if (result.isvalid () == true) return result;
    // execute M specific instruction
    t_iopc iopc = inst.getiopc ();
    switch (iopc) {

      // M01 instruction group
    case M_LD1:
    case M_LD1_ACQ:
      result = exec_ld1 (inst, oprd);
      break;
    case M_LD2:
    case M_LD2_ACQ:
      result = exec_ld2 (inst, oprd);
      break;
    case M_LD4:
    case M_LD4_ACQ:
      result = exec_ld4 (inst, oprd);
      break;
    case M_LD8:
    case M_LD8_ACQ:
      result = exec_ld8 (inst, oprd);
      break;
    case M_LD1_S:
      result = exec_ld1_s (inst, oprd);
      break;
    case M_LD2_S:
      result = exec_ld2_s (inst, oprd);
      break;
    case M_LD4_S:
      result = exec_ld4_s (inst, oprd);
      break;
    case M_LD8_S:
      result = exec_ld8_s (inst, oprd);
      break;
    case M_LD1_A:
      result = exec_ld1_a (inst, oprd);
      break;
    case M_LD2_A:
      result = exec_ld2_a (inst, oprd);
      break;
    case M_LD4_A:
      result = exec_ld4_a (inst, oprd);
      break;
    case M_LD8_A:
      result = exec_ld8_a (inst, oprd);
      break;
    case M_LD1_SA:
      result = exec_ld1_sa (inst, oprd);
      break;
    case M_LD2_SA:
      result = exec_ld2_sa (inst, oprd);
      break;
    case M_LD4_SA:
      result = exec_ld4_sa (inst, oprd);
      break;
    case M_LD8_SA:
      result = exec_ld8_sa (inst, oprd);
      break;
    case M_LD1_C_CLR:
      result = exec_ld1_c_clr (inst, oprd);
      break;
    case M_LD2_C_CLR:
      result = exec_ld2_c_clr (inst, oprd);
      break;
    case M_LD4_C_CLR:
      result = exec_ld4_c_clr (inst, oprd);
      break;
    case M_LD8_C_CLR:
      result = exec_ld8_c_clr (inst, oprd);
      break;
    case M_LD1_C_NC:
      result = exec_ld1_c_nc (inst, oprd);
      break;
    case M_LD2_C_NC:
      result = exec_ld2_c_nc (inst, oprd);
      break;
    case M_LD4_C_NC:
      result = exec_ld4_c_nc (inst, oprd);
      break;
    case M_LD8_C_NC:
      result = exec_ld8_c_nc (inst, oprd);
      break;
    case M_LD8_FILL:
      result = exec_ld8_fill (inst, oprd);
      break;

      // M02 instruction group
    case M_LD1_UPD_R:
      result = exec_ld1_upd_r (inst, oprd);
      break;
    case M_LD2_UPD_R:
      result = exec_ld2_upd_r (inst, oprd);
      break;
    case M_LD4_UPD_R:
      result = exec_ld4_upd_r (inst, oprd);
      break;
    case M_LD8_UPD_R:
      result = exec_ld8_upd_r (inst, oprd);
      break;
    case M_LD1_S_UPD_R:
      result = exec_ld1_s_upd_r (inst, oprd);
      break;
    case M_LD2_S_UPD_R:
      result = exec_ld2_s_upd_r (inst, oprd);
      break;
    case M_LD4_S_UPD_R:
      result = exec_ld4_s_upd_r (inst, oprd);
      break;
    case M_LD8_S_UPD_R:
      result = exec_ld8_s_upd_r (inst, oprd);
      break;
    case M_LD1_A_UPD_R:
      result = exec_ld1_a_upd_r (inst, oprd);
      break;
    case M_LD2_A_UPD_R:
      result = exec_ld2_a_upd_r (inst, oprd);
      break;
    case M_LD4_A_UPD_R:
      result = exec_ld4_a_upd_r (inst, oprd);
      break;
    case M_LD8_A_UPD_R:
      result = exec_ld8_a_upd_r (inst, oprd);
      break;
    case M_LD1_SA_UPD_R:
      result = exec_ld1_sa_upd_r (inst, oprd);
      break;
    case M_LD2_SA_UPD_R:
      result = exec_ld2_sa_upd_r (inst, oprd);
      break;
    case M_LD4_SA_UPD_R:
      result = exec_ld4_sa_upd_r (inst, oprd);
      break;
    case M_LD8_SA_UPD_R:
      result = exec_ld8_sa_upd_r (inst, oprd);
      break;
    case M_LD8_FILL_UPD_R:
      result = exec_ld8_fill_upd_r (inst, oprd);
      break;

      // M03 instruction group
    case M_LD1_UPD_I:
      result = exec_ld1_upd_i (inst, oprd);
      break;
    case M_LD2_UPD_I:
      result = exec_ld2_upd_i (inst, oprd);
      break;
    case M_LD4_UPD_I:
      result = exec_ld4_upd_i (inst, oprd);
      break;
    case M_LD8_UPD_I:
      result = exec_ld8_upd_i (inst, oprd);
      break;
    case M_LD1_S_UPD_I:
      result = exec_ld1_s_upd_i (inst, oprd);
      break;
    case M_LD2_S_UPD_I:
      result = exec_ld2_s_upd_i (inst, oprd);
      break;
    case M_LD4_S_UPD_I:
      result = exec_ld4_s_upd_i (inst, oprd);
      break;
    case M_LD8_S_UPD_I:
      result = exec_ld8_s_upd_i (inst, oprd);
      break;
    case M_LD1_A_UPD_I:
      result = exec_ld1_a_upd_i (inst, oprd);
      break;
    case M_LD2_A_UPD_I:
      result = exec_ld2_a_upd_i (inst, oprd);
      break;
    case M_LD4_A_UPD_I:
      result = exec_ld4_a_upd_i (inst, oprd);
      break;
    case M_LD8_A_UPD_I:
      result = exec_ld8_a_upd_i (inst, oprd);
      break;
    case M_LD1_SA_UPD_I:
      result = exec_ld1_sa_upd_i (inst, oprd);
      break;
    case M_LD2_SA_UPD_I:
      result = exec_ld2_sa_upd_i (inst, oprd);
      break;
    case M_LD4_SA_UPD_I:
      result = exec_ld4_sa_upd_i (inst, oprd);
      break;
    case M_LD8_SA_UPD_I:
      result = exec_ld8_sa_upd_i (inst, oprd);
      break;
    case M_LD1_C_CLR_UPD_I:
      result = exec_ld1_c_clr_upd_i (inst, oprd);
      break;
    case M_LD2_C_CLR_UPD_I:
      result = exec_ld2_c_clr_upd_i (inst, oprd);
      break;
    case M_LD4_C_CLR_UPD_I:
      result = exec_ld4_c_clr_upd_i (inst, oprd);
      break;
    case M_LD8_C_CLR_UPD_I:
      result = exec_ld8_c_clr_upd_i (inst, oprd);
      break;
    case M_LD8_FILL_UPD_I:
      result = exec_ld8_fill_upd_i (inst, oprd);
      break;

      // M04 instruction group
    case M_ST1:
    case M_ST1_REL:
      result = exec_st1 (inst, oprd);
      break;
    case M_ST2:
    case M_ST2_REL:
      result = exec_st2 (inst, oprd);
      break;
    case M_ST4:
    case M_ST4_REL:
      result = exec_st4 (inst, oprd);
      break;
    case M_ST8:
    case M_ST8_REL:
      result = exec_st8 (inst, oprd);
      break;
    case M_ST8_SPILL:
      result = exec_st8_spill (inst, oprd);
      break;

      // M05 instruction group
    case M_ST1_UPD_I:
    case M_ST1_REL_UPD_I:
      result = exec_st1_upd_i (inst, oprd);
      break;
    case M_ST2_UPD_I:
    case M_ST2_REL_UPD_I:
      result = exec_st2_upd_i (inst, oprd);
      break;
    case M_ST4_UPD_I:
    case M_ST4_REL_UPD_I:
      result = exec_st4_upd_i (inst, oprd);
      break;
    case M_ST8_UPD_I:
    case M_ST8_REL_UPD_I:
      result = exec_st8_upd_i (inst, oprd);
      break;
    case M_ST8_SPILL_UPD_I:
      result = exec_st8_spill_upd_i (inst, oprd);
      break;

      // M06 instruction group
    case M_LDFS:
      result = exec_ldfs (inst, oprd);
      break;
    case M_LDFD:
      result = exec_ldfd (inst, oprd);
      break;
    case M_LDFE:
      result = exec_ldfe (inst, oprd);
      break;
    case M_LDF8:
      result = exec_ldf8 (inst, oprd);
      break;
    case M_LDF_FILL:
      result = exec_ldf_fill (inst, oprd);
      break;
    case M_LDFS_S:
      result = exec_ldfs_s (inst, oprd);
      break;	
    case M_LDFD_S:
      result = exec_ldfd_s (inst, oprd);
      break;	
    case M_LDF8_S:
      result = exec_ldf8_s (inst, oprd);
      break;	
    case M_LDFE_S:
      result = exec_ldfe_s (inst, oprd);
      break;	
    case M_LDFS_A:
      result = exec_ldfs_a (inst, oprd);
      break;	
    case M_LDFD_A:
      result = exec_ldfd_a (inst, oprd);
      break;	
    case M_LDF8_A:
      result = exec_ldf8_a (inst, oprd);
      break;	
    case M_LDFE_A:
      result = exec_ldfe_a (inst, oprd);
      break;	
    case M_LDFS_SA:
      result = exec_ldfs_sa (inst, oprd);
      break;	
    case M_LDFD_SA:
      result = exec_ldfd_sa (inst, oprd);
      break;	
    case M_LDF8_SA:
      result = exec_ldf8_sa (inst, oprd);
      break;	
    case M_LDFE_SA:
      result = exec_ldfe_sa (inst, oprd);
      break;
    case M_LDFS_C_CLR:
      result = exec_ldfs_c_clr (inst, oprd);
      break;	
    case M_LDFD_C_CLR:
      result = exec_ldfd_c_clr (inst, oprd);
      break;	
    case M_LDF8_C_CLR:
      result = exec_ldf8_c_clr (inst, oprd);
      break;	
    case M_LDFE_C_CLR:
      result = exec_ldfe_c_clr (inst, oprd);
      break;
    case M_LDFS_C_NC:
      result = exec_ldfs_c_nc (inst, oprd);
      break;	
    case M_LDFD_C_NC:
      result = exec_ldfd_c_nc (inst, oprd);
      break;	
    case M_LDF8_C_NC:
      result = exec_ldf8_c_nc (inst, oprd);
      break;	
    case M_LDFE_C_NC:
      result = exec_ldfe_c_nc (inst, oprd);
      break;

    // M07 instruction group
    case M_LDFS_UPD_R:
      result = exec_ldfs_upd_r (inst, oprd);
      break;
    case M_LDFD_UPD_R:
      result = exec_ldfd_upd_r (inst, oprd);
      break;
    case M_LDFE_UPD_R:
      result = exec_ldfe_upd_r (inst, oprd);
      break;
    case M_LDF8_UPD_R:
      result = exec_ldf8_upd_r (inst, oprd);
      break;
    case M_LDFS_S_UPD_R:
      result = exec_ldfs_s_upd_r (inst, oprd);
      break;
    case M_LDFD_S_UPD_R:
      result = exec_ldfd_s_upd_r (inst, oprd);
      break;
    case M_LDFE_S_UPD_R:
      result = exec_ldfe_s_upd_r (inst, oprd);
      break;
    case M_LDF8_S_UPD_R:
      result = exec_ldf8_s_upd_r (inst, oprd);
      break;
    case M_LDFS_A_UPD_R:
      result = exec_ldfs_a_upd_r (inst, oprd);
      break;
    case M_LDFD_A_UPD_R:
      result = exec_ldfd_a_upd_r (inst, oprd);
      break;
    case M_LDFE_A_UPD_R:
      result = exec_ldfe_a_upd_r (inst, oprd);
      break;
    case M_LDF8_A_UPD_R:
      result = exec_ldf8_a_upd_r (inst, oprd);
      break;
    case M_LDFS_SA_UPD_R:
      result = exec_ldfs_sa_upd_r (inst, oprd);
      break;
    case M_LDFD_SA_UPD_R:
      result = exec_ldfd_sa_upd_r (inst, oprd);
      break;
    case M_LDFE_SA_UPD_R:
      result = exec_ldfe_sa_upd_r (inst, oprd);
      break;
    case M_LDF8_SA_UPD_R:
      result = exec_ldf8_sa_upd_r (inst, oprd);
      break;
    case M_LDF_FILL_UPD_R:
      result = exec_ldf_fill_upd_r (inst, oprd);
      break;

      // M08 instruction group
    case M_LDFS_UPD_I:
      result = exec_ldfs_upd_i (inst, oprd);
      break;
    case M_LDFD_UPD_I:
      result = exec_ldfd_upd_i (inst, oprd);
      break;
    case M_LDFE_UPD_I:
      result = exec_ldfe_upd_i (inst, oprd);
      break;
    case M_LDF8_UPD_I:
      result = exec_ldf8_upd_i (inst, oprd);
      break;
    case M_LDF_FILL_UPD_I:
      result = exec_ldf_fill_upd_i (inst, oprd);
      break;

      // M09 instruction group
    case M_STFS:
      result = exec_stfs (inst, oprd);
      break;
    case M_STFD:
      result = exec_stfd (inst, oprd);
      break;
    case M_STF8:
      result = exec_stf8 (inst, oprd);
      break;
    case M_STFE:
      result = exec_stfe (inst, oprd);
      break;
    case M_STF_SPILL:
      result = exec_stf_spill (inst, oprd);
      break;

      // M10 instruction group
    case M_STFS_UPD_I:
      result = exec_stfs_upd_i (inst, oprd);
      break;
    case M_STFD_UPD_I:
      result = exec_stfd_upd_i (inst, oprd);
      break;
    case M_STF8_UPD_I:
      result = exec_stf8_upd_i (inst, oprd);
      break;
    case M_STFE_UPD_I:
      result = exec_stfe_upd_i (inst, oprd);
      break;
    case M_STF_SPILL_UPD_I:
      result = exec_stf_spill_upd_i (inst, oprd);
      break;

      // M11 instruction group
    case M_LDFPS:
      result = exec_ldfps (inst, oprd);
      break;
    case M_LDFPD:
      result = exec_ldfpd (inst, oprd);
      break;
    case M_LDFP8:
      result = exec_ldfp8 (inst, oprd);
      break;

      // M12 instruction group
    case M_LDFPS_UPD:
      result = exec_ldfps_upd (inst, oprd);
      break;
    case M_LDFPD_UPD:
      result = exec_ldfpd_upd (inst, oprd);
      break;
    case M_LDFP8_UPD:
      result = exec_ldfp8_upd (inst, oprd);
      break;

      // M13 instruction group
    case M_LFETCH:
    case M_LFETCH_EXCL:
    case M_LFETCH_FAULT:
    case M_LFETCH_FAULT_EXCL:
      result = exec_lfetch (inst, oprd);
      break;

      // M14 instruction group
    case M_LFETCH_UPD_R:
    case M_LFETCH_EXCL_UPD_R:
    case M_LFETCH_FAULT_UPD_R:
    case M_LFETCH_FAULT_EXCL_UPD_R:
      result = exec_lfetch_upd_r (inst, oprd);
      break;

      // M15 instruction group
    case M_LFETCH_UPD_I:
    case M_LFETCH_EXCL_UPD_I:
    case M_LFETCH_FAULT_UPD_I:
    case M_LFETCH_FAULT_EXCL_UPD_I:
      result = exec_lfetch_upd_i (inst, oprd);      
      break;

      // M16 instruction group
    case M_CMPXCHG1_REL:
    case M_CMPXCHG1_ACQ:
      break;
    case M_CMPXCHG2_REL:
    case M_CMPXCHG2_ACQ:
      break;
    case M_CMPXCHG4_REL:
    case M_CMPXCHG4_ACQ:
      break;
    case M_CMPXCHG8_REL:
    case M_CMPXCHG8_ACQ:
      result = exec_cmpxchg8(inst, oprd);
      break;

      // M18 instruction group
    case M_SETF_SIG:
      result = exec_setf_sig (inst, oprd);
      break;
    case M_SETF_EXP:
      result = exec_setf_exp (inst, oprd);
      break;
    case M_SETF_S:
      result = exec_setf_s (inst, oprd);
      break;
    case M_SETF_D:
      result = exec_setf_d (inst, oprd);
      break;

      // M19 instruction group
    case M_GETF_SIG:
      result = exec_getf_sig (inst, oprd);
      break;
    case M_GETF_EXP:
      result = exec_getf_exp (inst, oprd);
      break;
    case M_GETF_S:
      result = exec_getf_s (inst, oprd);
      break;
    case M_GETF_D:
      result = exec_getf_d (inst, oprd);
      break;

      // M20 instruction group
    case M_CHK_S_M:
      result = exec_chk_s_m (inst, oprd);
      break;

      // M21 instruction group
    case M_CHK_S:
      result = exec_chk_s (inst, oprd);
      break;

      // M22 instruction group
    case M_CHK_A_CLR_I:
      result = exec_chk_a_clr_i (inst, oprd);
      break;
    case M_CHK_A_NC_I:
      result = exec_chk_a_nc_i (inst, oprd);
      break;

      // M23 instruction group
    case M_CHK_A_CLR_F:
      result = exec_chk_a_clr_f (inst, oprd);
      break;
    case M_CHK_A_NC_F:
      result = exec_chk_a_nc_f (inst, oprd);
      break;

      // M24 instruction group
    case M_MF:
    case M_FWB:
    case M_MF_A:
    case M_SRLZ_D:
    case M_SRLZ_I:  
    case M_SYNC_I:  
      result.setvalid (true);
      break;
    case M_INVALA:
      result = exec_invala (inst, oprd);
      break;

      // M29 instruction group
    case M_MOV_TO_AR_R:
      result = exec_mov_to_ar_r (inst, oprd);
      break;

      // M30 instruction group
    case M_MOV_TO_AR_I:
      result = exec_mov_to_ar_i (inst, oprd);
      break;

      // M31 instruction group
    case M_MOV_FROM_AR:
      result = exec_mov_from_ar (inst, oprd);
      break;

      // M32 instruction group
    case M_MOV_TO_CR:
      result = exec_mov_to_cr (inst, oprd);
      break;

      // M33 instruction group
    case M_MOV_FROM_CR:
      result = exec_mov_from_cr (inst, oprd);
      break;

      // M37 instruction group
    case M_BREAK:
      throw Interrupt (FAULT_IT_INST_BREAK, inst);
      break;
    case M_NOP:
      result.setvalid (true);
      break;

      // M34 instruction group
    case M_ALLOC:
      result = exec_alloc (inst, oprd);
      break;

      // M44 instruction group
    case M_SUM:
      result = exec_sum (inst, oprd);
      break;

    default:
      break;
    }
    // check for valid result
    if (result.isvalid () == false) {
      string reason = "unimplemented instruction [M]";
      throw Interrupt (FAULT_IT_OPER_LEGAL, inst, reason);
    }
    return result;
  }
}
