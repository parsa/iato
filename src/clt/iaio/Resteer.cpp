// ---------------------------------------------------------------------------
// - Resteer.cpp                                                             -
// - iato:iaio client - pipeline flush and resteer class implementation      -
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

#include "Srn.hpp"
#include "Env.hpp"
#include "Resteer.hpp"

namespace iato {

  // create a default resteer resource
  
  Resteer::Resteer (void) {
    p_rse = 0;
    p_iib = 0;
    p_ebn = 0;
    p_lbn = 0;
    p_psb = 0;
    reset ();
  }

  // create a new resteer with a context
  
  Resteer::Resteer (Mtx* mtx) : Restart (mtx) {
    p_rse = 0;
    p_iib = 0;
    p_ebn = 0;
    p_lbn = 0;
    p_psb = 0;
    reset ();
  }

  // reset this resteer

  void Resteer::reset (void) {
    Restart::reset ();
    clean ();
  }

  // flush this resource by marking the request flag

  void Resteer::flush (void) {
    d_pend = true;
  }

  // clean local resources
  
  void Resteer::clean (void) {
    d_pend = false;
    d_rioi = -1;
    d_intr.reset ();
  }

  // flush the pipe by default

  void Resteer::pfdef (void) {
    // preserve restart index
    long rioi = d_rioi;
    // set pipe flush data
    if (d_pend == false) {
      Restart::pfdef ();
      d_rioi = rioi;
    }
  }
    
  // update the restart with a local condition

  void Resteer::pflcl (void) {
    // preserve restart index
    long rioi = d_rioi;
    // set pipe flush data
    if (d_pend == false) {
      Restart::pflcl ();
      d_rioi = rioi;
    }
  }

  // update the restart with the current ip

  void Resteer::pfstd (const t_octa ip, const long slot) {
    // preserve restart index
    long rioi = d_rioi;
    // set pipe flush data
    if (d_pend == false) {
      Restart::pfstd (ip, slot);
      d_rioi = rioi;
    }
  }

  // update the restart with a serialization condition

  void Resteer::pfsrl (const t_octa ip, const long slot) {
    // preserve restart index
    long rioi = d_rioi;
    // set pipe flush data
    if (d_pend == false) {
      Restart::pfsrl (ip, slot);
      d_rioi = rioi;
    }
  }

  // update the restart with a next condition

  void Resteer::pfnxt (const t_octa ip, const long slot) {
    // preserve restart index
    long rioi = d_rioi;
    if (d_pend == false) {
      Restart::pfnxt (ip, slot);
      d_rioi = rioi;
    }
  }

  // bind the pipeline with this resource

  void Resteer::bind (Runnable* pipe, Env* env) {
    // bind the restart resource
    Restart::bind (pipe, env);
    // bind the irt
    p_irt = dynamic_cast <Irt*> (env->get (RESOURCE_IRT));
    if (!p_irt) {
      string msg = "cannot bind interrupt router within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the rse
    p_rse = dynamic_cast <Rse*> (env->get (RESOURCE_RSE));
    if (!p_rse) {
      string msg = "cannot bind rse for resteer ";
      throw Exception ("bind-error", msg);
    }
    // bind the iib
    p_iib = dynamic_cast <Iib*> (env->get (RESOURCE_IIB));
    if (!p_iib) {
      string msg = "cannot bind iib for resteer ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the early bypass network
    p_ebn = dynamic_cast <Bpn*> (env->get (RESOURCE_EBN));
    if (!p_ebn) {
      string msg = "cannot bind early bypass network for resteer ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the late bypass network
    p_lbn = dynamic_cast <Bpn*> (env->get (RESOURCE_LBN));
    if (!p_lbn) {
      string msg = "cannot bind late bypass network for resteer ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the scoreboard
    p_psb = dynamic_cast <Scoreboard*> (env->get (RESOURCE_PSB));
    if (!p_psb) {
      string msg = "cannot bind scoreboard for resteer ";
      throw Exception ("bind-error", msg + d_name);
    }
  }
    
  // set an interrupt to process 

  void Resteer::setintr (const Interrupt& vi) {
    // check for valid interrupt and save
    if (vi.isvalid () == false) return;
    // reset everything
    reset ();
    // set the restart condition
    pfnxt (vi.getip (), vi.getslot ());
    // save the interrupt
    d_intr = vi;
  }
    
  // process a pending restart condition
  
  void Resteer::process (void) {
    if (d_pend == true) {
      // check for an interrupt first
      if (d_intr.isvalid () == true) {
	p_irt->route (d_intr);
	d_intr.reset ();
      }
      // flush the pipeline
      p_pipe->flush ();
      // reset all resources
      p_iib->reset  ();
      p_ebn->reset  ();
      p_lbn->reset  ();
      p_psb->reset  ();
      p_rse->flush  ();
      // clean locally
      clean ();
    }
  }

  // return true if there is a pending restart

  bool Resteer::ispending (void) const {
    return d_pend;
  }

  // return true if there is an interrupt condition

  bool Resteer::isintr (void) const {
    return d_intr.isvalid ();
  }

  // return true if ther is a valid restart index

  bool Resteer::isrii (void) const {
    return (d_rioi != -1);
  }

  // set the restart instruction order index

  void Resteer::setrii (const long rii) {
    if (d_rioi == -1) {
      clean ();
      d_rioi = rii;
    } else {
      if (rii < d_rioi) {
	clean ();
	d_rioi = rii;
      }
    }
  }

  // return the restart instruction order index

  long Resteer::getrii (void) const {
    return d_rioi;
  }

  // check an index with the restart index

  bool Resteer::chkrii (const long rix) const {
    // check for valid index
    if (rix == -1) return false;
    // check for valid rii
    if (d_rioi == -1) return false;
    // check index range
    return (rix > d_rioi);
  }
}
