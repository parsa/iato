// ---------------------------------------------------------------------------
// - Resteer.hpp                                                             -
// - iato:iaoo client - pipeline flush and resteer class definition          -
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

#ifndef  IATO_RSE_HPP
#include "Rse.hpp"
#endif

#ifndef  IATO_ROB_HPP
#include "Rob.hpp"
#endif

#ifndef  IATO_IRB_HPP
#include "Irb.hpp"
#endif

#ifndef  IATO_MOB_HPP
#include "Mob.hpp"
#endif

#ifndef  IATO_IIB_HPP
#include "Iib.hpp"
#endif

#ifndef  IATO_URF_HPP
#include "Urf.hpp"
#endif

#ifndef  IATO_BPN_HPP
#include "Bpn.hpp"
#endif

#ifndef  IATO_RESTART_HPP
#include "Restart.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Resteer class is a derived class from the restart class. The
  /// derived class implements the resources that have to be flushed.

  class Resteer : public Restart {
  private:
    /// the rse
    Rse* p_rse;
    /// the rob
    Rob* p_rob;
    /// the irb
    Irb* p_irb;
    /// the mob
    Mob* p_mob;
    /// the iib
    Iib* p_iib;
    /// the urf
    Urf* p_urf;
    /// the bpn
    Bpn* p_bpn;

  public:
    /// create a new resteer
    Resteer (void);

    /// create a new resteer with a context
    /// @param mtx the architectural context
    Resteer (Mtx* mtx);

    /// flush all resource
    void flush (void);

    /// partially flush all resources
    void pflsh (void);

    /// bind the pipeline with this resteer engine
    /// @param pipe the pipeline to bind
    /// @param env  the resource environment
    void bind (Runnable* pipe, class Env* env);

  private:
    // make the copy constructor private
    Resteer (const Resteer&);
    // make the assignment operator private
    Resteer& operator = (const Resteer&);
  };
}

#endif
