// ---------------------------------------------------------------------------
// - Instr.hpp                                                               -
// - iato:isa library - instruction decode/recode class definition           -
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

#ifndef  IATO_INSTR_HPP
#define  IATO_INSTR_HPP

#ifndef  IATO_RPM_HPP
#include "Rpm.hpp"
#endif

#ifndef  IATO_OPERAND_HPP
#include "Operand.hpp"
#endif

#ifndef  IATO_RESULT_HPP
#include "Result.hpp"
#endif

#ifndef  IATO_OPCODE_HPP
#include "Opcode.hpp"
#endif

namespace iato {
  using namespace std;

  /// The Instr class is the instruction decode/recode for the IA64 ISA.
  /// The class holds as much as information that can be derived from the
  /// construction. The instruction holds an opcode that indicates the
  /// instruction type. The unit, slot and stop bit are also part of the
  /// instruction information. The bundle ip for that instruction is also
  /// stored. Register source and destination are store as rid (register id)
  /// object. Finally, instruction specific hints are stored as well.
  /// An instruction is generally constructed from the bundle object. The
  /// default instruction is an invalid instruction as indicated by a valid
  /// bit. Alternatively, an instruction can be built by giving the unit,
  /// slot number, stop bit flag and a 41 bit coding represented by an
  /// octa. Special long instruction uses a second field to pass the extension.
  /// Numerous methods are available to get the instruction information.
  /// A special method called 'recode' build a string a representation of
  /// that instruction as specified by the ISA. An invalid instruction
  /// can be marked by using the 'decode' method with the proper parameters.
  /// Trying to decode an invalid instruction produces an exception.

  class Instr {
  protected:
    /// valid bit
    bool d_valid;
    /// the instruction ip
    t_octa d_iip;
    /// the speculative ip
    t_octa d_sip;
    /// the speculative flag
    bool   d_sfl;
    /// the predictor history
    t_octa d_hist;
    /// instruction bundle unit
    t_unit d_bunit;
    /// instruction functional unit
    t_unit d_funit;
    /// instruction group
    string d_group;
    /// the slot index
    long d_slot;
    /// stop bit
    bool d_stop;
    /// the branch bit
    bool d_brch;
    /// encoded instruction 
    t_octa d_inst;
    /// extension data
    t_octa d_extd;
    /// major group opcode
    t_byte d_code;
    /// detailed opcode
    t_iopc d_opcd;
    /// the load bit
    bool   d_ildb;
    /// the store bit
    bool   d_istb;
    /// the immediate value
    t_octa d_immv[IA_MSRC];
    /// move to branch hint
    t_mhint d_mhint;
    /// move and branch predict completer
    t_ihint d_ihint;
    /// ip-relative predict hint
    t_bphint d_bphint;
    /// load hint
    t_ldhint d_lhint;
    /// store hint
    t_sthint d_shint;
    /// branch prefetch hint
    t_phint  d_phint;
    /// branch direction hint
    t_bhint  d_bhint;
    /// branch cache deallocation hint
    t_chint  d_chint;
    /// line prefetch hint
    t_lfhint d_lfhint;
    // the floating-point status field completer
    t_fpcomp d_fpcomp;
    /// the predicate register
    Rid d_rprd;
    /// the source register
    Rid d_rsrc[IA_MSRC];
    // the destination register
    Rid d_rdst[IA_MDST];
    /// the rid pair violation
    Rpm d_rrpm[IA_MRPM];

  public:
    /// @return the string representation of an instruction opcode
    static string tostr (const long opcd);

    /// @return true if the instruction group is valid
    static bool isvig (const string& s);

  public:
    /// create a default instruction
    Instr (void);

    /// create an instruction by type/slot/stop and data
    /// @param unit the instruction unit
    /// @param slot the instruction slot
    /// @param bstp the stop bit flag
    /// @param inst the encoded instruction
    Instr (t_unit unit, long slot , bool bstp, t_octa inst);

    /// create an instruction by type/slot/stop data and extension
    /// @param unit the instruction unit
    /// @param slot the instruction slot
    /// @param bstp the stop bit flag
    /// @param inst the encoded instruction
    /// @param extd the instruction extension
    Instr (t_unit unit, long slot , bool bstp, t_octa inst, t_octa extd);

