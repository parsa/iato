// ---------------------------------------------------------------------------
// - Executable.hpp                                                          -
// - iato:isa library - executable abstract class definition                 -
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

#ifndef  IATO_EXECUTABLE_HPP
#define  IATO_EXECUTABLE_HPP

#ifndef  IATO_INSTR_HPP
#include "Instr.hpp"
#endif

namespace iato {

  /// The Executable class is an abstract class for various execution unit. 
  /// The class defines an exec method that uses an operand and produces
  /// a result. The instruction to execute is also used as an argument
  /// to the exec method.

  class Executable {
  public:
    /// execute this instruction with its operand
    /// @param inst the instruction to execute
    /// @param oprd the operand object
    virtual Result exec (const Instr& inst, const Operand& oprd) const =0;
  };
}

#endif
