// ---------------------------------------------------------------------------
// - Fp1Stg.hpp                                                              -
// - iato:iaio client - floating-point stage 1 class definition              -
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

#ifndef  IATO_FP1STG_HPP
#define  IATO_FP1STG_HPP

#ifndef  IATO_STX_HPP
#include "Stx.hpp"
#endif

#ifndef  IATO_BPN_HPP
#include "Bpn.hpp"
#endif

#ifndef  IATO_IIB_HPP
#include "Iib.hpp"
#endif

#ifndef  IATO_DLYSTG_HPP
#include "DlyStg.hpp"
#endif

#ifndef  IATO_EXECUTABLE_HPP
#include "Executable.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Fp1Stg is the floating-point exception stage. The stage is used to
  /// detect if the floating point operands are free of exceptions. If an
  /// exception is found, the instruction is marked interrupted. If the 
  /// processor models a floating-point stage with a one cycle latency, 
  /// this stage is not used.
  
  class Fp1Stg : public DlyStg {
  private:
    /// the late bypass network
    Bpn* p_lbn;
    /// the interrupt buffer
    Iib* p_iib;
    /// the execution unit
    Executable* p_exe;

  public:
    /// create a new floating-point stage by context and type
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    Fp1Stg (Stx* stx, t_unit type);

    /// create a new floating-point stage by context, type and index
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    Fp1Stg (Stx* stx, t_unit type, const long sidx);

    /// create a new floating-point stage by context, type and name
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    /// @param name the stage name
    Fp1Stg (Stx* stx, t_unit type, const long sidx, const string& name);

    /// destroy this floating-point stage
    ~Fp1Stg (void);

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
    Fp1Stg (const Fp1Stg&);
    // make the assignement operator private
    Fp1Stg& operator = (const Fp1Stg&);
  };
}

#endif
