// ---------------------------------------------------------------------------
// - DecStg.hpp                                                              -
// - iato:iaoo client - instruction decode stage class definition            -
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

#ifndef  IATO_DECSTG_HPP
#define  IATO_DECSTG_HPP

#ifndef  IATO_STX_HPP
#include "Stx.hpp"
#endif

#ifndef  IATO_DSI_HPP
#include "Dsi.hpp"
#endif

#ifndef  IATO_ROB_HPP
#include "Rob.hpp"
#endif

#ifndef  IATO_MOB_HPP
#include "Mob.hpp"
#endif

#ifndef  IATO_IIB_HPP
#include "Iib.hpp"
#endif

#ifndef  IATO_STAGE_HPP
#include "Stage.hpp"
#endif

#ifndef  IATO_RESTART_HPP
#include "Restart.hpp"
#endif

namespace iato {

  /// The DecStg stage is the instruction decode stage. Instruction are decoded
  /// from a bundle. The number of decoded instructions depends on the
  /// instruction window size. Once decoded, the newly created instruction
  /// are placed in a buffer for use by the next stage.

  class DecStg : public Stage {
  private:
    /// the bundle window size
    long d_iwsz;
    /// the instruction buffer size
    long d_swsz;
    /// the ignore nop flag
    bool d_inop;
    /// the instruction buffer
    Dsi* p_inst;
    /// the reorder buffer
    Rob* p_rob;
    /// the memory ordering  buffer
    Mob* p_mob;
    /// the instruction interrupt buffer
    Iib* p_iib;
    /// the restart engine
    Restart* p_pfr;

  public:
    /// create a new decode stage by context
    /// @param stx the simulation context
    DecStg (Stx* stx);

    /// create a new decode stage by context and name
    /// @param stx the simulation context
    /// @param name the stage name
    DecStg (Stx* stx, const string& name);

    /// destroy this decode stage
    ~DecStg (void);

    /// reset this decode stage
    void reset (void);

    /// flush this decode stage
    void flush (void);
 
   /// @return true if the stage is holding
    bool isholding (void) const;

    /// activate this decode stage
    void activate (void);

    /// report some stage information
    void report (void) const;

    /// clear the decode resources
    void clear (void);

    /// bind this stage with an execution environment
    /// @param env the execution environment
    /// @param pstg the previous stage
    /// @param nstg the next stage
    void bind (Env* env, Stage* pstg, Stage* nstg);

    /// @return an instruction by index
    Dsi getinst (const long index) const;

  private:
    // make the copy constructor private
    DecStg (const DecStg&);
    // make the assignement operator private
    DecStg& operator = (const DecStg&);
  };
}

#endif
