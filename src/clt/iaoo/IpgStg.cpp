// ---------------------------------------------------------------------------
// - IpgStg.cpp                                                              -
// - iato:iaoo client - instruction pointer generation stage implementation  -
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
#include "IpgStg.hpp"

namespace iato {

  // create a new ipg stage by context and name

  IpgStg::IpgStg (Stx* stx) : Stage (stx, RESOURCE_IPG) {
    d_iwsz = stx->getiwsz (); assert (d_iwsz > 0);
    d_bwsz = stx->getbwsz (); assert (d_bwsz > 0);
    p_bndl = new Bundle[d_iwsz];
    p_preq = new Mpr (stx);
    p_bprd = 0;
    p_reng = 0;
    p_bank = 0;
    reset ();
  }

  // create a new ipg stage by context and name

  IpgStg::IpgStg (Stx* stx, const string& name) : Stage (stx, name) {
    d_iwsz = stx->getiwsz (); assert (d_iwsz > 0);
    d_bwsz = stx->getbwsz (); assert (d_bwsz > 0);
    p_bndl = new Bundle[d_iwsz];
    p_preq = new Mpr (stx);
    p_bprd = 0;
    p_reng = 0;
    p_bank = 0;
    reset ();
  }

  // destroy this ipg stage

  IpgStg::~IpgStg (void) {
    delete    p_preq;
    delete [] p_bndl;
  }

  // reset this ipg stage

  void IpgStg::reset (void) {
    Stage::reset ();
    // clear bundle arrays
    clear ();
    // reset the speculative ip
    d_sip  = OCTA_0;
    d_sflg = false;
    // reset port request
    if (p_preq) p_preq->reset ();
  }
    
  // flush this ipg stage

  void IpgStg::flush (void) {
    Stage::flush ();
    // clear bundle arrays
    clear ();
    // reset the speculative ip
    d_sip  = OCTA_0;
    d_sflg = false;
    // flush port request
    if (p_preq) p_preq->flush ();
  }

  // return true if the stage is holding

  bool IpgStg::isholding (void) const {
    if (p_nstg) return p_nstg->isholding ();
    return false;
  }

  // activate this ipg stage - this is a complex task since
  // he stage acts to holds transfer in both pipeline direction.
  // If data are not ready, the stage is not ready. If the next stage is
  // on hold, nothing happen.

  void IpgStg::activate (void) {
    // clear the bundle array
    clear ();
    // check if we have some data in the port bundle
    if (p_preq->istack () == true) {
      // get the bundle ip and compute next ip
      bool   btk = false;
      t_octa nip = p_bprd->nextip  (d_sip, d_bwsz);
      // copy the bundle locally and check for prediction
      for (long i = 0; i < d_iwsz; i++) {
	// check for taken branches
	if (btk == true) {
	  p_bndl[i].reset ();
	  continue;
	}
	// get the bundle from the port
	p_bndl[i] = p_preq->getbndl (i);
	// get the bundle ip
	t_octa bip = p_bndl[i].getbip ();
	// loop in the slots and check for branches
	for (long j = 0; j < BN_SLSZ; j++) {
	  // ignore instruction that are below the restarting point
	  if (p_reng->isvalid (bip, j) == false) continue;
	  // check for branches only
	  if (p_bndl[i].isbr (j) == false) continue;
	  // here we have a branch, let's see if we predict it taken
	  // we use ispredict, since we want to make sure that the ip
	  // can be computed correctly
	  btk = p_bprd->ispredict (bip);
	  // if the branch can be predicted, we mark the other slot
	  // as invalid, update the speculative ip and terminates the
	  // bundle mapping
	  if (btk == true) {
	    // get the predictor history
	    t_octa hist = p_bprd->gethist ();
	    // compute the speculative ip and update bundle
	    nip = p_bprd->predict (bip);
	    // update the bundle
	    p_bndl[i].setsip (nip, j, hist);
	    break;
	  }
	}
      }
      // update the speculative ip
      d_sip = nip;
    }
    // check if we issue a new request
    try {
      if (p_preq->isbusy () == false) {
	if (d_sflg == false) {
	  d_sip  = p_bank->getoval (IPRG, 0);
	  d_sflg = true;
	}
	// format and issue request
	Mrt mrt (Mrt::REQ_BND, d_sip);
	p_preq->request (mrt);
      }
    } catch (const Interrupt& vi) {
      d_intr = vi;
    }
    // update the tracer
    if (p_tracer) {
      for (long i = 0; i < d_iwsz; i++) {
	if (p_bndl[i].isvalid () == true) {
	  Record rcd (d_name, p_bndl[i]);
	  p_tracer->add (rcd);
	}
      }
      if (d_halt == true) {
	Record rcd (d_name);
	rcd.settype (Record::HALTED);
	p_tracer->add (rcd);
      }
    }
  }
  
  // report some stage information

  void IpgStg::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: instruction pointer generation" << endl;
    cout << "\tbundle window size \t: " << d_iwsz     << endl;
    cout << "\tbytes  window size \t: " << d_bwsz     << endl;
    cout << "\tdelayable latency  \t: " << getdlat () << endl;
  }

  // clear the bundle array
  
  void IpgStg::clear (void) {
    for (long i = 0; i < d_iwsz; i++) p_bndl[i].reset ();
    d_intr.reset ();
  }

  // bind this stage from the environment

  void IpgStg::bind (Env* env, Stage* pstg, Stage* nstg) {
    // bind the stage locally
    Stage::bind (env, pstg, nstg);
    // bind the branch prediction system
    p_bprd = dynamic_cast <Branch*>   (env->get (RESOURCE_BPS));
    if (!p_bprd) {
      string msg = "cannot bind branch predictor within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the restart resource
    p_reng = dynamic_cast <Restart*> (env->get (RESOURCE_PFR));
    if (!p_reng) {
      string msg = "cannot bind restart engine within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the register bank
    p_bank = dynamic_cast <Register*> (env->get (RESOURCE_RBK));
    if (!p_bank) {
      string msg = "cannot bind register bank within stage ";
      throw Exception ("bind-error", msg + d_name);
    }    
    // bind the mta
    Mta* mta = dynamic_cast <Mta*> (env->get (RESOURCE_MTA));
    if (!mta) {
      string msg = "cannot bind mta within port request ";
      throw Exception ("bind-error", msg + d_name);
    } else {
      p_preq->bind (mta);
    }
  }

  // return a bundle by index

  Bundle IpgStg::getbndl (const long index) {
    assert ((index >= 0) && (index < d_iwsz));
    Bundle bndl = p_bndl[index];;
    p_bndl[index].reset ();
    return bndl;
  }

  // return the ipg interrupt

  Interrupt IpgStg::getintr (void) {
    Interrupt result = d_intr;
    d_intr.reset ();
    return result;
  }
}
