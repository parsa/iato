// ---------------------------------------------------------------------------
// - Udecode.hxx                                                             -
// - iato:isa library - utility function for instruction decode              -
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

#ifndef IATO_UDECODE_HXX
#define IATO_UDECODE_HXX

#ifndef IATO_INSTR_HPP
#error  "cannot include this file outside an instruction decoder"
#endif

namespace iato {

  // get the major opcode from an instruction slot
  static inline t_byte slot_getmopc (const t_octa inst) {
    t_byte result = ((t_byte) (inst >> 37)) & 0x0F;
    return result;
  }

  // extract the predicate from an instruction
  static inline long get_pred (const t_octa inst) {
    t_byte result = ((t_byte) inst) & 0x3F;
    return (long) result;
  }
  
  // extract the small source 0 from an addl instruction
  static inline long get_small_src (const t_octa inst) {
    t_byte result = ((t_byte) (inst >> 20)) & 0x03;
    return (long) result;
  }

  // extract the source 0 from an instruction
  static inline long get_src0 (const t_octa inst) {
    t_byte result = ((t_byte) (inst >> 13)) & 0x7F;
    return (long) result;
  }
  
  // extract the source 1 from an instruction
  static inline long get_src1 (const t_octa inst) {
    t_byte result = ((t_byte) (inst >> 20)) & 0x7F;
    return (long) result;
  }

  // extract the source 2 from an instruction
  static inline long get_src2 (const t_octa inst) {
    t_byte result = ((t_byte) (inst >> 27)) & 0x7F;
    return (long) result;
  }

  // extract the branch source from an instruction
  static inline long get_src_br (const t_octa inst) {
    t_byte result = ((t_byte) (inst >> 13)) & 0x07;
    return (long) result;
  }
  
  // extract the branch destination from an instruction
  static inline long get_dst_br (const t_octa inst) {
    t_byte result = ((t_byte) (inst >> 6)) & 0x07;
    return (long) result;
  }
  
  // extract the first predicate destination from an instruction
  static inline long get_dst_pr0 (const t_octa inst) {
    t_byte result = ((t_byte) (inst >> 6)) & 0x3F;
    return (long) result;
  }

  // extract the second predicate destination from an instruction
  static inline long get_dst_pr1 (const t_octa inst) {
    t_byte result = ((t_byte) (inst >> 27)) & 0x3F;
    return (long) result;
  }

  // extract the destination from an instruction
  static inline long get_dst0 (const t_octa inst) {
    t_byte result = ((t_byte) (inst >> 6)) & 0x7F;
    return (long) result;
  }

  // extract the 2nd destination from an instruction
  static inline long get_dst1 (const t_octa inst) {
    return (get_src0 (inst));
  }

  // get the instruction immediate sign bit
  static inline bool get_sbit (const t_octa inst) {
    bool result = ((t_byte) (inst >> 36)) & 0x01 == 0x01;
    return result;
  }

  // get the instruction immediate sign bit at position 15
  static inline bool get_sbit15 (const t_octa inst) {
    bool result = ((t_byte) (inst >> 15)) & 0x01 == 0x01;
    return result;
  }
  // get the instruction immediate (2) value for M39
  static inline t_octa get_im2 (const t_octa inst) {
    t_octa result = (inst >> 13) & 0x0000000000000003ULL;
    return result;
  }

  // get the sign-extended immediate value inc (3)  for M17
  static inline t_octa get_inc3 (const t_octa inst) {
    t_byte i2b = ((t_byte) (inst >> 13)) & 0x03;
    t_octa result = (i2b == 0x03) ? OCTA_1 : OCTA_1 << (4 - i2b);
    if (get_sbit15 (inst) == true) result = ~result + OCTA_1;
    return result;
  }

  // get the instruction immediate (8) value
  static inline t_octa get_im08b (const t_octa inst) {
    t_octa result = (inst >> 13) & 0x000000000000007F;
    if (get_sbit (inst)) result |= ~0x000000000000003F;
    return result;
  }

