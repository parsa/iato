// ---------------------------------------------------------------------------
// - RenStg.hpp                                                              -
// - iato:iaio client - renaming stage class definition                      -
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

#ifndef  IATO_RENSTG_HPP
#define  IATO_RENSTG_HPP

#ifndef  IATO_STX_HPP
#include "Stx.hpp"
#endif

#ifndef  IATO_SPB_HPP
#include "Spb.hpp"
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

  /// The RenStg class is the instruction decode and renaming stage. The
  /// instruction are grabbed from the issue port and renamed by the RSE.
  /// Note that the renaming order is important in this implementation and
  /// assume the the first renaming operation is done with the first M issue
  /// port (for the alloc to operate correctly). Bundles with a B unit
  /// have an implicit stop bit. Branches, if taken, sets the RSE state
  /// speculatively.

  class RenStg : public Stage {
  private:
    /// the stage type
    t_unit d_unit;
    /// the issue slot
    Slot*  p_isl;
    /// the output slot
    Slot*  p_osl;
    /// the rse
    Rse*   p_rse;
    /// the input port buffer
    Spb*   p_ipb;
    /// the output port buffer
    Spb*   p_opb;
    /// the iib
    Iib*   p_iib;

  public:
    /// create a new renaming stage by context and unit
    /// @param stx  the simulation context
    /// @param unit the unit type
    RenStg (Stx* stx, t_unit unit);

    /// create a new renaming stage by context, type and index
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    RenStg (Stx* stx, t_unit type, const long sidx);

    /// create a new renaming stage by context, unit, index and name
    /// @param stx the simulation context
    /// @param unit the unit type
    /// @param sidx the stage index
    /// @param name the stage name
    RenStg (Stx* stx, t_unit unit, const long sidx, const string& name);

    /// destroy this logical rename stage
    ~RenStg (void);

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

    /// bind this stage with an execution environment
    /// @param env the execution environment
    /// @param pstg the previous stage
    /// @param nstg the next stage
    void bind (Env* env, Stage* pstg, Stage* nstg);

    /// @return the renamed instruction
    Ssi getinst (void) const;

    /// @return the renamed instruction and clean the stage
    Ssi grabinst (void);

  private:
    // make the copy constructor private
    RenStg (const RenStg&);
    // make the assignement operator private
    RenStg& operator = (const RenStg&);
  };
}

#endif
