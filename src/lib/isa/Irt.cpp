// ---------------------------------------------------------------------------
// - Irt.cpp                                                                 -
// - iato:isa library - interrupt routing table class implementation         -
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

#include "Isa.hpp"
#include "Lrn.hpp"
#include "Irt.hpp"

namespace iato {

  // the irt structure
  struct t_irt {
    // the irt mode
    Irt::t_imode d_mode;
    // the plugin pointer
    Plugin*      p_plug;
    // default constructor
    t_irt (void) {
      reset ();
    }
    // reset this irt
    void reset (void) {
      d_mode = Irt::THROWN;
      p_plug = 0; 
    }
  };

  // create a new default irt

  Irt::Irt (void) : Resource (RESOURCE_IRT) {
    p_irt = new t_irt[IA_ICSZ];
    clear ();
  }

  // create a irt with a context

  Irt::Irt (Ctx* ctx) : Resource (RESOURCE_IRT) {
    p_irt = new t_irt[IA_ICSZ];
    clear ();
  }

  // create a irt with a context and name

  Irt::Irt (Ctx* ctx, const string& name) : Resource (name) {
    p_irt = new t_irt[IA_ICSZ];
    clear ();
  }

  // destroy this irt

  Irt::~Irt (void) {
    delete [] p_irt;
  }

  // reset this irt

  void Irt::reset (void) {
  }

  // report this irt bank

  void Irt::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "interrupt routing table" << endl;
    cout << "\ttable size   \t\t: " << IA_ICSZ << endl;
  }

  // clear the irt bank

  void Irt::clear (void) {
    for (long i = 0; i < IA_ICSZ; i++) p_irt[i].reset ();
  }

  // set the irt mode by interrupt code

  void Irt::setmode (const t_icode code, const t_imode mode) {
    p_irt[code].d_mode = mode;
    p_irt[code].p_plug = 0;
  }

  // return the irt mode

  Irt::t_imode Irt::getmode (const t_icode code) const {
    return p_irt[code].d_mode;
  }

  // bind a plugin with an interrupt code

  void Irt::bind (const t_icode code, Plugin* plug) {
    p_irt[code].d_mode = PLUGIN;
    p_irt[code].p_plug = plug;
  }

  // return the irt plugin pointer

  Plugin* Irt::getplug (const t_icode code) const {
    return p_irt[code].p_plug;
  }

  // route an interrupt

  void Irt::route (const Interrupt& vi) const {
    // do nothing if the interrupt is invalid
    if (vi.isvalid () == false) return;
    // get the interrupt code
    t_icode icode = vi.getcode ();
    // extract mode and plugin pointer
    t_imode imode = p_irt[icode].d_mode;
    Plugin* plug  = p_irt[icode].p_plug;
    // route according to mode
    switch (imode) {
    case THROWN:
      throw vi;
      break;
    case IGNORE:
      break;
    case PLUGIN:
      if (plug) plug->apply (vi);
      break;
    }
  }
}
