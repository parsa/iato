// ---------------------------------------------------------------------------
// - Ctx.hpp                                                                 -
// - iato:isa library - object context class definition                      -
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

#ifndef  IATO_CTX_HPP
#define  IATO_CTX_HPP

#ifndef  IATO_TYPES_HPP
#include "Types.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Ctx class is a context class that is used to hold the architectural 
  /// parameters. The default parameters for an architecture are defined
  /// at construction. They can be changed globally or individually. Most
  /// of the parameters can also be defined with a configuration file. The
  /// query is done by name.

  class Ctx {
  protected:
    /// the boolean map
    map<string,bool>        d_bmap;
    /// the long map
    map<string,long>        d_lmap;
    /// the 64 bits long map
    map<string,t_long>      d_hmap;
    /// the real map
    map<string,long double> d_rmap;
    /// the string map
    map<string,string>      d_smap;

  public:
    /// create a new context
    Ctx (void);

    /// destroy this context
    virtual ~Ctx (void);

    /// reset this context
    virtual void reset (void);

    /// parse a string and update the context
    /// @param s the string to parse
    void parse (const string& s);

    /// update this context with a vector of parameters
    /// @param cprm the vector of parameters
    virtual void parse (const vector<string>& cprm);

    /// dump this context to the standard output
    void dump (void) const;

    /// update this context with a particular architecture
    /// @param arch the architecture used for the update
    virtual void update (const t_arch arch);
    
    /// set a boolean parameter value
    /// @param what the parameter to set
    /// @param value the parameter value
    virtual void setbool (const string& what, const bool value);

    /// return a boolean parameter value
    /// @param what the parameter to query
    virtual bool getbool (const string& what) const;

    /// set a long parameter value
    /// @param what the parameter to set
    /// @param value the parameter value
    virtual void setlong (const string& what, const long value);

    /// return a long parameter value
    /// @param what the parameter to query
    virtual long getlong (const string& what) const;

    /// set a long long parameter value
    /// @param what the parameter to set
    /// @param value the parameter value
    virtual void setllong (const string& what, const t_long value);

    /// return a long long parameter value
    /// @param what the parameter to query
    virtual t_long getllong (const string& what) const;

    /// set a real parameter value
    /// @param what the parameter to set
    /// @param value the parameter value
    virtual void setreal (const string& what, const long double value);

    /// return a real parameter value
    /// @param what the parameter to query
    virtual long double getreal (const string& what) const;

    /// set a string parameter value
    /// @param what the parameter to set
    /// @param value the parameter value
    virtual void setstr (const string& what, const string& value);

    /// return a string parameter value
    /// @param what the parameter to query
    virtual string getstr (const string& what) const;

    /// @return the issue width size in bundles
    virtual long getiwsz (void) const;

    /// @return the issue width size in slots
    virtual long getswsz (void) const;

    /// @return the issue width size in bytes
    virtual long getbwsz (void) const;

  private:
    // make the copy constructor private
    Ctx (const Ctx&);
    // make the assignment operator private
    Ctx& operator = (const Ctx&);
  };
}

#endif
