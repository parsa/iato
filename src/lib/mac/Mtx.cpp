// ---------------------------------------------------------------------------
// - Mtx.cpp                                                                -
// - iato:mac library - micro-architecture context class implementation      -
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

#include "Mtx.hpp"
#include "Mac.hpp"

namespace iato {

  // create a new context

  Mtx::Mtx (void) {
    reset ();
  }

  // reset this context to a default

  void Mtx::reset (void) {
    // reset base context
    Etx::reset ();
    // update architecture
    update (DEF_ARCH);
  }

  // update this context with a particular architecture

  void Mtx::update (const t_arch arch) {
    // update base system
    Etx::update (arch);

    // set default architecture info
    d_lmap["BDB-SIZE"]                   = DB_SIZE;
    d_lmap["RST-SIZE"]                   = RS_TBSZ;
    d_lmap["EIQ-SIZE"]                   = IQ_SIZE;
    d_lmap["ISB-SIZE"]                   = IS_SIZE;
    d_lmap["ROB-SIZE"]                   = RB_SIZE;
    d_lmap["IIB-SIZE"]                   = IB_SIZE;
    d_lmap["IRB-SIZE"]                   = IR_SIZE;
    d_lmap["MOB-SIZE"]                   = OB_SIZE;
    d_lmap["STB-SIZE"]                   = SB_SIZE;
    d_lmap["RIB-SIZE"]                   = RI_SIZE;

    // global prediction parameters
    d_lmap["HTR-SIZE"]                   = PS_HTRS;
    d_lmap["PHT-SIZE"]                   = PS_PHTS;

    // branch prediction parameters
    d_smap["BRANCH-PREDICTOR-TYPE"]      = BP_TYPE;
    d_lmap["BTB-SIZE"]                   = BP_BTBS;
    d_lmap["RTK-SIZE"]                   = BP_RTKS;

    // predicate predictions parameters
    d_smap["PREDICATE-PREDICTOR-TYPE"]   = PP_TYPE;
    d_bmap["USE-CONFIDENCE-FLAG"]        = PP_UCFG;
    d_bmap["PREDICATE-FORCE-VALUE"]      = PP_FVAL;

    // set register file parameters
    d_lmap["PR-UR-SIZE"]                 = UR_PRSZ;

    // set default memory model
    d_bmap["MEMORY-BYPASS-MODEL"]        = true;

    // set archiecture specific parameters
    switch (arch) {
    case MKINLEY:
      // unit info
      d_lmap["NUMBER-M-UNITS"]           = MK_MUSZ;
      d_lmap["NUMBER-I-UNITS"]           = MK_IUSZ;
      d_lmap["NUMBER-F-UNITS"]           = MK_FUSZ;
      d_lmap["NUMBER-B-UNITS"]           = MK_BUSZ;
      // unit latency
      d_lmap["LATENCY-M-UNIT"]           = MK_MLAT;
      d_lmap["LATENCY-I-UNIT"]           = MK_ILAT;
      d_lmap["LATENCY-F-UNIT"]           = MK_FLAT;
      d_lmap["LATENCY-B-UNIT"]           = MK_BLAT;
      // cache info
      d_lmap["CACHE-DEPTH"]              = MK_CLVL;
      d_lmap["CACHE-SIZE-IL1"]           = MK_IL1S;
      d_lmap["CACHE-SIZE-DL1"]           = MK_DL1S;
      d_lmap["CACHE-SIZE-UL2"]           = MK_UL2S;
      d_lmap["CACHE-SIZE-UL3"]           = MK_UL3S;
      break;
    case ITANIUM:
      // unit info
      d_lmap["NUMBER-M-UNITS"]           = IT_MUSZ;
      d_lmap["NUMBER-I-UNITS"]           = IT_IUSZ;
      d_lmap["NUMBER-F-UNITS"]           = IT_FUSZ;
      d_lmap["NUMBER-B-UNITS"]           = IT_BUSZ;
      // unit latency
      d_lmap["LATENCY-M-UNIT"]           = IT_MLAT;
      d_lmap["LATENCY-I-UNIT"]           = IT_ILAT;
      d_lmap["LATENCY-F-UNIT"]           = IT_FLAT;
      d_lmap["LATENCY-B-UNIT"]           = IT_BLAT;
      // cache info
      d_lmap["CACHE-DEPTH"]              = IT_CLVL;
      d_lmap["CACHE-SIZE-IL1"]           = IT_IL1S;
      d_lmap["CACHE-SIZE-DL1"]           = IT_DL1S;
      d_lmap["CACHE-SIZE-UL2"]           = IT_UL2S;
      d_lmap["CACHE-SIZE-UL3"]           = IT_UL3S;
      break;
    }
  }

  // return the system bus size

  long Mtx::getsbsz (void) const {
    return getlong ("SYSTEM-BUS-SIZE");
  }

  //return the system bus latency

  long Mtx::getsblt (void) const {
    return getlong ("SYSTEM-BUS-LATENCY");
  }

  // return the total number of units

  long Mtx::gettusz (void) const {
    long musz = getlong ("NUMBER-M-UNITS");
    long iusz = getlong ("NUMBER-I-UNITS");
    long fusz = getlong ("NUMBER-F-UNITS");
    long busz = getlong ("NUMBER-B-UNITS");
    long tusz = musz + iusz + fusz + busz;
    return tusz;
  }
}
