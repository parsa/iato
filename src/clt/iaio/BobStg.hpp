// ---------------------------------------------------------------------------
// - BobStg.hpp                                                              -
// - iato:iaio client - bypass operand branch class definition               -
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

#ifndef  IATO_BOBSTG_HPP
#define  IATO_BOBSTG_HPP

#ifndef  IATO_STX_HPP
#include "Stx.hpp"
#endif

#ifndef  IATO_BPN_HPP
#include "Bpn.hpp"
#endif

#ifndef  IATO_IIB_HPP
#include "Iib.hpp"
#endif

#ifndef  IATO_DLYSTG_HPP
#include "DlyStg.hpp"
#endif

namespace iato {
  using namespace std;

  /// The BobStg is the bypass operabd branch stage. It simply get some 
  /// operand data from the bypass network.
  
  class BobStg : public DlyStg {
  private:
    /// the late bypass network
    Bpn* p_lbn;
    /// the interrupt buffer
    Iib* p_iib;

  public:
    /// create a new bypass stage by context and type
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    BobStg (Stx* stx, t_unit type);

    /// create a new bypass stage by context, type and index
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    BobStg (Stx* stx, t_unit type, const long sidx);

    /// create a new bypass stage by context, type and name
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    /// @param name the stage name
    BobStg (Stx* stx, t_unit type, const long sidx, const string& name);

    /// reset this bypass stage
    void reset (void);

    /// flush this bypass stage
    void flush (void);

    /// activate this bypass stage
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
    BobStg (const BobStg&);
    // make the assignement operator private
    BobStg& operator = (const BobStg&);
  };
}

#endif
