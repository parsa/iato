// ---------------------------------------------------------------------------
// - Opcode.hpp                                                              -
// - iato:isa library - instruction operands and opcode definitions          -
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

#ifndef  IATO_OPCODE_HPP
#define  IATO_OPCODE_HPP

namespace iato {

  /// instruction opcode type
  enum t_iopc {
    U_INVALID,                 // invalid decoded instruction
    
    A_ADD,                     // A01 : add              r1 = r2, r3
    A_ADD_1,                   // A01 : add              r1 = r2, r3, 1
    A_SUB,                     // A01 : sub              r1 = r2, r3
    A_SUB_1,                   // A01 : sub              r1 = r2, r3, 1
    A_ADDP4,                   // A01 : addp4            r1 = r2, r3
    A_AND,                     // A01 : and              r1 = r2, r3
    A_ANDCM,                   // A01 : andcm            r1 = r2, r3
    A_OR,                      // A01 : or               r1 = r2, r3
    A_XOR,                     // A01 : xor              r1 = r2, r3
    A_SHLADD,                  // A02 : shladd           r1 = r2, count, r3
    A_SHLADDP4,                // A02 : shladdp4         r1 = r2, count, r3
    A_SUB_I,                   // A03 : sub              r1 = immv, r3
    A_AND_I,                   // A03 : and              r1 = immv, r3
    A_ANDCM_I,                 // A03 : andcm            r1 = immv, r3
    A_OR_I,                    // A03 : or               r1 = immv, r3
    A_XOR_I,                   // A03 : xor              r1 = immv, r3
    A_ADDS_I,                  // A04 : adds             r1 = immv, r3
    A_ADDP4_I,                 // A04 : addp4            r1 = immv, r3
    A_ADDL_I,                  // A05 : addl             r1 = immv, r3
    A_CMP_LT_RR,               // A06 : cmp.lt           p1, p2 = r2, r3
    A_CMP_LTU_RR,              // A06 : cmp.ltu          p1, p2 = r2, r3
    A_CMP_EQ_RR,               // A06 : cmp.eq           p1, p2 = r2, r3
    A_CMP_LT_UNC_RR,           // A06 : cmp.lt.unc       p1, p2 = r2, r3
    A_CMP_LTU_UNC_RR,          // A06 : cmp.ltu.unc      p1, p2 = r2, r3
    A_CMP_EQ_UNC_RR,           // A06 : cmp.eq.unc       p1, p2 = r2, r3
    A_CMP_EQ_AND_RR,           // A06 : cmp.eq.and       p1, p2 = r2, r3
    A_CMP_EQ_OR_RR,            // A06 : cmp.eq.or        p1, p2 = r2, r3
    A_CMP_EQ_OR_ANDCM_RR,      // A06 : cmp.eq.or.andcm  p1, p2 = r2, r3
    A_CMP_NE_AND_RR,           // A06 : cmp.ne.and       p1, p2 = r2, r3
    A_CMP_NE_OR_RR,            // A06 : cmp.ne.or        p1, p2 = r2, r3
    A_CMP_NE_OR_ANDCM_RR,      // A06 : cmp.ne.or.andcm  p1, p2 = r2, r3
    A_CMP4_LT_RR,              // A06 : cmp4.lt          p1, p2 = r2, r3
    A_CMP4_LTU_RR,             // A06 : cmp4.ltu         p1, p2 = r2, r3
    A_CMP4_EQ_RR,              // A06 : cmp4.eq          p1, p2 = r2, r3
    A_CMP4_LT_UNC_RR,          // A06 : cmp4.lt.unc      p1, p2 = r2, r3
    A_CMP4_LTU_UNC_RR,         // A06 : cmp4.ltu.unc     p1, p2 = r2, r3
    A_CMP4_EQ_UNC_RR,          // A06 : cmp4.eq.unc      p1, p2 = r2, r3
    A_CMP4_EQ_AND_RR,          // A06 : cmp4.eq.and      p1, p2 = r2, r3
    A_CMP4_EQ_OR_RR,           // A06 : cmp4.eq.or       p1, p2 = r2, r3
    A_CMP4_EQ_OR_ANDCM_RR,     // A06 : cmp4.eq.or.andcm p1, p2 = r2, r3
    A_CMP4_NE_AND_RR,          // A06 : cmp4.ne.and      p1, p2 = r2, r3
    A_CMP4_NE_OR_RR,           // A06 : cmp4.ne.or       p1, p2 = r2, r3
    A_CMP4_NE_OR_ANDCM_RR,     // A06 : cmp4.ne.or.andcm p1, p2 = r2, r3
    A_CMP_GT_AND_ZR,           // A07 : cmp.gt.and       p1, p2 = r0, r3
    A_CMP_GT_OR_ZR,            // A07 : cmp.gt.or        p1, p2 = r0, r3
    A_CMP_GT_OR_ANDCM_ZR,      // A07 : cmp.gt.or.andcm  p1, p2 = r0, r3
    A_CMP_LE_AND_ZR,           // A07 : cmp.le.and       p1, p2 = r0, r3
    A_CMP_LE_OR_ZR,            // A07 : cmp.le.or        p1, p2 = r0, r3
    A_CMP_LE_OR_ANDCM_ZR,      // A07 : cmp.le.or.andcm  p1, p2 = r0, r3
    A_CMP_GE_AND_ZR,           // A07 : cmp.ge.and       p1, p2 = r0, r3
    A_CMP_GE_OR_ZR,            // A07 : cmp.ge.or        p1, p2 = r0, r3
    A_CMP_GE_OR_ANDCM_ZR,      // A07 : cmp.ge.or.andcm  p1, p2 = r0, r3
    A_CMP_LT_AND_ZR,           // A07 : cmp.lt.and       p1, p2 = r0, r3
    A_CMP_LT_OR_ZR,            // A07 : cmp.lt.or        p1, p2 = r0, r3
    A_CMP_LT_OR_ANDCM_ZR,      // A07 : cmp.lt.or.andcm  p1, p2 = r0, r3
    A_CMP4_GT_AND_ZR,          // A07 : cmp4.gt.and      p1, p2 = r0, r3
    A_CMP4_GT_OR_ZR,           // A07 : cmp4.gt.or       p1, p2 = r0, r3
    A_CMP4_GT_OR_ANDCM_ZR,     // A07 : cmp4.gt.or.andcm p1, p2 = r0, r3
    A_CMP4_LE_AND_ZR,          // A07 : cmp4.le.and      p1, p2 = r0, r3
    A_CMP4_LE_OR_ZR,           // A07 : cmp4.le.or       p1, p2 = r0, r3
    A_CMP4_LE_OR_ANDCM_ZR,     // A07 : cmp4.le.or.andcm p1, p2 = r0, r3
    A_CMP4_GE_AND_ZR,          // A07 : cmp4.ge.and      p1, p2 = r0, r3
    A_CMP4_GE_OR_ZR,           // A07 : cmp4.ge.or       p1, p2 = r0, r3
    A_CMP4_GE_OR_ANDCM_ZR,     // A07 : cmp4.ge.or.andcm p1, p2 = r0, r3
    A_CMP4_LT_AND_ZR,          // A07 : cmp4.lt.and      p1, p2 = r0, r3
    A_CMP4_LT_OR_ZR,           // A07 : cmp4.lt.or       p1, p2 = r0, r3
    A_CMP4_LT_OR_ANDCM_ZR,     // A07 : cmp4.lt.or.andcm p1, p2 = r0, r3
    A_CMP_LT_IR,               // A08 : cmp.lt           p1, p2 = immv, r3
    A_CMP_LTU_IR,              // A08 : cmp.ltu          p1, p2 = immv, r3
    A_CMP_EQ_IR,               // A08 : cmp.eq           p1, p2 = immv, r3
    A_CMP_LT_UNC_IR,           // A08 : cmp.lt.unc       p1, p2 = immv, r3
    A_CMP_LTU_UNC_IR,          // A08 : cmp.ltu.unc      p1, p2 = immv, r3
    A_CMP_EQ_UNC_IR,           // A08 : cmp.eq.unc       p1, p2 = immv, r3
    A_CMP_EQ_AND_IR,           // A08 : cmp.eq.and       p1, p2 = immv, r3
    A_CMP_EQ_OR_IR,            // A08 : cmp.eq.or        p1, p2 = immv, r3
    A_CMP_EQ_OR_ANDCM_IR,      // A08 : cmp.eq.or.andcm  p1, p2 = immv, r3
    A_CMP_NE_AND_IR,           // A08 : cmp.ne.and       p1, p2 = immv, r3
    A_CMP_NE_OR_IR,            // A08 : cmp.ne.or        p1, p2 = immv, r3
    A_CMP_NE_OR_ANDCM_IR,      // A08 : cmp.ne.or.andcm  p1, p2 = immv, r3
    A_CMP4_LT_IR,              // A08 : cmp4.lt          p1, p2 = immv, r3
    A_CMP4_LTU_IR,             // A08 : cmp4.ltu         p1, p2 = immv, r3
    A_CMP4_EQ_IR,              // A08 : cmp4.eq          p1, p2 = immv, r3
    A_CMP4_LT_UNC_IR,          // A08 : cmp4.lt.unc      p1, p2 = immv, r3
    A_CMP4_LTU_UNC_IR,         // A08 : cmp4.ltu.unc     p1, p2 = immv, r3
    A_CMP4_EQ_UNC_IR,          // A08 : cmp4.eq.unc      p1, p2 = immv, r3
    A_CMP4_EQ_AND_IR,          // A08 : cmp4.eq.and      p1, p2 = immv, r3
    A_CMP4_EQ_OR_IR,           // A08 : cmp4.eq.or       p1, p2 = immv, r3
    A_CMP4_EQ_OR_ANDCM_IR,     // A08 : cmp4.eq.or.andcm p1, p2 = immv, r3
    A_CMP4_NE_AND_IR,          // A08 : cmp4.ne.and      p1, p2 = immv, r3
    A_CMP4_NE_OR_IR,           // A08 : cmp4.ne.or       p1, p2 = immv, r3
    A_CMP4_NE_OR_ANDCM_IR,     // A08 : cmp4.ne.or.andcm p1, p2 = immv, r3
    A_PADD1,                   // A09 : padd1            r1 = r2, r3
    A_PADD2,                   // A09 : padd2            r1 = r2, r3
    A_PADD4,                   // A09 : padd4            r1 = r2, r3
    A_PADD1_SSS,               // A09 : padd1.sss        r1 = r2, r3
    A_PADD2_SSS,               // A09 : padd2.sss        r1 = r2, r3
    A_PADD1_UUU,               // A09 : padd1.uuu        r1 = r2, r3
    A_PADD2_UUU,               // A09 : padd2.uuu        r1 = r2, r3
    A_PADD1_UUS,               // A09 : padd1.uus        r1 = r2, r3
    A_PADD2_UUS,               // A09 : padd2.uus        r1 = r2, r3
    A_PSUB1,                   // A09 : psub1            r1 = r2, r3
    A_PSUB2,                   // A09 : psub2            r1 = r2, r3
    A_PSUB4,                   // A09 : psub4            r1 = r2, r3
    A_PSUB1_SSS,               // A09 : psub1.sss        r1 = r2, r3
    A_PSUB2_SSS,               // A09 : psub2.sss        r1 = r2, r3
    A_PSUB1_UUU,               // A09 : psub1.uuu        r1 = r2, r3
    A_PSUB2_UUU,               // A09 : psub2.uuu        r1 = r2, r3
    A_PSUB1_UUS,               // A09 : psub1.uus        r1 = r2, r3
    A_PSUB2_UUS,               // A09 : psub2.uus        r1 = r2, r3    
    A_PAVG1,                   // A09 : pavg1            r1 = r2, r3
    A_PAVG2,                   // A09 : pavg2            r1 = r2, r3
    A_PAVG1_RAZ,               // A09 : pavg1.raz        r1 = r2, r3
    A_PAVG2_RAZ,               // A09 : pavg2.raz        r1 = r2, r3
    A_PAVGSUB1,                // A09 : pavgsub1         r1 = r2, r3
    A_PAVGSUB2,                // A09 : pavgsub2         r1 = r2, r3
    A_PCMP1_EQ,                // A09 : pcmp1.eq         r1 = r2, r3
    A_PCMP2_EQ,                // A09 : pcmp2.eq         r1 = r2, r3
    A_PCMP4_EQ,                // A09 : pcmp4.eq         r1 = r2, r3
    A_PCMP1_GT,                // A09 : pcmp1.gt         r1 = r2, r3
    A_PCMP2_GT,                // A09 : pcmp2.gt         r1 = r2, r3
    A_PCMP4_GT,                // A09 : pcmp4.gt         r1 = r2, r3
    A_PSHLADD2,                // A10 : pshladd2         r1 = r2, count2, r3
    A_PSHRADD2,                // A10 : pshradd2         r1 = r2, count2, r3
 
