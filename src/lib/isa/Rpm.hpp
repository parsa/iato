// ---------------------------------------------------------------------------
// - Rpm.hpp                                                                 -
// - iato:isa library - rid pair mapping class definition                    -
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

#ifndef  IATO_RPM_HPP
#define  IATO_RPM_HPP

#ifndef  IATO_RID_HPP
#include "Rid.hpp"
#endif

#ifndef  IATO_UVR_HPP
#include "Uvr.hpp"
#endif

namespace iato {

  /// The Rpm class is a rid pair mapping class that is to map register
  /// that can eventually generates RAW or WAW legal violation. For example,
  /// the br.ret instruction can read register b0 within the same instruction
  /// group. Such case is a RAW violation but is allowed by the ISA. Therefore,
  /// the Rpm object map a branch register with the ip register.

  class Rpm {
  public:
    /// the register mapping function
    typedef Uvr (*t_rfm) (const Uvr&);
    
  private:
    /// the source register
    Rid   d_rsrc;
    /// the target register
    Rid   d_rdst;
    /// the mapping function
    t_rfm p_fmap;

  public:
    /// create a default rpm
    Rpm (void);

    /// create a rpm with a source and destination rid
    /// @param src the source rid
    /// @param dst the destination rid
    Rpm (const Rid& src, const Rid& dst);

    /// copy construct this rpm
    /// @param that the rpm to copy
    Rpm (const Rpm& that);

    /// assign a rpm to this one
    /// @param that the rpm to assign
    Rpm& operator = (const Rpm& that);

    /// reset this rpm
    void reset (void);

    /// @return true if the rpm is valid
    bool isvalid (void) const;

    /// set the rpm mapping by source and destination
    /// @param src the source rid
    /// @param dst the destination rid
    void setmap (const Rid& src, const Rid& dst);

    /// set the rpm mapping by source, destination and function mapping
    /// @param src the source rid
    /// @param dst the destination rid
    /// @param rfm the rpm function mapping
    void setmap (const Rid& src, const Rid& dst, t_rfm rfm);

    /// @return the source rid
    Rid getsrc (void) const;

    /// @return the destination rid
    Rid getdst (void) const;

    /// @return the mapping function
    t_rfm getrfm (void) const;
  };
}

#endif
