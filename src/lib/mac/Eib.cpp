// ---------------------------------------------------------------------------
// - Eib.cpp                                                                 -
// - iato:mac library - external interrupt buffer class implementation       -
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

#include "Prn.hpp"
#include "Eib.hpp"

namespace iato {
  using namespace std;

  // create a default interrupt buffer

  Eib::Eib (void) : Resource (RESOURCE_EIB) {
    reset ();
  }

  // create an interrupt buffer with a context

  Eib::Eib (Mtx* mtx) : Resource (RESOURCE_EIB) {
    reset ();
  }

  // reset this interrupt buffer

  void Eib::reset (void) {
    while (d_ibuf.empty () == false) d_ibuf.pop ();
  }

  // report this resource

  void Eib::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "external interrupt buffer" << endl;
  }

  // return true if the buffer is empty

  bool Eib::isempty (void) const {
    return d_ibuf.empty ();
  }

  // push an interrupt in this buffer

  void Eib::push (const Interrupt& vi) {
    d_ibuf.push (vi);
  }

  // pop an interrupt from this buffer

  Interrupt Eib::pop (void) {
    Interrupt vi;
    if (d_ibuf.empty () == false) {
      vi = d_ibuf.front (); d_ibuf.pop ();
    }
    return vi;
  }
}
