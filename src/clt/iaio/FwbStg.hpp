// ---------------------------------------------------------------------------
// - FwbStg.hpp                                                              -
// - iato:iaio client - floating write-back stage class definition           -
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

#ifndef  IATO_FWBSTG_HPP
#define  IATO_FWBSTG_HPP

#ifndef  IATO_STX_HPP
#include "Stx.hpp"
#endif

#ifndef  IATO_IIB_HPP
#include "Iib.hpp"
#endif

#ifndef  IATO_IRT_HPP
#include "Irt.hpp"
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

namespace iato {
  using namespace std;

  /// The FwbStg is the floating write-back stage. This is the final stage, 
  /// during the instruction results are written into the architectural 
  /// register. If the instruction has been cancelled, the results are not 
  /// written. Interrupted instructions are routed also during this stage.
  
  class FwbStg : public ResStg {
  private:
    /// the latency flag
    bool d_lflg;
    /// the interrupt buffer
    Iib*      p_iib;
    /// the restart engine
    Resteer*  p_pfr;
    /// the register bank
    Register* p_rbk;

  public:
    /// create a new floating write-back stage by context and type
    /// @param stx the simulation context
    /// @param type the stage type
    FwbStg (Stx* stx, t_unit type);
 
    /// create a new floating write-back stage by context, type and index
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    FwbStg (Stx* stx, t_unit type, const long sidx);

    /// create a new floating write-back stage by context, type, index and name
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    /// @param name the stage name
    FwbStg (Stx* stx, t_unit type, const long sidx, const string& name);

    /// reset this write-back stage
    void reset (void);

    /// flush this write-back stage
    void flush (void);

    /// activate this floating write-back stage
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
    FwbStg (const FwbStg&);
    // make the assignement operator private
    FwbStg& operator = (const FwbStg&);
  };
}

#endif
