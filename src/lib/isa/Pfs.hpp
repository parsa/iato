// ---------------------------------------------------------------------------
// - Pfs.hpp                                                                 -
// - iato:isa library - previous frame state class definition                -
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

#ifndef  IATO_PFS_HPP
#define  IATO_PFS_HPP

#ifndef  IATO_TYPES_HPP
#include "Types.hpp"
#endif

namespace iato {
  
  /// The Pfs class is the previous frame state class object. It holds all
  /// the field that are defined by the IA64 ISA. The pfs value is stored
  /// as a single field but specific method can be used to access a particular
  /// one.

  class Pfs {
  public:
    /// the pfs fields
    enum t_field {
      PFM, // previous frame marker
      PEC, // previous epilog count
      PPL  // previous privilege level
    };

  private:
    /// the pfs value
    t_octa d_pfs;

  public:
    /// create a default pfs
    Pfs (void);

    /// create a pfs with a value
    /// @param value the pfs value to set
    Pfs (const t_octa value);

    /// copy construct this pfs
    /// @param that the pfs to copy
    Pfs (const Pfs& that);

    /// assign a pfs to this one
    /// @param that the pfs to assign
    Pfs& operator = (const Pfs& that);

    /// reset this pfs
    void reset (void);

    /// set the pfs value
    /// @param pfs the pfs to set
    void setpfs (const t_octa pfs);

    /// @return the resource pfs
    t_octa getpfs (void) const;

    /// set the pfs by field and value
    /// @param fld the pfs field
    /// @param val the pfs field value
    void setfld (t_field fld, t_octa val);

    /// @return the pfs field
    t_octa getfld (t_field fld) const;
  };
}

#endif
