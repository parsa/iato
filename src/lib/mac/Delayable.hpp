// ---------------------------------------------------------------------------
// - Delayable.hpp                                                           -
// - iato:mac library - abstract delayable object class definition           -
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

#ifndef  IATO_DELAYABLE_HPP
#define  IATO_DELAYABLE_HPP

#ifndef  IATO_RUNNABLE_HPP
#include "Runnable.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Delayable class is a special runnable class that provides latency
  /// controlled runnable method. Upon a call to the run method, a latency
  /// counter is decreased until reaching 0. At that point the activate method
  /// is called and the counter is resetted. Unlike the runnable method, the
  /// delayable class can be used for interface  that operates at different
  /// frequency that the main one, or that needs to provides delayed
  /// operation.

  class Delayable : public Runnable {
  protected:
    /// the delay latency
    long d_dlat;
    /// the delay counter
    long d_dcnt;

  public:
    /// create a default delayable
    Delayable (void);

    /// create a new delayable by name
    /// @param name the delayable resource name
    Delayable (const string& name);

    /// reset this delayable interface
    void reset (void);

    /// flush this delayable interface
    void flush (void);

    /// run this delayable object
    void run (void);

    /// activate this delayable object
    virtual void activate (void) =0;

    /// set the delayable latency
    /// @param dlat the delayable latency
    virtual void setdlat (const long dlat);

    /// @return the delayable latency
    virtual long getdlat (void) const;
  };
}

#endif
