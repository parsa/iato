// ---------------------------------------------------------------------------
// - DetStg.hpp                                                              -
// - iato:iaio client - detect stage class definition                        -
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

#ifndef  IATO_DETSTG_HPP
#define  IATO_DETSTG_HPP

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

#ifndef  IATO_DETECT_HPP
#include "Detect.hpp"
#endif

#ifndef  IATO_RESSTG_HPP
#include "ResStg.hpp"
#endif

#ifndef  IATO_RESTEER_HPP
#include "Resteer.hpp"
#endif

namespace iato {
  using namespace std;

  /// The DetStg is the detect stage. This is the stage, where all 
  /// exeception are detected prior the write-back stage. The detect stage
  /// is primarily used within the M and I unit. The B unit is supported
  /// in a specific branch execute and detect stage. The F unit is not
  /// supported here.
  
  class DetStg : public ResStg {
  private:
    /// the early bypass network
    Bpn*     p_ebn;
    /// the late bypass network
    Bpn*     p_lbn;
    /// the bypass element
    Bpe*     p_bpe;
    /// the late memory bypass network
    Mbn*     p_lmb;
    /// the late memory bypass element
    Mbe*     p_mbe;
    /// the interrupt buffer
    Iib*     p_iib;
    /// the detection logic
    Detect*  p_det;
    /// the restart engine
    Resteer* p_pfr;

  public:
    /// create a new detect stage by context and type
    /// @param stx the simulation context
    /// @param type the stage type
    DetStg (Stx* stx, t_unit type);

    /// create a new detect stage by context, type and index
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    DetStg (Stx* stx, t_unit type, const long sidx);

    /// create a new detect stage by context, type and name
    /// @param stx the simulation context
    /// @param type the stage type
    /// @param sidx the stage index
    /// @param name the stage name
    DetStg (Stx* stx, t_unit type, const long sidx, const string& name);

    /// destroy this detect stage
    ~DetStg (void);

    /// reset this detect stage
    void reset (void);

    /// flush this detect stage
    void flush (void);

    /// clean this stage data
    void clean (void);

    /// activate this detect stage
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
    DetStg (const DetStg&);
    // make the assignement operator private
    DetStg& operator = (const DetStg&);
  };
}

#endif
