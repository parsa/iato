// ---------------------------------------------------------------------------
// - Resteer.cpp                                                             -
// - iato:iaoo client - pipeline flush and resteer class implementation      -
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

#include "Srn.hpp"
#include "Env.hpp"
#include "Resteer.hpp"

namespace iato {

  // create a default resteer resource
  
  Resteer::Resteer (void) {
    p_rse  = 0;
    p_rob  = 0;
    p_irb  = 0;
    p_mob  = 0;
    p_iib  = 0;
    p_urf  = 0;
    p_bpn  = 0;
    reset ();
  }

  // create a new resteer with a context
  
  Resteer::Resteer (Mtx* mtx) : Restart (mtx) {
    p_rse  = 0;
    p_rob  = 0;
    p_irb  = 0;
    p_mob  = 0;
    p_iib  = 0;
    p_urf  = 0;
    p_bpn  = 0;
    reset ();
  }

  // flush the resteer resources
  
  void Resteer::flush (void) {
    // flush the pipeline
    p_pipe->flush ();
    // reset all resources
    p_rob->reset  ();
    p_irb->reset  ();
    p_mob->reset  ();
    p_iib->reset  ();
    p_bpn->reset  ();
    p_rse->flush  ();
  }

  // bind the pipeline with this resource

  void Resteer::bind (Runnable* pipe, Env* env) {
    // bind the restart resource
    Restart::bind (pipe, env);
    // bind the rse
    p_rse = dynamic_cast <Rse*> (env->get (RESOURCE_RSE));
    if (!p_rse) {
      string msg = "cannot bind rse for resteer ";
      throw Exception ("bind-error", msg);
    }
    // bind the rob
    p_rob = dynamic_cast <Rob*> (env->get (RESOURCE_ROB));
    if (!p_rob) {
      string msg = "cannot bind rob for resteer ";
      throw Exception ("bind-error", msg);
    }
    // bind the irb
    p_irb = dynamic_cast <Irb*> (env->get (RESOURCE_IRB));
    if (!p_irb) {
      string msg = "cannot bind irb for resteer ";
      throw Exception ("bind-error", msg);
    }
    // bind the mob
    p_mob = dynamic_cast <Mob*> (env->get (RESOURCE_MOB));
    if (!p_mob) {
      string msg = "cannot bind mob for resteer ";
      throw Exception ("bind-error", msg);
    }
    // bind the iib
    p_iib = dynamic_cast <Iib*> (env->get (RESOURCE_IIB));
    if (!p_iib) {
      string msg = "cannot bind iib for resteer ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the urf
    p_urf = dynamic_cast <Urf*> (env->get (RESOURCE_URF));
    if (!p_urf) {
      string msg = "cannot bind urf for resteer ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the bpn
    p_bpn = dynamic_cast <Bpn*> (env->get (RESOURCE_BPN));
    if (!p_bpn) {
      string msg = "cannot bind bypass network for resteer ";
      throw Exception ("bind-error", msg + d_name);
    }
  }
}
