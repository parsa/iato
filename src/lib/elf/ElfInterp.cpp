// ---------------------------------------------------------------------------
// - ElfInterp.cpp                                                           -
// - iato:elf library - elf default ia64 interpreter class implementation    -
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
#include "ElfInterp.hpp"
#include "Exception.hpp"

namespace iato {
  using namespace std;

  // supported interpreter name - do not accept ld-linux.so since
  // it is the IA32 elf interpreter
  const string ELF_INTERP         = "/lib/ld.so";
  const string ELF_INTERP_LINUX   = "/lib/ld-linux-ia64.so";
  const string ELF_INTERP_LINUX_2 = "/lib/ld-linux-ia64.so.2";

  // this procedure align a size with a stack alignemnt
  static long align_stack_size (const long size) {
    if ((size % ABI_VSTK_ALIGN) == 0) return size;
    long np = size / ABI_VSTK_ALIGN;
    return (np + 1) * ABI_VSTK_ALIGN;
  }
 
  // this procedure install one auxially vector on the stack, the top
  // stack pointer is automatically adjusted
  static inline void set_aux_data (ElfStack* stk, const t_octa type, 
				   const t_octa data) {
    // do nothing with nothing
    if (!stk) return;
    // get the stack pointer
    t_octa tsp = stk->getstkva ();
    stk->writeocta (tsp -= ABI_ADDR_ALIGN, data);
    stk->writeocta (tsp -= ABI_ADDR_ALIGN, type);
    stk->setstkva (tsp);
  }

  // this procedure install the auxilliary elf vector on the stack
  // the auxilliary vector is by definition aligned with the abi requirements
  static void set_elf_aux (ElfStack* stk, ElfKernel* ekp) {
    // do nothing with nothing
    if ((!stk) || (!ekp)) return;    
    // install the null record
    set_aux_data (stk, ELF_AT_NULL, OCTA_0);
    // install additional info
    set_aux_data (stk, ELF_AT_PAGESZ, ekp->getpgsz  ());
    set_aux_data (stk, ELF_AT_CLKTCK, ekp->getclktk ());
  }

  // this procedure copy a data block on the stack and adjust the stack
  // pointer automatically
  static void set_elf_tbl (ElfStack* stk, const t_byte* blk, const long size) {
    // do nothing with nothing
    if ((!stk) || (!blk) || (size == 0)) return;
    // align the stack size to allocate
    long slen = align_stack_size (size);
    // get top stack pointer and adjust
    t_octa tsp = stk->getstkva () - (t_octa) slen;
    t_octa csp = tsp;
    // copy the array
    for (long i = 0; i < size; i++) stk->writebyte (csp++, blk[i]);
    // save stack pointer
    stk->setstkva (tsp);
  }

  // this procedure install a table pointer array in the stack
  static void set_elf_vec (ElfStack* stk, const t_octa* tbl, const long size) {
    // do nothing with nothing
    if ((!stk) || (!tbl) || (size == 0)) return;
    // get top stack pointer and adjust
    t_octa tsp = stk->getstkva () - (t_octa) (size * ABI_ADDR_ALIGN);
    t_octa csp = tsp;
    // copy the array
    for (long i = 0; i < size; i++) {
      stk->writeocta (csp, tbl[i]);
      csp += ABI_ADDR_ALIGN;
    }
    // save stack pointer
    stk->setstkva (tsp);
  }

  // create a default interpreter

  ElfInterp::ElfInterp (void) {
    d_interp = "none";
    p_kernel = 0;
  }

  // create a default interpreter by name

  ElfInterp::ElfInterp (const string& interp) {
    if (isvalid (interp) == false) {
      string msg = "invalid elf interpreter ";
      throw Exception ("interp-error", msg + interp);
    }
    d_interp = interp;
    p_kernel = 0;
  }

  // destroy this interpreter

  ElfInterp::~ElfInterp (void) {
  }

  // bind the elf kernel parameters

  void ElfInterp::bind (ElfKernel* ekp) {
    p_kernel = ekp;
  }

  // bind the program arguments in the memory as defined by the ABI

  void ElfInterp::bind (ElfArgs* args, ElfEnvp* envp, ElfStack* stk) const {
    if ((!args) || (!envp) || (!stk)) return;
    // install the environment block
    t_byte* envb = envp->getargb ();
    set_elf_tbl (stk, envb, envp->getargs ());
    delete [] envb;
    t_octa  estk = stk->getstkva ();
    // install the argument block
    t_byte* argb = args->getargb ();
    set_elf_tbl (stk, argb, args->getargs ());
    delete [] argb;
    t_octa  astk = stk->getstkva ();
    // compute all size to fix pading
    long envc = envp->getargc ();
    long argc = args->getargc ();
    long nlen = envc + argc + 1;
    // check if we need to align  the stack
    if ((nlen % 2) != 0) {
      t_octa tsp = stk->getstkva ();
      stk->writeocta (tsp -= ABI_ADDR_ALIGN, OCTA_0);
      stk->setstkva (tsp);
    }
    // install the auxilliary vector
    set_elf_aux (stk, p_kernel);
    // relocate the environment array
    t_octa* envv = envp->getargv (estk);
    set_elf_vec (stk, envv, envc + 1);
    delete [] envv;
    // relocate the argument array
    t_octa* argv = args->getargv (astk);
    set_elf_vec (stk, argv, argc + 1);
    delete [] argv;
    // copy the argv pointer and argc
    t_octa  cstk = stk->getstkva ();
    stk->writeocta (cstk -= ABI_ADDR_ALIGN, (t_octa) argc);
    // pad the stack with abi call displacement
    cstk -= VSTK_PPAD;
    stk->setstkva (cstk);
    assert ((cstk % ABI_VSTK_ALIGN) == OCTA_0);
  }

  // return true is the name is a valid interpreter name

  bool ElfInterp::isvalid (const string& interp) {
    if (interp == ELF_INTERP)         return true;
    if (interp == ELF_INTERP_LINUX)   return true;
    if (interp == ELF_INTERP_LINUX_2) return true;
    return false;
  }
}
