// ---------------------------------------------------------------------------
// - Srn.hpp                                                                 -
// - iato:iaoo client - simulator resource name                              -
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
  /// resource decode name
  const string RESOURCE_DEC = "DEC";
  /// resource logical renaming name
  const string RESOURCE_LRN = "LRN";
  /// resource physical renaming name
  const string RESOURCE_PRN = "PRN";
  /// resource expand stage name
  const string RESOURCE_EXP = "EXP";
  /// resource select stage name
  const string RESOURCE_SLC = "SLC";
  /// resource acquisition stage name
  const string RESOURCE_REG = "REG";
  /// resource delay stage name
  const string RESOURCE_DLY = "DLY";
  /// resource execution stage name
  const string RESOURCE_EXE = "EXE";
  /// resource memory read stage name
  const string RESOURCE_MRD = "MRD";
  /// resource write back stage name
  const string RESOURCE_WBK = "WBK";
  /// resource commit stage name
  const string RESOURCE_CMT = "CMT";
}

#endif
