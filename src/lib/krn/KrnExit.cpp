// ---------------------------------------------------------------------------
// - KrnExit.cpp                                                             -
// - iato:isa library - exit exception class implementation                  -
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

#include "KrnExit.hpp"

namespace iato {
  using namespace std;

  // create an exit exception with a status code

  KrnExit::KrnExit (const t_octa status) {
    d_type   = "exit";
    d_reason = "exit system call";
    d_status = status;
  }

  // copy construct this interrupt

  KrnExit::KrnExit (const KrnExit& that) : Exception (that) {
    d_status = that.d_status;
  }

  // assign an interrupt to this one

  KrnExit& KrnExit::operator = (const KrnExit& that) {
    this->KrnExit::operator = (that);
    d_status = that.d_status;
    return *this;
  }

  // get the exit status

  t_octa KrnExit::getstatus (void) const {
    return d_status;
  }

  // print the interrupt message

  void KrnExit::print (void) const {
    cerr << "error   : " << d_type   << endl;
    cerr << "reason  : " << d_reason << endl;
    cerr << "status  : 0x";
    cerr << hex << setw (16) << setfill ('0') << d_status << dec << endl;
  }
}
