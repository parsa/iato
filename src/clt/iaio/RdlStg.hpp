// ---------------------------------------------------------------------------
// - RdlStg.hpp                                                              -
// - iato:iaio client - result delay stage class definition                   -
// ---------------------------------------------------------------------------
// - (c) inria 2002-2004                                                     -
// ---------------------------------------------------------------------------
// - authors                                      Amaury Darsch    2002:2004 -
// ---------------------------------------------------------------------------
// - This program  is  free software;  you can redistribute it and/or modify -
// - it  under  the  terms  of the GNU  General Public License version 2, as -
// - published by the Free Software Foundation.                              -
// ---------------------------------------------------------------------------

#ifndef IATO_RDLSTG_HPP
#define IATO_RDLSTG_HPP

#ifndef IATO_STX_HPP
#include "Stx.hpp"
#endif

#ifndef IATO_RESSTG_HPP
#include "ResStg.hpp"
#endif

namespace iato {
  /// The RdlStg is a 1-cycle delay stage for an instruction+result pair.
  /// It exists to delay writeback-side effects (e.g., branch flush/restart)
  /// without breaking the ResStg->ResStg handoff expected by WrbStg.
  class RdlStg : public ResStg {
  public:
    /// create a new result delay stage by context, unit and index
    /// @param stx  the simulation context
    /// @param unit the unit type
    /// @param sidx the stage index
    RdlStg (Stx* stx, t_unit unit, const long sidx);

    /// reset this stage
    void reset (void);

    /// flush this stage
    void flush (void);

    /// activate this stage
    void activate (void);

  private:
    // make the copy constructor private
    RdlStg (const RdlStg&);
    // make the assignement operator private
    RdlStg& operator = (const RdlStg&);
  };
}

#endif


