// ---------------------------------------------------------------------------
// - RseLogic.hpp                                                            -
// - iato:mac library - rse control logic class definition                   -
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

#ifndef  IATO_RSELOGIC_HPP
#define  IATO_RSELOGIC_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_RSESTACK_HPP
#include "RseStack.hpp"
#endif

namespace iato {

  /// The RseLogic class is the rse control logic with speculative support.
  /// The logic operates in a predict/update mode. The logic is associated
  /// with a predictive engine that is used to predict the rse state during 
  /// branch operations. The rse logic also implements a rse stack buffer 
  /// that hold the rse state in association with predicted branches.

  class RseLogic : public Rse {
  private:
    // the speculative state
    State d_spste;
    // the rse stack
    RseStack* p_rstk;

  public:
    /// create a default rse logic
    RseLogic (void);

    /// create a rse logic with a context
    /// @param mtx the architectural context
    RseLogic (Mtx* mtx);

    /// reset this rse logic
    void reset (void);

    /// flush this rse logic
    void flush (void);

    /// report this resource
    void report (void) const;

    /// set the speculative rse state by state
    /// @param state the state to set
    void setsst (const State& state);

    /// @return the speculative  rse state
    State getsst (void) const;

    /// rename an instruction with the speculative state
    /// @param inst the instruction to rename
    void rename (Instr& inst) const;
    
    /// preset the rse state with an instruction
    /// @param inst the instruction used to preset
    void preset (const Instr& inst);

    /// after set the rse state with an instruction
    /// @param inst the instruction used to preset
    void aftset (const Instr& inst);

    /// update the rse state with a result
    /// @param resl the result used to update
    void update (const Result& resl);
  };
}

#endif
