// ---------------------------------------------------------------------------
// - Plugin.cpp                                                              -
// - iato:isa library - abstract plugin class implementation                 -
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

#include "Utils.hpp"
#include "Plugin.hpp"

namespace iato {
  using namespace std;

  // create a plugin by name

  Plugin::Plugin (const string& name) : Resource (name) {
  }

  // reset this plugin

  void Plugin::reset (void) {
  }

  // apply this plugin

  void Plugin::apply (void) {
    throw Exception ("plugin-error", "invalid plugin call in void mode");
  }

  // apply this plugin with a virtual interrupt

  void Plugin::apply (const Interrupt& vi) {
    if (vi.isvalid () == false) return;
    string msg = "invalid plugin call with an interrupt";
    throw Exception ("plugin-error", msg);
  }  
}

