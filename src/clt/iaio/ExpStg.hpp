// ---------------------------------------------------------------------------
// - ExpStg.hpp                                                              -
// - iato:iaio client - expand (dispersal) stage class definition            -
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

#ifndef  IATO_EXPSTG_HPP
#define  IATO_EXPSTG_HPP

#ifndef  IATO_STX_HPP
#include "Stx.hpp"
#endif

#ifndef  IATO_BDB_HPP
#include "Bdb.hpp"
#endif

#ifndef  IATO_IIB_HPP
#include "Iib.hpp"
#endif

#ifndef  IATO_STAGE_HPP
#include "Stage.hpp"
#endif

#ifndef  IATO_DISPERSE_HPP
#include "Disperse.hpp"
#endif

namespace iato {
  using namespace std;

  /// The ExpStg stage is the expand stage class. Buring a cycle, the bundles
  /// are taken from the decoupling buffer and the instruction are disperse
  /// to their respective slot. If an instruction cannot be issued, the
  /// dispersion is stop and only the assigned instructions constitues an
  /// issue instruction group

  class ExpStg : public Stage {
  private:
    /// the bundle window size
    long d_iwsz;
    /// the decoupling buffer
    Bdb* p_bdb;
    /// the interuupt buffer
    Iib* p_iib;
    /// the input port buffer
    Spb* p_ipb;
    /// the scoreboard
    Scoreboard* p_psb;
    /// the dispersal engine
    Disperse* p_dis;

  public:
    /// create a new expand stage by context
    /// @param stx the simulation context
    ExpStg (Stx* stx);

    /// create a new expand stage by context and name
    /// @param stx the simulation context
    /// @param name the stage name
    ExpStg (Stx* stx, const string& name);

    /// reset this expand stage
    void reset (void);

    /// flush this expand stage
    void flush (void);
 
    /// @return true if the stage is holding
    bool isholding (void) const;

    /// activate this expand stage
    void activate (void);

    /// report some stage information
    void report (void) const;

    /// clear the expand resources
    void clear (void);

    /// bind this stage with an execution environment
    /// @param env the execution environment
    /// @param pstg the previous stage
    /// @param nstg the next stage
    void bind (Env* env, Stage* pstg, Stage* nstg);

  private:
    // make the copy constructor private
    ExpStg (const ExpStg&);
    // make the assignement operator private
    ExpStg& operator = (const ExpStg&);
  };
}

#endif
