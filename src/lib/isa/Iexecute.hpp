// ---------------------------------------------------------------------------
// - Iexecute.hpp                                                            -
// - iato:isa library - I execution unit class definition                    -
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

#ifndef  IATO_IEXECUTE_HPP
#define  IATO_IEXECUTE_HPP

#ifndef  IATO_AEXECUTE_HPP
#include "Aexecute.hpp"
#endif

namespace iato {

  /// The Iexecute class is the I execution unit. The class is derived from
  /// the A execution unit.

  class Iexecute : public Aexecute {
  public:
    /// execute this instruction with its operand
    /// @param inst the instruction to execute
    /// @param oprd the operand object
    Result exec (const Instr& inst, const Operand& oprd) const;
  };
}

#endif
