// ---------------------------------------------------------------------------
// - WrbStg.hpp                                                              -
// - iato:iaio client - write-back stage class definition                    -
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

#ifndef  IATO_WRBSTG_HPP
#define  IATO_WRBSTG_HPP

#ifndef  IATO_STX_HPP
#include "Stx.hpp"
#endif

#ifndef  IATO_RSE_HPP
#include "Rse.hpp"
#endif

#ifndef  IATO_IIB_HPP
#include "Iib.hpp"
#endif

#ifndef  IATO_MSI_HPP
#include "Msi.hpp"
#endif

#ifndef  IATO_BRANCH_HPP
#include "Branch.hpp"
#endif

#ifndef  IATO_RESSTG_HPP
#include "ResStg.hpp"
#endif

#ifndef  IATO_RESTEER_HPP
#include "Resteer.hpp"
#endif

#ifndef  IATO_REGISTER_HPP
#include "Register.hpp"
#endif

#ifndef  IATO_SCOREBOARD_HPP
#include "Scoreboard.hpp"
#endif

namespace iato {
  using namespace std;

  /// The WrbStg is the write-back stage. This is the final stage, during the
  /// instruction results are written into the architectural register. If the
  /// instruction has been cancelled, the results are not written. Interrupted
  /// instructions are routed also during this stage.
  
  class WrbStg : public ResStg {
  private:
    /// the restart engine flag
    bool   d_ref;
    /// the rse
    Rse*   p_rse;
    /// the interrupt buffer
    Iib*   p_iib;
    /// the memory logic
    Msi*   p_msi;
    /// the branch predictor
    Branch*   p_bpr;
    /// the restart engine
    Resteer*  p_pfr;
    /// the register bank
    Register* p_rbk;
    /// the scoreboard
    Scoreboard* p_psb;

  public:
    /// create a new write-back stage by context and type
    /// @param stx the simulation context
    /// @param type the stage type
    WrbStg (Stx* stx, t_unit type);

    /// create a new write-back stage by context, type and index
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    WrbStg (Stx* stx, t_unit type, const long sidx);

    /// create a new write-back stage by context, type, index and name
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    /// @param name the stage name
    WrbStg (Stx* stx, t_unit type, const long sidx, const string& name);

    /// destroy this stage
    ~WrbStg (void);

    /// reset this write-back stage
    void reset (void);

    /// flush this write-back stage
    void flush (void);

    /// activate this write-back stage
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
    WrbStg (const WrbStg&);
    // make the assignement operator private
    WrbStg& operator = (const WrbStg&);
  };
}

#endif
