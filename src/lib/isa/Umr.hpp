// ---------------------------------------------------------------------------
// - Umr.hpp                                                                 -
// - iato:isa library - user mask register class definition                  -
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

#ifndef  IATO_UMR_HPP
#define  IATO_UMR_HPP

#ifndef  IATO_TYPES_HPP
#include "Types.hpp"
#endif

namespace iato {
  
  /// The Umr class is the user mask register. The UMR is a subset of the
  /// processor status register (PSR). The UMR can be modified here and
  /// later inserted into the PSR

  class Umr {
  public:
    /// the umr fields
    enum t_field {
      BE,  // big endian
      UP,  // user performance monitor
      AC,  // alignement check
      MFL, // lower fp reg written
      MFH  // upper fp reg writen
    };

  private:
    /// the umr value
    t_byte d_umr;

  public:
    /// create a default umr
    Umr (void);

    /// create a umr with a value
    /// @param value the umr value to set
    Umr (const t_byte value);

    /// copy construct this umr
    /// @param that the umr to copy
    Umr (const Umr& that);

    /// assign a umr to this one
    /// @param that the umr to assign
    Umr& operator = (const Umr& that);

    /// reset this umr
    void reset (void);

    /// set the umr value
    /// @param umr the umr to set
    void setumr (const t_byte umr);

    /// @return the resource umr
    t_byte getumr (void) const;

    /// set the umr by field and value
    /// @param fld the umr field
    /// @param val the umr field value
    void setfld (t_field fld, const bool val);

    /// @return the umr field
    bool getfld (t_field fld) const;
  };
}

#endif
