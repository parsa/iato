// ---------------------------------------------------------------------------
// - Ip.hpp                                                                  -
// - iato:isa library - instruction pointer resource class definition        -
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

#ifndef  IATO_IP_HPP
#define  IATO_IP_HPP

#ifndef  IATO_TYPES_HPP
#include "Types.hpp"
#endif

namespace iato {
  
  /// The Ip is the instruction pointer resource. It is built as a resource.
  /// The sole purpose of defining the IP as a resource is to unify all
  /// special registers.

  class Ip {
  private:
    /// the instruction pointer
    t_octa d_ip;

  public:
    /// create a default ip
    Ip (void);

    /// create an ip with a value
    /// @param value the ip value to set
    Ip (const t_octa value);
 
    /// create an ip with a value and a displacement
    /// @param value the ip value to set
    /// @param disp  the displacement to set
    Ip (const t_octa value, const t_octa disp);

    /// copy construct this ip
    /// @param that the ip copy
    Ip (const Ip& that);

    /// assign an ip to this one
    /// @param that the ip to assign
    Ip& operator = (const Ip& that);

    /// increase that ip by one bundle (prefix)
    Ip& operator ++ (void);

    /// increase that ip by one bundle (postfix)
    Ip operator ++ (int);

    /// reset this ip
    void reset (void);

    /// set the ip value
    /// @param ip the ip to set
    void setip (const t_octa ip);

    /// @return the resource ip
    t_octa getip (void) const;
  };
}

#endif