    M_LD1,                     // M01 : ld1              r1 = [r3]
    M_LD2,                     // M01 : ld2              r1 = [r3]
    M_LD4,                     // M01 : ld4              r1 = [r3]
    M_LD8,                     // M01 : ld8              r1 = [r3]
    M_LD1_S,                   // M01 : ld1.s            r1 = [r3]
    M_LD2_S,                   // M01 : ld2.s            r1 = [r3]
    M_LD4_S,                   // M01 : ld4.s            r1 = [r3]
    M_LD8_S,                   // M01 : ld8.s            r1 = [r3]
    M_LD1_A,                   // M01 : ld1.a            r1 = [r3]
    M_LD2_A,                   // M01 : ld2.a            r1 = [r3]
    M_LD4_A,                   // M01 : ld4.a            r1 = [r3]
    M_LD8_A,                   // M01 : ld8.a            r1 = [r3]
    M_LD1_SA,                  // M01 : ld1.sa           r1 = [r3]
    M_LD2_SA,                  // M01 : ld2.sa           r1 = [r3]
    M_LD4_SA,                  // M01 : ld4.sa           r1 = [r3]
    M_LD8_SA,                  // M01 : ld8.sa           r1 = [r3]
    M_LD1_BIAS,                // M01 : ld1.bias         r1 = [r3]
    M_LD2_BIAS,                // M01 : ld2.bias         r1 = [r3]
    M_LD4_BIAS,                // M01 : ld4.bias         r1 = [r3]
    M_LD8_BIAS,                // M01 : ld8.bias         r1 = [r3]
    M_LD1_ACQ,                 // M01 : ld1.acq          r1 = [r3]
    M_LD2_ACQ,                 // M01 : ld2.acq          r1 = [r3]
    M_LD4_ACQ,                 // M01 : ld4.acq          r1 = [r3]
    M_LD8_ACQ,                 // M01 : ld8.acq          r1 = [r3]
    M_LD8_FILL,                // M01 : ld8.fill         r1 = [r3]
    M_LD1_C_CLR,               // M01 : ld1.c.clr        r1 = [r3]
    M_LD2_C_CLR,               // M01 : ld2.c.clr        r1 = [r3]
    M_LD4_C_CLR,               // M01 : ld4.c.clr        r1 = [r3]
    M_LD8_C_CLR,               // M01 : ld8.c.clr        r1 = [r3]
    M_LD1_C_NC,                // M01 : ld1.c.nc         r1 = [r3]
    M_LD2_C_NC,                // M01 : ld2.c.nc         r1 = [r3]
    M_LD4_C_NC,                // M01 : ld4.c.nc         r1 = [r3]
    M_LD8_C_NC,                // M01 : ld8.c.nc         r1 = [r3]
    M_LD1_C_CLR_ACQ,           // M01 : ld1.c.clr.acq    r1 = [r3]
    M_LD2_C_CLR_ACQ,           // M01 : ld2.c.clr.acq    r1 = [r3]
    M_LD4_C_CLR_ACQ,           // M01 : ld4.c.clr.acq    r1 = [r3]
    M_LD8_C_CLR_ACQ,           // M01 : ld8.c.clr.acq    r1 = [r3]
    M_LD1_UPD_R,               // M02 : ld1              r1 = [r3], r2
    M_LD2_UPD_R,               // M02 : ld2              r1 = [r3], r2
    M_LD4_UPD_R,               // M02 : ld4              r1 = [r3], r2
    M_LD8_UPD_R,               // M02 : ld8              r1 = [r3], r2
    M_LD1_S_UPD_R,             // M02 : ld1.s            r1 = [r3], r2
    M_LD2_S_UPD_R,             // M02 : ld2.s            r1 = [r3], r2
    M_LD4_S_UPD_R,             // M02 : ld4.s            r1 = [r3], r2
    M_LD8_S_UPD_R,             // M02 : ld8.s            r1 = [r3], r2
    M_LD1_A_UPD_R,             // M02 : ld1.a            r1 = [r3], r2
    M_LD2_A_UPD_R,             // M02 : ld2.a            r1 = [r3], r2
    M_LD4_A_UPD_R,             // M02 : ld4.a            r1 = [r3], r2
    M_LD8_A_UPD_R,             // M02 : ld8.a            r1 = [r3], r2
    M_LD1_SA_UPD_R,            // M02 : ld1.sa           r1 = [r3], r2
    M_LD2_SA_UPD_R,            // M02 : ld2.sa           r1 = [r3], r2
    M_LD4_SA_UPD_R,            // M02 : ld4.sa           r1 = [r3], r2
    M_LD8_SA_UPD_R,            // M02 : ld8.sa           r1 = [r3], r2
    M_LD1_BIAS_UPD_R,          // M02 : ld1.bias         r1 = [r3], r2
    M_LD2_BIAS_UPD_R,          // M02 : ld2.bias         r1 = [r3], r2
    M_LD4_BIAS_UPD_R,          // M02 : ld4.bias         r1 = [r3], r2
    M_LD8_BIAS_UPD_R,          // M02 : ld8.bias         r1 = [r3], r2
    M_LD1_ACQ_UPD_R,           // M02 : ld1.acq          r1 = [r3], r2
    M_LD2_ACQ_UPD_R,           // M02 : ld2.acq          r1 = [r3], r2
    M_LD4_ACQ_UPD_R,           // M02 : ld4.acq          r1 = [r3], r2
    M_LD8_ACQ_UPD_R,           // M02 : ld8.acq          r1 = [r3], r2
    M_LD8_FILL_UPD_R,          // M02 : ld8.fill         r1 = [r3], r2
    M_LD1_C_CLR_UPD_R,         // M02 : ld1.c.clr        r1 = [r3], r2
    M_LD2_C_CLR_UPD_R,         // M02 : ld2.c.clr        r1 = [r3], r2
    M_LD4_C_CLR_UPD_R,         // M02 : ld4.c.clr        r1 = [r3], r2
    M_LD8_C_CLR_UPD_R,         // M02 : ld8.c.clr        r1 = [r3], r2
    M_LD1_C_NC_UPD_R,          // M02 : ld1.c.nc         r1 = [r3], r2
    M_LD2_C_NC_UPD_R,          // M02 : ld2.c.nc         r1 = [r3], r2
    M_LD4_C_NC_UPD_R,          // M02 : ld4.c.nc         r1 = [r3], r2
    M_LD8_C_NC_UPD_R,          // M02 : ld8.c.nc         r1 = [r3], r2
    M_LD1_C_CLR_ACQ_UPD_R,     // M02 : ld1.c.clr.acq    r1 = [r3], r2
    M_LD2_C_CLR_ACQ_UPD_R,     // M02 : ld2.c.clr.acq    r1 = [r3], r2
    M_LD4_C_CLR_ACQ_UPD_R,     // M02 : ld4.c.clr.acq    r1 = [r3], r2
    M_LD8_C_CLR_ACQ_UPD_R,     // M02 : ld8.c.clr.acq    r1 = [r3], r2
    M_LD1_UPD_I,               // M03 : ld1              r1 = [r3], imm9
    M_LD2_UPD_I,               // M03 : ld2              r1 = [r3], imm9
    M_LD4_UPD_I,               // M03 : ld4              r1 = [r3], imm9
    M_LD8_UPD_I,               // M03 : ld8              r1 = [r3], imm9
    M_LD1_S_UPD_I,             // M03 : ld1.s            r1 = [r3], imm9
    M_LD2_S_UPD_I,             // M03 : ld2.s            r1 = [r3], imm9
    M_LD4_S_UPD_I,             // M03 : ld4.s            r1 = [r3], imm9
    M_LD8_S_UPD_I,             // M03 : ld8.s            r1 = [r3], imm9
    M_LD1_A_UPD_I,             // M03 : ld1.a            r1 = [r3], imm9
    M_LD2_A_UPD_I,             // M03 : ld2.a            r1 = [r3], imm9
    M_LD4_A_UPD_I,             // M03 : ld4.a            r1 = [r3], imm9
    M_LD8_A_UPD_I,             // M03 : ld8.a            r1 = [r3], imm9
    M_LD1_SA_UPD_I,            // M03 : ld1.sa           r1 = [r3], imm9
    M_LD2_SA_UPD_I,            // M03 : ld2.sa           r1 = [r3], imm9
    M_LD4_SA_UPD_I,            // M03 : ld4.sa           r1 = [r3], imm9
    M_LD8_SA_UPD_I,            // M03 : ld8.sa           r1 = [r3], imm9
    M_LD1_BIAS_UPD_I,          // M03 : ld1.bias         r1 = [r3], imm9
    M_LD2_BIAS_UPD_I,          // M03 : ld2.bias         r1 = [r3], imm9
    M_LD4_BIAS_UPD_I,          // M03 : ld4.bias         r1 = [r3], imm9
    M_LD8_BIAS_UPD_I,          // M03 : ld8.bias         r1 = [r3], imm9
    M_LD1_ACQ_UPD_I,           // M03 : ld1.acq          r1 = [r3], imm9
    M_LD2_ACQ_UPD_I,           // M03 : ld2.acq          r1 = [r3], imm9
    M_LD4_ACQ_UPD_I,           // M03 : ld4.acq          r1 = [r3], imm9
    M_LD8_ACQ_UPD_I,           // M03 : ld8.acq          r1 = [r3], imm9
    M_LD8_FILL_UPD_I,          // M03 : ld8.fill         r1 = [r3], imm9
    M_LD1_C_CLR_UPD_I,         // M03 : ld1.c.clr        r1 = [r3], imm9
    M_LD2_C_CLR_UPD_I,         // M03 : ld2.c.clr        r1 = [r3], imm9
    M_LD4_C_CLR_UPD_I,         // M03 : ld4.c.clr        r1 = [r3], imm9
    M_LD8_C_CLR_UPD_I,         // M03 : ld8.c.clr        r1 = [r3], imm9
    M_LD1_C_NC_UPD_I,          // M03 : ld1.c.nc         r1 = [r3], imm9
    M_LD2_C_NC_UPD_I,          // M03 : ld2.c.nc         r1 = [r3], imm9
    M_LD4_C_NC_UPD_I,          // M03 : ld4.c.nc         r1 = [r3], imm9
    M_LD8_C_NC_UPD_I,          // M03 : ld8.c.nc         r1 = [r3], imm9
    M_LD1_C_CLR_ACQ_UPD_I,     // M03 : ld1.c.clr.acq    r1 = [r3], imm9
    M_LD2_C_CLR_ACQ_UPD_I,     // M03 : ld2.c.clr.acq    r1 = [r3], imm9
    M_LD4_C_CLR_ACQ_UPD_I,     // M03 : ld4.c.clr.acq    r1 = [r3], imm9
    M_LD8_C_CLR_ACQ_UPD_I,     // M03 : ld8.c.clr.acq    r1 = [r3], imm9
    M_ST1,                     // M04 : st1              [r3] = r1
    M_ST2,                     // M04 : st2              [r3] = r1
    M_ST4,                     // M04 : st4              [r3] = r1
    M_ST8,                     // M04 : st8              [r3] = r1
    M_ST1_REL,                 // M04 : st1.rel          [r3] = r1
    M_ST2_REL,                 // M04 : st2.rel          [r3] = r1
    M_ST4_REL,                 // M04 : st4.rel          [r3] = r1
    M_ST8_REL,                 // M04 : st8.rel          [r3] = r1
    M_ST8_SPILL,               // M04 : st8.spill        [r3] = r1
    M_ST1_UPD_I,               // M05 : st1              [r3] = r1, imm9
    M_ST2_UPD_I,               // M05 : st2              [r3] = r1, imm9
    M_ST4_UPD_I,               // M05 : st4              [r3] = r1, imm9
    M_ST8_UPD_I,               // M05 : st8              [r3] = r1, imm9
    M_ST1_REL_UPD_I,           // M05 : st1.rel          [r3] = r1, imm9
    M_ST2_REL_UPD_I,           // M05 : st2.rel          [r3] = r1, imm9
    M_ST4_REL_UPD_I,           // M05 : st4.rel          [r3] = r1, imm9
    M_ST8_REL_UPD_I,           // M05 : st8.rel          [r3] = r1, imm9
    M_ST8_SPILL_UPD_I,         // M05 : st8.spill        [r3] = r1, imm9
    M_LDFS,                    // M06 : ldfs             f1 = [r3]
    M_LDFD,                    // M06 : ldfd             f1 = [r3]
    M_LDF8,                    // M06 : ldf8             f1 = [r3]
    M_LDFE,                    // M06 : ldfe             f1 = [r3]
    M_LDFS_S,                  // M06 : ldfs.s           f1 = [r3]
    M_LDFD_S,                  // M06 : ldfd.s           f1 = [r3]
    M_LDF8_S,                  // M06 : ldf8.s           f1 = [r3]
    M_LDFE_S,                  // M06 : ldfe.s           f1 = [r3]
    M_LDFS_A,                  // M06 : ldfs.a           f1 = [r3]
    M_LDFD_A,                  // M06 : ldfd.a           f1 = [r3]
    M_LDF8_A,                  // M06 : ldf8.a           f1 = [r3]
    M_LDFE_A,                  // M06 : ldfe.a           f1 = [r3]
    M_LDFS_SA,                 // M06 : ldfs.sa          f1 = [r3]
    M_LDFD_SA,                 // M06 : ldfd.sa          f1 = [r3]
    M_LDF8_SA,                 // M06 : ldf8.sa          f1 = [r3]
    M_LDFE_SA,                 // M06 : ldfe.sa          f1 = [r3]
    M_LDF_FILL,                // M06 : ldf.fill         f1 = [r3]
    M_LDFS_C_CLR,              // M06 : ldfs.c.clr       f1 = [r3]
    M_LDFD_C_CLR,              // M06 : ldfd.c.clr       f1 = [r3]
    M_LDF8_C_CLR,              // M06 : ldf8.c.clr       f1 = [r3]
    M_LDFE_C_CLR,              // M06 : ldfe.c.clr       f1 = [r3]
    M_LDFS_C_NC,               // M06 : ldfs.c.nc        f1 = [r3]
    M_LDFD_C_NC,               // M06 : ldfd.c.nc        f1 = [r3]
    M_LDF8_C_NC,               // M06 : ldf8.c.nc        f1 = [r3]
    M_LDFE_C_NC,               // M06 : ldfe.c.nc        f1 = [r3]
    M_LDFS_UPD_R,              // M07 : ldfs             f1 = [r3], r2
    M_LDFD_UPD_R,              // M07 : ldfd             f1 = [r3], r2
    M_LDF8_UPD_R,              // M07 : ldf8             f1 = [r3], r2
    M_LDFE_UPD_R,              // M07 : ldfe             f1 = [r3], r2
    M_LDFS_S_UPD_R,            // M07 : ldfs.s           f1 = [r3], r2
    M_LDFD_S_UPD_R,            // M07 : ldfd.s           f1 = [r3], r2
    M_LDF8_S_UPD_R,            // M07 : ldf8.s           f1 = [r3], r2
    M_LDFE_S_UPD_R,            // M07 : ldfe.s           f1 = [r3], r2
    M_LDFS_A_UPD_R,            // M07 : ldfs.a           f1 = [r3], r2
    M_LDFD_A_UPD_R,            // M07 : ldfd.a           f1 = [r3], r2
    M_LDF8_A_UPD_R,            // M07 : ldf8.a           f1 = [r3], r2
    M_LDFE_A_UPD_R,            // M07 : ldfe.a           f1 = [r3], r2
    M_LDFS_SA_UPD_R,           // M07 : ldfs.sa          f1 = [r3], r2
    M_LDFD_SA_UPD_R,           // M07 : ldfd.sa          f1 = [r3], r2
    M_LDF8_SA_UPD_R,           // M07 : ldf8.sa          f1 = [r3], r2
    M_LDFE_SA_UPD_R,           // M07 : ldfe.sa          f1 = [r3], r2
    M_LDF_FILL_UPD_R,          // M07 : ldf.fill         f1 = [r3], r2
    M_LDFS_C_CLR_UPD_R,        // M07 : ldfs.c.clr       f1 = [r3], r2
    M_LDFD_C_CLR_UPD_R,        // M07 : ldfd.c.clr       f1 = [r3], r2
    M_LDF8_C_CLR_UPD_R,        // M07 : ldf8.c.clr       f1 = [r3], r2
    M_LDFE_C_CLR_UPD_R,        // M07 : ldfe.c.clr       f1 = [r3], r2
    M_LDFS_C_NC_UPD_R,         // M07 : ldfs.c.nc        f1 = [r3], r2
    M_LDFD_C_NC_UPD_R,         // M07 : ldfd.c.nc        f1 = [r3], r2
    M_LDF8_C_NC_UPD_R,         // M07 : ldf8.c.nc        f1 = [r3], r2
    M_LDFE_C_NC_UPD_R,         // M07 : ldfe.c.nc        f1 = [r3], r2
    M_LDFS_UPD_I,              // M08 : ldfs             f1 = [r3], imm9
    M_LDFD_UPD_I,              // M08 : ldfd             f1 = [r3], imm9
    M_LDF8_UPD_I,              // M08 : ldf8             f1 = [r3], imm9
    M_LDFE_UPD_I,              // M08 : ldfe             f1 = [r3], imm9
    M_LDFS_S_UPD_I,            // M08 : ldfs.s           f1 = [r3], imm9
    M_LDFD_S_UPD_I,            // M08 : ldfd.s           f1 = [r3], imm9
    M_LDF8_S_UPD_I,            // M08 : ldf8.s           f1 = [r3], imm9
    M_LDFE_S_UPD_I,            // M08 : ldfe.s           f1 = [r3], imm9
    M_LDFS_A_UPD_I,            // M08 : ldfs.a           f1 = [r3], imm9
    M_LDFD_A_UPD_I,            // M08 : ldfd.a           f1 = [r3], imm9
    M_LDF8_A_UPD_I,            // M08 : ldf8.a           f1 = [r3], imm9
    M_LDFE_A_UPD_I,            // M08 : ldfe.a           f1 = [r3], imm9
    M_LDFS_SA_UPD_I,           // M08 : ldfs.sa          f1 = [r3], imm9
    M_LDFD_SA_UPD_I,           // M08 : ldfd.sa          f1 = [r3], imm9
    M_LDF8_SA_UPD_I,           // M08 : ldf8.sa          f1 = [r3], imm9
    M_LDFE_SA_UPD_I,           // M08 : ldfe.sa          f1 = [r3], imm9
    M_LDF_FILL_UPD_I,          // M08 : ldf.fill         f1 = [r3], imm9
    M_LDFS_C_CLR_UPD_I,        // M08 : ldfs.c.clr       f1 = [r3], imm9
    M_LDFD_C_CLR_UPD_I,        // M08 : ldfd.c.clr       f1 = [r3], imm9
    M_LDF8_C_CLR_UPD_I,        // M08 : ldf8.c.clr       f1 = [r3], imm9
    M_LDFE_C_CLR_UPD_I,        // M08 : ldfe.c.clr       f1 = [r3], imm9
    M_LDFS_C_NC_UPD_I,         // M08 : ldfs.c.nc        f1 = [r3], imm9
    M_LDFD_C_NC_UPD_I,         // M08 : ldfd.c.nc        f1 = [r3], imm9
    M_LDF8_C_NC_UPD_I,         // M08 : ldf8.c.nc        f1 = [r3], imm9
    M_LDFE_C_NC_UPD_I,         // M08 : ldfe.c.nc        f1 = [r3], imm9
    M_STFS,                    // M09 : stfs             [r3] = f2 
    M_STFD,                    // M09 : stfd             [r3] = f2 
    M_STF8,                    // M09 : stf8             [r3] = f2 
    M_STFE,                    // M09 : stfe             [r3] = f2 
    M_STF_SPILL,               // M09 : stf.fill         [r3] = f2 
    M_STFS_UPD_I,              // M10 : stfs             [r3] = f2, imm9
    M_STFD_UPD_I,              // M10 : stfd             [r3] = f2, imm9 
    M_STF8_UPD_I,              // M10 : stf8             [r3] = f2, imm9 
    M_STFE_UPD_I,              // M10 : stfe             [r3] = f2, imm9 
    M_STF_SPILL_UPD_I,         // M10 : stf.fill         [r3] = f2, imm9
    M_LDFPS,                   // M11 : ldfps            f1, f2 = [r3]
    M_LDFPD,                   // M11 : ldfpd            f1, f2 = [r3]
    M_LDFP8,                   // M11 : ldfp8            f1, f2 = [r3]
    M_LDFPS_S,                 // M11 : ldfps.s          f1, f2 = [r3]
    M_LDFPD_S,                 // M11 : ldfpd.s          f1, f2 = [r3]
    M_LDFP8_S,                 // M11 : ldfp8.s          f1, f2 = [r3]
    M_LDFPS_A,                 // M11 : ldfps.a          f1, f2 = [r3]
    M_LDFPD_A,                 // M11 : ldfpd.a          f1, f2 = [r3]
    M_LDFP8_A,                 // M11 : ldfp8.a          f1, f2 = [r3]
    M_LDFPS_SA,                // M11 : ldfps.sa         f1, f2 = [r3]
    M_LDFPD_SA,                // M11 : ldfpd.sa         f1, f2 = [r3]
    M_LDFP8_SA,                // M11 : ldfp8.sa         f1, f2 = [r3]
    M_LDFPS_C_CLR,             // M11 : ldfps.c.clr      f1, f2 = [r3]
    M_LDFPD_C_CLR,             // M11 : ldfpd.c.clr      f1, f2 = [r3]
    M_LDFP8_C_CLR,             // M11 : ldfp8.c.clr      f1, f2 = [r3]
    M_LDFPS_C_NC,              // M11 : ldfps.c.nc       f1, f2 = [r3]
    M_LDFPD_C_NC,              // M11 : ldfpd.c.nc       f1, f2 = [r3]
    M_LDFP8_C_NC,              // M11 : ldfp8.c.nc       f1, f2 = [r3]
    M_LDFPS_UPD,               // M12 : ldfps            f1, f2 = [r3], 8
    M_LDFPD_UPD,               // M12 : ldfpd            f1, f2 = [r3], 16
    M_LDFP8_UPD,               // M12 : ldfp8            f1, f2 = [r3], 16
    M_LDFPS_S_UPD,             // M12 : ldfps.s          f1, f2 = [r3], 8
    M_LDFPD_S_UPD,             // M12 : ldfpd.s          f1, f2 = [r3], 16
    M_LDFP8_S_UPD,             // M12 : ldfp8.s          f1, f2 = [r3], 16
    M_LDFPS_A_UPD,             // M12 : ldfps.a          f1, f2 = [r3], 8
    M_LDFPD_A_UPD,             // M12 : ldfpd.a          f1, f2 = [r3], 16
    M_LDFP8_A_UPD,             // M12 : ldfp8.a          f1, f2 = [r3], 16
    M_LDFPS_SA_UPD,            // M12 : ldfps.sa         f1, f2 = [r3], 8
    M_LDFPD_SA_UPD,            // M12 : ldfpd.sa         f1, f2 = [r3], 16
    M_LDFP8_SA_UPD,            // M12 : ldfp8.sa         f1, f2 = [r3], 16
    M_LDFPS_C_CLR_UPD,         // M12 : ldfps.c.clr      f1, f2 = [r3], 8
    M_LDFPD_C_CLR_UPD,         // M12 : ldfpd.c.clr      f1, f2 = [r3], 16
    M_LDFP8_C_CLR_UPD,         // M12 : ldfp8.c.clr      f1, f2 = [r3], 16
    M_LDFPS_C_NC_UPD,          // M12 : ldfps.c.nc       f1, f2 = [r3], 8
    M_LDFPD_C_NC_UPD,          // M12 : ldfpd.c.nc       f1, f2 = [r3], 16
    M_LDFP8_C_NC_UPD,          // M12 : ldfp8.c.nc       f1, f2 = [r3], 16
    M_LFETCH,                  // M13 : lfetch            [r3]
    M_LFETCH_EXCL,             // M13 : lfetch.excl       [r3]
    M_LFETCH_FAULT,            // M13 : lfetch.fault      [r3]
    M_LFETCH_FAULT_EXCL,       // M13 : lfetch.fault.excl [r3]
    M_LFETCH_UPD_R,            // M14 : lfetch            [r3],r2
    M_LFETCH_EXCL_UPD_R,       // M14 : lfetch.excl       [r3],r2
    M_LFETCH_FAULT_UPD_R,      // M14 : lfetch.fault      [r3],r2
    M_LFETCH_FAULT_EXCL_UPD_R, // M14 : lfetch.fault.excl [r3],r2
    M_LFETCH_UPD_I,            // M15 : lfetch            [r3],imm9
    M_LFETCH_EXCL_UPD_I,       // M15 : lfetch.excl       [r3],imm9
    M_LFETCH_FAULT_UPD_I,      // M15 : lfetch.fault      [r3],imm9
    M_LFETCH_FAULT_EXCL_UPD_I, // M15 : lfetch.fault.excl [r3],imm9
    M_CMPXCHG1_ACQ,            // M16 : cmpxchg1.acq     r1 = [r3], r2, ar.ccv
    M_CMPXCHG2_ACQ,            // M16 : cmpxchg2.acq     r1 = [r3], r2, ar.ccv
    M_CMPXCHG4_ACQ,            // M16 : cmpxchg4.acq     r1 = [r3], r2, ar.ccv
    M_CMPXCHG8_ACQ,            // M16 : cmpxchg8.acq     r1 = [r3], r2, ar.ccv
    M_CMPXCHG1_REL,            // M16 : cmpxchg1.rel     r1 = [r3], r2, ar.ccv
    M_CMPXCHG2_REL,            // M16 : cmpxchg2.rel     r1 = [r3], r2, ar.ccv
    M_CMPXCHG4_REL,            // M16 : cmpxchg4.rel     r1 = [r3], r2, ar.ccv
    M_CMPXCHG8_REL,            // M16 : cmpxchg8.rel     r1 = [r3], r2, ar.ccv
    M_XCHG1,                   // M16 : xchg1            r1 = [r3], r2
    M_XCHG2,                   // M16 : xchg2            r1 = [r3], r2
    M_XCHG4,                   // M16 : xchg4            r1 = [r3], r2
    M_XCHG8,                   // M16 : xchg8            r1 = [r3], r2
    M_FETCHADD4_ACQ,           // M17 : fetchadd4.acq    r1 = [r3], inc3
    M_FETCHADD8_ACQ,           // M17 : fetchadd8.acq    r1 = [r3], inc3
    M_FETCHADD4_REL,           // M17 : fetchadd4.rel    r1 = [r3], inc3
    M_FETCHADD8_REL,           // M17 : fetchadd8.rel    r1 = [r3], inc3
    M_SETF_SIG,                // M18 : setf.sig         f1 = r2
    M_SETF_EXP,                // M18 : setf.exp         f1 = r2
    M_SETF_S,                  // M18 : setf.s           f1 = r2
    M_SETF_D,                  // M18 : setf.d           f1 = r2
    M_GETF_SIG,                // M19 : getf.sig         r1 = f2
    M_GETF_EXP,                // M19 : getf.exp         r1 = f2
    M_GETF_S,                  // M19 : getf.s           r1 = f2
    M_GETF_D,                  // M19 : getf.d           r1 = f2
    M_CHK_S_M,                 // M20 : chk.s.m          r2, target25
    M_CHK_S,                   // M21 : chk.s            f2, target25
    M_CHK_A_NC_I,              // M22 : chk.a.nc         r1, target25
    M_CHK_A_CLR_I,             // M22 : chk.a.clr        r1, target25
    M_CHK_A_NC_F,              // M23 : chk.a.nc         f1, target25
    M_CHK_A_CLR_F,             // M23 : chk.a.clr        f1, target25
    M_INVALA,                  // M24 : invala
    M_FWB,                     // M24 : fwb
    M_MF,                      // M24 : mf
    M_MF_A,                    // M24 : mf.a
    M_SRLZ_D,                  // M24 : srlz.d
    M_SRLZ_I,                  // M24 : srlz.i
    M_SYNC_I,                  // M24 : sync.i
    M_FLUSHRS,                 // M25 : flushrs
    M_LOADRS,                  // M25 : loadrs
    M_INVALA_E,                // M26 : invala.e         r1
    M_INVALA_E_F,              // M27 : invala.e         f1
    M_FC,                      // M28 : fc               r3
    M_PTC_E,                   // M28 : ptc.e            r3
    M_MOV_TO_AR_R,             // M29 : mov.m            ar3 = r2
    M_MOV_TO_AR_I,             // M30 : mov.m            ar3 = imm8
    M_MOV_FROM_AR,             // M31 : mov.m            r1 = ar3          
    M_MOV_TO_CR,               // M32 : mov              cr2 = r1
    M_MOV_FROM_CR,             // M33 : mov              r1 = cr3
    M_ALLOC,                   // M34 : alloc            r1 = ar.pfs, i,l,o,r
    M_MOV_TO_PSR_L,            // M35 : mov              psr.l  = r2
    M_MOV_TO_PSR_UM,           // M35 : mov              psr.um = r2
    M_MOV_FROM_PSR,            // M36 : mov              r1 = psr 
    M_MOV_FROM_PSR_UM,         // M36 : mov              r1 = psr.um
    M_BREAK,                   // M37 : break.m          immv
    M_NOP,                     // M37 : nop.m            immv
    M_PROBE_R_R,               // M38 : probe.r          r1 = r3, r2
    M_PROBE_W_R,               // M38 : probe.w          r1 = r3, r2
    M_PROBE_R_I,               // M39 : probe.r          r1 = r3, imm2
    M_PROBE_W_I,               // M39 : probe.w          r1 = r3, imm2
    M_PROBE_RW_FAULT,          // M40 : probe.rw.fault   r3, imm2
    M_PROBE_R_FAULT,           // M40 : probe.r.fault    r3, imm2
    M_PROBE_W_FAULT,           // M40 : probe.w.fault    r3, imm2
    M_ITC_D,                   // M41 : itc.d            r2
    M_ITC_I,                   // M41 : itc.i            r2
    M_MOV_TO_IR,               // M42 : mov              ind[r3] = r2
    M_ITR_D,                   // M43 : itr.d            ind[r3] = r2
    M_ITR_C,                   // M43 : itr.i            ind[r3] = r2
    M_MOV_FROM_IR,             // M43 : mov              r1 = ind[r3]
    M_SUM,                     // M44 : sum              imm24
    M_RUM,                     // M44 : rum              imm24
    M_SSM,                     // M44 : ssm              imm24
    M_RSM,                     // M44 : rsm              imm24
    M_PTC_L,                   // M45 : ptc.l            r3, r2
    M_PTC_G,                   // M45 : ptc.g            r3, r2
    M_PTC_GA,                  // M45 : ptc.ga           r3, r2
    M_PTR_D,                   // M45 : ptr.d            r3, r2
    M_PTR_I,                   // M45 : ptr.i            r3, r2
    M_THASH,                   // M46 : thash            r1 = r3
    M_TTAG,                    // M46 : ttag             r1 = r3
    M_TPA,                     // M46 : tpa              r1 = r3
    M_TAK,                     // M46 : tak              r1 = r3

