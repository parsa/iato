// ---------------------------------------------------------------------------
// - Mbn.hpp                                                                 -
// - iato:mac library - memory bypass network class definition               -
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

#ifndef  IATO_MBN_HPP
#define  IATO_MBN_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_MBE_HPP
#include "Mbe.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Mbn class is the memory bypass network. It is a collection of 
  /// memory bypass elements. Each element is allocated by a stage that 
  /// needs store forwarding. The bypass network can be used directly to 
  /// evaluate a load value.

  class Mbn : public Resource {
  private:
    /// the vector of mbe
    vector<Mbe*> d_vmbe;

  public:
    /// create an empty network
    Mbn (void);

    /// create an empty network by context
    /// @param mtx the architectural context
    Mbn (Mtx* mtx);

    /// create an empty network by context and name
    /// @param mtx the architectural context
    /// @param name the resource name
    Mbn (Mtx* mtx, const string& name);

    /// reset this network
    void reset (void);

    /// report this resource
    void report (void) const;

    /// add a memory element
    /// @param mbe the element to add
    void add (Mbe* mbe);

    /// update an mrt with the bypass data
    /// @param mrt the mrt to update
    /// @param mix the maximum index
    Mrt update (Mrt& mrt, const long mix) const;

  private:
    // make the copy constructor private
    Mbn (const Mbn&);
    // make the assignment operator private
    Mbn& operator = (const Mbn&);
  };
}

#endif
