// ---------------------------------------------------------------------------
// - Fp3Stg.hpp                                                              -
// - iato:iaio client - floating-point stage 3 class definition              -
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

#ifndef  IATO_FP3STG_HPP
#define  IATO_FP3STG_HPP

#ifndef  IATO_STX_HPP
#include "Stx.hpp"
#endif

#ifndef  IATO_DLYSTG_HPP
#include "DlyStg.hpp"
#endif

#ifndef  IATO_RESTEER_HPP
#include "Resteer.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Fp3Stg is the floating-point stage 3. This is the stage, where
  /// the instruction are nullified eventually by the restart engine. After
  /// this point the instructions are ready to complete.
  
  class Fp3Stg : public DlyStg {
  private:
    /// the restart engine
    Resteer* p_pfr;

  public:
    /// create a new floating-point stage by context and type
    /// @param stx the simulation context
    /// @param type the stage type
    Fp3Stg (Stx* stx, t_unit type);

    /// create a new floating-point stage by context, type and index
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    Fp3Stg (Stx* stx, t_unit type, const long sidx);

    /// create a new floating-point stage by context, type and name
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    /// @param name the stage name
    Fp3Stg (Stx* stx, t_unit type, const long sidx, const string& name);

    /// flush this stage
    void flush (void);

    /// activate this floating-point stage
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
    Fp3Stg (const Fp3Stg&);
    // make the assignement operator private
    Fp3Stg& operator = (const Fp3Stg&);
  };
}

#endif
