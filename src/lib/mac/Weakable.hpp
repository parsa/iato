// ---------------------------------------------------------------------------
// - Weakable.hpp                                                            -
// - iato:mac library - abstract weakable object class definition            -
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

#ifndef  IATO_WEAKABLE_HPP
#define  IATO_WEAKABLE_HPP

#ifndef  IATO_DELAYABLE_HPP
#include "Delayable.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Weakable class is an abstract class that defines the "notify"
  /// method. The 'notify' method is used by an object to request an
  /// immediate action. Most likely, the notify method will invoke the
  /// activate method, although the notify method has an argument that
  /// is the caller.

  class Weakable : public Delayable {
  public:
    /// create a default weakable
    Weakable (void);

    /// create a new weakable by name
    /// @param name the weakable resource name
    Weakable (const string& name);

    /// notify this weakable interface
    /// @param res the resource notifier
    virtual void notify (Resource* res) =0;
  };
}

#endif
