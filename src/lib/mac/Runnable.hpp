// ---------------------------------------------------------------------------
// - Runnable.hpp                                                            -
// - iato:mac library - abstract runnable object class definition            -
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

#ifndef  IATO_RUNNABLE_HPP
#define  IATO_RUNNABLE_HPP

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Runnable class is an abstract class that defines the "run" and
  /// "flush" methods. The run method executes virtually one cycle of
  /// the runnbale object. The runnable interface can be reset and flushed.
  /// The difference is interface dependent.

  class Runnable : public Resource {
  public:
    /// create a default runnable
    Runnable (void);

    /// create a new runnable by name
    /// @param name the runnable resource name
    Runnable (const string& name);

    /// flush this runnable interface
    virtual void flush (void) =0;

    /// run this runnable object
    virtual void run (void) =0;
  };
}

#endif
