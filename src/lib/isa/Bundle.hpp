// ---------------------------------------------------------------------------
// - Bundle.hpp                                                              -
// - iato:isa library - bundle class definition                              -
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

#ifndef  IATO_BUNDLE_HPP
#define  IATO_BUNDLE_HPP

#ifndef  IATO_INSTR_HPP
#include "Instr.hpp"
#endif

namespace iato {
  using namespace std;

  /// The "Bundle" class is a converter class that is loaded by bytes. Once
  /// loaded, the byte stream is decoded like a bundle. The bundle template,
  /// and the instruction can be accessed as well. A string representation
  /// of the template encoding is also available.

  class Bundle {
  public:
    /// MII template
    static const t_byte BN_MxIxIx = 0x00;
    /// MII| template
    static const t_byte BN_MxIxIs = 0x01;
    /// MI|I template
    static const t_byte BN_MxIsIx = 0x02;
    /// MI|I| template
    static const t_byte BN_MxIsIs = 0x03;
    /// MLX template
    static const t_byte BN_MxLxXx = 0x04;
    /// MLX| template
    static const t_byte BN_MxLxXs = 0x05;
    /// MMI template
    static const t_byte BN_MxMxIx = 0x08;
    /// MMI| template
    static const t_byte BN_MxMxIs = 0x09;
    /// M|MI template
    static const t_byte BN_MsMxIx = 0x0A;
    /// M|MI| template
    static const t_byte BN_MsMxIs = 0x0B;
    /// MFI template
    static const t_byte BN_MxFxIx = 0x0C;
    /// MFI| template
    static const t_byte BN_MxFxIs = 0x0D;
    /// MMF template
    static const t_byte BN_MxMxFx = 0x0E;
    /// MMF| template
    static const t_byte BN_MxMxFs = 0x0F;
    /// MIB template
    static const t_byte BN_MxIxBx = 0x10;
    /// MIB| template
    static const t_byte BN_MxIxBs = 0x11;
    /// MBB template
    static const t_byte BN_MxBxBx = 0x12;
    /// MBB| template
    static const t_byte BN_MxBxBs = 0x13;
    /// BBB template
    static const t_byte BN_BxBxBx = 0x16;
    /// BBB| template
    static const t_byte BN_BxBxBs = 0x17;
    /// MMB template
    static const t_byte BN_MxMxBx = 0x18;
    /// MMB| template
    static const t_byte BN_MxMxBs = 0x19;
    /// MFB template
    static const t_byte BN_MxFxBx = 0x1C;
    /// MFB| template
    static const t_byte BN_MxFxBs = 0x1D;
    /// max number of bundle templates
    static const long   BN_MAXTPL = 32;

    /// return the string representation of a template with stop bits
    /// @param tmpl the template value
    static string tostrws (const t_byte tmpl);

    /// return the string representation of a template without stop bits
    /// @param tmpl the template value
    static string tostrwo (const t_byte tmpl);

  private:
    /// the number of bytes
    long d_blen;
    /// the bundle buffer
    t_byte* p_data;
    /// the predictor history
    t_octa  d_hist;
    /// the bundle ip
    t_octa d_bip;
    /// the speculative ip
    t_octa d_sip;    
    /// the speculative slot
    long   d_ssl;
    /// the valid slot bit
    bool   d_vsb[BN_SLSZ];

  public:
    /// create a default bundle
    Bundle (void);

    /// destroy this bundle
    ~Bundle (void);

    /// copy construct this bundle
    /// @param that the bundle to copy
    Bundle (const Bundle& that);

    /// assign a bundle to this one
    /// @param that the bundle to assign
    Bundle& operator = (const Bundle& that);

    /// reset this bundle
    void reset (void);

    /// @return true if the bundle is valid
    bool isvalid (void) const;

    /// @return true if the bundle slot is a branch
    bool isbr (const long slot) const;

    /// set the predictor history
    /// @param hist the history to set
    void sethist (const t_octa hist);
    
    /// @return the predictor history
    t_octa gethist (void) const;

    /// set the bundle instruction pointer
    /// @param ip the instruction pointer
    void setbip (const t_octa ip);

    /// @return the bundle instruction pointer
    t_octa getbip (void) const;

    /// set the bundle speculative ip
    /// @param sip the speculative ip
    /// @param ssl the speculative slot
    void setsip (const t_octa sip, const long ssl);

    /// set the bundle speculative ip with the history
    /// @param sip  the speculative ip
    /// @param ssl  the speculative slot
    /// @param hist the proedictor history
    void setsip (const t_octa sip, const long ssl, const t_octa hist);

    /// @return the bundle speculative ip
    t_octa getsip (void) const;

    /// set the valid slot bit
    /// @param slot the slot index
    /// @param flag the bit value
    void setvsb (const long slot, const bool flag);

    /// @return the valid slot bit by index
    bool getvsb (const long slot) const;

    /// @return the bundle length in bytes
    long length (void) const;

    /// push a byte in the buffer
    /// @param byte the byte to push
    void push (const t_byte byte);

    // get a byte from this buffer by index
    /// @param index the buffer index
    t_byte get (const long index) const;

    /// @return the bundle template code
    t_byte gettmpl (void) const;

    /// get an instruction code by slot index
    /// @param slot the instruction slot
    t_octa getslot (const long slot) const;

    /// get an instruction by slot index
    Instr getinstr (const long slot) const;
    
    /// @return a string representation of this bundle
    string tostring (void) const;

    /// @return a string representation of this bundle with stop bit
    string repr (void) const;
  };
}

#endif
