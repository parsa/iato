// ---------------------------------------------------------------------------
// - Renamer.hpp                                                             -
// - iato:ecu library - rse renaming class definition                        -
// ---------------------------------------------------------------------------
// - (c) inria 2002-2004                                                     -
// ---------------------------------------------------------------------------
// - authors                                      Amaury Darsch    2002:2004 -
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

#ifndef  IATO_RENAMER_HPP
#define  IATO_RENAMER_HPP

#ifndef  IATO_UTX_HPP
#include "Utx.hpp"
#endif

#ifndef  IATO_RSE_HPP
#include "Rse.hpp"
#endif

#ifndef  IATO_MEMORY_HPP
#include "Memory.hpp"
#endif

#ifndef  IATO_REGISTER_HPP
#include "Register.hpp"
#endif

namespace iato {

  /// The Renamer class is the rse implementation with direct memory access.
  /// The class can be built with a context and the memory is bind to the
  /// object at run time. The register bank is also used here to access the
  /// bsp and bspstore values. The renamer class is designed to operate with
  /// an emulation environment.

  class Renamer : public Rse {
  private:
    /// the register bank
    Register* p_rbk;
    /// the backing store memory
    Memory*   p_mem;
    
  public:
    /// create a default renamer
    Renamer (void);

    /// create a renamer with a context
    /// @param utx the utility context
    Renamer (Utx* utx);

    /// bind the memory and the register bank to this rse
    /// @param mem the memory to bind
    /// @param rbk the register bank
    void bind (Memory* mem, Register* rbk);

    /// preset the rse state with an instruction
    /// @param inst the instruction used to preset
    void preset (const Instr& inst);

    /// after set the rse state with an instruction
    /// @param inst the instruction used to preset
    void aftset (const Instr& inst);

    /// update the rse state with a result
    /// @param resl the result used to update
    void update (const Result& resl);

  private:
    // make the copy constructor private
    Renamer (const Renamer&);
    // make the assignement operqtor private
    Renamer& operator =  (const Renamer&);
  };
}

#endif
