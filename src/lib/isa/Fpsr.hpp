// ---------------------------------------------------------------------------
// - Fpsr.hpp                                                                -
// - iato:isa library - floating point status register class definition      -
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

#ifndef  IATO_FPSR_HPP
#define  IATO_FPSR_HPP

#ifndef  IATO_TYPES_HPP
#include "Types.hpp"
#endif

#ifndef  IATO_OPCODE_HPP
#include "Opcode.hpp"
#endif

namespace iato {
  
  /// The Fpsr class is the floating point status register class object. It 
  /// holds all the field that are defined by the IA64 ISA. The Fpsr value is 
  /// stored as a single field but specific method can be used to access a 
  /// particular one. It stands in the application register bank at ar40.

  class Fpsr {
  public:
    /// the fpsr field
    enum t_mfield {
      TRAPS,  // Trap setting
      SF0,    // Main status field
      SF1,    // Alternate status field 1
      SF2,    // Alternate status field 2
      SF3     // Alternate status field 3
    };
    /// trap fields and status fields
    enum t_field {
      // trap field
      VD,     // Invalid operation floating-point exception trap
      DD,     // Denormal/Unormal operand floating-point exception trap
      ZD,     // Zero divide floating-point exception trap
      OD,     // Overflow floating-point exception trap
      UD,     // Underflow floating-point exception trap
      ID,     // Inexact floating-point exception trap
      // status field
      FTZ,    // Flush-to-zero mode
      WRE,    // Widest range exponent
      PC,     // Precision control
      RC,     // Rounding control
      TD,     // Traps disabled
      V,      // Invalid operation
      D,      // Denormal/Unnormal operand
      Z,      // Zero divide
      O,      // Overflow
      U,      // Underflow
      I       // Inexact
    };

  private:
    t_octa d_fpsr;

  public:
    /// create a default fpsr
    Fpsr (void);

    /// create a fpsr with a value
    /// @param value the fpsr value to set
    Fpsr (const t_octa value);

    /// copy construct this fpsr
    /// @param that the fpsr to copy
    Fpsr (const Fpsr& that);

    /// assign a fpsr to this one
    /// @param that the fpsr to assign
    Fpsr& operator = (const Fpsr& that);

    /// reset this fpsr
    void reset (void);

    /// set the fpsr by value
    /// @param value the value to set
    void setfpsr (const t_octa value);

    /// get the current fpsr value
    t_octa getfpsr (void) const;

    /// set the fpsr by mainfield, field and value
    /// @param mfld the main field
    /// @param fld the field
    /// @param value the value to set
    void setfld (const t_mfield mfld, const t_field fld, const bool value);

    /// set the fpsr by mainfield, field and value
    /// @param mfld the main field
    /// @param fld the field
    /// @param value the value to set
    void setfld (const t_mfield mfld, const t_field fld, const t_byte value);

    /// @return the fpsr bool field by mainfield, field
    bool getbfld (const t_mfield mfld, const t_field fld) const;

    /// @return the fpsr 2 bits field by mainfield, field
    t_byte getbsfld (const t_mfield mfld, const t_field fld) const;

    /// convert ia floating point register
    /// @param ipc   the instruction precision control completer
    /// @param field the floating point status field format
    /// @param val the real to convert
    void convert (const t_fpipc ipc, const t_mfield field, t_real& val);

    /// convert ia floating point register for parallel computing
    /// @param field the floating point status field format
    /// @param val1 the first real to convert
    /// @param val2 the second real to convert
    void pconvert (const t_mfield field, t_real& val1, t_real& val2);
  };
}

#endif
