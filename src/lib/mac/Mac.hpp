// ---------------------------------------------------------------------------
// - Mac.hpp                                                                 -
// - iato:mac library - default architecture parameters                      -
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

#ifndef IATO_MAC_HPP
#define IATO_MAC_HPP

#ifndef  IATO_ABI_HPP
#include "Abi.hpp"
#endif

namespace iato {
  using namespace std;

  // -------------------------------------------------------------------------
  // -  global resource modes                                                -
  // -------------------------------------------------------------------------

  /// default partial flush mode
  const bool   RM_PFLS = false;

  // -------------------------------------------------------------------------
  // - generic resource parameters                                           -
  // -------------------------------------------------------------------------

  /// default decoupling buffer size
  const long   DB_SIZE = 8;
  /// default instruction queue size
  const long   IQ_SIZE = 16;
  /// default scoreboard size
  const long   IS_SIZE = 128;
  /// default interrupt buffer size
  const long   IB_SIZE = 128;
  /// default station size
  const long   RS_TBSZ = 16;
  /// default rob size
  const long   RB_SIZE = 256;
  /// default irb size
  const long   IR_SIZE = 256;
  /// default ordering buffer size
  const long   OB_SIZE = 64;
  /// default store buffer size
  const long   SB_SIZE = 64;
  /// default rib size
  const long   RI_SIZE = 64;

  // -------------------------------------------------------------------------
  // -  global prediction resources                                          -
  // -------------------------------------------------------------------------

  /// default htr size
  const long   PS_HTRS = 12;
  /// default pht size (2**PS_HTRS)
  const long   PS_PHTS = 4096;

  // -------------------------------------------------------------------------
  // - branch prediction resources                                           -
  // -------------------------------------------------------------------------

  /// default branch predictor type
  const string BP_TYPE = "none";

  /// default btb size
  const long   BP_BTBS = PS_PHTS;
  /// default rse stack size
  const long   BP_RTKS = 32;

  // -------------------------------------------------------------------------
  // - predicate prediction resources                                        -
  // -------------------------------------------------------------------------

  /// default predicate predictor type
  const string PP_TYPE = "none";

  /// use branch for update
  const bool   PP_BUPD = true;
  /// use confidence flag
  const bool   PP_UCFG = true;
  /// branch history update only
  const bool   PP_BHUO = false;

  /// the master hybrid predictor
  const string PP_HYPM = "pimodal";
  /// the slave hybrid predictor
  const string PP_HYPS = "pshare";

  /// default perceptron size
  const long   PP_PSIZ = PS_HTRS;
  /// default perceptron bound
  const long   PP_PBND = 128;
  /// default perceptron threshold
  const long   PP_PTHR = PP_PBND / 2;
  /// default perceptron confidence bound
  const long   PP_PCBN = 16;

  // -------------------------------------------------------------------------
  // - register file                                                         -
  // -------------------------------------------------------------------------

  /// number of universal registers
  const long UR_PRSZ = 256;

  /// max number of gr source operands
  const long GR_MSRC = 2;
  /// max number of gr destination
  const long GR_MDST = 1;
  /// max number of gr destination with loads
  const long GR_MLDS = 2;

  // -------------------------------------------------------------------------
  // - itanium specific parameters                                           -
  // -------------------------------------------------------------------------

  /// default number of M units
  const long IT_MUSZ = 2;
  /// default number of I units
  const long IT_IUSZ = 2;
  /// default number of F units
  const long IT_FUSZ = 2;
  /// default number of B units
  const long IT_BUSZ = 3;
  /// total number of units
  const long IT_TUSZ = IT_MUSZ + IT_IUSZ + IT_FUSZ + IT_BUSZ;

  /// system bus size in bytes
  const long IT_SBSZ = 8;
  /// system bus latency [133 Mhz @ 800Mhz]
  const long IT_SBLT = 6;
  /// system memory latency
  const long IT_SMLT = 1;

  /// M unit latency
  const long IT_MLAT = 1;
  /// I unit latency
  const long IT_ILAT = 1;
  /// F unit latency
  const long IT_FLAT = 3;
  /// B unit latency
  const long IT_BLAT = 1;

  /// max number of gr loads
  const long IT_GRLD = 1;
  /// max number of gr store
  const long IT_GRST = 1;

  /// register file port latency
  const long IT_GRFL = 2;
  const long IT_FRFL = 2;
  const long IT_PRFL = 2;
  const long IT_BRFL = 2;
  const long IT_ARFL = 2;

  /// number of cache level
  const long IT_CLVL = 3;

  /// level-1 instruction size, 16 Kib
  const long IT_IL1S = 16 * 1024;  
  /// level-1 instruction line size, 256
  const long IT_IL1L = 256;
  /// level-1 data size, 16 Kib
  const long IT_DL1S = 16 * 1024;
  /// level-1 data line size, 256
  const long IT_DL1L = 256;
  /// level-2 unified size, 96 Kib
  const long IT_UL2S = 96 * 1024;
  /// level-2 data line size, 128
  const long IT_UL2L = 128;
  /// level-3 unified size, 4 Mib
  const long IT_UL3S = 4 * 1024 * 1024;
  /// level-3 data line size, 128
  const long IT_UL3L = 128;

  // -------------------------------------------------------------------------
  // - mckinley specific parameters                                          -
  // -------------------------------------------------------------------------

  /// default number of M units
  const long MK_MUSZ = 4;
  /// default number of I units
  const long MK_IUSZ = 2;
  /// default number of F units
  const long MK_FUSZ = 2;
  /// default number of B units
  const long MK_BUSZ = 3;
  /// total number of units
  const long MK_TUSZ = MK_MUSZ + MK_IUSZ + MK_FUSZ + MK_BUSZ;

  /// system bus size in bytes
  const long MK_SBSZ = 16;
  /// system bus latency [200 Mhz @ 1Ghz]
  const long MK_SBLT = 5;
  /// system memory latency
  const long MK_SMLT = 1;

  /// M unit latency
  const long MK_MLAT = 1;
  /// I unit latency
  const long MK_ILAT = 1;
  /// F unit latency
  const long MK_FLAT = 2;
  /// B unit latency
  const long MK_BLAT = 1;

  /// max number of gr loads
  const long MK_GRLD = 2;
  /// max number of gr store
  const long MK_GRST = 2;

  /// register file port latency
  const long MK_GRFL = 1;
  const long MK_FRFL = 1;
  const long MK_PRFL = 1;
  const long MK_BRFL = 1;
  const long MK_ARFL = 1;

  /// number of cache level
  const long MK_CLVL = 3;

  /// level-1 instruction size, 16 Kib
  const long MK_IL1S = 16 * 1024;
  /// level-1 instruction line size, 256
  const long MK_IL1L = 256;
  /// level-1 data size, 16 Kib
  const long MK_DL1S = 16 * 1024;
  /// level-1 data line size, 256
  const long MK_DL1L = 256;
  /// level-2 unified size, 256 Kib
  const long MK_UL2S = 256 * 1024;
  /// level-2 data line size, 128
  const long MK_UL2L = 128;
  /// level-3 unified size, 3 Mib
  const long MK_UL3S = 3 * 1024 * 1024;
  /// level-3 data line size, 128
  const long MK_UL3L = 128;
}

#endif
