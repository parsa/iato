// ---------------------------------------------------------------------------
// - SysArgs.cpp                                                             -
// - iato:krn library - system call argument function implementation         -
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
#include "SysArgs.hpp"

namespace iato {
  
  // the system call number register index
  const long SC_RG_NUM = 15;
  // the system call status register index
  const long SS_RG_ERR = 10;
  // the system call return register index
  const long SS_RG_RET = 8;

  // return the system call number that is stored in gr[15]

  long sys_args_getnr (Register* rbk) {
    assert (rbk);
    return (long) rbk->getoval (GREG, SC_RG_NUM);
  }

  // get a string from the memory

  t_byte* sys_args_getstr (const t_octa addr, Memory* mem) {
    if (!mem) return 0;
    // compute string length
    long   len = 0;
    t_octa sma = addr;
    while (mem->isvalid (sma) && (mem->readbyte (sma++) != BYTE_0)) len++;
    // allocate result buffer
    t_byte* result = new t_byte[len+1];
    // copy result buffer
    sma = addr;
    for (long i = 0; i < len; i++) result[i] = mem->readbyte (sma++);
    result[len] = BYTE_0;
    return result;
  }

  // set a data buffer in memory
  void sys_args_setdata (const t_octa base,const t_byte* buf,const long size,
			 Memory* mem) {
    if (!buf || (size == 0) || !mem) return;
    for (long i = 0; i < size; i++) {
      t_octa addr = base + i;
      mem->writebyte (addr, buf[i]);
    }
  }

  // set a register return result

  void sys_args_setretn (const t_octa resl, Register* rbk) {
     assert (rbk);
     rbk->write (GREG, SS_RG_ERR, OCTA_0);
     rbk->write (GREG, SS_RG_RET, resl);
  }

  // set the system call error only

  void sys_args_seterrn (const t_octa resl, const t_octa errn, Register* rbk) {
     assert (rbk);
     // set return value
     rbk->write (GREG, SS_RG_RET, OCTA_0);
     rbk->write (GREG, SS_RG_ERR, (t_octa) -1);
  }

  // get an argument by index

  t_octa sys_args_getoval (const long argi, Rse* rse, Register* rbk) {
    assert (rse); assert (rbk);
    // get the rse state
    Rse::State state = rse->getste ();
    long index = state.mapgr (argi +  GR_STBS, false);
    return rbk->getoval (GREG, index);
  }
}

