// ---------------------------------------------------------------------------
// - ExeStg.hpp                                                              -
// - iato:iaoo client - execute stage class definition                       -
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

#ifndef  IATO_EXESTG_HPP
#define  IATO_EXESTG_HPP

#ifndef  IATO_MLI_HPP
#include "Mli.hpp"
#endif

#ifndef  IATO_BPN_HPP
#include "Bpn.hpp"
#endif

#ifndef  IATO_GCS_HPP
#include "Gcs.hpp"
#endif

#ifndef  IATO_MLI_HPP
#include "Mli.hpp"
#endif

#ifndef  IATO_RESSTG_HPP
#include "ResStg.hpp"
#endif

#ifndef  IATO_EXECUTABLE_HPP
#include "Executable.hpp"
#endif

namespace iato {
  using namespace std;

  /// The ExeStg is the execute stage. The stage grab instruction from
  /// the evaluation stage and execute them. At the end, a result is
  /// produced and placed in the bypass element.
  
  class ExeStg : public ResStg {
  private:
    /// the stage unit type
    t_unit   d_unit;
    /// incomplete bypass with pp
    bool     d_ibyp;
    /// the execution unit
    Executable* p_exec;
    /// the central station
    Gcs* p_gcs;
    /// the bypass network
    Bpn* p_bpn;
    /// the bypass element
    Bpe* p_bpe;
    /// the memory logic
    Mli* p_mli;

  public:
    /// create a new executable stage by context and type
    /// @param stx the simulation context
    /// @param type the stage type
    ExeStg (Stx* stx, t_unit type);

    /// create a new executable stage by context, type and name
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param name the stage name
    ExeStg (Stx* stx, t_unit type, const string& name);

    /// destroy this executable stage
    ~ExeStg (void);

    /// reset this execute stage
    void reset (void);

    /// flush this execute stage
    void flush (void);

    /// activate this executable stage
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
    ExeStg (const ExeStg&);
    // make the assignement operator private
    ExeStg& operator = (const ExeStg&);
  };
}

#endif
