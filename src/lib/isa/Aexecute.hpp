// ---------------------------------------------------------------------------
// - Aexecute.hpp                                                            -
// - iato:isa library - A execution unit class definition                    -
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

#ifndef  IATO_AEXECUTE_HPP
#define  IATO_AEXECUTE_HPP

#ifndef  IATO_EXECUTABLE_HPP
#include "Executable.hpp"
#endif

namespace iato {

  /// The Aexecute is the A execution unit. This unit is shared by both
  /// M and I units. Normally, this class should not be used directly.

  class Aexecute : public Executable {
  public:
    /// execute this instruction with its operand
    /// @param inst the instruction to execute
    /// @param oprd the operand object
    Result exec (const Instr& inst, const Operand& oprd) const;
  };
}

#endif
