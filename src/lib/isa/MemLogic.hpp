// ---------------------------------------------------------------------------
// - MemLogic.hpp                                                            -
// - iato:isa library - memory and alat logic class definition               -
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

#ifndef  IATO_MEMLOGIC_HPP
#define  IATO_MEMLOGIC_HPP

#ifndef  IATO_ALAT_HPP
#include "Alat.hpp"
#endif

#ifndef  IATO_RESULT_HPP
#include "Result.hpp"
#endif

#ifndef  IATO_TRACER_HPP
#include "Tracer.hpp"
#endif

#ifndef  IATO_MEMORY_HPP
#include "Memory.hpp"
#endif

namespace iato {

  /// The MemLogic class is a class that handle the interferences between 
  /// the memory and the alat. The class is updated from a result object.
  /// The ALAT is a member of the class while the memory needs to be binded.

  class MemLogic : public Resource {
  private:
    /// the memlogic alat
    Alat*   p_alat;
    /// the memlogic memory
    Memory* p_mem;

  public:
    /// create a default memory logic
    MemLogic (void);

    /// create a memory logic with a context
    /// @param ctx the current context
    MemLogic (Ctx* ctx);

    /// destroy this memory logic
    ~MemLogic (void);

    /// reset this memlogic
    void reset (void);

    /// bind a memory to the logic
    /// @param mem the memory to bind
    void bind (Memory* mem);

    /// update the memory, the alat and a result
    /// @param resl the result to update
    void update (Result& resl);

  private:
    // make the copy constructor private
    MemLogic (const MemLogic&);
    // make the assignment operator private
    MemLogic& operator = (const MemLogic&);
  };
}

#endif
