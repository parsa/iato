// ---------------------------------------------------------------------------
// - Mbe.hpp                                                                 -
// - iato:mac library - memory bypass element class definition               -
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

#ifndef  IATO_MBE_HPP
#define  IATO_MBE_HPP

#ifndef  IATO_MRT_HPP
#include "Mrt.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Mbe class is a memory bypass element. The element is built
  /// primarily with a memory request object (mrt). An element index 
  /// and an ordering flag is used to decide whether or not the element
  /// can be used in a bypass network; in order to maintain the store/load
  /// ordering.

  class Mbe {
  private:
    /// the element index
    long d_eix;
    /// use element ordering
    bool d_ueo;
    /// the element mrt
    Mrt  d_mrt;

  public:
    /// create a default memory element
    Mbe (void);

    /// create a memory element by index and ordering flag
    /// @param eix the element index
    /// @param ueo the use element ordering flag
    Mbe (const long eix, const bool ueo);

    /// reset this memory element
    void reset (void);

    /// set the element index
    /// @param eix the element index
    void seteix (const long eix);

    /// @return the element index
    long geteix (void) const;

    /// set the element ordering flag
    /// @param ueo the use element ordering flag
    void setueo (const bool ueo);

    /// @return the element ordering flag
    bool getueo (void) const;

    /// set a memory element by mrt
    /// @param mrt the memory request type
    void setmrt (const Mrt& mrt);

    /// @return the element mrt
    Mrt getmrt (void) const;

    /// @return the element mrt and reset
    Mrt grabmrt (void);

  private:
    // make the copy constructor private
    Mbe (const Mbe&);
    // make the assignement operator private
    Mbe& operator =(const Mbe&);
  };
}

#endif
