// ---------------------------------------------------------------------------
// - Intcode.hpp                                                             -
// - iato:isa library - interrupt code definition                            -
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

#ifndef  IATO_INTCODE_HPP
#define  IATO_INTCODE_HPP

namespace iato {

  /// interrupt code type
  enum t_icode {
    ABORT_RESET         =  1, // machine reset
    ABORT_MCA           =  2, // machine check
    INTER_INIT          =  3, // initialization
    INTER_PMI           =  4, // platform management
    INTER_INT           =  5, // external interrupt
    FAULT_IR_DATA_UADDR =  6, // incomplete frame unimplemented data address
    FAULT_IR_DATA_NSTLB =  7, // incomplete frame data nested tlb fault
    FAULT_IR_DATA_ALTLB =  8, // incomplete frame alternate data tlb fault
    FAULT_IR_DATA_VHPT  =  9, // incomplete frame vhpt data fault
    FAULT_IR_DATA_TLB   = 10, // incomplete frame data tlb fault
    FAULT_IR_DATA_PAGE  = 11, // incomplete frame data page not present
    FAULT_IR_DATA_NATPG = 12, // incomplete frame data nat page consumption
    FAULT_IR_DATA_KMISS = 13, // incomplete frame data key miss
    FAULT_IR_DATA_KPERM = 14, // incomplete frame data key permission
    FAULT_IR_DATA_ACRGT = 15, // incomplete frame data access rights
    FAULT_IR_DATA_ACBIT = 16, // incomplete frame data access bit
    FAULT_IR_DATA_DEBUG = 17, // incomplete frame data debug
    FAULT_IT_INST_ALTLB = 20, // alternate instruction tlb fault
    FAULT_IT_INST_VHPT  = 21, // vhpt instruction fault
    FAULT_IT_INST_TLB   = 22, // instruction tlb fault
    FAULT_IT_INST_PAGE  = 23, // instruction page not present fault
    FAULT_IT_INST_NATPG = 24, // instruction nat page consumption fault
    FAULT_IT_INST_KMISS = 25, // instruction key miss fault
    FAULT_IT_INST_KPERM = 26, // instruction key permission
    FAULT_IT_INST_ACRGT = 27, // instruction access rights fault
    FAULT_IT_INST_ACBIT = 28, // instruction access bit fault
    FAULT_IT_INST_DEBUG = 29, // instruction debug fault
    FAULT_IT_OPER_LEGAL = 33, // illegal operation fault
    FAULT_IT_DEPD_LEGAL = 34, // illegal dependency fault
    FAULT_IT_INST_BREAK = 35, // break instruction fault
    FAULT_IT_OPER_PRIV  = 36, // privilege operation fault
    FAULT_IT_FPRG_DISB  = 37, // disable floating point register fault
    FAULT_IT_INST_DISB  = 38, // disable instuction set transition fault
    FAULT_IT_RNAT_CONS  = 41, // register nat consumption fault
    FAULT_IT_RESV_RGFD  = 42, // reserved register/field fault
    FAULT_IT_DATA_UADDR = 43, // unimplemented data address fault
    FAULT_IT_PRIV_REG   = 44, // privileged register fault
    FAULT_IT_OPER_SPEC  = 45, // speculative operation fault
    FAULT_IT_DATA_NSTLB = 48, // data nested tlb fault
    FAULT_IT_DATA_ALTLB = 49, // alternate data tlb fault
    FAULT_IT_DATA_VHPT  = 50, // vhpt data fault
    FAULT_IT_DATA_TLB   = 51, // data tlb fault
    FAULT_IT_DATA_PAGE  = 52, // data page not present fault
    FAULT_IT_DATA_NATPG = 53, // data nat page consumption fault
    FAULT_IT_DATA_KMISS = 54, // data key miss fault
    FAULT_IT_DATA_KPERM = 55, // data key permission fault
    FAULT_IT_DATA_ACRGT = 56, // data access rights fault
    FAULT_II_DATA_DTBIT = 57, // data dirty bit fault
    FAULT_IT_DATA_ACBIT = 58, // data access bit fault
    FAULT_IT_DATA_DEBUG = 59, // data debug fault
    FAULT_IT_DATA_ALIGN = 60, // unaligned data reference fault
    FAULT_IT_DATA_UNREF = 67, // unsupported data reference fault
    FAULT_IT_FP         = 68, // floating point fault
    TRAPS_IT_INST_UADDR = 69, // unimplemented instruction address trap
    TRAPS_IT_FP         = 70, // floating point trap
    TRAPS_IT_PRIV_LOWER = 71, // lower privilege transfer trap
    TRAPS_IT_BR_TAKEN   = 72, // taken branch trap
    TRAPS_IT_STEP       = 73, // single step trap
    TRAPS_IT_MAX        = 81  // maximum interrupt number
  };

  /// the abort floor code
  const t_icode ABORT_FLOOR = ABORT_RESET;
  /// the abort ceiling code
  const t_icode ABORT_CEIL  = ABORT_MCA;
  /// the interrupt floor code
  const t_icode INTER_FLOOR = INTER_INIT;
  /// the interrupt ceiling code
  const t_icode INTER_CEIL  = INTER_INT;
  /// the fault floor code
  const t_icode FAULT_FLOOR = FAULT_IR_DATA_UADDR;
  /// the fault ceiling code
  const t_icode FAULT_CEIL  = FAULT_IT_FP;
  /// the trap floor code
  const t_icode TRAPS_FLOOR = TRAPS_IT_INST_UADDR;
  /// the trap ceiling code
  const t_icode TRAPS_CEIL  = TRAPS_IT_MAX;
}

#endif
