// ---------------------------------------------------------------------------
// - Isa.hpp                                                                 -
// - iato:isa library - default isa parameters                               -
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

#ifndef IATO_ISA_HPP
#define IATO_ISA_HPP

#ifndef  IATO_TYPES_HPP
#include "Types.hpp"
#endif

namespace iato {

  // -------------------------------------------------------------------------
  // - architectural definitions                                             -
  // -------------------------------------------------------------------------

  /// number of slots per bundles
  const long BN_SLSZ = 3;
  /// size of bundle in bytes
  const long BN_BYSZ = 16;
  /// default bundle window size
  const long BN_IWSZ = 2;

  /// number of general registers, bank 1
  const long GR_LRSZ = 128;
  /// number of alternate general registers, bank 0
  const long GR_BKSZ = 16;
  /// base of alternate general register, bank 0
  const long GR_BKBS = 16;
  /// top of alternate general register, bank 0
  const long GR_BKTP = GR_BKBS + GR_BKSZ;
  /// base of first stack register
  const long GR_STBS = 32;
  /// number of logical stacked registers
  const long GR_STSZ = GR_LRSZ - GR_STBS;
  /// number of floating registers
  const long FR_LRSZ = 128;
  /// base of first rotating floating register
  const long FR_RTBS = 32;
  /// number of rotating floating registers
  const long FR_RTSZ = FR_LRSZ - FR_RTBS;
  /// number of predicate registers
  const long PR_LRSZ = 64;
  /// base of first rotating predicate register
  const long PR_RTBS = 16;
  /// number of rotating predicate registers
  const long PR_RTSZ = PR_LRSZ - PR_RTBS;
  /// number of branch registers
  const long BR_LRSZ = 8;
  /// number of application registers
  const long AR_LRSZ = 128;
  /// number of control registers
  const long CR_LRSZ = 128;

  /// index of rsc register (register stack configuration)
  const long AR_RSC  = 16;
  /// index of bsp register (backing store pointer)
  const long AR_BSP  = 17;
  /// index of bspstore register (backing store pointer for memory store)
  const long AR_BSPS = 18;
  /// index of rnat register (rse nat collection)
  const long AR_RNAT = 19;
  /// index of ccv register (compare and exchange compare value register)
  const long AR_CCV  = 32;
  /// index of unat register (user nat collection)
  const long AR_UNAT = 36;
  /// index of fpsr register (floating point status register)
  const long AR_FPSR = 40;
  /// index of pfs register (previous function state)
  const long AR_PFS  = 64;
  /// index of lc register (loop count)
  const long AR_LC   = 65;
  /// index of ec register (epilog count)
  const long AR_EC   = 66;

  /// index of the interuption vector address control register
  const long CR_IVA  = 2;
  /// index of the page table address control register
  const long CR_PTA  = 8;
  /// index of first interuption control register
  const long CR_FIR  = 16;
  /// index of last interuption control register
  const long CR_LIR  = 25;

  /// maximum number of source operands
  const long IA_MSRC = 4;
  /// maximum number of destination operands
  const long IA_MDST = 5;
  /// maximum number of rpm for raw or waw violations
  const long IA_MRPM = 2;

  /// number of interrupt code
  const long IA_ICSZ = 82;

  /// maximum register data request (in bytes)
  const long IA_MRDR = 16;

  // -------------------------------------------------------------------------
  // - boot and initial constants                                            -
  // -------------------------------------------------------------------------

  /// default rse sof value
  const long   DEF_SOF = 96;
  /// default rse sol value
  const long   DEF_SOL = 0;
  /// default rse sor value
  const long   DEF_SOR = 0;
  /// default rse rrb gr value
  const long   DEF_RGR = 0;
  /// default rse rrb fr value
  const long   DEF_RFR = 0;
  /// default rse rrb pr value
  const long   DEF_RPR = 0;
  /// default cfm value
  const t_octa DEF_CFM  = DEF_SOF + (DEF_SOL << 7) + (DEF_SOR << 14);
  /// default psr value
  const t_octa DEF_PSR  = 0x0000100000000000ULL;
  /// default umr value
  const t_byte DEF_UMR  = 0x00U;
  /// default fpsr value
  const t_octa DEF_FPSR = 0x0009804C0270033FULL;
  /// default rnat value
  const t_octa DEF_RNAT = OCTA_0;
  /// default unat value
  const t_octa DEF_UNAT = OCTA_0;

  // -------------------------------------------------------------------------
  // - global resource definitions                                           -
  // -------------------------------------------------------------------------

  /// the alat default size
  const long AL_SIZE = 32;

  // -------------------------------------------------------------------------
  // - tracer definitions                                                    -
  // -------------------------------------------------------------------------

  /// the tracer threshold
  const long   TR_THRS = 512;
  /// the tracer begin index
  const t_long TR_BICC = -1;
  /// the tracer end index
  const t_long TR_EICC = -1;
  /// max record per trace
  const long   TR_RMAX = 0;
}

#endif
