// ---------------------------------------------------------------------------
// - Printing.hpp                                                            -
// - iato:iata client - trace printing class definition                      -
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

#ifndef  IATO_PRINTING_HPP
#define  IATO_PRINTING_HPP

#ifndef  IATO_ATX_HPP
#include "Atx.hpp"
#endif

#ifndef  IATO_TRACER_HPP
#include "Tracer.hpp"
#endif

#ifndef  IATO_FILTERING_HPP
#include "Filtering.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Printing class is a simple class that handle the trace printing.
  /// It operates with a file name, and creates trace reader object.

  class Printing {
  private:
    /// the trace reader
    Tracer::Reader* p_reader;
    /// the filter
    Filtering*      p_filter;

  public:
    /// create a new printing object by name and context
    /// @param name the trace file name
    /// @param atx  the analysis context
    Printing (const string& name, Atx* atx);

    /// destroy this printing object
    ~Printing (void);

    /// print all traces
    void print (void) const;

  private:
    // make the copy constructor private
    Printing (const Printing&);
    // make the assignment operator private
    Printing& operator = (const Printing&);
  };
}

#endif
