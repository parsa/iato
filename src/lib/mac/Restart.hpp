// ---------------------------------------------------------------------------
// - Restart.hpp                                                             -
// - iato:mac library - pipeline flush and restart class definition          -
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

#ifndef  IATO_RESTART_HPP
#define  IATO_RESTART_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_REGISTER_HPP
#include "Register.hpp"
#endif

#ifndef  IATO_RUNNABLE_HPP
#include "Runnable.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Restart class is a special class designed to act as a pipeline
  /// restart under certain circumstances. When a pipeline flush request
  /// occurs, all stages are flushed and a restart condition is placed
  /// in the restart. The fetch and decode stages uses that information
  /// to reset appropriately the associated resources and restart the
  /// execution of the instruction at the proper instruction slot.

  class Restart : public Resource {
  protected:
    /// the instruction restart flag
    bool d_ipfr;
    /// the pre-serialization flag
    bool d_srlz;
    /// the restart ip
    t_octa d_rip;
    /// the restart slot
    long d_slot;
    /// the pipeline to flush
    Runnable* p_pipe;
    /// the register bank
    Register* p_rbk;

  public:
    /// create a new restart
    Restart (void);

    /// create a new restart with a context
    /// @param mtx the architectural context
    Restart (Mtx* mtx);

    /// reset this restart
    void reset (void);

    /// flush all resource
    virtual void flush (void) =0;

    /// report this resource
    void report (void) const;

    /// bind the pipeline with this restart engine
    /// @param pipe the pipeline to bind
    /// @param env  the resource environment
    virtual void bind (Runnable* pipe, class Env* env);

    /// @return the serialization restart flag
    virtual bool getsrlz (void) const;
 
    /// check if an instruction is valid
    /// @param iip the instruction ip to check
    /// @param slot the slot to check
    virtual bool isvalid (const t_octa iip, const long slot) const;

    /// check if an instruction must be cancelled and reset
    /// @param iip the instruction ip to check
    /// @param slot the slot to check
    virtual bool check (const t_octa iip, const long slot);

    /// flush everything by default
    virtual void pfdef (void);

    /// flush the pipe and restart with local conditions
    virtual void pflcl (void);

    /// flush the pipe with a standard restart condition
    /// @param ip   the offending ip
    /// @param slot the offending slot
    virtual void pfstd (const t_octa ip, const long slot);

    /// flush the pipe with a serialization condition
    /// @param ip   the offending ip
    /// @param slot the offending slot
    virtual void pfsrl (const t_octa ip, const long slot);

    /// flush the pipe with a next restart condition
    /// @param ip   the offending ip
    /// @param slot the offending slot
    virtual void pfnxt (const t_octa ip, const long slot);

  private:
    // make the copy constructor private
    Restart (const Restart&);
    // make the assignment operator private
    Restart& operator = (const Restart&);
  };
}

#endif