  // get the instruction immediate (9) value for M3 M8 and M15
  static inline t_octa get_im09b (const t_octa inst) {
    bool     ibit = ((t_byte) (inst >> 27) & 0x01) == 0x01;
    t_octa result = (inst >> 13) & 0x000000000000007F;
    if (ibit == true) result |= 0x80;
    if (get_sbit (inst)) result |= ~0x00000000000000FF;
    return result;
  }

  // get the instruction immediate (9) value for M5 M10
  static inline t_octa get_im09a (const t_octa inst) {
    bool     ibit = ((t_byte) (inst >> 27) & 0x01) == 0x01;
    t_octa result = (inst >> 6) & 0x000000000000007F;
    if (ibit == true) result |= 0x80;
    if (get_sbit (inst)) result |= ~0x00000000000000FF;
    return result;
  }

  // get the instruction immediate (14) value
  static inline t_octa get_im14 (const t_octa inst) {
    t_octa result = (((inst >> 13) & 0x007F) | ((inst >> 20) & 0x003F80));
    if (get_sbit (inst)) result |= ~0x0000000000001FFF;
    return result;
  }

  // get the instruction immediate (20) value for break/nop
  static inline t_octa get_i20a (const t_octa inst) {
    t_octa result = (inst >> 6) & 0x00000000000FFFFF;
    if (get_sbit (inst)) result |= 0x0000000000100000;
    return result;
  }

  // get the instruction immediate (22) value
  static inline t_octa get_im22 (const t_octa inst) {
    t_octa result = (((inst >> 13) & 0x007F) | ((inst >> 20) & 0x00FF80) |
		     ((inst >> 6) & 0x1F0000));
    if (get_sbit (inst)) result |= ~0x00000000001FFFFF;
    return result;
  }

  // get the instruction immediate (21) value for M44
  static inline t_octa get_im24 (const t_octa inst) {
    t_octa result = OCTA_0;
    if (get_sbit (inst) == true) result |= 0x0000000000800000ULL;  
    result |= ((inst >> 31) & 0x0000000000000003ULL) << 21;
    result |=  (inst >> 6)  & 0x00000000001FFFFFULL;
    return result;
  }

  // get the instruction target25 value for floating point check flags
  // check. The value is shifted by 4 after the sign extension.
  static inline t_octa get_i25a (const t_octa inst) {
    t_octa result = ((inst >> 6) & 0x00000000000FFFFFULL); 
    if (get_sbit (inst) == true) result |= ~0x0000000000FFFFFULL;
    result = result << 4;
    return result;
  }

  // get the instruction immediate (25) value for branch and advanced load
  // check. The value is shifted by 4 after the sign extension.
  static inline t_octa get_i25b (const t_octa inst) {
    t_octa result = (inst >> 13) & 0x00000000000FFFFF;
    if (get_sbit (inst) == true) result |= ~0x00000000000FFFFF;
    result = result << 4;
    return result;
  }
  
  // get the instruction target25 value for speculation check
  // it is shifted by 4 after the sign extension.
  static inline t_octa get_spectg (const t_octa inst) {
    t_octa result = ((inst >> 6) & 0x000000000000007F)
      |((inst >> 13) & 0x00000000000FFF80);
    if (get_sbit (inst) == true) result |= ~0x00000000000FFFFF;
    result = result << 4;
    return result;
  }

  // get the instruction immediate (62) value
  static inline t_octa get_im62 (const t_octa inst, const t_octa extd) {
    t_octa result = extd << 21;
    if (get_sbit (inst) == true) result |= (t_octa) 1 << 20;
    result |= (inst >> 6) & 0x00000000000FFFFF;
    return result;
  }

  // get the instruction immediate (64) value
  static inline t_octa get_im64 (const t_octa inst, const t_octa extd) {
    t_octa result = 0;
    if (get_sbit (inst) == true) result |= 0x8000000000000000LL;
    result |= extd << 22;
    result |= ((inst >> 21) & 0x0000000000000001LL) << 21;
    result |= ((inst >> 22) & 0x000000000000001FLL) << 16;
    result |= ((inst >> 27) & 0x00000000000001FFLL) << 7;
    result |=  (inst >> 13) & 0x000000000000007FLL;
    return result;
  }
}

#endif
