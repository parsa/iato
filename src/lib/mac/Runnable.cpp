// ---------------------------------------------------------------------------
// - Runnable.cpp                                                            -
// - iato:mac library - abstract runnable object class implementation        -
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

#include "Mac.hpp"
#include "Runnable.hpp"

namespace iato {

  // create a default runnable

  Runnable::Runnable (void) {
    d_flsh = RM_PFLS;
  }

  // create a runnable by name

  Runnable::Runnable (const string& name) : Resource (name) {
    d_flsh = RM_PFLS;
  }

  // partially flush this interface

  void Runnable::pflsh (void) {
    if (d_flsh == true) flush ();
  }
}
