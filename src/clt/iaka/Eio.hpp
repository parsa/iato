// ---------------------------------------------------------------------------
// - Eio.hpp                                                                 -
// - iato:iaka client - in-order emulator parameters definition              -
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

#ifndef  IATO_EIO_HPP
#define  IATO_EIO_HPP

#ifndef  IATO_ECU_HPP
#include "Ecu.hpp"
#endif

namespace iato {
  /// maximum cycle count (0 = infinite)
  const long EM_MXCC = 0;
  /// default pipeline refill
  const long EM_PFIL = 10;

  /// default tracer branch only
  const bool EM_BOFL = false;
  /// default tracer predicate only flag
  const bool EM_POFL = false;
  /// default tracer predicate targets only flag
  const bool EM_PGFL = false;
}

#endif
