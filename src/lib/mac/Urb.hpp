// ---------------------------------------------------------------------------
// - Urb.hpp                                                                 -
// - iato:mac library - universal register bank class definition             -
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

#ifndef  IATO_URB_HPP
#define  IATO_URB_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_UVR_HPP
#include "Uvr.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Urb class is the universal register bank class definition. An urb
  /// entry holds a uvr value and a valid bit. The urb is used to store
  /// in-flight instruction register result before the value is commited
  /// into the logical register file. The size of the urb is an architectural
  /// parameter.

  class Urb : public Resource {
  private:
    /// the bank size
    long d_size;
    /// urb bank
    struct t_urb* p_vurb;

  public:
    /// create a default universal bank
    Urb (void);

    /// create a universal bank by context
    /// @param mtx the architectural context
    Urb (Mtx* mtx);

    /// create a universal bank by context and name
    /// @param mtx the architectural context
    /// @param name the rat resource name
    Urb (Mtx* mtx, const string& name);

    /// destroy this bank
    ~Urb (void);

    /// reset this bank
    void reset (void);

    /// report this resource
    void report (void) const;

    /// set the urb entry by index and value
    /// @param rnum the register number
    /// @param uvr  the uvr value to set
    void setuvr (const long rnum, const Uvr& uvr);

    /// @return the uvr value by index
    Uvr getuvr (const long rnum) const;

    /// @return the index of a new allocated urb entry
    long alloc (void);

    /// clean an urb entry
    /// param rnum the register to clean
    void clean (const long rnum);

  private:
    // make the copy constructor private
    Urb (const Urb&);
    // make the assignement operator private
    Urb& operator =(const Urb&);
  };
}

#endif