    I_PMPYSHR2,                // I01 : pmpyshr2         r1 = r2, r3, count2
    I_PMPYSHR2_U,              // I01 : pmpyshr2.u       r1 = r2, r3, count2
    I_PMPY2_R,                 // I02 : pmpy2.r          r1 = r2, r3
    I_PMPY2_L,                 // I02 : pmpy2.l          r1 = r2, r3
    I_MIX1_R,                  // I02 : mix1.r           r1 = r2, r3
    I_MIX2_R,                  // I02 : mix2.r           r1 = r2, r3
    I_MIX4_R,                  // I02 : mix4.r           r1 = r2, r3
    I_MIX1_L,                  // I02 : mix1.l           r1 = r2, r3
    I_MIX2_L,                  // I02 : mix2.l           r1 = r2, r3
    I_MIX4_L,                  // I02 : mix4.l           r1 = r2, r3
    I_PACK2_USS,               // I02 : pack2.uss        r1 = r2, r3
    I_PACK2_SSS,               // I02 : pack2.sss        r1 = r2, r3
    I_PACK4_SSS,               // I02 : pack4.sss        r1 = r2, r3
    I_UNPACK1_H,               // I02 : unpack1.h        r1 = r2, r3
    I_UNPACK2_H,               // I02 : unpack2.h        r1 = r2, r3
    I_UNPACK4_H,               // I02 : unpack4.h        r1 = r2, r3
    I_UNPACK1_L,               // I02 : unpack1.l        r1 = r2, r3
    I_UNPACK2_L,               // I02 : unpack2.l        r1 = r2, r3
    I_UNPACK4_L,               // I02 : unpack4.l        r1 = r2, r3
    I_PMIN1_U,                 // I02 : pmin1.u          r1 = r2, r3
    I_PMAX1_U,                 // I02 : pmax1.u          r1 = r2, r3
    I_PMIN2,                   // I02 : pmin2            r1 = r2, r3
    I_PMAX2,                   // I02 : pmax2            r1 = r2, r3
    I_PSAD1,                   // I02 : psad1            r1 = r2, r3
    I_MUX1,                    // I03 : mux1             r1 = r2, mbtype4
    I_MUX2,                    // I04 : mux2             r1 = r2, mbtype8
    I_PSHR2,                   // I05 : pshr2            r1 = r3, r2
    I_PSHR4,                   // I05 : pshr4            r1 = r3, r2
    I_SHR,                     // I05 : shr              r1 = r3, r2
    I_PSHR2_U,                 // I05 : pshr2.u          r1 = r3, r2
    I_PSHR4_U,                 // I05 : pshr4.u          r1 = r3, r2
    I_SHR_U,                   // I05 : shr.u            r1 = r3, r2
    I_PSHR2_I,                 // I06 : pshr2            r1 = r3, count5
    I_PSHR4_I,                 // I06 : pshr4            r1 = r3, count5
    I_PSHR2_U_I,               // I06 : pshr2.u          r1 = r3, count5
    I_PSHR4_U_I,               // I06 : pshr4.u          r1 = r3, count5
    I_PSHL2,                   // I07 : pshl2            r1 = r2, r3
    I_PSHL4,                   // I07 : pshl4            r1 = r2, r3
    I_SHL,                     // I07 : shl              r1 = r2, r3
    I_PSHL2_I,                 // I08 : pshl2            r1 = r2, count5
    I_PSHL4_I,                 // I08 : pshl4            r1 = r2, count5
    I_POPCNT,                  // I09 : popcnt           r1 = r3
    I_SHRP,                    // I10 : shrp             r1 = r2, r3, count6
    I_EXTR,                    // I11 : extr             r1 = r3, pos6, len6
    I_EXTR_U,                  // I11 : extr.u           r1 = r3, pos6, len6
    I_DEP_Z,                   // I12 : dep.z            r1 = r2, pos6, len6
    I_DEP_Z_I,                 // I13 : dep.z            r1 = imm8, pos6, len6
    I_DEP_I,                   // I14 : dep              r1 = imm1,r3,pos6,len6
    I_DEP,                     // I15 : dep              r1 = r2, r3, pos6,len4
    I_TBIT_Z,                  // I16 : tbit.z           p1, p2 = r3, pos6
    I_TBIT_Z_UNC,              // I16 : tbit.z.unc       p1, p2 = r3, pos6
    I_TBIT_Z_AND,              // I16 : tbit.z.and       p1, p2 = r3, pos6
    I_TBIT_NZ_AND,             // I16 : tbit.nz.and      p1, p2 = r3, pos6
    I_TBIT_Z_OR,               // I16 : tbit.z.or        p1, p2 = r3, pos6
    I_TBIT_NZ_OR,              // I16 : tbit.nz.or       p1, p2 = r3, pos6
    I_TBIT_Z_OR_ANDCM,         // I16 : tbit.z.or.andcm  p1, p2 = r3, pos6
    I_TBIT_NZ_OR_ANDCM,        // I16 : tbit.nz.or.andcm p1, p2 = r3, pos6
    I_TNAT_Z,                  // I17 : tnat.z           p1, p2 = r3
    I_TNAT_Z_UNC,              // I17 : tnat.z.unc       p1, p2 = r3
    I_TNAT_Z_AND,              // I17 : tnat.z.and       p1, p2 = r3
    I_TNAT_NZ_AND,             // I17 : tnat.nz.and      p1, p2 = r3
    I_TNAT_Z_OR,               // I17 : tnat.z.or        p1, p2 = r3
    I_TNAT_NZ_OR,              // I17 : tnat.nz.or       p1, p2 = r3
    I_TNAT_Z_OR_ANDCM,         // I17 : tnat.z.or.andcm  p1, p2 = r3
    I_TNAT_NZ_OR_ANDCM,        // I17 : tnat.nz.or.andcm p1, p2 = r3
    I_BREAK,                   // I19 : break.i          immv
    I_NOP,                     // I19 : nop.i            immv
    I_CHK_S_I,                 // I20 : chk.s.i          r2, target25
    I_MOV_TO_B,                // I21 : mov              b1 = r2, tag13
    I_MOV_RET_TO_B,            // I21 : mov.ret          b1 = r2, tag13
    I_MOV_FROM_B,              // I22 : mov              r1 = b2
    I_MOV_TO_PR,               // I23 : mov              pr = r2, mask17
    I_MOV_TO_PR_ROT,           // I24 : mov              pr.rot = imm44
    I_MOV_FROM_IP,             // I25 : mov              r1 = ip
    I_MOV_FROM_PR,             // I25 : mov              r1 = pr
    I_MOV_TO_AR_R,             // I26 : mov.i            ar3 = r2
    I_MOV_TO_AR_I,             // I27 : mov.i            ar3 = imm8
    I_MOV_FROM_AR,             // I28 : mov.i            r1 = ar3       
    I_ZXT1,                    // I29 : zxt1             r1 = r3
    I_ZXT2,                    // I29 : zxt2             r1 = r3
    I_ZXT4,                    // I29 : zxt4             r1 = r3
    I_SXT1,                    // I29 : sxt1             r1 = r3
    I_SXT2,                    // I29 : sxt2             r1 = r3
    I_SXT4,                    // I29 : sxt4             r1 = r3
    I_CZX1_L,                  // I29 : czx1.l           r1 = r3
    I_CZX2_L,                  // I29 : czx2.l           r1 = r3
    I_CZX1_R,                  // I29 : czx1.r           r1 = r3
    I_CZX2_R,                  // I29 : czx2.r           r1 = r3

