// ---------------------------------------------------------------------------
// - Checker.cpp                                                             -
// - iato:isa library - record checker class implementation                  -
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

#include "Checker.hpp"
#include "Exception.hpp"

namespace iato {

  // create a default checker

  Checker::Checker (void) {
    d_vchk = true;
    reset ();
  }

 // create a checker with a context

  Checker::Checker (Ctx* ctx) {
    d_vchk = ctx->getbool ("CHECKER-VERBOSE-FLAG");
    reset ();
  }

  // destroy this checker

  Checker::~Checker (void) {
  }

  // reset this checker

  void Checker::reset (void) {
    d_vrcd.clear ();
  }

  // add a record to the current cycle
  
  void Checker::add (const Record& rcd) {
    if (rcd.gettype () != Record::REGCHK) return;
    d_vrcd.push_back (rcd);
  }

  // check one record against the register bank

  bool Checker::check (Register* rbk) const {
    if (!rbk) return false;
    long size   = d_vrcd.size ();
    bool result = true;
    for (long i = 0; i < size; i++) {
      const Record rcd = d_vrcd[i];
      bool status = rbk->check (rcd);
      if ((status == false) && (d_vchk == true)) {
	throw Exception ("check-error", rcd.repr ());
      }
      result &= status;
    }
    return result;
  }
}
