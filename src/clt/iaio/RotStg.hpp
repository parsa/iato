// ---------------------------------------------------------------------------
// - RotStg.hpp                                                              -
// - iato:iaio client - bundle rotation stage class definition               -
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

#ifndef  IATO_ROTSTG_HPP
#define  IATO_ROTSTG_HPP

#ifndef  IATO_STX_HPP
#include "Stx.hpp"
#endif

#ifndef  IATO_BDB_HPP
#include "Bdb.hpp"
#endif

#ifndef  IATO_STAGE_HPP
#include "Stage.hpp"
#endif

namespace iato {

  /// The BdbStg stage is the bundle rotation stage. Unless the decoupling
  /// fgull, the bundles are placed in the decoupling buffer and are ready
  /// for dispersal.

  class RotStg : public Stage {
  private:
    /// the bundle window size
    long d_iwsz;
    /// the decoupling buffer
    Bdb* p_bdb;

  public:
    /// create a new rotate stage by context
    /// @param stx the simulation context
    RotStg (Stx* stx);

    /// create a new rotate stage by context and name
    /// @param stx the simulation context
    /// @param name the stage name
    RotStg (Stx* stx, const string& name);

    /// reset this rotate stage
    void reset (void);

    /// flush this rotate stage
    void flush (void);
 
   /// @return true if the stage is holding
    bool isholding (void) const;

    /// activate this rotate stage
    void activate (void);

    /// report some stage information
    void report (void) const;

    /// clear the rotate resources
    void clear (void);

    /// bind this stage with an execution environment
    /// @param env the execution environment
    /// @param pstg the previous stage
    /// @param nstg the next stage
    void bind (Env* env, Stage* pstg, Stage* nstg);

  private:
    // make the copy constructor private
    RotStg (const RotStg&);
    // make the assignement operator private
    RotStg& operator = (const RotStg&);
  };
}

#endif
