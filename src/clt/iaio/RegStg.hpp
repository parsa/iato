// ---------------------------------------------------------------------------
// - RegStg.hpp                                                              -
// - iato:iaio client - register read stage class definition                 -
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

#ifndef  IATO_REGSTG_HPP
#define  IATO_REGSTG_HPP

#ifndef  IATO_STX_HPP
#include "Stx.hpp"
#endif

#ifndef  IATO_IIB_HPP
#include "Iib.hpp"
#endif

#ifndef  IATO_BPN_HPP
#include "Bpn.hpp"
#endif

#ifndef  IATO_HAZARD_HPP
#include "Hazard.hpp"
#endif

#ifndef  IATO_DLYSTG_HPP
#include "DlyStg.hpp"
#endif

#ifndef  IATO_REGISTER_HPP
#include "Register.hpp"
#endif

#ifndef  IATO_SCOREBOARD_HPP
#include "Scoreboard.hpp"
#endif

namespace iato {

  /// The RegStg class is the register read class. The class attempts to read
  /// the register value from the register bank or the bypass network. If one
  /// operand is hazardous, the instruction is stalled. This condition
  /// is obtained by checking the register scoreboard for any hazards.

  class RegStg : public DlyStg {
  private:
    /// the iib
    Iib* p_iib;
    /// the early bypass network
    Bpn* p_ebn;
    /// the late bypass network
    Bpn* p_lbn;
    /// the hazard detection logic
    Hazard* p_hdl;
    /// the register bank
    Register*   p_rbk;
    /// the scoreboard
    Scoreboard* p_psb;

  public:
    /// create a new register read stage by context and unit
    /// @param stx  the simulation context
    /// @param unit the unit type
    RegStg (Stx* stx, t_unit unit);

    /// create a new register read stage by context, type and index
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    RegStg (Stx* stx, t_unit type, const long sidx);

    /// create a new register read stage by context, unit, index and name
    /// @param stx the simulation context
    /// @param unit the unit type
    /// @param sidx the stage index
    /// @param name the stage name
    RegStg (Stx* stx, t_unit unit, const long sidx, const string& name);

    /// reset this register read stage
    void reset (void);

    /// flush this register read stage
    void flush (void);

    /// @return true if the stage is holding
    bool isholding (void) const;

    /// activate this register read stage
    void activate (void);

    /// report some stage information
    void report (void) const;

    /// bind this stage with an execution environment
    /// @param env the execution environment
    /// @param pstg the previous stage
    /// @param nstg the next stage
    void bind (Env* env, Stage* pstg, Stage* nstg);

  private:
    // make the copy constructor private
    RegStg (const RegStg&);
    // make the assignement operator private
    RegStg& operator = (const RegStg&);
  };
}

#endif
