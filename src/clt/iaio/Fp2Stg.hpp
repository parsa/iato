// ---------------------------------------------------------------------------
// - Fp2Stg.hpp                                                              -
// - iato:iaio client - floating-point stage 2 class definition              -
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

#ifndef  IATO_FP2STG_HPP
#define  IATO_FP2STG_HPP

#ifndef  IATO_STX_HPP
#include "Stx.hpp"
#endif

#ifndef  IATO_IIB_HPP
#include "Iib.hpp"
#endif

#ifndef  IATO_DLYSTG_HPP
#include "DlyStg.hpp"
#endif

#ifndef  IATO_RESTEER_HPP
#include "Resteer.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Fp2Stg is the floating-point stage 2. This is the stage, where
  /// the floating-point exeception are detected prior the execution stages.
  /// This stage permits to instruct the restart engine in case an
  /// exception was detected.
  
  class Fp2Stg : public DlyStg {
  private:
    /// the interrupt buffer
    Iib*     p_iib;
    /// the restart engine
    Resteer* p_pfr;

  public:
    /// create a new floating-point stage by context and type
    /// @param stx the simulation context
    /// @param type the stage type
    Fp2Stg (Stx* stx, t_unit type);

    /// create a new floating-point stage by context, type and index
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    Fp2Stg (Stx* stx, t_unit type, const long sidx);

    /// create a new floating-point stage by context, type and name
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    /// @param name the stage name
    Fp2Stg (Stx* stx, t_unit type, const long sidx, const string& name);

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
    Fp2Stg (const Fp2Stg&);
    // make the assignement operator private
    Fp2Stg& operator = (const Fp2Stg&);
  };
}

#endif
