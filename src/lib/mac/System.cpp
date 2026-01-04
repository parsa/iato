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
#include "Abi.hpp"
#include <cassert>
#include <cstdlib>
#include <iostream>

namespace iato {
  using namespace std;

  // create a new system interface with a context and a program name

  System::System (Mtx* mtx, const string& name) {
    p_elf = new ElfImage (mtx, name);
    p_mem = p_elf->getimage ();
    p_sci = new Syscall (p_mem);
    p_hma = new Hma     (mtx, p_mem);
    patchsdata ();
  }

  // create a new system interface with a context, program name and arguments

  System::System (Mtx* mtx, const string& name, const vector<string>& argv) {
    p_elf = new ElfImage (mtx, name, argv);
    p_mem = p_elf->getimage ();
    p_sci = new Syscall (p_mem);
    p_hma = new Hma     (mtx, p_mem);
    patchsdata ();
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

  // return the arg block base

  t_octa System::getargva (void) const {
    if (!p_mem) return OCTA_0;
    ElfStack* stk = p_mem->getstkm ();
    return (stk == 0) ? OCTA_0 : stk->getargva ();
  }

  // return the TLS base

  t_octa System::gettlsva (void) const {
    if (!p_mem) return OCTA_0;
    ElfStack* stk = p_mem->getstkm ();
    return (stk == 0) ? OCTA_0 : stk->gettlsva ();
  }

  // return the entry global pointer

  t_octa System::getgpva (void) const {
    if (!p_elf) return OCTA_0;
    return p_elf->getentrygp ();
  }

  // seed small data globals (argc/argv/env, stack end, etc.)

  void System::patchsdata (void) {
    if ((!p_elf) || (!p_mem)) return;
    ElfStack* stk = p_mem->getstkm ();
    if (!stk) return;

    auto write_octa = [&](const string& sym, const t_octa value) {
      if (value == OCTA_0) return;
      t_octa addr = p_elf->getsymaddr (sym);
      if ((addr != OCTA_0) && p_mem->isvalid (addr)) p_mem->writeocta (addr, value);
    };
    auto write_quad = [&](const string& sym, const t_quad value) {
      t_octa addr = p_elf->getsymaddr (sym);
      if ((addr != OCTA_0) && p_mem->isvalid (addr)) p_mem->writequad (addr, value);
    };

    write_octa ("__libc_stack_end", stk->getstackend ());
    write_octa ("__libc_argv", stk->getargv ());
    const char* envsyms[] = {"__environ", "_environ", "environ", "last_environ"};
    for (const char* sym : envsyms) write_octa (sym, stk->getenvp ());
    long argc = stk->getargc ();
    if (argc >= 0) write_quad ("__libc_argc", static_cast<t_quad> (argc));
    ElfBrk* brk = p_mem->getbrkm ();
    if (brk) write_quad ("__libc_pagesize", static_cast<t_quad> (brk->getpgsz ()));

    // seed the gp-relative slots consumed by __libc_start_main
    t_octa gp = getgpva ();
    const t_octa bootva = stk->getbootparam ();
    const bool dump_gp_slots = (getenv ("IATO_DEBUG_GP_SLOTS") != nullptr);
    auto write_slot = [&](const t_octa offset, const t_octa value) {
      if (value == OCTA_0) return;
      t_octa addr = gp + offset;
      assert (p_mem->isvalid (addr));
      p_mem->writeocta (addr, value);
      assert (p_mem->readocta (addr) == value);
    };
    auto dump_slot = [&](const t_octa offset, const char* label) {
      if (dump_gp_slots == false) return;
      const t_octa addr = gp + offset;
      if (p_mem->isvalid (addr) == false) return;
      const t_octa val = p_mem->readocta (addr);
      cerr << "[gp] slot " << label << " @gp+0x"
           << hex << offset << " = 0x" << val << dec << endl;
    };
    write_slot (696, bootva);
    dump_slot (696, "bootparam");
    // main/init/fini descriptors live in .opd; store their addresses
    write_slot (688, p_elf->getopdaddr ("main"));
    dump_slot (688, "main.opd");
    write_slot (400, p_elf->getopdaddr ("_init"));
    dump_slot (400, "_init.opd");
    write_slot (800, p_elf->getopdaddr ("_fini"));
    dump_slot (800, "_fini.opd");
    // stack_end pointer slot expects the address of the global itself
    write_slot (1064, p_elf->getsymaddr ("__libc_stack_end"));
    dump_slot (1064, "__libc_stack_end");
    // GOT slot 792 (gp+792) is _dl_starting_up; glibc expects it to be non-null (or null?)
    // For static binaries, it might expect 0 or 1. Let's see if it helps to seed it.
    // Actually, 792 = 0x318. The instruction `addl r15=792,r1` computes this address.
    write_slot (792, p_elf->getsymaddr ("_dl_starting_up"));
    dump_slot (792, "_dl_starting_up");
    // GOT slot 48 (gp+48) is __libc_multiple_libcs
    write_slot (48, p_elf->getsymaddr ("__libc_multiple_libcs"));
    dump_slot (48, "__libc_multiple_libcs");
    // GOT slot 104 (gp+104) is also __libc_stack_end
    write_slot (104, p_elf->getsymaddr ("__libc_stack_end"));
    dump_slot (104, "__libc_stack_end (alt)");
    // Inspect the gp+408 slot which feeds the ctor/brk trampolines.
    dump_slot (408, "gp+408");
    // Patch the ia64 boot parameter entry descriptor so _start can call libc.
    // Some static binaries omit the __libc_start_main entry in .opd, so fall
    // back to the raw symbol address paired with the module GP.
    if (bootva != OCTA_0) {
      const t_octa entry = p_elf->getsymaddr ("__libc_start_main");
      const t_octa gpval = gp;
      if ((entry != OCTA_0) && (gpval != OCTA_0)) {
	p_mem->writeocta (bootva, entry);
	p_mem->writeocta (bootva + ABI_ADDR_ALIGN, gpval);
      }
    }
  }

  // return the backing store base

  t_octa System::getbspva (void) const {
    if (!p_mem) return OCTA_0;
    ElfBsa* bsa = p_mem->getbsam ();
    t_octa bspva = bsa ? bsa->getbase () : OCTA_0;
    return bspva;
  }

  // return the current break value

  t_octa System::getbrkva (void) const {
    if (!p_mem) return OCTA_0;
    ElfBrk* brk = p_mem->getbrkm ();
    return (brk == 0) ? OCTA_0 : brk->getheaplimit ();
  }

  // return the elf image checker

  Checker* System::getchecker (void) const {
    if (!p_elf) return 0;
    return p_elf->getchecker ();
  }
}