    /// copy construct an instruction
    /// @param that the instruction to copy
    Instr (const Instr& that);

    /// assign an instruction to this one
    /// @param that the instruction to assign
    Instr& operator = (const Instr& that);

    /// reset this instruction
    void reset (void);

    /// @return the instruction valid bit
    bool isvalid (void) const;

    /// set the instruction ip
    /// @param ip the instruction ip
    void setiip (const t_octa ip);

    /// @return the instruction ip
    t_octa getiip (void) const;

    /// set the speculative ip
    /// @param sip the speculative ip
    void setsip (const t_octa sip);

    /// @return the speculative ip
    t_octa getsip (void) const;

    /// @return the speculative flag
    bool getsfl (void) const;

    /// set the predictor history
    /// @param hist the history to set
    void sethist (const t_octa hist);
    
    /// @return the predictor history
    t_octa gethist (void) const;

    /// @return the instruction slot
    long getslot (void) const;

    /// @return the instruction stop bit
    bool getstop (void) const;

    /// @return the instruction group
    string getgroup (void) const;

    /// @return the instruction data
    t_octa getdata (void) const;

    /// @return the extension data
    t_octa getextd (void) const;
    
    /// @return the instruction opcode
    t_iopc getiopc (void) const;

    /// @return the instruction bundle unit
    t_unit getbunit (void) const;

    /// @return the instruction functional unit
    t_unit getfunit (void) const;

    /// @return the instruction slot unit
    t_unit getsunit (void) const;

    /// @return true if the instruction is a load
    bool getldb (void) const;

    /// @return true if the instruction is a store
    bool getstb (void) const;

    /// @return true if the instruction is a branch
    bool isbr (void) const;

    /// @return true if the instruction is a nop
    bool isnop (void) const;

    /// @return the floating point status completer
    t_fpcomp getfpcomp (void) const;

    /// @return the instruction immediate value
    t_octa getimmv (const long index) const;
    
    /// @return true if the predicate matches the rid
    bool ispnum (const Rid& rid) const;

    /// @return the predicate register number
    Rid getpnum (void) const;

    /// set the predicate register number
    /// @param pnum the register to set
    void setpnum (const Rid& pnum);

    /// @return true if one source matches the rid
    bool issnum (const Rid& rid) const;

    /// get the source register number by index
    /// @param index the source operand index
    Rid getsnum (const long index) const;

    /// set the source register number
    /// @param index the source index
    /// @param snum the register to set
    void setsnum (const long index, const Rid& snum);

    /// @return true if one destination matches the rid
    bool isdnum (const Rid& rid) const;

    /// get the destination register number by index
    /// @param index the destination operand index
    Rid getdnum (const long index) const;

    /// set the destination register number
    /// @param index the source index
    /// @param dnum the register to set
    void setdnum (const long index, const Rid& dnum);

    /// @return the rid pair map by index
    Rpm getrrpm (const long index) const;

    /// @return the register operand
    Operand getoper (void) const;

    /// @return the result registers
    Result getresl (void) const;

    /// @return true if the instruction is predicated
    bool ispred (void) const;

    /// @return true if the instruction generates predicates
    bool ispgen (void) const;

    /// set and decode an instruction
    /// @param unit the instruction unit
    /// @param slot the instruction slot
    /// @param bstp the stop bit flag
    /// @param inst the encoded instruction
    void decode (t_unit unit, long slot, bool bstp, t_octa inst);

    /// set and decode an instruction with extensiion
    /// @param unit the instruction unit
    /// @param slot the instruction slot
    /// @param bstp the stop bit flag
    /// @param inst the encoded instruction
    /// @param extd the extended data
    void decode (t_unit unit, long slot, bool bstp, t_octa inst, t_octa extd);

    /// @return a string representation by doing a recoding
    string recode (void) const;

