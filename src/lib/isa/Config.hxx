// ---------------------------------------------------------------------------
// - Config.hxx                                                              -
// - iato:isa library - base configuration definition                        -
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

#ifndef IATO_CONFIG_HXX
#define IATO_CONFIG_HXX

// ---------------------------------------------------------------------------
// - gcc version checking                                                    -
// ---------------------------------------------------------------------------

#if defined (__GNUC__) && defined (__ia64__) && (__GNUC__ < 3)
#error "invalid gcc version for IA64 target, version 3 required"
#endif

// ---------------------------------------------------------------------------
// - base type definition                                                    -
// ---------------------------------------------------------------------------

namespace iato {
    
  /// The Config type definitions holds the machine independant type. 
  /// For portability reasons, most of the address and register values are
  /// treated as an unsigned type. The "basic" IA64 types are also defined
  /// here. A special case for IA64 real requires a special class that is
  /// defined elsewhere.

  /// one byte unsigned type
  typedef unsigned char      t_byte; // 1 byte
  /// two bytes unsigned type
  typedef unsigned short     t_word; // 2 bytes
  /// four bytes unsigned type
  typedef unsigned int       t_quad; // 4 bytes
  /// eight bytes unsigned type
  typedef unsigned long long t_octa; // 8 bytes
  /// four bytes signed type
  typedef int                t_sint; // 4 bytes
  /// eight bytes signed type
  typedef long long int      t_long; // 8 bytes
  /// unsined long : 4 or 8 bytes 
  typedef unsigned long      t_size;

  /// basic byte constants used everywhere
  const t_byte BYTE_0 = 0x00U;
  const t_byte BYTE_1 = 0x00U;
  const t_byte BYTE_A = 0xFFU;

  /// basic word constants used everywhere
  const t_word WORD_0 = 0x0000U;
  const t_word WORD_1 = 0x0001U;
  const t_word WORD_A = 0xFFFFU;

  /// basic quad constants used everywhere
  const t_quad QUAD_0 = 0x00000000U;
  const t_quad QUAD_1 = 0x00000001U;
  const t_quad QUAD_A = 0xFFFFFFFFU;

  /// basic octa constants used everywhere
  const t_octa OCTA_0 = 0x0000000000000000ULL;
  const t_octa OCTA_1 = 0x0000000000000001ULL;
  const t_octa OCTA_A = 0xFFFFFFFFFFFFFFFFULL;

  /// basic sign extend mask used in execution unit
  const t_octa MASK_1 = 0x00000000000000FFULL;
  const t_octa MASK_2 = 0x000000000000FFFFULL;
  const t_octa MASK_4 = 0x00000000FFFFFFFFULL;
  const t_octa MASK_8 = 0xFFFFFFFFFFFFFFFFULL;
  
  /// basic sign extract mask used in execution unit
  const t_octa SIGN_1 = 0x0000000000000080ULL;
  const t_octa SIGN_2 = 0x0000000000008000ULL;
  const t_octa SIGN_4 = 0x0000000080000000ULL;
  const t_octa SIGN_8 = 0x8000000000000000ULL;

  /// the iato major number
  const t_byte RMAJOR = 0x01;
  /// the iato minor number
  const t_byte RMINOR = 0x05; 
}

// ---------------------------------------------------------------------------
// - linux configuration                                                     -
// ---------------------------------------------------------------------------

#ifdef   OS_LINUX
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <set>
#include <map>
#include <queue>
#include <vector>
#include <cassert>
#include <cmath>
#endif

// ---------------------------------------------------------------------------
// - sparc/solaris configuration                                             -
// ---------------------------------------------------------------------------

#ifdef   OS_SOLARIS
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <set>
#include <map>
#include <queue>
#include <vector>
#include <cassert>
#include <cmath>
#endif

// ---------------------------------------------------------------------------
// - pthread extension                                                       -
// ---------------------------------------------------------------------------

#ifdef   HAS_PTHREAD
#include <pthread.h>
#endif

#endif
