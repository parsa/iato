// ---------------------------------------------------------------------------
// - Soo.hpp                                                                 -
// - iato:iaio client - out-of-order simulator parameters definition         -
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

#ifndef  IATO_SOO_HPP
#define  IATO_SOO_HPP

#ifndef  IATO_MAC_HPP
#include "Mac.hpp"
#endif

namespace iato {
  using namespace std;

  /// maximum cycle count (0 = infinite)
  const long   SM_MXCC = 0;

  /// global predicate prediction
  const bool   PP_FGPP = false;
  /// incomplete bypassing with pp
  const bool   PP_IBYP = false;
}

#endif
