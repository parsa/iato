// ---------------------------------------------------------------------------
// - IpgStg.hpp                                                              -
// - iato:iaoo client - instruction pointer generation stage definition      -
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

#ifndef  IATO_IPGSTG_HPP
#define  IATO_IPGSTG_HPP

#ifndef  IATO_STX_HPP
#include "Stx.hpp"
#endif

#ifndef  IATO_MPR_HPP
#include "Mpr.hpp"
#endif

#ifndef  IATO_STAGE_HPP
#include "Stage.hpp"
#endif

#ifndef  IATO_BRANCH_HPP
#include "Branch.hpp"
#endif

#ifndef  IATO_RESTART_HPP
#include "Restart.hpp"
#endif

#ifndef  IATO_REGISTER_HPP
#include "Register.hpp"
#endif

#ifndef  IATO_INTERRUPT_HPP
#include "Interrupt.hpp"
#endif

namespace iato {

  /// The Ipg stage is  the instruction pointer generation stage that can be 
  /// associated with or without a branch prediction system. The stage is
  /// designed to interface with a port buffer. If a branch  prediction 
  /// system exists, the stage takes care of recomputing the
  /// the instruction pointer from its predicted value. The IPG stage
  /// also implements a bundle arrys those size is obtained from the
  /// context window size parameter. Note that this stage uses a speculative
  /// ip. Such ip is initialized by the request logic from the architectural
  /// ip only if the stage has been reset or flushed.
  
  class IpgStg : public Stage {
  private:
    /// the bundle window size
    long      d_iwsz;
    /// the byte window size
    long      d_bwsz;
    /// the speculative ip
    t_octa    d_sip;
    /// the valid sip flag
    bool      d_sflg;
    /// the bundle array
    Bundle*   p_bndl;
    /// the request port
    Mpr*      p_preq;
    /// the branch prediction
    Branch*   p_bprd;
    /// the restart engine
    Restart*  p_reng;
    /// the register bank
    Register* p_bank;
    /// the ipg interrupt
    Interrupt d_intr;

  public:
    /// create a new ipg stage by context
    /// @param stx the simulation context
    IpgStg (Stx* stx);

    /// create a new ipg stage by context and name
    /// @param stx the simulation context
    /// @param name the stage name
    IpgStg (Stx* stx, const string& name);

    /// destroy this ipg stage
    ~IpgStg (void);

    /// reset this ipg stage
    void reset (void);

    /// flush this ipg stage
    void flush (void);

    /// @return true if the stage is holding
    bool isholding (void) const;

    /// activate this ipg stage
    void activate (void);

    /// report some stage information
    void report (void) const;

    /// clear the ipg resources
    void clear (void);

    /// bind this stage with an execution environment
    /// @param env the execution environment
    /// @param pstg the previous stage
    /// @param nstg the next stage
    void bind (Env* env, Stage* pstg, Stage* nstg);

    /// @return a bundle by index
    Bundle getbndl (const long index);

    /// @return the ipg interrupt
    Interrupt getintr (void);

  private:
    // make the copy constructor private
    IpgStg (const IpgStg&);
    // make the assignement operator private
    IpgStg& operator = (const IpgStg&);
  };
}

#endif
