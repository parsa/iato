// ---------------------------------------------------------------------------
// - Env.hpp                                                                 -
// - iato:isa library - abstract execution environment class definition      -
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

#ifndef  IATO_ENV_HPP
#define  IATO_ENV_HPP

#ifndef  IATO_RESOURCE_HPP
#include "Resource.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Environment class is an abstract class that defines an execution
  /// environment. Such environment is used to hold the global processor
  /// resources. The environment is by itself dependant on the processor
  /// architecture. For example, an out-of-order architecture might have
  /// a physical register file, a reorder buffer and other things.

  class Env {
  private:
    /// the environment map
    map<string, Resource*> d_rmap;

  public:
    /// create a new environment
    Env (void);

    /// destroy this environment
    virtual ~Env (void);

    /// reset this environment
    virtual void reset (void);

    /// add a new resource to this environment
    /// @param res the resource to add
    virtual void add (Resource* res);

    /// get a resource by name
    virtual Resource* get (const string& name) const;

    /// report all resources
    virtual void report (void) const;

    /// set the resource stat collector
    /// @param stc the stat collector
    virtual void setstc (Stat* stc);

    /// set the resource tracer to all resources
    /// @param tracer the resource tracer to bind
    virtual void settrc (Tracer* tracer);

  private:
    // make the copy constructor private
    Env (const Env&);
    // make the assignment oerator private
    Env& operator = (const Env&);
  };
}

#endif
