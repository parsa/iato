// ---------------------------------------------------------------------------
// - DlyStg.hpp                                                              -
// - iato:iaoo client - execute delay stage class definition                 -
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

#ifndef  IATO_DLYSTG_HPP
#define  IATO_DLYSTG_HPP

#ifndef  IATO_STX_HPP
#include "Stx.hpp"
#endif

#ifndef  IATO_STAGE_HPP
#include "Stage.hpp"
#endif

#ifndef  IATO_DSI_HPP
#include "Dsi.hpp"
#endif

#ifndef  IATO_GCS_HPP
#include "Gcs.hpp"
#endif

namespace iato {
  using namespace std;

  /// The DlyStg is the execute delay.  This stage is a fake stage that
  /// grabs a scheduled instruction from the select stage and decrease
  /// the execution latency counter. When it reaches 0, the instruction
  /// will complete in the next cycle and the operands will be ready.
  
  class DlyStg : public Stage {
  protected:
    /// the stage unit type
    t_unit d_unit;
    /// the selected instruction
    Dsi d_inst;
    /// the evaluated operand
    Operand d_oprd;
    /// the central station
    Gcs* p_gcs;

  public:
    /// create a new delay stage by context and type
    /// @param stx the simulation context
    /// @param type the stage type
    DlyStg (Stx* stx, t_unit type);

    /// create a new delay stage by context, type and name
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param name the stage name
    DlyStg (Stx* stx, t_unit type, const string& name);

    /// reset this delay stage
    void reset (void);

    /// flush this delay stage
    void flush (void);

    /// partially flush this delay stage
    void pflsh (void);

    /// activate this delay stage
    void activate (void);

    /// report some stage information
    void report (void) const;
 
    /// bind this stage with an execution environment
    /// @param env the execution environment
    /// @param pstg the previous stage
    /// @param nstg the next stage
    void bind (Env* env, Stage* pstg, Stage* nstg);

    /// @return the selected instruction
    virtual Dsi getinst (void) const;

    /// @return the evaluated operand
    virtual Operand getoprd (void) const;

  private:
    // make the copy constructor private
    DlyStg (const DlyStg&);
    // make the assignement operator private
    DlyStg& operator = (const DlyStg&);
  };
}

#endif
