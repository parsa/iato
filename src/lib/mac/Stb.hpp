// ---------------------------------------------------------------------------
// - Stb.hpp                                                                 -
// - iato:mac library - store buffer class definition                        -
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

#ifndef  IATO_STB_HPP
#define  IATO_STB_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_MEMDEF_HPP
#include "MemDef.hpp"
#endif

#ifndef  IATO_MEMORY_HPP
#include "Memory.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Stb class is the store buffer. The store buffer is a queue of
  /// pair (address/value) and a size. Entries are added in order in the
  /// buffer. The store buffer is primarily used at the commit stage to
  /// perform the write operations.

  class Stb : public Resource {
  private:
    /// the buffer size
    long d_size;
    /// the buffer array
    struct t_stbe* p_stbe;
    /// the input push index
    long d_iptr;
    /// the output pop index
    long d_optr;
    /// the bypass memory
    Memory* p_bmem;

  public:
    /// create a new stb
    Stb (void);

    /// create a new stb with a context
    /// @param mtx the architectural context
    Stb (Mtx* mtx);

    /// destroy this stb
    ~Stb (void);

    /// reset this stb
    void reset (void);

    /// report this resource
    void report (void) const;

    /// bind a bypass memory to this buffer
    /// @param mem the bypass memory to bind
    void bind (Memory* mem);

    /// @return true if the stb is empty
    bool isempty (void) const;

    /// push a store request in the store buffer
    /// @param type the request type
    /// @param addr the request address
    /// @param data the request data
    void push (t_mreq type, const t_octa addr, const t_octa data);

    /// push a store request in the store buffer
    /// @param type the request type
    /// @param addr the request address
    /// @param data the request data
    void push (t_mreq type, const t_octa addr, const t_real& data);

  private:
    // make the copy constructor private
    Stb (const Stb&);
    // make the assignment operator private
    Stb& operator = (const Stb&);
  };
}

#endif
