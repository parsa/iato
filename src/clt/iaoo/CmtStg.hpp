// ---------------------------------------------------------------------------
// - CmtStg.hpp                                                              -
// - iato:iaoo client - commit stage class definition                        -
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

#ifndef  IATO_CMTSTG_HPP
#define  IATO_CMTSTG_HPP

#ifndef  IATO_STX_HPP
#include "Stx.hpp"
#endif

#ifndef  IATO_RSE_HPP
#include "Rse.hpp"
#endif

#ifndef  IATO_ROB_HPP
#include "Rob.hpp"
#endif

#ifndef  IATO_IRB_HPP
#include "Irb.hpp"
#endif

#ifndef  IATO_IRT_HPP
#include "Irt.hpp"
#endif

#ifndef  IATO_URF_HPP
#include "Urf.hpp"
#endif

#ifndef  IATO_IIB_HPP
#include "Iib.hpp"
#endif

#ifndef  IATO_GCS_HPP
#include "Gcs.hpp"
#endif

#ifndef  IATO_MLI_HPP
#include "Mli.hpp"
#endif

#ifndef  IATO_STAGE_HPP
#include "Stage.hpp"
#endif

#ifndef  IATO_BRANCH_HPP
#include "Branch.hpp"
#endif

#ifndef  IATO_DETECT_HPP
#include "Detect.hpp"
#endif

#ifndef  IATO_RESTART_HPP
#include "Restart.hpp"
#endif

#ifndef  IATO_WATCHDOG_HPP
#include "Watchdog.hpp"
#endif

#ifndef  IATO_REGISTER_HPP
#include "Register.hpp"
#endif

#ifndef  IATO_PREDICATE_HPP
#include "Predicate.hpp"
#endif


namespace iato {
  using namespace std;

  /// The CmtStg stage is the commit stage that writes registers in
  /// the register bank. In parallel, the instruction are retired from the
  /// rob and the reservation station. Interrupts are also processed here.
  /// The stage grabs the instructions and the associated results from 
  /// the instruction result buffer.
  
  class CmtStg : public Stage {
  private:
    /// the window size
    long d_iwsz;
    /// the slot window size
    long d_swsz;
    /// the rse
    Rse* p_rse;
    /// the rob
    Rob* p_rob;
    /// the irb
    Irb* p_irb;
    /// the irt
    Irt* p_irt;
    /// the urf
    Urf* p_urf;
    /// the iib
    Iib* p_iib;
    /// the gcs
    Gcs* p_gcs;
    /// the ordering buffer
    Mob* p_mob;
    /// the memory logic
    Mli* p_mli;
    /// the detection logic
    Detect*    p_dtl;
    /// the restart engine
    Restart*   p_pfr;
    /// the register bank
    Register*  p_rbk;
    /// the watchdog system
    Watchdog*  p_wdg;
    /// the branch predictor
    Branch*    p_bpr;
    /// the predicate predictor
    Predicate* p_ppr;

  public:
    /// create a new commit stage by contex
    /// @param stx the simulation context
    CmtStg (Stx* stx);

    /// create a new commit stage by context and name
    /// @param stx the simulation context
    /// @param name the stage name
    CmtStg (Stx* stx, const string& name);

    /// destroy this commit stage
    ~CmtStg (void);

    /// reset this stage
    void reset (void);

    /// activate this commit stage
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
    CmtStg (const CmtStg&);
    // make the assignement operator private
    CmtStg& operator = (const CmtStg&);
    // update the architectural ip
    void updip (const t_octa, const long);
    void updip (const Dsi&);
    void updip (const Dsi&, const Result&, const bool);
  };
}

#endif
