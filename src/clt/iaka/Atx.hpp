// ---------------------------------------------------------------------------
// - Atx.hpp                                                                 -
// - iato:iaka client - application context class definition                 -
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

#ifndef  IATO_ATX_HPP
#define  IATO_ATX_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

namespace iato {

  /// The Atx class is the application context that is build from the
  /// the library context. The application context holds additional parameters
  /// that are specific to the emulation environment.

  class Atx : public Mtx {
  public:
    /// create a new context
    Atx (void);

    /// reset this context
    void reset (void);

  private:
    // make the copy constructor private
    Atx (const Atx&);
    // make the assignment operator private
    Atx& operator = (const Atx&);
  };
}

#endif

