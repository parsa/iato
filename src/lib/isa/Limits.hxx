// ---------------------------------------------------------------------------
// - Limits.hxx                                                              -
// - iato:isa library - base limits definition - compiler dependent          -
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

#ifndef IATO_LIMITS_HXX
#define IATO_LIMITS_HXX

// With g++ version less than 3.2, it is not possible to use the standard
// stl limit file. Therefore, we need to go back with the ISOC99 standard
// in order to define NAN and and infinite numbers. With limits available
// this is no longer a problem. All values are defined as constant and
// hopefully, it will work fine. But, under some bad glibc configuration,
// such limits might not even works (NAN is defined as 0.0). Therefore, we 
// force the use of non standard limits.

#if defined(__GNUC__)

// using gcc less than 3.2
#if (__GNUC__ < 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ < 2))
#ifndef HAVE_NONSTD_LIMITS
#define HAVE_NONSTD_LIMITS
#endif
// using gcc 3.2 and above
#else
#ifndef HAVE_STD_LIMITS
#define HAVE_STD_LIMITS
#endif
#endif

#endif

// private numeric limits used only by us
namespace iato {
  static long double ldi_div (const long double x, const long double y) {
    return x / y;
  }
  const long double PRIVATE_LDI_NUL  = 0.0L;
  const long double PRIVATE_LDI_ONE  = 1.0L;
  const long double PRIVATE_LDI_NAN  = sqrt    (-PRIVATE_LDI_ONE);
  const long double PRIVATE_LDI_PINF = ldi_div (PRIVATE_LDI_ONE, 
						PRIVATE_LDI_NUL);
}

#ifdef   HAVE_STD_LIMITS
#include <limits>
namespace iato {
  // machine long double limits
  typedef std::numeric_limits<long double> t_ldi;
  // constant definition
  const long double LDI_NAN  = t_ldi::has_quiet_NaN ? t_ldi::quiet_NaN () :
                               PRIVATE_LDI_NAN;
  const long double LDI_PINF = t_ldi::has_infinity  ? t_ldi::infinity  () :
                               PRIVATE_LDI_PINF;
  const long double LDI_NINF = -LDI_PINF;
}
#endif

#ifdef  HAVE_NONSTD_LIMITS
namespace iato {
  const long double LDI_NAN  = PRIVATE_LDI_NAN;
  const long double LDI_PINF = PRIVATE_LDI_PINF;
  const long double LDI_NINF = -LDI_PINF;
}
#endif

#endif
