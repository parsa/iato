// ---------------------------------------------------------------------------
// - Resteer.hpp                                                             -
// - iato:iaio client - pipeline flush and resteer class definition          -
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

#ifndef  IATO_RESTEER_HPP
#define  IATO_RESTEER_HPP

#ifndef  IATO_IRT_HPP
#include "Irt.hpp"
#endif

#ifndef  IATO_RSE_HPP
#include "Rse.hpp"
#endif

#ifndef  IATO_IIB_HPP
#include "Iib.hpp"
#endif

#ifndef  IATO_BPN_HPP
#include "Bpn.hpp"
#endif

#ifndef  IATO_RESTART_HPP
#include "Restart.hpp"
#endif

#ifndef  IATO_SCOREBOARD_HPP
#include "Scoreboard.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Resteer class is a derived class from the restart class. The
  /// derived class implements the resources that have to be flushed. it also
  /// provides for an in-order machine a support for interrupted instruction
  /// notification. The class stores the interrupted instruction order index.
  /// During the write-back stage, all instructions prior the interrupted one
  /// are executed and the interrupted instruction set the interrupt to
  /// be processed prior any pipe flush.

  class Resteer : public Restart {
  private:
    /// the pending flag
    bool d_pend;
    /// the restart instruction order index
    long d_rioi;
    /// the interrupt to process
    Interrupt d_intr;
    /// the interrupt router
    Irt* p_irt;
    /// the rse
    Rse* p_rse;
    /// the iib
    Iib* p_iib;
    /// the early bypass network
    Bpn* p_ebn;
    /// the late bypass network
    Bpn* p_lbn;
    /// the scoreboard
    Scoreboard* p_psb;

  public:
    /// create a new resteer
    Resteer (void);

    /// create a new resteer with a context
    /// @param mtx the architectural context
    Resteer (Mtx* mtx);

    /// reset this resource
    void reset (void);

    /// flush all resource
    void flush (void);

    /// clean all resteer resource
    void clean (void);

    /// flush everything by default
    void pfdef (void);

    /// flush the pipe and restart with local conditions
    void pflcl (void);

    /// flush the pipe with a standard restart condition
    /// @param ip   the offending ip
    /// @param slot the offending slot
    void pfstd (const t_octa ip, const long slot);

    /// flush the pipe with a serialization condition
    /// @param ip   the offending ip
    /// @param slot the offending slot
    void pfsrl (const t_octa ip, const long slot);

    /// flush the pipe with a next restart condition
    /// @param ip   the offending ip
    /// @param slot the offending slot
    void pfnxt (const t_octa ip, const long slot);

    /// bind the pipeline with this resteer engine
    /// @param pipe the pipeline to bind
    /// @param env  the resource environment
    void bind (Runnable* pipe, class Env* env);

    /// set an interrupt to process 
    /// @param vi the virtual interrupt to process
    void setintr (const Interrupt& vi);

    /// process a pending restart condition
    void process (void);

    /// @return true if there is a pending restart
    bool ispending (void) const;

    /// @return true if there is a valid interrupt condition
    bool isintr (void) const;

    /// @return true if there is a valid restart index
    bool isrii (void) const;
    
    /// set the restart instruction order index
    /// @param rii the index to set
    void setrii (const long rii);

    /// @return the restart instruction order index
    long getrii (void) const;

    /// check if an index match a restart condition
    /// @param rix the restart index
    bool chkrii (const long rix) const;

  private:
    // make the copy constructor private
    Resteer (const Resteer&);
    // make the assignment operator private
    Resteer& operator = (const Resteer&);
  };
}

#endif
