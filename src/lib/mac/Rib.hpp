// ---------------------------------------------------------------------------
// - Rib.hpp                                                                 -
// - iato:mac library - replay identification buffer class definition        -
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

#ifndef  IATO_RIB_HPP
#define  IATO_RIB_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_DSI_HPP
#include "Dsi.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Rib class is the replay identification buffer. The class acts as a 
  /// buffer that holds rid that can be use to mark instructions that needs
  /// to be replayed. The buffer holds a flags that permits to detect if an
  /// overflow occured in order to force a pipe flush.

  class Rib : public Resource {
  private:
    /// the rib size
    long d_size;
    /// the rid buffer
    Rid* p_rids;
    /// the overflow flag
    bool d_over;

  public:
    /// create a new rib
    Rib (void);

    /// create a new rib with a context
    /// @param mtx the architectural context
    Rib (Mtx* mtx);

    /// destroy this rib
    ~Rib (void);

    /// reset this rib
    void reset (void);

    /// report this resource
    void report (void) const;

    /// @return true if the rib is empty
    bool isempty (void) const;

    /// @return true if the irb entry is valid
    bool isvalid (const long index) const;

    /// clear an rib entry by idex
    void clear (const long index);

    /// @return true if an rid exists in this buffer
    bool exists (const Rid& rid) const;

    /// add a new rid in the dependency list
    /// @param rid the rid to add
    void add (const Rid& rid);

    /// add an instruction dependency
    /// @param inst the instruction to add
    void add (const Dsi& inst);

    /// @return if an instruction match a dependency
    bool isdep (const Dsi& dsi) const;

  private:
    // make the copy constructor private
    Rib (const Rib&);
    // make the assignment operator private
    Rib& operator = (const Rib&);
  };
}

#endif
