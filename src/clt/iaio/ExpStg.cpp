// ---------------------------------------------------------------------------
// - ExpStg.cpp                                                              -
// - iato:iaio client - expand (dispersal) stage class implementation        -
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
#include "ExpStg.hpp"
#include "Exception.hpp"

namespace iato {

  // create a new expand stage by context and name

  ExpStg::ExpStg (Stx* stx) : Stage (stx, RESOURCE_EXP) {
    d_iwsz = stx->getiwsz (); assert (d_iwsz > 0);
    p_bdb  = 0;
    p_iib  = 0;
    p_ipb  = 0;
    p_psb  = 0;
    p_dis  = 0;
    reset ();
  }

  // create a new expand stage by context and name

  ExpStg::ExpStg (Stx* stx, const string& name) : Stage (stx, name) {
    d_iwsz = stx->getiwsz (); assert (d_iwsz > 0);
    p_bdb  = 0;
    p_iib  = 0; 
    p_ipb  = 0;
    p_psb  = 0;
    p_dis  = 0;
    reset ();
  }

  // reset this expand stage

  void ExpStg::reset (void) {
    Stage::reset ();
    if (p_bdb) p_bdb->reset ();
    if (p_dis) p_dis->reset ();
  }
    
  // flush this expand stage

  void ExpStg::flush (void) {
    Stage::flush ();
    if (p_bdb) p_bdb->reset ();
    if (p_ipb) p_ipb->reset ();
    if (p_dis) p_dis->reset ();
  }

  // return true if the stage is holding

  bool ExpStg::isholding (void) const {
    if (p_ipb) return !p_ipb->isfree ();
    return false;
  }

  // activate this expand stage - the bundle are queried from the
  // previous stage and expandd on the fly.

  void ExpStg::activate (void) {
    // reset the issue port buffer
    p_ipb->reset ();
    // disperse the bundles
    for (long i = 0; i < d_iwsz; i++) {
      // grab a bundle from the decoupling buffer
      Bundle bndl = p_bdb->pop ();
      if (bndl.isvalid () == false) continue;
      // disperse the bundle
      p_dis->expand (bndl,i);
      // check if the bundle must be pushed back
      if (p_dis->isback (bndl) == true) {
	p_bdb->back (bndl);
	break;
      }
      // check if the port buffer has been interrupted
      if (p_ipb->isintr () == true) break;
      // check if we continue
      if (p_dis->isnext (bndl) == false) break;
    }
    // update the tracer
    if (p_tracer) {
      // loop in the ipb for instruction
      long mbsz = p_ipb->getsize (MUNIT);
      for (long i = 0; i < mbsz; i++) {
	Instr inst = p_ipb->getinst (MUNIT, i);
	if (inst.isvalid () == false) continue;
	Record rcd (d_name, inst);
	p_tracer->add (rcd);
      }
      long ibsz = p_ipb->getsize (IUNIT);
      for (long i = 0; i < ibsz; i++) {
	Instr inst = p_ipb->getinst (IUNIT, i);
	if (inst.isvalid () == false) continue;
	Record rcd (d_name, inst);
	p_tracer->add (rcd);
      }
      long fbsz = p_ipb->getsize (FUNIT);
      for (long i = 0; i < fbsz; i++) {
	Instr inst = p_ipb->getinst (FUNIT, i);
	if (inst.isvalid () == false) continue;
	Record rcd (d_name, inst);
	p_tracer->add (rcd);
      }
      long bbsz = p_ipb->getsize (BUNIT);
      for (long i = 0; i < bbsz; i++) {
	Instr inst = p_ipb->getinst (BUNIT, i);
	if (inst.isvalid () == false) continue;
	Record rcd (d_name, inst);
	p_tracer->add (rcd);
      }
      if (d_halt == true) {
	Record rcd (d_name);
	rcd.settype (Record::HALTED);
	p_tracer->add (rcd);
      }
    }
  }

  // report some stage information

  void ExpStg::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: bundle expand and dispersal" << endl;
    cout << "\tbundle window size \t: " << d_iwsz << endl;
  }

  // bind this stage from the environment

  void ExpStg::bind (Env* env, Stage* pstg, Stage* nstg) {
    // bind the stage locally
    Stage::bind (env, pstg, nstg);
    // bind the bdb
    p_bdb = dynamic_cast <Bdb*> (env->get (RESOURCE_BDB));
    if (!p_bdb) {
      string msg = "cannot bind decoupling buffer within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the iib
    p_iib = dynamic_cast <Iib*> (env->get (RESOURCE_IIB));
    if (!p_iib) {
      string msg = "cannot bind interrupt buffer within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the ipb
    p_ipb = dynamic_cast <Spb*> (env->get (RESOURCE_IPB));
    if (!p_ipb) {
      string msg = "cannot bind issue port buffer within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the scoreboard
    p_psb = dynamic_cast <Scoreboard*> (env->get (RESOURCE_PSB));
    if (!p_psb) {
      string msg = "cannot bind scoreboard within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
    // bind the dispersal engine
    p_dis = dynamic_cast <Disperse*> (env->get (RESOURCE_BDS));
    if (!p_dis) {
      string msg = "cannot bind dispersal engine within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
  }
}
