// ---------------------------------------------------------------------------
// - ResStg.hpp                                                              -
// - iato:iaoo client - result stage abstract class definition               -
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

#ifndef  IATO_RESSTG_HPP
#define  IATO_RESSTG_HPP

#ifndef  IATO_STX_HPP
#include "Stx.hpp"
#endif

#ifndef  IATO_DSI_HPP
#include "Dsi.hpp"
#endif

#ifndef  IATO_STAGE_HPP
#include "Stage.hpp"
#endif

#ifndef  IATO_INTERRUPT_HPP
#include "Interrupt.hpp"
#endif

namespace iato {

  /// The ResStg class is an abstract class used for stages that produces
  /// instruction result. The natural stage for that class is the execution
  /// stage. For M unit, the memory read stage is also an exemple of
  /// such object.
  
  class ResStg : public Stage {
  protected:
    /// the stage unit type
    t_unit    d_unit;
    /// the executed instruction
    Dsi       d_inst;
    /// the instruction result
    Result    d_resl;
    /// the interrupt result
    Interrupt d_intr;

  public:
    /// create a new result stage by context, type and name
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param name the stage name
    ResStg (Stx* stx, t_unit type, const string& name);

    /// reset this result stage
    void reset (void);

    /// flush this result stage
    void flush (void);

    /// @return the executed instruction
    virtual Dsi getinst (void) const;

    /// @return the executed result
    virtual Result getresl (void) const;

    /// @return the executed interrupt
    virtual Interrupt getintr (void) const;

  private:
    // make the copy constructor private
    ResStg (const ResStg&);
    // make the assignement operator private
    ResStg& operator = (const ResStg&);
  };
}

#endif
