// ---------------------------------------------------------------------------
// - Bexecute.hpp                                                            -
// - iato:isa library - B execution unit class definition                    -
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

#ifndef  IATO_BEXECUTE_HPP
#define  IATO_BEXECUTE_HPP

#ifndef  IATO_EXECUTABLE_HPP
#include "Executable.hpp"
#endif

namespace iato {

  /// The Bexecute class is the B execution unit. The class is responsible
  /// for all branch operations.

  class Bexecute : public Executable {
  public:
    /// execute this instruction with its operand
    /// @param inst the instruction to execute
    /// @param oprd the operand object
    Result exec (const Instr& inst, const Operand& oprd) const;
  };
}

#endif
