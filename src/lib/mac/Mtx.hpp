// ---------------------------------------------------------------------------
// - Mtx.hpp                                                                 -
// - iato:mac library - micro architecture context class definition          -
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

#ifndef  IATO_MTX_HPP
#define  IATO_MTX_HPP

#ifndef  IATO_ETX_HPP
#include "Etx.hpp"
#endif

namespace iato {

  /// The Mtx class is the micro-architecture context class. This class
  /// is derived from the isa context class and provides additional
  /// parameters that are used by the micro-architectural implementation.
  
  class Mtx : public Etx {
  public:
    /// create a new context
    Mtx (void);

    /// reset this context
    void reset (void);

    /// update this contect with a particular archietcture
    /// @param arch the archietcture used for update
    void update (const t_arch arch);

    /// @return the system bus size
    long getsbsz (void) const;

    /// @return the system bus latency
    long getsblt (void) const;

    /// @return the total number of units
    long gettusz (void) const;

  private:
    // make the copy constructor private
    Mtx (const Mtx&);
    // make the assignment operator private
    Mtx& operator = (const Mtx&);
  };
}

#endif

