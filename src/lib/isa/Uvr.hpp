// ---------------------------------------------------------------------------
// - Uvr.hpp                                                                 -
// - iato:isa library - universal value representation class definition      -
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

#ifndef  IATO_UVR_HPP
#define  IATO_UVR_HPP

#ifndef  IATO_TYPES_HPP
#include "Types.hpp"
#endif

namespace iato {
  
  /// The Uvr class is a simple class that defines a universal value 
  /// representation. The default representation is NAV (not a value. The
  /// value can be either an octa, a combination of octa and nat bit,
  /// a boolean value or a real value.

  class Uvr {
  public:
    /// the uvr types
    enum t_uvrt {
      NAV, // not a value
      OBV, // octa base value
      ONV, // octa and nat bit value
      BBV, // boolean base value
      RBV  // real base value
    };

  private:
    /// the value type
    t_uvrt d_type;
    /// the boolean value or nat bit
    bool   d_bval;
    /// the octa value
    t_octa d_oval;
    /// the real value
    t_real d_rval;

  public:
    /// create a default uvr
    Uvr (void);

    /// copy construct this uvr
    /// @param that the uvr to copy
    Uvr (const Uvr& that);

    /// assign a uvr to this one
    /// @param that the uvr to assign
    Uvr& operator = (const Uvr& that);

    /// reset this uvr
    void reset (void);

    /// @return true if the uvr is valid
    bool isvalid (void) const;

    /// @return the uvr type
    t_uvrt gettype (void) const;

    /// set a boolean value
    /// @param bval the boolean value to set
    void setbval (const bool bval);

    /// @return the boolean value
    bool getbval (void) const;

    /// set an octa value
    /// @param oval the octa value to set
    void setoval (const t_octa oval);

    /// @return the octa value
    t_octa getoval (void) const;
    
    /// set a dual value (octa and nat bit)
    /// @param oval the octa value to set
    /// @param bval the nat bit to set
    void setdval (const t_octa oval, const bool bval);

    /// set a real value
    /// @param rval the real value to set
    void setrval (const t_real rval);

    /// @return the real value
    t_real getrval (void) const;
  };
}

#endif
