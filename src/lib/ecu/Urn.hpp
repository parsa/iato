// ---------------------------------------------------------------------------
// - Urn.hpp                                                                 -
// - iato:ecu library - emulator extension resource name                     -
// ---------------------------------------------------------------------------
// - (c) inria 2002-2004                                                     -
// ---------------------------------------------------------------------------
// - authors                                      Amaury Darsch    2002:2004 -
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

#ifndef  IATO_URN_HPP
#define  IATO_URN_HPP

#ifndef  IATO_PRN_HPP
#include "Prn.hpp"
#endif

namespace iato {
  using namespace std;

  /// resource fetcher name
  const string RESOURCE_FTC = "FTC";
  /// resource mapper name
  const string RESOURCE_MMP = "MMP";
}

#endif
