// ---------------------------------------------------------------------------
// - Watchdog.hpp                                                            -
// - iato:mac library - watchdog class definition                            -
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

#ifndef  IATO_WATCHDOG_HPP
#define  IATO_WATCHDOG_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Watchdog class is a simple class used to detect infinite loop.
  /// The watch dog operate with a counter which is decreased at each cycle.
  /// When a particular stage judge pertinent to say that it is alive
  /// it notify the watchdog which reload its counter. The watchdog is
  /// initialized with a 100 default cycle counter.

  class Watchdog : public Resource {
  private:
    /// the watchdog time
    long d_time;
    /// the watchdog counter
    long d_wcnt;

  public:
    /// create a new watchdog
    Watchdog (void);

    /// create a new watchdog with a context
    /// @param mtx the architectural context
    Watchdog (Mtx* mtx);

    /// reset this watchdog
    void reset (void);

    /// report this resource
    void report (void) const;

    /// notify this watchdog
    void notify (void);

  private:
    // make the copy constructor private
    Watchdog (const Watchdog&);
    // make the assignment operator private
    Watchdog& operator = (const Watchdog&);
  };
}

#endif
