// ---------------------------------------------------------------------------
// - Lrn.hpp                                                                 -
// - iato:isa library - default logical resource name                        -
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

#ifndef  IATO_LRN_HPP
#define  IATO_LRN_HPP

#ifndef  IATO_TYPES_HPP
#include "Types.hpp"
#endif

namespace iato {
  using namespace std;
  
  /// resource register bank name 
  const string RESOURCE_RBK = "RBK";
  /// resource register stack engine name
  const string RESOURCE_RSE = "RSE";
  /// resource interrupt routing table
  const string RESOURCE_IRT = "IRT";
  /// resource alat name
  const string RESOURCE_ALT = "ALT";
  /// resource memory logic name
  const string RESOURCE_MLG = "MLG";
}

#endif
