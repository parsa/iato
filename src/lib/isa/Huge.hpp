// ---------------------------------------------------------------------------
// - Huge.hpp                                                                -
// - iato:isa library - 128 bits huge unsigned integer type class definition -
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

#ifndef  IATO_HUGE_HPP
#define  IATO_HUGE_HPP

#ifndef  IATO_CONFIG_HXX
#include "Config.hxx"
#endif

namespace iato {

  /// The t_huge class is the implementation of a 128 bits integer
  /// representation. Since that type is not necessarely available in 
  /// almost all target machine, it has to be emulated. This class is 
  /// a minimal class that is supposed to be used internally, to perform
  /// some precise computation. Tyically, it is used to support the xmpy and
  /// xma instructions. The class can be constructed with normal octa value 
  /// or with two octa value to full define it.
  /// the lowest and highest representation can be obtained with the 'getlow'
  /// and 'gethigh' methods.

  class t_huge {
  public:
    /// the huge size
    static const long HUGE_SIZE = 16;

  private:
    /// the 128 bit representation
    t_byte d_hval[HUGE_SIZE];

  public:
    /// create a default huge number
    t_huge (void);

    /// create a huge number with an octa
    /// @param value the value to use
    t_huge (const t_octa value);

    /// create a huge number with an two octa
    /// @param hip the high part to assign
    /// @param lop the low part to assign
    t_huge (const t_octa hip, const t_octa lop);

    /// copy construct a huge with a huge
    /// @param that the huge value to copy
    t_huge (const t_huge& that);

    /// assign an octa value to this huge
    /// @param value the value to assign
    t_huge& operator = (const t_octa value);

    /// assign a huge to this one
    /// @param that the huge to assign
    t_huge& operator = (const t_huge& that);
    
    /// add one huge with another one
    /// @param x the huge operand
    t_huge operator + (const t_huge& x) const;

    /// multiply one huge with another one
    /// @param x the huge operand
    t_huge operator * (const t_huge& x) const;

    /// shift right one huge
    /// @param shc the shift count value
    t_huge operator >> (const t_octa shc) const;

    /// set high 64 bits of a huge
    /// @param value the high part value
    void sethigh (const t_octa value);

    /// @return the lowest part of this huge number
    t_octa getlow (void) const;
    
    /// @return the highest part of this huge number
    t_octa gethigh (void) const;
  };
}

#endif
