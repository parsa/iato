// ---------------------------------------------------------------------------
// - Bdb.hpp                                                                 -
// - iato:mac library - bundle decoupling buffer class definition            -
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

#ifndef  IATO_BDB_HPP
#define  IATO_BDB_HPP

#ifndef  IATO_MTX_HPP
#include "Mtx.hpp"
#endif

#ifndef  IATO_BUNDLE_HPP
#include "Bundle.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

namespace iato {

  /// The Bdb class is a bundle decoupling buffer. The Bdb is used primarily
  /// within an in-order machine to accumulate bundle and rotate them. Bundle
  /// are pushed on a line basis and consumed on a bundle basis. A threshold
  /// that represents the buffer size minus one line indicates that the
  /// decoupling buffer is almost full.

  class Bdb : public Resource {
  private:
    /// the buffer size
    long d_dbsz;
    /// the window size
    long d_iwsz;
    /// the bdb size
    long d_size;
    /// the bdb threshold
    long d_bthr;
    /// the bdb length
    long d_blen;
    /// the bundle array
    Bundle* p_bndl;
    
  public:
    /// create a default bdb
    Bdb (void);

    /// create a bdb by size
    /// @param size the bdb size
    Bdb (const long size);

    /// create a bdb by context
    /// @param mtx the architectural context
    Bdb (Mtx* mtx);

    /// destroy this bdb
    ~Bdb (void);

    /// reset this bdb
    void reset (void);

    /// report this resource
    void report (void) const;

    /// @return the bdb size
    long getsize (void) const;

    /// @return true if the buffer is empty
    bool isempty (void) const;

    /// @return true if the buffer is full
    bool isfull (void) const;

    /// @return true if the buffer is at the threshold
    bool isthr (void) const;

    /// push a bundle in the buffer
    /// @param bndl the bundle to push
    void push (const Bundle& bndl);

    /// push back a bundle in the buffer
    /// @param bndl the bundle to push back
    void back (const Bundle& bndl);

    /// @return a bundle from the buffer
    Bundle pop (void);
    
  private:
    // make the copy constructor private
    Bdb (const Bdb&);
    // make the assignment operator private
    Bdb& operator = (const Bdb&);
  };
}

#endif
