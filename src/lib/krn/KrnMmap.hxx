// ---------------------------------------------------------------------------
// - KrnMmap.hxx                                                             -
// - iato:krn library - mmap related system call argument definition         -
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

#ifndef  IATO_KRNMMAP_HXX
#define  IATO_KRNMMAP_HXX

#ifndef  IATO_KRNTYPES_HXX
#include "KrnTypes.hxx"
#endif

#ifndef  IATO_KRNERRNO_HXX
#include "KrnErrno.hxx"
#endif

// ---------------------------------------------------------------------------
// - linux 2.4 - gnu libc 2                                                  -
// ---------------------------------------------------------------------------

namespace iato {
  // mmap protections
  const t_octa KRN_MMAP_READ    = 0x0000000000000001ULL;
  const t_octa KRN_MMAP_WRITE   = 0x0000000000000002ULL;
  const t_octa KRN_MMAP_EXEC    = 0x0000000000000004ULL;
  const t_octa KRN_MMAP_NONE    = 0x0000000000000000ULL;

  // mmap types
  const t_octa KRN_MMAP_SHARED  = 0x0000000000000001ULL;
  const t_octa KRN_MMAP_PRIVATE = 0x0000000000000002ULL;
  const t_octa KRN_MMAP_FIXED   = 0x0000000000000010ULL;
  const t_octa KRN_MMAP_ANON    = 0x0000000000000020ULL;
}

// ---------------------------------------------------------------------------
// - linux based mapping                                                     -
// ---------------------------------------------------------------------------

#ifdef   OS_LINUX
#include <sys/mman.h>
#include <errno.h>
#endif

// ---------------------------------------------------------------------------
// - solaris base mapping                                                    -
// ---------------------------------------------------------------------------

#ifdef   OS_SOLARIS
#include <sys/mman.h>
#include <errno.h>
#endif

#endif
