// ---------------------------------------------------------------------------
// - Lru.hpp                                                                 -
// - iato:isa library - least recently used matrix class definition          -
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

#ifndef  IATO_LRU_HPP
#define  IATO_LRU_HPP

#ifndef  IATO_TYPES_HPP
#include "Types.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Lru class implements the least recently used algorithm with a
  /// n-square matrix. When an update is made, the matrix is automatically
  /// updated. The lru index is return with the getlru method. The mru index
  /// is return with the getmru method.

  class Lru {
  private:
    /// the matrix size
    long d_size;
    /// the lru matrix
    bool** p_vlru;

  public:
    /// create a lru matrix by size
    Lru (const long size);

    /// destroy this lru matrix
    ~Lru (void);

    /// reset this matrix
    void reset (void);

    /// update an access by index
    /// @param index the lru matrix index
    void update (const long index);

    /// @return the lru index
    long getlru (void) const;

    /// @return the mru index
    long getmru (void) const;

  private:
    // make the copy constructor private
    Lru (const Lru&);
    // make the assignement operator private
    Lru& operator = (const Lru&);
  };
}

#endif
