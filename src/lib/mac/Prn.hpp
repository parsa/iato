// ---------------------------------------------------------------------------
// - Prn.hpp                                                                 -
// - iato:mac library - default physical resource name                       -
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

#ifndef  IATO_PRN_HPP
#define  IATO_PRN_HPP

#ifndef  IATO_LRN_HPP
#include "Lrn.hpp"
#endif

namespace iato {
  using namespace std;

  /// resource external interrupt buffer name
  const string RESOURCE_EIB = "EIB";
  /// resource instruction interrupt buffer name
  const string RESOURCE_IIB = "IIB";
  /// resource bypass network element name
  const string RESOURCE_BNE = "BNE";
  /// resource bypass network name
  const string RESOURCE_BPN = "BPN";
  /// resource memory bypass network name
  const string RESOURCE_MBN = "MBN";

  /// resource decoupling buffer name
  const string RESOURCE_BDB = "BDB";
  /// resource slot port buffer name
  const string RESOURCE_SPB = "SPB";
  /// resource bundle dispersal name
  const string RESOURCE_BDS = "BDS";
  /// resource pipeline scoreboard name
  const string RESOURCE_PSB = "PSB";
  /// resource hazard detection logic
  const string RESOURCE_HDL = "HDL";
  /// resource memory ordering buffer name
  const string RESOURCE_MOB = "MOB";

  /// resource translation register bank
  const string RESOURCE_TRB = "TRB";
  /// resource universal register bank
  const string RESOURCE_URB = "URB";
  /// resource universal register file
  const string RESOURCE_URF = "URF";
  /// resource register alias table name
  const string RESOURCE_RAT = "RAT";
  /// resource instruction queue name
  const string RESOURCE_EIQ = "EIQ";
  /// resource reervation station name
  const string RESOURCE_RST = "RST";
  /// resource reorder buffer name
  const string RESOURCE_ROB = "ROB";
  /// resource instruction result name
  const string RESOURCE_IRB = "IRB";
  /// resource grand central station name
  const string RESOURCE_GCS = "GCS";
  /// resource detection logic name
  const string RESOURCE_DTL = "DTL";
  /// resource flush and restart name
  const string RESOURCE_PFR = "PFR";
  /// resource store buffer name
  const string RESOURCE_STB = "STB";
  /// resource replay instruction buffer  name
  const string RESOURCE_RIB = "RIB";

  /// resource branch predictor name
  const string RESOURCE_BPS = "BPS";
  /// resource branch predictor bimodal
  const string RESOURCE_BBP = "BBP";
  /// resource rse stack name
  const string RESOURCE_RTK = "RTK";
  /// resource pattern history table name
  const string RESOURCE_PHT = "PHT";

  /// resource predicate predictor name
  const string RESOURCE_PPS = "PPS";

  /// resource memory port request name
  const string RESOURCE_MPR = "MPR";
  /// resource memory transaction adapter
  const string RESOURCE_MTA = "MTA";
  /// resource hierarchical memory architecture
  const string RESOURCE_HMA = "HMA";
  /// memory logic resource name
  const string RESOURCE_MLI = "MLI";
  /// memory synchro resource name
  const string RESOURCE_MSI = "MSI";

  /// resource pipelane name
  const string RESOURCE_PLN = "PLN";
  /// resource pipeline name
  const string RESOURCE_PPL = "PPL";

  /// resource system memory bus name
  //const string RESOURCE_SMB = "SMB";
  /// resource system memory port name
  //const string RESOURCE_SMP = "SMP";
}

#endif
