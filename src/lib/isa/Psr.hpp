// ---------------------------------------------------------------------------
// - Psr.hpp                                                                 -
// - iato:isa library - processor status register class definition           -
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

#ifndef  IATO_PSR_HPP
#define  IATO_PSR_HPP

#ifndef  IATO_UMR_HPP
#include "Umr.hpp"
#endif

namespace iato {
  
  /// The Psr class is the processor status register class object. It holds all
  /// the field that are defined by the IA64 ISA. The psr value is stored
  /// as a single field but specific method can be used to access a particular
  /// one. Note that the psr holds also the user mask register as subset.

  class Psr {
  public:
    /// the psr fields
    enum t_field {
      UM,  // user mask
      IC,  // interrupt collection
      IB,  // interrupt bit
      PK,  // protection key
      DT,  // data address translation
      DFL, // lower fp disabled
      DFH, // higher fp disabled
      SP,  // secure performance monitor
      PP,  // priviledge performance monitor
      DI,  // instruction set translation disabled
      SI,  // secure interval timer
      DB,  // debug breakpoint fault
      LP,  // lower privilege transfer trap
      TB,  // taken branch trap
      RT,  // register stack translation
      CPL, // current privilege level
      IS,  // instruction set
      MC,  // machine check abort
      IT,  // instruction address translation
      ID,  // instruction debug
      DA,  // data access and dirty bit fault
      DD,  // data debug fault
      SS,  // single step enable
      RI,  // restart instruction
      ED,  // exception deferal
      BN,  // register bank
      IA   // instruction access bit fault
    };

    /// the psr test reserved field
    enum t_trvfd {
      UMB,    // user mask bits
      SMB,    // system mask bits
      ALL     // the whole register
    };

  private:
    /// the psr value
    t_octa d_psr;
    
  public:
    /// create a default psr
    Psr (void);

    /// create a psr with a value
    /// @param value the psr value to set
    Psr (const t_octa value);

    /// copy construct this psr
    /// @param that the psr to copy
    Psr (const Psr& that);

    /// assign a psr to this one
    /// @param that the psr to assign
    Psr& operator = (const Psr& that);

    /// reset this psr
    void reset (void);

    /// set the psr value
    /// @param psr the psr to set
    void setpsr (const t_octa psr);

    /// @return the resource psr
    t_octa getpsr (void) const;

    /// set the umr value
    /// @param umr the umr value to set
    void setumr (const Umr& umr);

    /// @return the umr value
    Umr getumr (void) const;

    /// set the psr by field and value
    /// @param fld the psr field
    /// @param val the psr field value
    void setfld (const t_field fld, const bool val);

    /// set the psr by field and value
    /// @param fld the psr field
    /// @param val the psr field value
    void setfld (const t_field fld, const t_byte val);
 
    /// @return the psr boolean field
    bool getfld (const t_field fld) const;

    /// @return the psr byte field
    t_byte getbyte (const t_field fld) const;
    
    /// @return true if the value marks a reserved field
    bool isrvfd (const t_trvfd fd, const t_octa value) const;
  };
}

#endif
