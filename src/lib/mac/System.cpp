// ---------------------------------------------------------------------------
// - System.hpp                                                              -
// - iato:mac library - system interface class implementation                -
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

#include "System.hpp"

namespace iato {
  using namespace std;

  // create a new system interface with a context and a program name

  System::System (Mtx* mtx, const string& name) {
    p_elf = new ElfImage (mtx, name);
    p_mem = p_elf->getimage ();
    p_sci = new Syscall (p_mem);
    p_hma = new Hma     (mtx, p_mem);
  }

  // create a new system interface with a context, program name and arguments

  System::System (Mtx* mtx, const string& name, const vector<string>& argv) {
    p_elf = new ElfImage (mtx, name, argv);
    p_mem = p_elf->getimage ();
    p_sci = new Syscall (p_mem);
    p_hma = new Hma     (mtx, p_mem);
  }

  // destroy this system interface

  System::~System (void) {
    delete p_elf;
    delete p_sci;
    delete p_hma;
  }

  // reset this system interface

  void System::reset (void) {
    p_hma->reset ();
  }

  // flush this system interface

  void System::flush (void) {
    p_hma->reset ();
  }

  // run this system interface

  void System::run (void) {
    p_hma->run ();
  }

  // report some system information

  void System::report (void) const {
    using namespace std;
    Resource::report ();
    cout << "\tresource type\t\t: " << "processor system resources" << endl;
    p_sci->report ();
    p_hma->report ();
  }

  // return the system call plugin

  Syscall* System::getsci (void) const {
    return p_sci;
  }

  // return the hierarchical memory architecture

  Hma* System::gethma (void) const {
    return p_hma;
  }

  // return the entry point

  t_octa System::getentry (void) const {
    t_octa entry = p_elf ? p_elf->getentry () : OCTA_0;
    return entry;
  }

  // return the top of stack

  t_octa System::getstkva (void) const {
    if (!p_mem) return OCTA_0;
    ElfStack* stk = p_mem->getstkm ();
    t_octa stkva = stk ? stk->getstkva () : OCTA_0;
    return stkva;
  }

  // return the backing store base

  t_octa System::getbspva (void) const {
    if (!p_mem) return OCTA_0;
    ElfBsa* bsa = p_mem->getbsam ();
    t_octa bspva = bsa ? bsa->getbase () : OCTA_0;
    return bspva;
  }

  // return the elf image checker

  Checker* System::getchecker (void) const {
    if (!p_elf) return 0;
    return p_elf->getchecker ();
  }
}