    F_FMA,                     // F01 : fma              f1 = f3, f4, f2
    F_FMA_S,                   // F01 : fma.s            f1 = f3, f4, f2
    F_FMA_D,                   // F01 : fma.d            f1 = f3, f4, f2
    F_FPMA,                    // F01 : fpma             f1 = f3, f4, f2
    F_FMS,                     // F01 : fms              f1 = f3, f4, f2
    F_FMS_S,                   // F01 : fms.s            f1 = f3, f4, f2
    F_FMS_D,                   // F01 : fms.d            f1 = f3, f4, f2
    F_FPMS,                    // F01 : fpms             f1 = f3, f4, f2
    F_FNMA,                    // F01 : fnma             f1 = f3, f4, f2
    F_FNMA_S,                  // F01 : fnma.s           f1 = f3, f4, f2
    F_FNMA_D,                  // F01 : fnma.d           f1 = f3, f4, f2
    F_FPNMA,                   // F01 : fpnma            f1 = f3, f4, f2
    F_XMA_L,                   // F02 : xma.l            f1 = f3, f4, f2
    F_XMA_H,                   // F02 : xma.h            f1 = f3, f4, f2
    F_XMA_HU,                  // F02 : xma.hu           f1 = f3, f4, f2
    F_FSELECT,                 // F03 : fselect          f1 = f3, f4, f2
    F_FCMP_EQ,                 // F04 : fcmp.eq          p1, p2 = f2, f3
    F_FCMP_LT,                 // F04 : fcmp.lt          p1, p2 = f2, f3
    F_FCMP_LE,                 // F04 : fcmp.le          p1, p2 = f2, f3
    F_FCMP_UNORD,              // F04 : fcmp.unord       p1, p2 = f2, f3
    F_FCMP_EQ_UNC,             // F04 : fcmp.eq_unc      p1, p2 = f2, f3
    F_FCMP_LT_UNC,             // F04 : fcmp.lt_unc      p1, p2 = f2, f3
    F_FCMP_LE_UNC,             // F04 : fcmp.le_unc      p1, p2 = f2, f3
    F_FCMP_UNORD_UNC,          // F04 : fcmp.unord_unc   p1, p2 = f2, f3
    F_FCLASS_M,                // F05 : fclass.m         p1, p2 = f2, fclass9
    F_FCLASS_M_UNC,            // F05 : fclass.m.unc     p1, p2 = f2, fclass9
    F_FRCPA,                   // F06 : frcpa            f1, p2 = f2, f3
    F_FRSQRTA,                 // F07 : frsqrta          f1, p2 = f3
    F_FPRSQRTA,                // F07 : fprsqrta         f1, p2 = f3
    F_FMIN,                    // F08 : fmin             f1 = f2, f3 
    F_FMAX,                    // F08 : fmax             f1 = f2, f3 
    F_FAMIN,                   // F08 : famin            f1 = f2, f3 
    F_FAMAX,                   // F08 : famax            f1 = f2, f3 
    F_FPMIN,                   // F08 : fpmin            f1 = f2, f3 
    F_FPMAX,                   // F08 : fpmax            f1 = f2, f3 
    F_FPAMIN,                  // F08 : fpamin           f1 = f2, f3 
    F_FPAMAX,                  // F08 : fpamax           f1 = f2, f3
    F_FPCMP_EQ,                // F08 : fpcmp.eq         f1 = f2, f3
    F_FPCMP_LT,                // F08 : fpcmp.lt         f1 = f2, f3
    F_FPCMP_LE,                // F08 : fpcmp.le         f1 = f2, f3
    F_FPCMP_UNORD,             // F08 : fpcmp.unord      f1 = f2, f3
    F_FPCMP_NEQ,               // F08 : fpcmp.neq        f1 = f2, f3
    F_FPCMP_NLT,               // F08 : fpcmp.nlt        f1 = f2, f3
    F_FPCMP_NLE,               // F08 : fpcmp.nle        f1 = f2, f3
    F_FPCMP_ORD,               // F08 : fpcmp.ord        f1 = f2, f3
    F_FMERGE_S,                // F09 : fmerge.s         f1 = f2, f3
    F_FMERGE_NS,               // F09 : fmerge.ns        f1 = f2, f3
    F_FMERGE_SE,               // F09 : fmerge.se        f1 = f2, f3
    F_FMIX_LR,                 // F09 : fmix.lr          f1 = f2, f3
    F_FMIX_R,                  // F09 : fmix.r           f1 = f2, f3
    F_FMIX_L,                  // F09 : fmix.l           f1 = f2, f3
    F_FSXT_R,                  // F09 : fsxt.r           f1 = f2, f3
    F_FSXT_L,                  // F09 : fsxt.l           f1 = f2, f3
    F_FPACK,                   // F09 : fpack            f1 = f2, f3
    F_FSWAP,                   // F09 : fswap            f1 = f2, f3
    F_FSWAP_NL,                // F09 : fswap.nl         f1 = f2, f3
    F_FSWAP_NR,                // F09 : fswap.nr         f1 = f2, f3
    F_FAND,                    // F09 : fand             f1 = f2, f3
    F_FANDCM,                  // F09 : fandcm           f1 = f2, f3
    F_FOR,                     // F09 : for              f1 = f2, f3
    F_FXOR,                    // F09 : fxor             f1 = f2, f3
    F_FPMERGE_S,               // F09 : fpmerge.s        f1 = f2, f3
    F_FPMERGE_NS,              // F09 : fpmerge.ns       f1 = f2, f3
    F_FPMERGE_SE,              // F09 : fpmerge.se       f1 = f2, f3
    F_FCVT_FX,                 // F10 : fcvt.fx          f1 = f2
    F_FCVT_FXU,                // F10 : fcvt.fxu         f1 = f2
    F_FCVT_FX_TRUNC,           // F10 : fcvt.fx          f1 = f2
    F_FCVT_FXU_TRUNC,          // F10 : fcvt.fxu.trunc   f1 = f2
    F_FPCVT_FX,                // F10 : fpcvt.fx         f1 = f2
    F_FPCVT_FXU,               // F10 : fpcvt.fxu        f1 = f2
    F_FPCVT_FX_TRUNC,          // F10 : fpcvt.fx         f1 = f2
    F_FPCVT_FXU_TRUNC,         // F10 : fpcvt.fxu.trunc  f1 = f2
    F_FCVT_XF,                 // F11 : fcvt.xf          f1 = f2
    F_FSETC,                   // F12 : fsetc            amask7, omask7
    F_FCLRF,                   // F13 : fclrf
    F_FCHKF,                   // F14 : fchkf            target25
    F_BREAK,                   // F15 : break.f          immv
    F_NOP,                     // F15 : nop.f            immv

