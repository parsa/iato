// ---------------------------------------------------------------------------
// - Rid.hpp                                                                 -
// - iato:isa library - register number class definition                     -
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

#ifndef  IATO_RID_HPP
#define  IATO_RID_HPP

#ifndef  IATO_TYPES_HPP
#include "Types.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Rid class is a simple class that binds a logical register
  /// number with a physical register number. The type of register
  /// and a valid bit is also part of this class. The rid class is
  /// primarily used used in the instruction class during decoding.
  /// The logical number is thre register number as specified by the
  /// instruction encoding. The physical number is a register number
  /// used by any renaming logic. By default the physical number
  /// equals the logical number. For example, the physical number
  /// can be used by the rse to rename some stacked or rotating 
  /// register. Associated with the physical number is a virtual register
  /// bit. If set, the bit indicates that the physical register number is
  /// is a virtual register that implies a kind of indirection before
  /// matching the real physical register number. Implementation that do
  /// not perform physical register renaming, do not need this bit.
  /// The gettnum method return the virtual number if the virtual bit
  /// is set or the physical number.
  /// CAUTION: the isready method returns true if the rid is not valid
  /// or the register does not have to be renamed, otherwise it return
  /// the ready bit.

  class Rid {
  protected:
    /// the valid bit
    bool d_valid;
    /// the register type
    t_lreg d_type;
    /// the logical register number
    long d_lnum;
    /// the physical register number
    long d_pnum;
    /// the virtual register bit
    bool d_vbit;
    /// the virtual register number
    long d_vnum;
    /// the old virtual number
    long d_onum;
    /// the eval ready bit
    bool d_erdy;

  public:
    /// create register number
    Rid (void);

    /// copy construct a register number
    /// @param that the register to copy
    Rid (const Rid& that);

    /// assign a register number to this one
    /// @param that the register to assign
    Rid& operator = (const Rid& that);

    /// @return true if two rid are equals
    bool operator == (const Rid& rid) const;

    /// reset this register number
    void reset (void);

    /// @return true if the register is valid
    bool isvalid (void) const {return d_valid;}

    /// @return true if the rid is a valid predicate
    bool ispred (void) const;

    /// set the eval ready bit
    /// @param erdy the eval ready bit to set
    void seterdy (const bool erdy);

    /// set the eval ready bit if both rid are equal
    /// @param rid the rid to compare
    void seterdy (const Rid& rid);

    /// @return the eval ready bit
    bool geterdy (void) const {return d_erdy;}

    /// clear the virtual bit
    void clrvbit (void);

    /// clear the virtual bit by rid
    /// @param rid the rid to compare
    void clrvbit (const Rid& rid);

    /// @return true if the rid must be logically renamed
    bool islrnm (void) const;

    /// @return true if the rid is ready, invalid rid returns true
    bool isready (void) const;

    /// return the register type
    t_lreg gettype (void) const {return d_type;}

    /// return the logical register number
    long getlnum (void) const {return d_lnum;}

    /// return the physical  register number
    long getpnum (void) const {return d_pnum;}

    /// @return the register virtual bit
    bool getvbit (void) const {return d_vbit;}

    /// @return the register virtual number
    long getvnum (void) const {return d_vnum;}

    /// @return the old register virtual number
    long getonum (void) const {return d_onum;}

    /// @return the target register number
    long gettnum (void) const {return d_vbit ? d_vnum : d_pnum;}

    /// set the logical number by type
    /// @param type the register type
    /// @param lnum the logical register number
    void setlnum (t_lreg type, const long lnum);

    /// set the physical register number
    /// @param pnum the physical register number
    void setpnum (const long pnum) {d_pnum = pnum;}

    /// set the virtual register number and virtual bit
    /// @param vnum the virtual register number
    void setvnum (const long vnum);

    /// set the virtual register number and virtual bit
    /// @param vnum the virtual register number
    /// @param onum the old virtual number
    void setvnum (const long vnum, const long onum);

    /// set a register by type and number
    /// @param type the register type
    /// @param lnum the logical register number
    /// @param pnum the physical register number
    void setreg (t_lreg type, const long lnum, const long pnum);

    /// @return a string representation of the rid
    string tostring (void) const;
  };
}

#endif
