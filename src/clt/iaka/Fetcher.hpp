// ---------------------------------------------------------------------------
// - Fetcher.hpp                                                             -
// - iato:iaka client - instruction fetch class definition                   -
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

#ifndef  IATO_FETCHER_HPP
#define  IATO_FETCHER_HPP

#ifndef  IATO_ATX_HPP
#include "Atx.hpp"
#endif

#ifndef  IATO_BUNDLE_HPP
#include "Bundle.hpp"
#endif

#ifndef  IATO_MEMORY_HPP
#include "Memory.hpp"
#endif

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Fetcher class is a base class used to accumulate bundles prior their
  /// dispersion. The number of bundles that can be processed depends on the
  /// issue width in disperse mode or is one by default.

  class Fetcher : public Resource {
  private:
    /// the disperse flag
    bool d_dflg;
    /// the issue width
    long d_iwsz;
    /// the bundle array
    Bundle* p_bndl;
    /// the instruction memory
    Memory* p_imem;

  public:
    /// create a default fetcher
    Fetcher (void);
    
    /// create a fetcher by context
    /// @param atx the application context
    Fetcher (Atx* atx);

    /// destroy this fetcher
    ~Fetcher (void);

    /// reset this fetcher
    void reset (void);

    /// bind the instruction memory
    /// @param imem the instruction memory to bind
    void bind (Memory* imem);

    /// @return true if no bundle is available
    bool isempty (void) const;

    /// fill the fetch buffer at a certain ip
    /// @param ip the ip used for fetching
    void fill (const t_octa ip);

    /// pack the tech buffer
    void pack (void);

    /// @return the next available bundle
    Bundle getbndl (void);

  private:
    // make the copy constructor private
    Fetcher (const Fetcher&);
    // make the assignement operqtor private
    Fetcher& operator =  (const Fetcher&);
  };
}

#endif
