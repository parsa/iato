// ---------------------------------------------------------------------------
// - Resource.hpp                                                            -
// - iato:isa library - abstract resource class definition                   -
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

#ifndef  IATO_RESOURCE_HPP
#define  IATO_RESOURCE_HPP

#ifndef  IATO_STAT_HPP
#include "Stat.hpp"
#endif

#ifndef  IATO_TRACER_HPP
#include "Tracer.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Resource class is an abstract class that is used by the environment
  /// for reference purpose. A resource is a simple unit that can be shared
  /// by others. A resource is defined by name and is generally installed
  /// in a global environment. The environment can be later queried by name
  /// to retreive a particular resource.

  class Resource {
  protected:
    /// the resource name
    string  d_name;
    /// the resource stat
    Stat*   p_stat;
    /// the resource tracer
    Tracer* p_tracer;

  public:

    /// create a default resource
    Resource (void);

    /// create a new resource by name
    /// @param name the reource name
    Resource (const string& name);
    
    /// copy construct this resource
    /// @param that the resource to copy
    Resource (const Resource& that);
   
    /// destroy this resource
    virtual ~Resource (void);

    /// assign a resource to this one
    /// @param that the resource to assign
    Resource& operator = (const Resource& that);

    /// reset this resource
    virtual void reset (void) =0;

    /// report some resource information
    virtual void report (void) const;

    /// set the resource name
    /// @param name the resource name to set
    virtual void setname (const string& name);

    /// @return the resource name
    virtual string getname (void) const;

    /// set the resource stat collector
    /// @param stc the stat collector
    virtual void setstc (Stat* stc);

    /// set the resource tracer
    /// @param tracer the resource tracer
    virtual void settrc (Tracer* tracer);
  };
}

#endif