    B_COND_IP,                 // B01 : br.cond          <ip relative branch>
    B_WEXIT_IP,                // B01 : br.wexit         <ip relative branch>
    B_WTOP_IP,                 // B01 : br.wtop          <ip relative branch>
    B_CLOOP_IP,                // B02 : br.cloop         <ip relative branch>
    B_CEXIT_IP,                // B02 : br.cexit         <ip relative branch>
    B_CTOP_IP,                 // B02 : br.ctop          <ip relative branch>
    B_CALL_IP,                 // B03 : br.call          b1=<ip rel branch>
    B_COND,                    // B04 : br.cond          b2
    B_IA,                      // B04 : br.ia            b1
    B_RET,                     // B04 : br.ret           b0
    B_CALL,                    // B05 : br.call          b1 = b2
    B_BRP_IP,                  // B06 : brp              <ip rel predict>, tag
    B_BRP,                     // B07 : brp              b2, tag13
    B_BRP_RET,                 // B07 : brp.ret          b2, tag13
    B_COVER,                   // B08 : cover
    B_CLRRRB,                  // B08 : clrrrb
    B_CLRRRB_PR,               // B08 : clrrrb.pr
    B_RFI,                     // B08 : rfi
    B_BSW_0,                   // B08 : bsw.0
    B_BSW_1,                   // B08 : bsw.1
    B_EPC,                     // B08 : epc
    B_BREAK,                   // B09 : break.b          immv
    B_NOP,                     // B09 : nop.b            immv

