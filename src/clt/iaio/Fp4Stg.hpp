// ---------------------------------------------------------------------------
// - Fp4Stg.hpp                                                              -
// - iato:iaio client - floating-point stage 4 class definition              -
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

#ifndef  IATO_FP4STG_HPP
#define  IATO_FP4STG_HPP

#ifndef  IATO_STX_HPP
#include "Stx.hpp"
#endif

#ifndef  IATO_BPN_HPP
#include "Bpn.hpp"
#endif

#ifndef  IATO_RESSTG_HPP
#include "ResStg.hpp"
#endif

#ifndef  IATO_EXECUTABLE_HPP
#include "Executable.hpp"
#endif

#ifndef  IATO_SCOREBOARD_HPP
#include "Scoreboard.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Fp4Stg is the floating-point stage 4. The stage grabs the
  /// instruction from the delay stage and execute them. At the end, a result 
  /// is produced and placed in the bypass element. This stage is used with
  /// floating-point architecture those latency is greater than 1.
  
  class Fp4Stg : public ResStg {
  private:
    /// the late bypass network
    Bpn*   p_lbn;
    /// the late bypass element
    Bpe*   p_bpe;
    /// the execution unit
    Executable* p_exe;
    /// the scoreboard
    Scoreboard* p_psb;

  public:
    /// create a new floating-point stage by context and type
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    Fp4Stg (Stx* stx, t_unit type);

    /// create a new floating-point stage by context, type and index
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    Fp4Stg (Stx* stx, t_unit type, const long sidx);

    /// create a new floating-point stage by context, type and name
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    /// @param name the stage name
    Fp4Stg (Stx* stx, t_unit type, const long sidx, const string& name);

    /// destroy this floating-point stage
    ~Fp4Stg (void);

    /// reset this floating-point stage
    void reset (void);

    /// flush this floating-point stage
    void flush (void);

    /// activate this executable stage
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
    Fp4Stg (const Fp4Stg&);
    // make the assignement operator private
    Fp4Stg& operator = (const Fp4Stg&);
  };
}

#endif
