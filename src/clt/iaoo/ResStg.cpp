// ---------------------------------------------------------------------------
// - ResStg.cpp                                                              -
// - iato:iaoo client - result stage abstract class implementation           -
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

#include "ResStg.hpp"

namespace iato {

  // create a new execute stage by context, unit and name

  ResStg::ResStg (Stx* stx, t_unit unit, 
		  const string& name) : Stage (stx, name) {
    d_unit = unit;
    reset ();
  }

  // reset this execute stage

  void ResStg::reset (void) {
    Stage::reset ();
    d_inst.reset ();
    d_resl.reset ();
    d_intr.reset ();
  }
    
  // flush this execute stage

  void ResStg::flush (void) {
    Stage::flush ();
    d_inst.reset ();
    d_resl.reset ();
    d_intr.reset ();
  }

  // return the executed instruction

  Dsi ResStg::getinst (void) const {
    return d_inst;
  }

  // return the instruction result

  Result ResStg::getresl (void) const {
    return d_resl;
  }

  // return the instruction interrupt

  Interrupt ResStg::getintr (void) const {
    return d_intr;
  }
}
