// ---------------------------------------------------------------------------
// - ElfStack.cpp                                                            -
// - iato:elf library - elf stack class implementation                       -
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

#include "Abi.hpp"
#include "ElfStack.hpp"

namespace iato {

  // create a default stack

  ElfStack::ElfStack (void) {
    // create a default kernel parameter
    ElfKernel ekp;
    // set stack parameters
    d_size  = ekp.getstksz ();
    p_data  = new t_byte[d_size];
    d_stkva = ekp.getstkva ();
    // set base, protection and mode
    setbase (d_stkva - (t_octa) d_size);
    setprot (PROT_FU);
    setmode (ekp.getmode ());
  }

  // create a new stack by kernel parameters
  
  ElfStack::ElfStack (ElfKernel* ekp) {
    // set stack parameters
    d_size  = ekp->getstksz ();
    p_data  = new t_byte[d_size];
    d_stkva = ekp->getstkva ();
    // set base, protection and mode
    setbase (d_stkva - (t_octa) d_size);
    setprot (PROT_FU);
    setmode (ekp->getmode ());
  }

  // set the elf stack address

  void ElfStack::setstkva (const t_octa addr) {
    d_stkva = addr;
  }

  // return the stack address

  t_octa ElfStack::getstkva (void) const {
    return d_stkva;
  }

  // print the contents of the initialized stack

  void ElfStack::pargs (void) const {
    using namespace std;
    // the stack pointer has been put down with 16 bytes (VSTK_PPAD)
    t_octa csp = d_stkva + VSTK_PPAD;
    // first argument ia argc
    t_octa argc = readocta (csp);
    cout << "argc    = 0x" << hex << argc << dec << endl;
    // second argument is argv
    csp += ABI_OCTA_ALIGN;
    t_octa argv = readocta (csp);
    cout << "argv[0] = 0x" << hex << argv << dec << endl;
    t_byte data = 0x00;
    while ((data = readbyte (argv++)) != 0x00) cout << data;
    cout << endl;
  }
}
