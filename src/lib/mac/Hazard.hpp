// ---------------------------------------------------------------------------
// - Hazard.hpp                                                              -
// - iato:mac library -hazard detection logic class definition               -
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

#ifndef  IATO_HAZARD_HPP
#define  IATO_HAZARD_HPP

#ifndef  IATO_SPB_HPP
#include "Spb.hpp"
#endif

#ifndef  IATO_SCOREBOARD_HPP
#include "Scoreboard.hpp"
#endif

namespace iato {
  using namespace std;

  /// the Hazard class is a generic class that can detect hazardous condition
  /// based on a particular model. The detection logic operates with a slot
  /// port buffer and a scoreboard.

  class Hazard : public Resource {
  protected:
    /// the output port buffer
    Spb* p_spb;
    /// the scoreboard
    Scoreboard* p_psb;

  public:
    /// create a default dispersal engine
    Hazard (void);

    /// create a dispersal engine by name
    /// @param name the dispersal engine resource name
    Hazard (const string& name);

    /// create a new dispersal engine system by context
    /// @param mtx the architectural context
    Hazard (Mtx* mtx);

    /// create a new dispersal engine system by context and name
    /// @param mtx  the architectural context
    /// @param name the dispersal engine resource name
    Hazard (Mtx* mtx, const string& name);

    /// reset this resource
    void reset (void);

    /// report this resource
    void report (void) const;

    /// @return true if there is a hazard
    virtual bool ishazard (void) const;

    /// bind the opb and the scoreboard
    /// @param spb the slot port buffer to bind
    /// @param psb the scoreboard to bind
    virtual void bind (Spb* spb, Scoreboard* psb);

  private:
    // make the copy constructor private
    Hazard (const Hazard&);
    // make the assignment operator private
    Hazard& operator = (const Hazard&);
  };
}

#endif
