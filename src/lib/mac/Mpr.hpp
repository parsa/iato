// ---------------------------------------------------------------------------
// - Mpr.hpp                                                             -
// - iato:mac library - memory port request class definition                 -
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

#ifndef  IATO_MPR_HPP
#define  IATO_MPR_HPP
 
#ifndef  IATO_MTA_HPP
#include "Mta.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

namespace iato {

  /// The Mpr class is a memory request port. The port is bound with a memory
  /// transaction adapter (mta). If the port is associated with a memory
  /// request interface (mri), the port can be operating in blocking mode.
  /// Without mri, the port operates in direct mode (i.e without delay).

  class Mpr : public Resource {
  private:
    /// the busy bit
    bool d_busy;
    /// the acknowldge bit
    bool d_tack;
    /// the memory request
    Mrt  d_mrt;
    /// the memory adapter
    Mta* p_mta;
    
  public:
    /// create a default port
    Mpr (void);

    /// create a port by context
    /// @param mtx  the architectural context
    Mpr (Mtx* mtx);

    /// create a port by context and name
    /// @param mtx  the architectural context
    /// @param name the port name
    Mpr (Mtx* mtx, const string& name);

    /// reset this port mta
    void reset (void);

    /// flush this port mta
    void flush (void);

    /// report this port mta
    void report (void) const;

    /// @return the busy bit
    virtual bool isbusy (void) const;

    /// @return the transaction acknowldge bit
    virtual bool istack (void) const;

    /// @return the memory request type
    virtual Mrt getmrt (void) const;

    /// @return a transaction bundle by index
    virtual Bundle getbndl (const long index) const;

    /// issue a port request by mrt
    /// @param mrt the memory request to process
    virtual void request (const Mrt& mrt);

    /// issue a port load request by mrt
    /// @param mrt the memory request to process
    virtual void preset (const Mrt& mrt);

    /// issue a port store request by mrt
    /// @param mrt the memory request to process
    virtual void update (const Mrt& mrt);

    /// bind the port resources
    /// @param mta the mta to bind
    virtual void bind (Mta* mta);

  private:
    // make the copy constructor private
    Mpr (const Mpr&);
    // make the assignment operator private
    Mpr& operator = (const Mpr&);
  };
}

#endif
