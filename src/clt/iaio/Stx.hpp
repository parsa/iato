// ---------------------------------------------------------------------------
// - Stx.hpp                                                                 -
// - iato:iaio client - simulation context class definition                  -
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

#ifndef  IATO_STX_HPP
#define  IATO_STX_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

namespace iato {

  /// The Stx class is the simulation context that is build from the
  /// the library context. The simulation context holds additional parameters
  /// that are specific to the simulation environment.

  class Stx : public Mtx {
  public:
    /// create a new context
    Stx (void);

    /// reset this context
    void reset (void);

  private:
    // make the copy constructor private
    Stx (const Stx&);
    // make the assignment operator private
    Stx& operator = (const Stx&);
  };
}

#endif

