// ---------------------------------------------------------------------------
// - RegStg.hpp                                                              -
// - iato:iaoo client - operand acquisition stage class definition           -
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

#ifndef  IATO_URF_HPP
#include "Urf.hpp"
#endif

#ifndef  IATO_BPN_HPP
#include "Bpn.hpp"
#endif

#ifndef  IATO_DLYSTG_HPP
#include "DlyStg.hpp"
#endif

#ifndef  IATO_REGISTER_HPP
#include "Register.hpp"
#endif

#ifndef  IATO_PREDICATE_HPP
#include "Predicate.hpp"
#endif

namespace iato {
  using namespace std;

  /// The RegStg is the operand qcquisition stage. For each instruction, 
  /// the associated operands are evaluated. This stage operates with the
  /// urf, the register bank and the bypass network.
  
  class RegStg : public DlyStg {
  private:
    /// the universal file
    Urf* p_urf;
    /// the bypass network
    Bpn* p_bpn;
    /// the register bank
    Register* p_rbk;
    // the predicate predictor
    Predicate* p_pps;

  public:
    /// create a new evaluation stage by context and unit
    /// @param stx the simulation context
    /// @param unit the evaluation unit
    RegStg (Stx* stx, t_unit unit);

    /// create a new evaluation by context, unit and name
    /// @param stx the simulation context
    /// @param unit the evaluation unit
    /// @param name the stage name
    RegStg (Stx* stx, t_unit unit, const string& name);

    /// activate this evaluation stage
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
