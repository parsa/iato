// ---------------------------------------------------------------------------
// - Srn.hpp                                                                 -
// - iato:iaio client - in-order simulator resource name                     -
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

#ifndef  IATO_SRN_HPP
#define  IATO_SRN_HPP

#ifndef  IATO_PRN_HPP
#include "Prn.hpp"
#endif

namespace iato {
  using namespace std;

  /// resource ip generation name
  const string RESOURCE_IPG = "IPG";
  /// resource bundle rotation name
  const string RESOURCE_ROT = "ROT";
  /// resource bundle expand name
  const string RESOURCE_EXP = "EXP";
  /// resource instruction rename name
  const string RESOURCE_REN = "REN";
  /// resource delay stage name
  const string RESOURCE_DLY = "DLY";
  /// resource register read name
  const string RESOURCE_REG = "REG";
  /// resource instruction execute name
  const string RESOURCE_EXE = "EXE";
  /// resource bypass operand branch name
  const string RESOURCE_BOB = "BOB";
  /// resource exception detect name
  const string RESOURCE_DET = "DET";
  /// resource branch execute and detect name
  const string RESOURCE_BED = "BED";
  /// resource instruction write-back name
  const string RESOURCE_WRB = "WRB";
  /// resource instruction floating write-back name
  const string RESOURCE_FWB = "FWB";
  /// resource floating-point stage 1
  const string RESOURCE_FP1 = "FP1";
  /// resource floating-point stage 2
  const string RESOURCE_FP2 = "FP2";
  /// resource floating-point stage 3
  const string RESOURCE_FP3 = "FP3";
  /// resource floating-point stage 4
  const string RESOURCE_FP4 = "FP4";

  /// early bypass network resource name
  const string RESOURCE_EBN = "EBN";
  /// late bypass network resource name
  const string RESOURCE_LBN = "LBN";
  /// early memory bypass network resource name
  const string RESOURCE_EMB = "EMB";
  /// late memory bypass network resource name
  const string RESOURCE_LMB = "LMB";
  /// issue port buffer name
  const string RESOURCE_IPB = "IPB";
  /// output port buffer name
  const string RESOURCE_OPB = "OPB";
}

#endif
