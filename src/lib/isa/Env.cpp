// ---------------------------------------------------------------------------
// - Env.cpp                                                                 -
// - iato:isa library - execution environment class implementation           -
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

#include "Env.hpp"

namespace iato {

  // create a new environment

  Env::Env (void) {
  }

  // destroy this environment

  Env::~Env (void) {
    typedef map<string, Resource*>::iterator t_it;
    for (t_it it = d_rmap.begin (); it != d_rmap.end (); it++) {
      delete (*it).second;
    }
  }

  // reset this environment

  void Env::reset (void) {
    typedef map<string, Resource*>::iterator t_it;
    for (t_it it = d_rmap.begin (); it != d_rmap.end (); it++) {
      (*it).second->reset ();
    }
  }

  // add a new resource to this environment

  void Env::add (Resource* res) {
    if (!res) return;
    string name = res->getname ();
    d_rmap[name] = res;
  }

  // find a resource by name

  Resource* Env::get (const string& name) const {
    map<string, Resource*>::const_iterator it = d_rmap.find (name);
    if (it == d_rmap.end ()) return 0;
    return (*it).second;
  }

  // report all resources

  void Env::report (void) const {
    using namespace std;
    typedef map<string, Resource*>::const_iterator t_it;
    for (t_it it = d_rmap.begin (); it != d_rmap.end (); it++) {
      Resource* res = (*it).second;
      if (!res) continue;
      res->report ();
      cout << endl;
    }
  }

  // bind the stat collector to all resources

  void Env::setstc (Stat* stc) {
    using namespace std;
    typedef map<string, Resource*>::iterator t_it;
    for (t_it it = d_rmap.begin (); it != d_rmap.end (); it++) {
      Resource* res = (*it).second;
      if (!res) continue;
      res->setstc (stc);
    }
  }

  // bind the resource tracer to all resources

  void Env::settrc (Tracer* tracer) {
    using namespace std;
    typedef map<string, Resource*>::iterator t_it;
    for (t_it it = d_rmap.begin (); it != d_rmap.end (); it++) {
      Resource* res = (*it).second;
      if (!res) continue;
      res->settrc (tracer);
    }
  }
}