    X_BREAK,                   // X01 : break.x          immv
    X_NOP,                     // X01 : nop.x            immv
    X_MOVL,                    // X02 : movl             r1 = immv
    X_BRL_COND,                // X03 : brl.cond         target64
    X_BRL_CALL,                // X04 : brl.call         b1 = target64

    OPCODE_MAX                 // leave this one at the end
  };   

  /// move to branch hint
  enum t_mhint   {MSPTK, MNONE, MDPTK};
  /// move and branch predict important completer
  enum t_ihint   {INONE, IIMP};
  /// ip-relative predict hint
  enum t_bphint  {BPSPTK, BPLOOP, BPDPTK, BPEXIT};
  /// load hints
  enum t_ldhint  {LDNONE, LDNT1, LDNTA};
  /// store hints
  enum t_sthint  {STNONE, STNTA};
  /// branch prefetch hint 
  enum t_phint   {FEW, MANY};
  /// branch direction hint
  enum t_bhint   {SPTK, SPNT, DPTK, DPNT};
  /// branch cache deallocation hint
  enum t_chint   {NONE, CLR};
  /// line prefetch hint
  enum t_lfhint  {LFNONE, LFNT1, LFNT2, LFNTA};
  /// floating-point status field completer
  enum t_fpcomp  {S0, S1, S2, S3};
  /// floating point classes
  enum t_fpclass {NAT, QNAN, SNAN, POS, NEG, ZERO, UNORM, NORM, INF};
  /// floating point instruction precision control completer
  enum t_fpipc   {NONEPC, S, D};
}

#endif
