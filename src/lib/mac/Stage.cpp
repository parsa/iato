// ---------------------------------------------------------------------------
// - Stage.cpp                                                               -
// - iato:mac library - abstract stage class implementation                  -
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

#include "Stage.hpp"

namespace iato {
  
   // create a new stage with context by name
  
  Stage::Stage (Mtx* mtx, const string& name) : Delayable  (name) {
    p_pstg = 0;
    p_nstg = 0;
    reset ();
  }

  // reset this stage

  void Stage::reset (void) {
    Delayable::reset ();
    d_halt = false;
  }

  // flush this stage

  void Stage::flush (void) {
    Delayable::flush ();
    d_halt = false;
  }

  // activate this stage

  void Stage::run (void) {
    // check if this stage is halted
    if (d_halt == true) return;
    // check for latency - normally should be one
    if (--d_dcnt > 0) return;
    d_dcnt = d_dlat;
    // check for holding stage
    if (isholding () == true) return;
    activate ();
  }

  // by default the stage is not halted

  bool Stage::ishalted (void) const {
    return d_halt;
  }  

  // by default holding means halted

  bool Stage::isholding (void) const {
    return ishalted ();
  }  

  // set the previous stage with an environment

  void Stage::setprev (Env* env, Stage* stg) {
    p_pstg = stg;
  }

  // set the next stage with an environment

  void Stage::setnext (Env* env, Stage* stg) {
    p_nstg = stg;
  }

  // bind the previous and next stages

  void Stage::bind (Env* env, Stage* pstg, Stage* nstg) {
    p_pstg = pstg;
    p_nstg = nstg;
  }
}
