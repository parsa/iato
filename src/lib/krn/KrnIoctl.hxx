// ---------------------------------------------------------------------------
// - KrnIoctl.hxx                                                            -
// - iato:krn library - ioctl related system call argument definition        -
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

#ifndef  IATO_KRNIOCTL_HXX
#define  IATO_KRNIOCTL_HXX

#ifndef  IATO_KRNTYPES_HXX
#include "KrnTypes.hxx"
#endif

#ifndef  IATO_EXCEPTION_HPP
#include "Exception.hpp"
#endif

// ---------------------------------------------------------------------------
// - linux 2.4 - gnu libc 2                                                  -
// ---------------------------------------------------------------------------

namespace iato {
  // ioctl definitions
  const t_octa KRN_TCGETS          = 0x5401;
  const t_octa KRN_TCSETS          = 0x5402;
  const t_octa KRN_TCSETSW         = 0x5403;
  const t_octa KRN_TCSETSF         = 0x5404;
  const t_octa KRN_TCGETA          = 0x5405;
  const t_octa KRN_TCSETA          = 0x5406;
  const t_octa KRN_TCSETAW         = 0x5407;
  const t_octa KRN_TCSETAF         = 0x5408;
  const t_octa KRN_TCSBRK          = 0x5409;
  const t_octa KRN_TCXONC          = 0x540A;
  const t_octa KRN_TCFLSH          = 0x540B;
  const t_octa KRN_TIOCEXCL        = 0x540C;
  const t_octa KRN_TIOCNXCL        = 0x540D;
  const t_octa KRN_TIOCSCTTY       = 0x540E;
  const t_octa KRN_TIOCGPGRP       = 0x540F;
  const t_octa KRN_TIOCSPGRP       = 0x5410;
  const t_octa KRN_TIOCOUTQ        = 0x5411;
  const t_octa KRN_TIOCSTI         = 0x5412;
  const t_octa KRN_TIOCGWINSZ      = 0x5413;
  const t_octa KRN_TIOCSWINSZ      = 0x5414;
  const t_octa KRN_TIOCMGET        = 0x5415;
  const t_octa KRN_TIOCMBIS        = 0x5416;
  const t_octa KRN_TIOCMBIC        = 0x5417;
  const t_octa KRN_TIOCMSET        = 0x5418;
  const t_octa KRN_TIOCGSOFTCAR    = 0x5419;
  const t_octa KRN_TIOCSSOFTCAR    = 0x541A;
  const t_octa KRN_FIONREAD        = 0x541B;
  const t_octa KRN_TIOCLINUX       = 0x541C;
  const t_octa KRN_TIOCCONS        = 0x541D;
  const t_octa KRN_TIOCGSERIAL     = 0x541E;
  const t_octa KRN_TIOCSSERIAL     = 0x541F;
  const t_octa KRN_TIOCPKT         = 0x5420;
  const t_octa KRN_FIONBIO         = 0x5421;
  const t_octa KRN_TIOCNOTTY       = 0x5422;
  const t_octa KRN_TIOCSETD        = 0x5423;
  const t_octa KRN_TIOCGETD        = 0x5424;
  const t_octa KRN_TCSBRKP         = 0x5425;

  // number of control characters
  const long KRN_NCCS = 32;
  // the termios structure
  struct krn_termios {
    t_quad d_iflag;        // input mode flags
    t_quad d_oflag;        // output mode flags
    t_quad d_cflag;        // control mode flags
    t_quad d_lflag;        // local mode flags
    t_byte d_line;         // line discipline
    t_byte d_cc[KRN_NCCS]; // control characters
    t_quad d_ispeed;       // input speed
    t_quad d_ospeed;       // output speed
    // reset this structure
    void reset (void) {
      d_iflag = QUAD_0;
      d_oflag = QUAD_0;
      d_cflag = QUAD_0;
      d_lflag = QUAD_0;
      d_line  = '\0';
      for (long i = 0; i < KRN_NCCS; i++) d_cc[i] = '\0';
      d_ispeed = QUAD_0;
      d_ospeed = QUAD_0;
    }
  };
}

// ---------------------------------------------------------------------------
// - linux based mapping                                                     -
// ---------------------------------------------------------------------------

#ifdef   OS_LINUX
#include <sys/ioctl.h>
#include <termios.h>
#include <errno.h>

namespace iato {
  // the ioctl request mapping with linux
  static t_octa map_ioctl_request (const t_octa req) {
    return req;
  }
  // map the system termios to kernel termios
  static void map_termios (krn_termios& kt, const termios& ut) {
    kt.reset ();
    kt.d_iflag = lfixocta (ut.c_iflag);
    kt.d_oflag = lfixocta (ut.c_iflag);
    kt.d_cflag = lfixocta (ut.c_iflag);
    kt.d_lflag = lfixocta (ut.c_iflag);
    kt.d_line  = ut.c_line;
    for (long i = 0; i < NCCS; i++) kt.d_cc[i] = ut.c_cc[i];
    kt.d_ispeed = lfixocta (ut.c_ispeed);
    kt.d_ospeed = lfixocta (ut.c_ospeed);
  }
}

#endif

// ---------------------------------------------------------------------------
// - solaris base mapping                                                    -
// ---------------------------------------------------------------------------

#ifdef   OS_SOLARIS
#include <termio.h>
#include <unistd.h>

namespace iato {
  // the ioctl request mapping with solaris
  static t_octa map_ioctl_request (const t_octa req) {
    t_octa result = OCTA_0;
    switch (req) {
    case KRN_TCGETS:
      result = TCGETS;
      break;
    default:
      throw Exception ("syscall-error", "unimplemented ioctl request");
      break;
    }
    return result;
  }
  // map the system termios to kernel termios
  static void map_termios (krn_termios& kt, const termios& ut) {
    kt.reset ();
    kt.d_iflag = lfixocta (ut.c_iflag);
    kt.d_oflag = lfixocta (ut.c_iflag);
    kt.d_cflag = lfixocta (ut.c_iflag);
    kt.d_lflag = lfixocta (ut.c_iflag);
    for (long i = 0; i < NCCS; i++) kt.d_cc[i] = ut.c_cc[i];
  }
}

#endif

#endif
