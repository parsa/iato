// ---------------------------------------------------------------------------
// - ExeStg.hpp                                                              -
// - iato:iaio client - execute stage class definition                       -
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

#ifndef  IATO_EXESTG_HPP
#define  IATO_EXESTG_HPP

#ifndef  IATO_STX_HPP
#include "Stx.hpp"
#endif

#ifndef  IATO_BPN_HPP
#include "Bpn.hpp"
#endif

#ifndef  IATO_MBN_HPP
#include "Mbn.hpp"
#endif

#ifndef  IATO_IIB_HPP
#include "Iib.hpp"
#endif

#ifndef  IATO_MSI_HPP
#include "Msi.hpp"
#endif

#ifndef  IATO_RESSTG_HPP
#include "ResStg.hpp"
#endif

#ifndef  IATO_EXECUTABLE_HPP
#include "Executable.hpp"
#endif

#ifndef  IATO_SCOREBOARD_HPP
#include "Scoreboard.hpp"
#endif

namespace iato {
  using namespace std;

  /// The ExeStg is the execute stage. The stage grab instruction from
  /// the register stage and execute them. At the end, a result is
  /// produced and placed in the bypass element. Note that this stage
  /// does not support the B unit.
  
  class ExeStg : public ResStg {
  private:
    /// the early bypass network
    Bpn*   p_ebn;
    /// the late bypass network
    Bpn*   p_lbn;
    /// the early bypass element
    Bpe*   p_bpe;
    /// the early memory bypass network
    Mbn*   p_emb;
    /// the late memory bypass network
    Mbn*   p_lmb;
    /// the early memory bypass element
    Mbe*   p_mbe;
    /// the interrupt buffer
    Iib*   p_iib;
    /// the memory logic
    Msi*   p_msi;
    /// the execution unit
    Executable* p_exe;
    /// the scoreboard
    Scoreboard* p_psb;

  public:
    /// create a new executable stage by context and type
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    ExeStg (Stx* stx, t_unit type);

    /// create a new executable stage by context, type and index
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    ExeStg (Stx* stx, t_unit type, const long sidx);

    /// create a new executable stage by context, type and name
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    /// @param name the stage name
    ExeStg (Stx* stx, t_unit type, const long sidx, const string& name);

    /// destroy this executable stage
    ~ExeStg (void);

    /// reset this execute stage
    void reset (void);

    /// flush this execute stage
    void flush (void);

    /// clean this stage data
    void clean (void);

    /// activate this executable stage
    void activate (void);

    /// report some stage information
    void report (void) const;
 
    /// bind this stage with an execution environment
    /// @param env the execution environment
    /// @param pstg the previous stage
    /// @param nstg the next stage
    void bind (Env* env, Stage* pstg, Stage* nstg);

    /// @return the bypass stage mrt
    Mrt getbmrt (void) const;

    /// @return the bypass element index
    long getbeix (void) const;

  private:
    // make the copy constructor private
    ExeStg (const ExeStg&);
    // make the assignement operator private
    ExeStg& operator = (const ExeStg&);
  };
}

#endif
