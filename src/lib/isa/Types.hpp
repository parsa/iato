// ---------------------------------------------------------------------------
// - Types.hpp                                                               -
// - iato:isa library - base types definition                                -
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

#ifndef  IATO_TYPES_HPP
#define  IATO_TYPES_HPP

#ifndef  IATO_HUGE_HPP
#include "Huge.hpp"
#endif

#ifndef  IATO_REAL_HPP
#include "Real.hpp"
#endif

namespace iato {
    
  /// The Types definitions holds the machine independant type definition,
  /// as well as the extended types like the real type and basic IA64 type
  /// definitions. These definitions are related to the regsiter types,
  /// and the executing unit. Other definition related to the IA64 ISA are
  /// contained in specific files.

  /// the supported architecture
  enum t_arch {
    ITANIUM,  // Itanium I architecture
    MKINLEY   // McKinley  architecture
  };

  /// the default architecture
  const t_arch DEF_ARCH = MKINLEY;

  /// base register types
  enum t_lreg {
    GREG = 0x01, // general register
    NREG = 0x02, // nat register
    FREG = 0x03, // floating register
    PREG = 0x04, // predicate register
    BREG = 0x05, // branch register
    AREG = 0x06, // application register
    IPRG = 0x07, // instruction pointer register
    CFMR = 0x08, // current frame marker register
    PRRG = 0x09, // all predicate register
    PROT = 0x0A, // all rotating predicate register.
    UMRG = 0x0B, // user mask register
    PSRG = 0x0C, // processor status register
    CREG = 0x0D  // processor control register
  };

  /// no register encoding
  const t_byte LREG_NONE = 0x00;
  /// max register encoding
  const t_byte LREG_RMAX = 0x0E;

  /// instruction unit types
  enum t_unit {
    AUNIT = 0x01, // A unit
    MUNIT = 0x02, // M unit
    IUNIT = 0x03, // I unit 
    FUNIT = 0x04, // F unit 
    BUNIT = 0x05, // B unit 
    XUNIT = 0x06  // X unit
  };

  /// no unit encoding
  const t_byte UNIT_NONE = 0x00;
  /// max UNIT encoding
  const t_byte UNIT_UMAX = 0x07;
}

#endif
