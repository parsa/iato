// ---------------------------------------------------------------------------
// - Mapper.hpp                                                              -
// - iato:mac library - memory mapper class definition                       -
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

#ifndef  IATO_MAPPER_HPP
#define  IATO_MAPPER_HPP

#ifndef  IATO_UTX_HPP
#include "Utx.hpp"
#endif

#ifndef  IATO_MEMORY_HPP
#include "Memory.hpp"
#endif

#ifndef  IATO_RESULT_HPP
#include "Result.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

namespace iato {

  /// The Mapper class is a memory mapper class that adapts a memory
  /// transaction with a result object. Additionally, the adapter is 
  /// designed respond to alat request.

  class Mapper : public Resource {
  public:
    /// the mapper memory types
    enum t_tmem {
      DMEM, // data memory
      FMEM, // floating point memory
      BMEM  // backing-store memory
    };

  private:
    /// the data memory
    Memory* p_dmem;
    /// the floating point memory
    Memory* p_fmem;
    /// the backing store memory
    Memory* p_bmem;

  public:
    /// create a default mapper
    Mapper (void);

    /// create a mapper with a context
    /// @param mtx the architectural context
    Mapper (Mtx* mtx);

    /// create a mapper with a context and a name
    /// @param mtx the architectural context
    /// @param name the resource name
    Mapper (Mtx* mtx, const string& name);

    /// reset this mapper
    void reset (void);

    /// bind a memory to the mapper
    /// @param mem the memory to bind
    void bind (Memory* mem);

    /// bind a memory to the mapper by type
    /// @param type the memory type to bind
    /// @param mem the memory to bind
    void bind (t_tmem type, Memory* mem);

    /// process a memory request
    /// @param mrt the memory request to process
    void process (Mrt& mrt);

    /// update a result with a memory request
    /// @param result the result to update
    void update (Result& result);
  
  private:
    // make the copy constructor private
    Mapper (const Mapper&);
    // make the assignment operator private
    Mapper& operator = (const Mapper&);
  };
}

#endif
