// ---------------------------------------------------------------------------
// - ResStg.hpp                                                              -
// - iato:iaio client - result stage class definition                        -
// ---------------------------------------------------------------------------
// - (c) inria 2002-2004                                                     -
// ---------------------------------------------------------------------------
// - authors                                      Amaury Darsch    2002:2004 -
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

#ifndef  IATO_RESSTG_HPP
#define  IATO_RESSTG_HPP

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

  /// The ResStg is the result base stage class.  This stage is a base stage
  /// that stores an instruction along with a result. It is use by the execute
  /// and detect stage as an intermediate stage.
  
  class ResStg : public Stage {
  protected:
    /// the stage unit type
    t_unit d_unit;
    /// the selected instruction
    Ssi    d_inst;
    /// the instruction result
    Result d_resl;

  public:

    /// create a new result stage by context, type and name
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param name the stage name
    ResStg (Stx* stx, t_unit type, const string& name);

    /// create a new result stage by context, type, index and name
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    /// @param name the stage name
    ResStg (Stx* stx, t_unit type, const long sidx, const string& name);

    /// reset this result stage
    void reset (void);

    /// flush this result stage
    void flush (void);

    /// clean the stage data
    virtual void clean (void);

    /// @return the resulted instruction
    virtual Ssi getinst (void) const;

    /// @return the instruction result
    virtual Result getresl (void) const;


  private:
    // make the copy constructor private
    ResStg (const ResStg&);
    // make the assignement operator private
    ResStg& operator = (const ResStg&);
  };
}

#endif
