// ---------------------------------------------------------------------------
// - LrnStg.hpp                                                              -
// - iato:iaoo client - logical renaming stage class definition              -
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

#ifndef  IATO_LRNSTG_HPP
#define  IATO_LRNSTG_HPP

#ifndef  IATO_STX_HPP
#include "Stx.hpp"
#endif

#ifndef  IATO_DSI_HPP
#include "Dsi.hpp"
#endif

#ifndef  IATO_RSE_HPP
#include "Rse.hpp"
#endif

#ifndef  IATO_IIB_HPP
#include "Iib.hpp"
#endif

#ifndef  IATO_STAGE_HPP
#include "Stage.hpp"
#endif

namespace iato {

  /// The LrnStg class is the logical renaming stage. It operates with
  /// instruction generated by the decode stage. The number of renaming
  /// operation depends on the instruction window size. The logical renaming
  /// operation uses the Register stack engine (RSE) to rename stacked and
  /// rotating registers.

  class LrnStg : public Stage {
  private:
    /// the instruction buffer size
    long d_swsz;
    /// the instruction buffer
    Dsi* p_inst;
    /// the rse
    Rse* p_rse;
    /// the iib
    Iib* p_iib;

  public:
    /// create a new logical rename stage by context
    /// @param stx the simulation context
    LrnStg (Stx* stx);

    /// create a new logical rename by context and name
    /// @param stx the simulation context
    /// @param name the stage name
    LrnStg (Stx* stx, const string& name);

    /// destroy this logical rename stage
    ~LrnStg (void);

    /// reset this logical rename stage
    void reset (void);

    /// flush this logical rename stage
    void flush (void);
 
   /// @return true if the stage is holding
    bool isholding (void) const;

    /// activate this logical rename stage
    void activate (void);

    /// report some stage information
    void report (void) const;

    /// clear the logical rename resources
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
    LrnStg (const LrnStg&);
    // make the assignement operator private
    LrnStg& operator = (const LrnStg&);
  };
}

#endif