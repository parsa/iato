// ---------------------------------------------------------------------------
// - Hma.cpp                                                                 -
// - iato:mac library - hierarchical memory architecture class implementation-
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
#include "Hma.hpp"

namespace iato {

  // create a new memory architecture with a memory image
  
  Hma::Hma (Memory* mem) : Runnable (RESOURCE_HMA) {
    // initialize mta
    p_mta = new Mta;
    p_mem = mem;
    d_mbb = true;
    // bind the mta
    if (d_mbb == true) {
      p_mta->bind (mem);
    } else {
      assert (false);
    }
  }

  // create a new memory model with a context and memory image
  
  Hma::Hma (Mtx* mtx, Memory* mem) : Runnable (RESOURCE_HMA) {
    // initialize mta
    p_mta = new Mta (mtx);
    p_mem = mem;
    d_mbb = mtx->getbool ("MEMORY-BYPASS-MODEL");
    // bind the mta
    if (d_mbb == true) {
      p_mta->bind (mem);
    } else {
      assert (false);
    }
  }

  // reset this interface

  void Hma::reset (void) {
    p_mta->reset ();
  }

  // flush this interface

  void Hma::flush (void) {
    p_mta->reset ();
  }

  // run this interface

  void Hma::run (void) {
  }

  // report this resource

  void Hma::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "hierarchical memory" << endl;
  }

  // get the memory transaction adapter

  Mta* Hma::getmta (void) const {
    return p_mta;
  }
}
