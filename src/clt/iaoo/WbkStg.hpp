// ---------------------------------------------------------------------------
// - WbkStg.hpp                                                              -
// - iato:iaoo client - write back stage class definition                    -
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

#ifndef  IATO_WBKSTG_HPP
#define  IATO_WBKSTG_HPP

#ifndef  IATO_STAGE_HPP
#include "Stage.hpp"
#endif

#ifndef  IATO_IRB_HPP
#include "Irb.hpp"
#endif

#ifndef  IATO_ROB_HPP
#include "Rob.hpp"
#endif

#ifndef  IATO_URF_HPP
#include "Urf.hpp"
#endif

#ifndef  IATO_GCS_HPP
#include "Gcs.hpp"
#endif

#ifndef  IATO_BPN_HPP
#include "Bpn.hpp"
#endif

#ifndef  IATO_DETECT_HPP
#include "Detect.hpp"
#endif

namespace iato {
  using namespace std;

  /// The WbkStg is the write-back stage. When the instruction has been
  /// executed, the instruction result, if available, is written into
  /// the physical register file. The rob is updated to notify that the
  /// instruction has completed execution.
  
  class WbkStg : public Stage {
  private:
    /// the stage unit type
    t_unit d_unit;
    /// the irb
    Irb* p_irb;
    /// the rob
    Rob* p_rob;
    /// the urf
    Urf* p_urf;
    /// the gcs
    Gcs* p_gcs;
    /// the bpn
    Bpn* p_bpn;
    /// the detection logic
    Detect* p_det;

  public:
    /// create a new write-back stage by context and type
    /// @param stx the simulation context
    /// @param type the stage type
    WbkStg (Stx* stx, t_unit type);

    /// create a new write-back stage by context, type and name
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param name the stage name
    WbkStg (Stx* stx, t_unit type, const string& name);

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
    WbkStg (const WbkStg&);
    // make the assignement operator private
    WbkStg& operator = (const WbkStg&);
  };
}

#endif
