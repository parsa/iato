// ---------------------------------------------------------------------------
// - PrnStg.hpp                                                              -
// - iato:iaoo client - physical renaming stage class definition             -
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

#ifndef  IATO_PRNSTG_HPP
#define  IATO_PRNSTG_HPP

#ifndef  IATO_STX_HPP
#include "Stx.hpp"
#endif

#ifndef  IATO_DSI_HPP
#include "Dsi.hpp"
#endif

#ifndef  IATO_URF_HPP
#include "Urf.hpp"
#endif

#ifndef  IATO_IIB_HPP
#include "Iib.hpp"
#endif

#ifndef  IATO_EIQ_HPP
#include "Eiq.hpp"
#endif

#ifndef  IATO_STAGE_HPP
#include "Stage.hpp"
#endif

namespace iato {

  /// The PrnStg class is the physical renaming stage. It operates with
  /// logically renamed instruction and uses the translation bank combined
  /// with a rat to achieve such renamikng. After the renaming is done
  /// the rid of each instruction is set with their respective virtual bits.

  class PrnStg : public Stage {
  private:
    /// the instruction buffer size
    long d_swsz;
    /// the urf
    Urf* p_urf;
    /// the iib
    Iib* p_iib;
    /// the M unit queue
    Eiq d_mbuf;
    /// the I unit queue
    Eiq d_ibuf;
    /// the F unit queue
    Eiq d_fbuf;
    /// the B unit queue
    Eiq d_bbuf;

  public:
    /// create a new physical rename stage by context
    /// @param stx the simulation context
    PrnStg (Stx* stx);

    /// create a new physical rename by context and name
    /// @param stx the simulation context
    /// @param name the stage name
    PrnStg (Stx* stx, const string& name);

    /// reset this physical rename stage
    void reset (void);

    /// flush this physical rename stage
    void flush (void);

    /// @return true if the stage is holding
    bool isholding (void) const;

    /// activate this physical rename stage
    void activate (void);

    /// report some stage information
    void report (void) const;

    /// bind this stage with an execution environment
    /// @param env the execution environment
    /// @param pstg the previous stage
    /// @param nstg the next stage
    void bind (Env* env, Stage* pstg, Stage* nstg);

    /// clear the physical rename resources
    void clear (void);

    /// @return true if all queues are empty
    bool isempty (void) const;

    /// @return true if a queue is empty by unit
    bool isempty (const t_unit unit) const;
 
    /// @return true if all queues are full
    bool isfull (void) const;

    /// @return true if a queue is full by unit
    bool isfull (const t_unit unit) const;

    /// @return an instruction in the queue by unit
    Dsi pop (const t_unit unit);

  private:
    // make the copy constructor private
    PrnStg (const PrnStg&);
    // make the assignement operator private
    PrnStg& operator = (const PrnStg&);
  };
}

#endif
