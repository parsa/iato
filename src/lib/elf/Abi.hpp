// ---------------------------------------------------------------------------
// - Abi.hpp                                                                 -
// - iato:isa library - application binary interface definition              -
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

#ifndef IATO_ABI_HPP
#define IATO_ABI_HPP

#ifndef  IATO_ISA_HPP
#include "Isa.hpp"
#endif

namespace iato {

  /// ------------------------------------------------------------------------
  /// - reserved register mapping                                            -
  /// ------------------------------------------------------------------------

  /// abi global pointer mapping
  const long ABI_GP = 1;
  /// abi stack pointer mapping
  const long ABI_SP = 12;
  /// abi thread pointer mapping
  const long ABI_TP = 13;

  /// ------------------------------------------------------------------------
  /// - type alignment                                                       -
  /// ------------------------------------------------------------------------

  /// word alignment
  const long ABI_WORD_ALIGN = 2;
  /// quad alignment
  const long ABI_QUAD_ALIGN = 4;
  /// octa alignment
  const long ABI_OCTA_ALIGN = 8;
  /// address alignment
  const long ABI_ADDR_ALIGN = 8;
  /// stack address alignment
  const long ABI_VSTK_ALIGN = 16;

  /// ------------------------------------------------------------------------
  /// - mappable memory configuration                                        -
  /// ------------------------------------------------------------------------

  const t_octa ABI_MMAP_BASE = 0x2000000000000000ULL;

  /// ------------------------------------------------------------------------
  /// - stack and rse backing store configuration                            -
  /// ------------------------------------------------------------------------

  /// the default page shift - 16 Kb = 2^^14
  const long   PAGE_SHFT = 14;
  /// the virtual base address for all special area (reserved)
  const t_octa SPBS_BASE = 0x6000000000000000ULL;
  /// the bsa reference base address
  const t_octa VBSA_BREF = 0x0000000080000000ULL;

  /// default stack size 8Mb
  const long   VSTK_SIZE = 1024 * 1024 * 8;
  /// default stack padding within call
  const long   VSTK_PPAD = 16;
  /// default backing store size
  const long   VBSA_SIZE = 1024 * 1024;

  /// ------------------------------------------------------------------------
  /// - hardware configuration - for auxilliary vector                       -
  /// ------------------------------------------------------------------------

  /// end of vector
  const t_octa ELF_AT_NULL     = 0;
  /// entry should be ignored
  const t_octa ELF_AT_IGNORE   = 1;
  /// file descriptor of program
  const t_octa ELF_AT_EXECFD   = 2;
  /// program headers for program
  const t_octa ELF_AT_PHDR     = 3;
  /// size of program header
  const t_octa ELF_AT_PHENT    = 4;
  /// number of program headers
  const t_octa ELF_AT_PHNUM    = 5;
  /// system page size
  const t_octa ELF_AT_PAGESZ   = 6;
  /// base address of interpreter
  const t_octa ELF_AT_BASE     = 7;
  /// special flags
  const t_octa ELF_AT_FLAGS    = 8;
  /// program entry point
  const t_octa ELF_AT_ENTRY    = 9;
  /// proram is not elf
  const t_octa ELF_AT_NOTELF   = 10;
  /// real uid
  const t_octa ELF_AT_UID      = 11;
  /// effective uid
  const t_octa ELF_AT_EUID     = 12;
  /// real gid
  const t_octa ELF_AT_GID      = 13;
  /// effective gid
  const t_octa ELF_AT_EGID     = 14;
  /// frequency of times
  const t_octa ELF_AT_CLKTCK   = 17;
  /// platform identification
  const t_octa ELF_AT_PLATFORM = 15;
  /// machine hints
  const t_octa ELF_AT_HWCAP    = 16;
  /// exec-setuid flag
  const t_octa ELF_AT_SECURE   = 23;

  /// processor capabilities
  const t_octa ELF_AUX_HWCAP   = OCTA_0;
  /// processor timer count in Hz
  const t_octa ELF_AUX_CLKTK   = 1024;
}

#endif
