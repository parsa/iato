// ---------------------------------------------------------------------------
// - Mob.hpp                                                                 -
// - iato:mac library - memory ordering buffer class definition              -
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

#ifndef  IATO_MOB_HPP
#define  IATO_MOB_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_MRT_HPP
#include "Mrt.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Mob class is the memory ordering buffer. It is a rotating resource 
  /// that maintain the load/store ordering. An entry is made with a valid bit,
  /// a size (aka an address mask) and an address. This mob does not hold the
  /// associated value due to the cache interference.

  class Mob : public Resource {
  private:
    /// the buffer size
    long d_size;
    /// the buffer array
    struct t_mobe* p_mobe;
    /// the alloc index 
    long d_aidx;

  public:
    /// create a new mob
    Mob (void);

    /// create a new mob with a context
    /// @param mtx the architectural context
    Mob (Mtx* mtx);

    /// destroy this mob
    ~Mob (void);

    /// reset this mob
    void reset (void);

    /// report this resource
    void report (void) const;

    /// @return true if the mob is empty
    bool isempty (void) const;

    /// @return true if the mob entry is valid
    bool isvalid (const long index) const;

    /// @return true if the mob entry has been cancelled
    bool iscancel (const long index) const;

    /// clear an mob entry by index
    void clear (const long index);

    /// allocate a new mob entry
    /// @param ildb the instruction load  flag
    /// @param istb the instruction store flag
    long alloc (const bool ildb, const bool istb);

    /// preset a load by index and mrt
    /// @param index the mob store index
    /// @param mrt the memory request type
    void preset (const long index, const Mrt& mrt);

    /// commit a store by index and mrt
    /// @param index the mob store index
    /// @param mrt the memory request type
    void update (const long index, const Mrt& mrt);

    /// process a memory request by index and mrt
    /// @param index the mob store index
    /// @param mrt the memory request type
    void process (const long index, const Mrt& mrt);

  private:
    // make the copy constructor private
    Mob (const Mob&);
    // make the assignment operator private
    Mob& operator = (const Mob&);
  };
}

#endif
