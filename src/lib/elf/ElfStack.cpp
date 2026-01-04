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
#include "Exception.hpp"

namespace {
  using namespace iato;

  struct Ia64ConsoleInfo {
    t_word num_cols;
    t_word num_rows;
    t_word orig_x;
    t_word orig_y;
  };

  struct Ia64BootParam {
    t_octa entry_ip;
    t_octa entry_gp;
    t_octa command_line;
    t_octa efi_systab;
    t_octa efi_memmap;
    t_octa efi_memmap_size;
    t_octa efi_memdesc_size;
    t_quad efi_memdesc_version;
    Ia64ConsoleInfo console_info;
    t_octa fpswa;
    t_octa initrd_start;
    t_octa initrd_size;
  };
}

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
    d_bootva = OCTA_0;
    d_cmdva = OCTA_0;
    d_loader_off = 0;
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
    d_bootva = OCTA_0;
    d_cmdva = OCTA_0;
    d_loader_off = 0;
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

  t_octa ElfStack::allocloader (const long size, const long align) {
    if (size <= 0) {
      throw Exception ("elf-stack-error", "invalid loader allocation size");
    }
    const long al = (align <= 0) ? ABI_ADDR_ALIGN : align;
    const long mask = al - 1;
    long offset = (d_loader_off + mask) & ~mask;
    long next   = offset + size;
    if (next >= d_size) {
      throw Exception ("elf-stack-error", "loader data exceeds stack bounds");
    }
    d_loader_off = next;
    return getbase () + static_cast<t_octa> (offset);
  }

  t_octa ElfStack::installbootparam (const std::string& cmdline) {
    if (d_bootva != OCTA_0) return d_bootva;
    std::string text = cmdline;
    if (text.empty ()) text = "";
    long slen = static_cast<long> (text.size ()) + 1;
    t_octa cmdva = allocloader (slen, ABI_ADDR_ALIGN);
    for (long i = 0; i < slen - 1; ++i) {
      writebyte (cmdva + static_cast<t_octa> (i),
		 static_cast<t_byte> (text[static_cast<size_t> (i)]));
    }
    writebyte (cmdva + static_cast<t_octa> (slen - 1), BYTE_0);
    d_cmdva = cmdva;

    Ia64BootParam bp {};
    bp.command_line = cmdva;
    bp.console_info.num_cols = static_cast<t_word> (80);
    bp.console_info.num_rows = static_cast<t_word> (25);
    bp.console_info.orig_x   = 0;
    bp.console_info.orig_y   = 0;

    const t_octa bootva = allocloader (static_cast<long> (sizeof (bp)),
				       ABI_ADDR_ALIGN);
    const t_byte* raw = reinterpret_cast<const t_byte*> (&bp);
    for (size_t i = 0; i < sizeof (bp); ++i) {
      writebyte (bootva + static_cast<t_octa> (i), raw[i]);
    }
    d_bootva = bootva;
    return d_bootva;
  }

  t_octa ElfStack::getbootparam (void) const {
    return d_bootva;
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