  private:
    // private decode methods
    void adecode (void);
    void aupdate (void);
    void mdecode (void);
    void mupdate (void);
    void idecode (void);
    void iupdate (void);
    void fdecode (void);
    void fupdate (void);
    void bdecode (void);
    void bupdate (void);
    void xdecode (void);
    void xupdate (void);
    // private recode methods
    string arecode_01 (void) const;
    string arecode_02 (void) const;
    string arecode_03 (void) const;
    string arecode_04 (void) const;
    string arecode_05 (void) const;
    string arecode_06 (void) const;
    string arecode_07 (void) const;
    string arecode_08 (void) const;
    string arecode_09 (void) const;
    string arecode_10 (void) const;
    string mrecode_01 (void) const;
    string mrecode_02 (void) const;
    string mrecode_03 (void) const;
    string mrecode_04 (void) const;
    string mrecode_05 (void) const;
    string mrecode_06 (void) const;
    string mrecode_07 (void) const;
    string mrecode_08 (void) const;
    string mrecode_09 (void) const;
    string mrecode_10 (void) const;
    string mrecode_11 (void) const;
    string mrecode_12 (void) const;
    string mrecode_13 (void) const;
    string mrecode_14 (void) const;
    string mrecode_15 (void) const;
    string mrecode_16 (void) const;
    string mrecode_17 (void) const;
    string mrecode_18 (void) const;
    string mrecode_19 (void) const;
    string mrecode_20 (void) const;
    string mrecode_21 (void) const;
    string mrecode_22 (void) const;
    string mrecode_23 (void) const;
    string mrecode_24 (void) const;
    string mrecode_25 (void) const;
    string mrecode_26 (void) const;
    string mrecode_27 (void) const;
    string mrecode_28 (void) const;
    string mrecode_29 (void) const;
    string mrecode_30 (void) const;
    string mrecode_31 (void) const;
    string mrecode_32 (void) const;
    string mrecode_33 (void) const;
    string mrecode_34 (void) const;
    string mrecode_35 (void) const;
    string mrecode_36 (void) const;
    string mrecode_37 (void) const;
    string mrecode_38 (void) const;
    string mrecode_39 (void) const;
    string mrecode_40 (void) const;
    string mrecode_41 (void) const;
    string mrecode_44 (void) const;
    string mrecode_45 (void) const;
    string mrecode_46 (void) const;
    string irecode_01 (void) const;
    string irecode_02 (void) const;
    string irecode_03 (void) const;
    string irecode_04 (void) const;
    string irecode_05 (void) const;
    string irecode_06 (void) const;
    string irecode_07 (void) const;
    string irecode_08 (void) const;
    string irecode_09 (void) const;
    string irecode_10 (void) const;
    string irecode_11 (void) const;
    string ipseudo_11 (void) const;
    string irecode_12 (void) const;
    string irecode_13 (void) const;
    string irecode_14 (void) const;
    string irecode_15 (void) const;
    string irecode_16 (void) const;
    string irecode_17 (void) const;
    string irecode_19 (void) const;
    string irecode_20 (void) const;
    string irecode_21 (void) const;
    string irecode_22 (void) const;
    string irecode_23 (void) const;
    string irecode_24 (void) const;
    string irecode_25 (void) const;
    string irecode_26 (void) const;
    string irecode_27 (void) const;
    string irecode_28 (void) const;
    string irecode_29 (void) const;
    string frecode_01 (void) const;
    string fpseudo_01 (void) const;
    string frecode_02 (void) const;
    string frecode_03 (void) const;
    string frecode_04 (void) const;
    string frecode_05 (void) const;
    string frecode_06 (void) const;
    string frecode_07 (void) const;
    string frecode_08 (void) const;
    string frecode_09 (void) const;
    string fpseudo_09 (void) const;
    string frecode_10 (void) const;
    string frecode_11 (void) const;
    string frecode_12 (void) const;
    string frecode_13 (void) const;
    string frecode_14 (void) const;
    string frecode_15 (void) const;
    string brecode_01 (void) const;
    string brecode_02 (void) const;
    string brecode_03 (void) const;
    string brecode_04 (void) const;
    string brecode_05 (void) const;
    string brecode_06 (void) const;
    string brecode_07 (void) const;
    string brecode_08 (void) const;
    string brecode_09 (void) const;
    string xrecode_01 (void) const;
    string xrecode_02 (void) const;
    string arecode    (void) const;
    string mrecode    (void) const;
    string irecode    (void) const;
    string frecode    (void) const;
    string brecode    (void) const;
    string xrecode    (void) const;
  };
}

#endif
