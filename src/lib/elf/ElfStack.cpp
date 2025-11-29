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
    d_argva = OCTA_0;
    d_tlsva = OCTA_0;
    d_tlssz = VTLS_SIZE;
    d_argc  = 0;
    d_argvp = OCTA_0;
    d_envpp = OCTA_0;
    // set stack parameters
    d_size  = ekp.getstksz ();
    p_data  = new t_byte[d_size];
    d_stkva = ekp.getstkva ();
    d_initva = d_stkva;
    // set base, protection and mode
    setbase (d_stkva - (t_octa) d_size);
    setprot (PROT_FU);
    setmode (ekp.getmode ());
    // reserve the TLS window at the top of the stack
    inittls ();
  }

  // create a new stack by kernel parameters
  
  ElfStack::ElfStack (ElfKernel* ekp) {
    d_argva = OCTA_0;
    d_tlsva = OCTA_0;
    d_tlssz = VTLS_SIZE;
    d_argc  = 0;
    d_argvp = OCTA_0;
    d_envpp = OCTA_0;
    // set stack parameters
    d_size  = ekp->getstksz ();
    p_data  = new t_byte[d_size];
    d_stkva = ekp->getstkva ();
    d_initva = d_stkva;
    // set base, protection and mode
    setbase (d_stkva - (t_octa) d_size);
    setprot (PROT_FU);
    setmode (ekp->getmode ());
    // reserve the TLS window at the top of the stack
    inittls ();
  }

  // set the elf stack address

  void ElfStack::setstkva (const t_octa addr) {
    d_stkva = addr;
  }

  // return the stack address

  t_octa ElfStack::getstkva (void) const {
    return d_stkva;
  }

  // set the stack argument vector base (argc slot)

  void ElfStack::setargva (const t_octa addr) {
    d_argva = addr;
  }

  // return the stack argument vector base

  t_octa ElfStack::getargva (void) const {
    return d_argva;
  }

  // return the TLS base pointer

  t_octa ElfStack::gettlsva (void) const {
    return d_tlsva;
  }

  void ElfStack::setargc (const long argc) {
    d_argc = argc;
  }

  long ElfStack::getargc (void) const {
    return d_argc;
  }

  void ElfStack::setargv (const t_octa addr) {
    d_argvp = addr;
  }

  t_octa ElfStack::getargv (void) const {
    return d_argvp;
  }

  void ElfStack::setenvp (const t_octa addr) {
    d_envpp = addr;
  }

  t_octa ElfStack::getenvp (void) const {
    return d_envpp;
  }

  t_octa ElfStack::getinitva (void) const {
    return d_initva;
  }

  t_octa ElfStack::getstackend (void) const {
    return d_stkva + VSTK_PPAD;
  }

  // reserve a TLS page at the top of the stack and adjust SP

  void ElfStack::inittls (void) {
    if ((d_tlssz <= 0) || (d_size <= 0)) {
      d_tlsva = d_stkva;
      return;
    }
    long tlsz = d_tlssz;
    long align = VTLS_ALIGN;
    long rem = tlsz % align;
    if (rem != 0) tlsz += (align - rem);
    if (tlsz >= d_size) tlsz = d_size / 16;
    if (tlsz <= 0) {
      d_tlsva = d_stkva;
      return;
    }
    t_octa top = getbase () + (t_octa) d_size;
    d_tlsva = top - (t_octa) tlsz;
    if (d_tlsva < getbase ()) d_tlsva = getbase ();
    long offset = static_cast<long> (d_tlsva - getbase ());
    for (long i = 0; i < tlsz; i++) p_data[offset + i] = 0x00;
    d_stkva = d_tlsva;
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
