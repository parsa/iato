// ---------------------------------------------------------------------------
// - SlcStg.hpp                                                              -
// - iato:iaoo client - select stage class implementation                    -
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

#ifndef  IATO_SLCSTG_HPP
#define  IATO_SLCSTG_HPP

#ifndef  IATO_STX_HPP
#include "Stx.hpp"
#endif

#ifndef  IATO_GCS_HPP
#include "Gcs.hpp"
#endif

#ifndef  IATO_URF_HPP
#include "Urf.hpp"
#endif

#ifndef  IATO_STAGE_HPP
#include "Stage.hpp"
#endif

#ifndef  IATO_STATION_HPP
#include "Station.hpp"
#endif

#ifndef  IATO_REGISTER_HPP
#include "Register.hpp"
#endif

namespace iato {
  using namespace std;

  /// The SlcStg is the select stage. The stage grab instruction from
  /// the associated unit queue store it into the reservation station.
  /// At the same time, the next ready instruction is scheduled.
  
  class SlcStg : public Stage {
  private:
    /// the stage unit type
    t_unit    d_unit;
    /// the unit latency
    long      d_ulat;
    /// the station table
    Station*  p_tsta;
    /// the ready instruction
    Dsi       d_inst;
    /// the universal file
    Urf*      p_urf;
    /// the register bank
    Register* p_rbk;
    /// the central station
    Gcs*      p_gcs;

  public:
    /// create a new select stage by context and type
    /// @param stx the simulation context
    /// @param type the stage type
    SlcStg (Stx* stx, t_unit type);

    /// create a new select stage by context, type and name
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param name the stage name
    SlcStg (Stx* stx, t_unit type, const string& name);

    /// destroy this select stage
    ~SlcStg (void);

    /// reset this select stage
    void reset (void);

    /// flush this select stage
    void flush (void);

    /// activate this select stage
    void activate (void);

    /// report some stage information
    void report (void) const;

    /// bind this stage with an execution environment
    /// @param env the execution environment
    /// @param pstg the previous stage
    /// @param nstg the next stage
    void bind (Env* env, Stage* pstg, Stage* nstg);

    /// @return the selected instruction
    Dsi getinst (void) const;

  private:
    // make the copy constructor private
    SlcStg (const SlcStg&);
    // make the assignement operator private
    SlcStg& operator = (const SlcStg&);
  };
}

#endif
