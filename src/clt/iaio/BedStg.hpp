// ---------------------------------------------------------------------------
// - BedStg.hpp                                                              -
// - iato:iaio client - branch execute and detect stage class definition     -
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

#ifndef  IATO_BEDSTG_HPP
#define  IATO_BEDSTG_HPP

#ifndef  IATO_STX_HPP
#include "Stx.hpp"
#endif

#ifndef  IATO_BPN_HPP
#include "Bpn.hpp"
#endif

#ifndef  IATO_IIB_HPP
#include "Iib.hpp"
#endif

#ifndef  IATO_BRANCH_HPP
#include "Branch.hpp"
#endif

#ifndef  IATO_DETECT_HPP
#include "Detect.hpp"
#endif

#ifndef  IATO_RESSTG_HPP
#include "ResStg.hpp"
#endif

#ifndef  IATO_RESTEER_HPP
#include "Resteer.hpp"
#endif

#ifndef  IATO_EXECUTABLE_HPP
#include "Executable.hpp"
#endif

#ifndef  IATO_SCOREBOARD_HPP
#include "Scoreboard.hpp"
#endif

namespace iato {
  using namespace std;

  /// The BedStg is the branch execute and detect stage. This stage is shifted
  /// by one with the normal execute stage as it interacts directly with the
  /// the control flow but also used bypass data from other execute stage.
  /// The most noticeable exemple is the br.wtop branch that uses the predicate
  /// not for cancelling the instruction but rather change the computation
  /// behavior (this is actually the proof that this ISA is not very good).

  class BedStg : public ResStg {
  private:
    /// the early clean flag
    bool   d_ecf;
    /// the early bypass network
    Bpn*   p_ebn;
    /// the late bypass network
    Bpn*   p_lbn;
    /// the late bypass element
    Bpe*   p_bpe;
    /// the interrupt buffer
    Iib*   p_iib;
    /// the detection logic
    Detect*  p_det;
    /// the branch predictor
    Branch*  p_bpr;
    /// the restart engine
    Resteer* p_pfr;
    /// the execution unit
    Executable* p_exe;
    /// the scoreboard
    Scoreboard* p_psb;

  public:
    /// create a new branch execute and detect stage by context and type
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    BedStg (Stx* stx, t_unit type);

    /// create a new branch execute and detect stage by context, type and index
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    BedStg (Stx* stx, t_unit type, const long sidx);

    /// create a new branch execute and detect stage by context, type and name
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    /// @param name the stage name
    BedStg (Stx* stx, t_unit type, const long sidx, const string& name);

    /// destroy this branch execute and detect stage
    ~BedStg (void);

    /// reset this branch execute and detect stage
    void reset (void);

    /// flush this branch execute and detect stage
    void flush (void);

    /// clean this branch execute and detect data
    void clean (void);

    /// activate this branch execute and detect stage
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
    BedStg (const BedStg&);
    // make the assignement operator private
    BedStg& operator = (const BedStg&);
  };
}

#endif
