// ---------------------------------------------------------------------------
// - DlyStg.hpp                                                              -
// - iato:iaio client - execute delay stage class definition                 -
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

#ifndef  IATO_SSI_HPP
#include "Ssi.hpp"
#endif

namespace iato {
  using namespace std;

  /// The DlyStg is the execute delay.  This stage is a fake stage that
  /// grabs a scheduled instruction from the rename stage and propagate
  /// it to the next stage.
  
  class DlyStg : public Stage {
  protected:
    /// the stage unit type
    t_unit d_unit;
    /// the selected instruction
    Ssi d_inst;
    /// the evaluated operand
    Operand d_oprd;

  public:
    /// create a new delay stage by context, type and index
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param name the stage name
    DlyStg (Stx* stx, t_unit type, const string& name);

    /// create a new delay stage by context, type, index and name
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    /// @param name the stage name
    DlyStg (Stx* stx, t_unit type, const long sidx, const string& name);

    /// reset this delay stage
    void reset (void);

    /// flush this delay stage
    void flush (void);

    /// @return true if the stage is holding
    bool isholding (void) const;

    /// activate this delay stage
    void activate (void);

    /// report some stage information
    void report (void) const;

    /// clean this stage data
    void clean (void);

    /// @return the delayed instruction
    virtual Ssi getinst (void) const;

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
