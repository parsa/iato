// ---------------------------------------------------------------------------
// - KrnUname.hxx                                                            -
// - iato:krn library - uname related system call argument definition        -
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

#ifndef  IATO_KRNUNAME_HXX
#define  IATO_KRNUNAME_HXX

// ---------------------------------------------------------------------------
// - linux 2.4 - gnu libc 2                                                  -
// ---------------------------------------------------------------------------

namespace iato {
  // uname definition
  const long    KRN_UTSNAME_LENGTH  = 65;
  // supported release for this implementation
  const t_byte* KRN_UTSNAME_SYSNAME = (const t_byte*) "Linux";
  // supported release for this implementation
  const t_byte* KRN_UTSNAME_RELEASE = (const t_byte*) "2.4.0";
  // this version implementation
  const t_byte* KRN_UTSNAME_VERSION = (const t_byte*) "iato, caps/inria, 2004";
  // this machine implementation
  const t_byte* KRN_UTSNAME_MACHINE = (const t_byte*) "ia64";

  // uname krn structure
  struct krn_utsname {
    t_byte sysname [KRN_UTSNAME_LENGTH]; // operating system name
    t_byte nodename[KRN_UTSNAME_LENGTH]; // network node name 
    t_byte release [KRN_UTSNAME_LENGTH]; // current release
    t_byte version [KRN_UTSNAME_LENGTH]; // current version
    t_byte machine [KRN_UTSNAME_LENGTH]; // current machine
  };
}

// ---------------------------------------------------------------------------
// - linux based mapping                                                     -
// ---------------------------------------------------------------------------

#ifdef   OS_LINUX
#include <sys/utsname.h>
#include <errno.h>
#endif

// ---------------------------------------------------------------------------
// - solaris base mapping                                                    -
// ---------------------------------------------------------------------------

#ifdef   OS_SOLARIS
#include <sys/utsname.h>
#include <errno.h>
#endif

#endif
