// ---------------------------------------------------------------------------
// - Cfm.hpp                                                                 -
// - iato:isa library - current frame marker class definition                -
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

#ifndef  IATO_CFM_HPP
#define  IATO_CFM_HPP

#ifndef  IATO_TYPES_HPP
#include "Types.hpp"
#endif

namespace iato {
  
  /// The Cfm class is the current frame marker class object. It holds all
  /// the field that are defined by the IA64 ISA. The cfm value is stored
  /// as a single field but specific method can be used to access a particular
  /// one.

  class Cfm {
  public:
    /// the cfm fields
    enum t_field {
      SOF, // size of frame
      SOL, // size of locals
      SOR, // size of rotating
      RGR, // GR rename base
      RFR, // FR rename base
      RPR  // PR rename base
    };
    /// max value of rrb gr
    static const long GR_RRBM = 128;
    /// max value of rrb fr
    static const long FR_RRBM = 128;
    /// max value of rrb pr
    static const long PR_RRBM = 64;

  private:
    /// the cfm value
    t_octa d_cfm;

  public:
    /// create a default cfm
    Cfm (void);

    /// create a cfm with a value
    /// @param value the cfm value to set
    Cfm (const t_octa value);

    /// copy construct this cfm
    /// @param that the cfm to copy
    Cfm (const Cfm& that);

    /// assign a cfm to this one
    /// @param that the cfm to assign
    Cfm& operator = (const Cfm& that);

    /// @return true if two cfm are equals
    bool operator == (const Cfm& cfm) const;

    /// @return true if the cfm match an octa
    bool operator == (const t_octa val) const;

    /// @return true if two cfm are not equal
    bool operator != (const Cfm& cfm) const;

    /// @return true if the cfm do not match an octa
    bool operator != (const t_octa val) const;

    /// reset this cfm
    void reset (void);

    /// set the cfm value
    /// @param cfm the cfm to set
    void setcfm (const t_octa cfm);

    /// @return the resource cfm
    t_octa getcfm (void) const;

    /// @return the updated rotated cfm
    t_octa rotate (void);

    /// set the cfm by field and value
    /// @param fld the cfm field
    /// @param val the cfm field value
    void setfld (t_field fld, t_byte val);

    /// @return the cfm field
    t_byte getfld (t_field fld) const;

    /// @return the rrb field as a negative value
    long getrrb (t_field fld) const;

    /// set the cfm rrb by field and value
    /// @param fld the cfm field
    /// @param val the cfm field value
    void setrrb (t_field fld, const long val);
  };
}

#endif
