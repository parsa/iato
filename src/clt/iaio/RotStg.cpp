// ---------------------------------------------------------------------------
// - RotStg.cpp                                                              -
// - iato:iaio client - instruction rotate stage class implementation        -
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
#include "RotStg.hpp"
#include "Exception.hpp"

namespace iato {

  // create a new rotate stage by context and name

  RotStg::RotStg (Stx* stx) : Stage (stx, RESOURCE_ROT) {
    d_iwsz = stx->getiwsz (); assert (d_iwsz > 0);
    p_bdb  = 0;
    reset ();
  }

  // create a new rotate stage by context and name

  RotStg::RotStg (Stx* stx, const string& name) : Stage (stx, name) {
    d_iwsz = stx->getiwsz (); assert (d_iwsz > 0);
    p_bdb  = 0;
    reset ();
  }

  // reset this rotate stage

  void RotStg::reset (void) {
    Stage::reset ();
    if (p_bdb) p_bdb->reset ();
  }
    
  // flush this rotate stage

  void RotStg::flush (void) {
    Stage::flush ();
    if (p_bdb) p_bdb->reset ();
  }

  // return true if the stage is holding

  bool RotStg::isholding (void) const {
    if ((p_bdb) && (p_bdb->isthr () == true)) return true;
    return false;
  }

  // activate this rotate stage - the bundle are queried from the
  // previous stage and rotated on the fly.

  void RotStg::activate (void) {
    // get the previous stage and map it to ipg
    IpgStg* ipg = dynamic_cast <IpgStg*> (p_pstg);
    assert (ipg);
    // get the bundle and rotate them
    for (long i = 0; i < d_iwsz; i++) {
      Bundle bndl = ipg->getbndl (i);
      p_bdb->push (bndl);
      // update the tracer
      if (p_tracer) {
	Record rcd (d_name, bndl);
	p_tracer->add (rcd);
      }
    }
    // check if the previous stage is halted
    if (p_pstg && (p_pstg->ishalted () == true)) {
      d_halt = true;
    }
    // update the tracer
    if (p_tracer) {
      if (d_halt == true) {
	Record rcd (d_name);
	rcd.settype (Record::HALTED);
	p_tracer->add (rcd);
      }
    }
  }

  // report some stage information

  void RotStg::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: bundle rotate" << endl;
    cout << "\tbundle window size \t: " << d_iwsz << endl;
  }

  // bind this stage from the environment

  void RotStg::bind (Env* env, Stage* pstg, Stage* nstg) {
    // bind the stage locally
    Stage::bind (env, pstg, nstg);
    // bind the bdb
    p_bdb = dynamic_cast <Bdb*> (env->get (RESOURCE_BDB));
    if (!p_bdb) {
      string msg = "cannot bind decoupling buffer within stage ";
      throw Exception ("bind-error", msg + d_name);
    }
  }
}
