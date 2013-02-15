// ---------------------------------------------------------------------------
// - Real.cpp                                                                -
// - iato:isa library - base real type class implementation                  -
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

#include "Real.hpp"
#include "Bits.hpp"
#include "Limits.hxx"

namespace iato {

  // basic constants used for representation
  const t_octa SGF_BOR = 0x8000000000000000ULL;
  const t_quad EXP_MAX = 0x0001FFFFU;
  const t_quad EXP_BOR = 0x0001FFFEU;
  const t_quad EXP_INT = 0x0001003EU;
  const t_quad Z_BIAS  = 16382;
  const t_quad N_BIAS  = 65535;
  const t_quad SIN_MIN = 0x0000FF81U;
  const t_quad SIN_MAX = 0x0001007EU;
  const t_quad DOU_MIN = 0x0000FC01U;
  const t_quad DOU_MAX = 0x000103FEU;
  const t_quad EXT_MIN = 0x0000C001U;
  const t_quad EXT_MAX = 0x00013FFEU;

  // quiet nan, ia representation
  const t_byte IA_QNAN[t_real::TR_IASZ] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0xE0, 0xFF, 0xFF, 0x01};
  const t_byte IA_PINF[t_real::TR_IASZ] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					   0x00, 0x80, 0xFF, 0xFF, 0x01};
  const t_byte IA_NINF[t_real::TR_IASZ] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					   0x00, 0x80, 0xFF, 0xFF, 0x03};
  const t_byte IA_ZERO[t_real::TR_IASZ] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					   0x00, 0x00, 0x00, 0x00, 0x00};
  const t_byte IA_NATV[t_real::TR_IASZ] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0xFE, 0xFF, 0x01};
  const t_byte IA_NAN_INDEFINITE[t_real::TR_IASZ] 
                                        = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0xC0, 0xFF, 0xFF, 0x03};

  // ieee reciprocal table
  static t_word IEEE_RCPA[256] = {
    0x03fc, 0x03f4, 0x03ec, 0x03e4, 0x03dd, 0x03d5, 0x03cd, 0x03c6,
    0x03be, 0x03b7, 0x03af, 0x03a8, 0x03a1, 0x0399, 0x0392, 0x038b,
    0x0384, 0x037d, 0x0376, 0x036f, 0x0368, 0x0361, 0x035b, 0x0354,
    0x034d, 0x0346, 0x0340, 0x0339, 0x0333, 0x032c, 0x0326, 0x0320,
    0x0319, 0x0313, 0x030d, 0x0307, 0x0300, 0x02fa, 0x02f4, 0x02ee,
    0x02e8, 0x02e2, 0x02dc, 0x02d7, 0x02d1, 0x02cb, 0x02c5, 0x02bf,
    0x02ba, 0x02b4, 0x02af, 0x02a9, 0x02a3, 0x029e, 0x0299, 0x0293,
    0x028e, 0x0288, 0x0283, 0x027e, 0x0279, 0x0273, 0x026e, 0x0269,
    0x0264, 0x025f, 0x025a, 0x0255, 0x0250, 0x024b, 0x0246, 0x0241,
    0x023c, 0x0237, 0x0232, 0x022e, 0x0229, 0x0224, 0x021f, 0x021b,
    0x0216, 0x0211, 0x020d, 0x0208, 0x0204, 0x01ff, 0x01fb, 0x01f6,
    0x01f2, 0x01ed, 0x01e9, 0x01e5, 0x01e0, 0x01dc, 0x01d8, 0x01d4,
    0x01cf, 0x01cb, 0x01c7, 0x01c3, 0x01bf, 0x01bb, 0x01b6, 0x01b2,
    0x01ae, 0x01aa, 0x01a6, 0x01a2, 0x019e, 0x019a, 0x0197, 0x0193,
    0x018f, 0x018b, 0x0187, 0x0183, 0x017f, 0x017c, 0x0178, 0x0174,
    0x0171, 0x016d, 0x0169, 0x0166, 0x0162, 0x015e, 0x015b, 0x0157,
    0x0154, 0x0150, 0x014d, 0x0149, 0x0146, 0x0142, 0x013f, 0x013b,
    0x0138, 0x0134, 0x0131, 0x012e, 0x012a, 0x0127, 0x0124, 0x0120,
    0x011d, 0x011a, 0x0117, 0x0113, 0x0110, 0x010d, 0x010a, 0x0107,
    0x0103, 0x0100, 0x00fd, 0x00fa, 0x00f7, 0x00f4, 0x00f1, 0x00ee,
    0x00eb, 0x00e8, 0x00e5, 0x00e2, 0x00df, 0x00dc, 0x00d9, 0x00d6,
    0x00d3, 0x00d0, 0x00cd, 0x00ca, 0x00c8, 0x00c5, 0x00c2, 0x00bf,
    0x00bc, 0x00b9, 0x00b7, 0x00b4, 0x00b1, 0x00ae, 0x00ac, 0x00a9,
    0x00a6, 0x00a4, 0x00a1, 0x009e, 0x009c, 0x0099, 0x0096, 0x0094,
    0x0091, 0x008e, 0x008c, 0x0089, 0x0087, 0x0084, 0x0082, 0x007f,
    0x007c, 0x007a, 0x0077, 0x0075, 0x0073, 0x0070, 0x006e, 0x006b,
    0x0069, 0x0066, 0x0064, 0x0061, 0x005f, 0x005d, 0x005a, 0x0058,
    0x0056, 0x0053, 0x0051, 0x004f, 0x004c, 0x004a, 0x0048, 0x0045,
    0x0043, 0x0041, 0x003f, 0x003c, 0x003a, 0x0038, 0x0036, 0x0033,
    0x0031, 0x002f, 0x002d, 0x002b, 0x0029, 0x0026, 0x0024, 0x0022,
    0x0020, 0x001e, 0x001c, 0x001a, 0x0018, 0x0015, 0x0013, 0x0011,
    0x000f, 0x000d, 0x000b, 0x0009, 0x0007, 0x0005, 0x0003, 0x0001
  };

  // ieee reciprocal square root table
  static t_word IEEE_SQRTA[256] = {
    0x01a5, 0x01a0, 0x019a, 0x0195, 0x018f, 0x018a, 0x0185, 0x0180,
    0x017a, 0x0175, 0x0170, 0x016b, 0x0166, 0x0161, 0x015d, 0x0158,
    0x0153, 0x014e, 0x014a, 0x0145, 0x0140, 0x013c, 0x0138, 0x0133,
    0x012f, 0x012a, 0x0126, 0x0122, 0x011e, 0x011a, 0x0115, 0x0111,
    0x010d, 0x0109, 0x0105, 0x0101, 0x00fd, 0x00fa, 0x00f6, 0x00f2,
    0x00ee, 0x00ea, 0x00e7, 0x00e3, 0x00df, 0x00dc, 0x00d8, 0x00d5,
    0x00d1, 0x00ce, 0x00ca, 0x00c7, 0x00c3, 0x00c0, 0x00bd, 0x00b9,
    0x00b6, 0x00b3, 0x00b0, 0x00ad, 0x00a9, 0x00a6, 0x00a3, 0x00a0,
    0x009d, 0x009a, 0x0097, 0x0094, 0x0091, 0x008e, 0x008b, 0x0088,
    0x0085, 0x0082, 0x007f, 0x007d, 0x007a, 0x0077, 0x0074, 0x0071,
    0x006f, 0x006c, 0x0069, 0x0067, 0x0064, 0x0061, 0x005f, 0x005c,
    0x005a, 0x0057, 0x0054, 0x0052, 0x004f, 0x004d, 0x004a, 0x0048,
    0x0045, 0x0043, 0x0041, 0x003e, 0x003c, 0x003a, 0x0037, 0x0035,
    0x0033, 0x0030, 0x002e, 0x002c, 0x0029, 0x0027, 0x0025, 0x0023,
    0x0020, 0x001e, 0x001c, 0x001a, 0x0018, 0x0016, 0x0014, 0x0011,
    0x000f, 0x000d, 0x000b, 0x0009, 0x0007, 0x0005, 0x0003, 0x0001,
    0x03fc, 0x03f4, 0x03ec, 0x03e5, 0x03dd, 0x03d5, 0x03ce, 0x03c7,
    0x03bf, 0x03b8, 0x03b1, 0x03aa, 0x03a3, 0x039c, 0x0395, 0x038e,
    0x0388, 0x0381, 0x037a, 0x0374, 0x036d, 0x0367, 0x0361, 0x035a,
    0x0354, 0x034e, 0x0348, 0x0342, 0x033c, 0x0336, 0x0330, 0x032b,
    0x0325, 0x031f, 0x031a, 0x0314, 0x030f, 0x0309, 0x0304, 0x02fe,
    0x02f9, 0x02f4, 0x02ee, 0x02e9, 0x02e4, 0x02df, 0x02da, 0x02d5,
    0x02d0, 0x02cb, 0x02c6, 0x02c1, 0x02bd, 0x02b8, 0x02b3, 0x02ae,
    0x02aa, 0x02a5, 0x02a1, 0x029c, 0x0298, 0x0293, 0x028f, 0x028a,
    0x0286, 0x0282, 0x027d, 0x0279, 0x0275, 0x0271, 0x026d, 0x0268,
    0x0264, 0x0260, 0x025c, 0x0258, 0x0254, 0x0250, 0x024c, 0x0249,
    0x0245, 0x0241, 0x023d, 0x0239, 0x0235, 0x0232, 0x022e, 0x022a,
    0x0227, 0x0223, 0x0220, 0x021c, 0x0218, 0x0215, 0x0211, 0x020e,
    0x020a, 0x0207, 0x0204, 0x0200, 0x01fd, 0x01f9, 0x01f6, 0x01f3,
    0x01f0, 0x01ec, 0x01e9, 0x01e6, 0x01e3, 0x01df, 0x01dc, 0x01d9,
    0x01d6, 0x01d3, 0x01d0, 0x01cd, 0x01ca, 0x01c7, 0x01c4, 0x01c1,
    0x01be, 0x01bb, 0x01b8, 0x01b5, 0x01b2, 0x01af, 0x01ac, 0x01aa
  };

  // this procedure returns true if a machine representation is a nan
  static inline bool ma_isnan (const long double x) {
    return (isnan (x) == 0) ? false : true;
  }

  // this procedure returns true if a machine representation is an infinite
  static inline bool ma_ispinf (const long double x) {
    return (x == LDI_PINF);
  }

  // this procedure returns true if a machine representation is a negative
  // infinite
  static inline bool ma_isninf (const long double x) {
    return (x == LDI_NINF);
  }

  // this procedure returns true if a machine representation is an infinite
  static inline bool ma_isinf (const long double x) {
    return (ma_ispinf (x) || ma_isninf (x));
  }

  // this procedure copy one ia buffer to another
  static void copy_ia_buffer (t_byte* dst, const t_byte* src) {
    const t_byte* end = &src[t_real::TR_IASZ];
    while (src < end) *dst++ = *src++;
  }

  // this procedure initialize an ia buffer from a sign, an exponent and a 
  // significand field;
  static void init_ia_buffer (t_byte* dest, const bool sign, const t_quad exp, 
			      const t_octa sgfd) {
    // copy values
    for (long i = 0; i < 8;  i++) dest[i] = (t_byte) (sgfd >> (8 * i));
    for (long i = 8; i < 10; i++) dest[i] = (t_byte) (exp  >> (8 * (i-8)));
    t_byte temp = (t_byte) ((exp >> 16) & 0x01);
    if (sign == true) 
      dest[10] = (0x02 | temp);
    else 
      dest[10] = temp;
  }

  // this procedure returns the sign of an ia representation
  static bool ia_get_sign (const t_byte* buffer) {
    return ((buffer[10] & 0x02 ) == 0x02);
  }

  // this procedure returns the exponent of an ia representation
  static t_quad ia_get_exp (const t_byte* buffer) {
    t_quad qval =  (((t_quad) buffer[8] | (t_quad) buffer[9] << 8 |
		     (t_quad) buffer[10] << 16) & EXP_MAX);
    return qval;
  }

  // this procedure returns the significand of an ia representation
  static t_octa ia_get_sgfd (const t_byte* buffer) {
    union {
      t_byte bval[8];
      t_octa oval;
    };
    for (long i = 0; i < 8; i++) bval[i] = buffer[i];
    return lfixocta (oval);
  }

  // this procedure returns true if ia value is nan otherwise false
  static bool ia_isnan (const t_byte* buffer) {
    if (ia_get_exp (buffer) == EXP_MAX)
      if (ia_get_sgfd (buffer) > SGF_BOR) return true;
    return false;
  }
  
  // this procedure returns true if ia value is positive infinite
  static bool ia_isninf (const t_byte* buffer) {
    if (ia_get_sign (buffer) == true)
      if (ia_get_exp (buffer) == EXP_MAX)
	if (ia_get_sgfd (buffer) == SGF_BOR)
	  return true;
    return false;
  }

  // this procedure returns true if ia value is negative infinite
  static bool ia_ispinf (const t_byte* buffer) {
    if (ia_get_sign (buffer) == false)
      if (ia_get_exp (buffer) == EXP_MAX)
	if (ia_get_sgfd (buffer) == SGF_BOR)
	  return true;
    return false;
  }

  // this procedure returns true if the number is an integer
  static bool ia_isint (const t_byte* buffer) {
    t_quad exp  = ia_get_exp  (buffer);
    bool   sign = ia_get_sign (buffer);
    if ((exp == EXP_INT) && (sign == false)) return true;
    return false;
  }

  // this procedure returns true if ia64 value is positive zero otherwise false
  static bool ia_ispzero (const t_byte* buffer) {
    if (ia_get_sign (buffer) == false)
      if (ia_get_exp (buffer) == QUAD_0)
	if (ia_get_sgfd (buffer) == OCTA_0)
	  return true;
    return false;
  }

  // this procedure returns true if ia64 value is negative zero otherwise false
  static bool ia_isnzero (const t_byte* buffer) {
    if (ia_get_sign (buffer) == true)
      if (ia_get_exp (buffer) == QUAD_0)
	if (ia_get_sgfd (buffer) == OCTA_0)
	  return true;
    return false;
  }

  // this procedure returns true if ia64 value is a psz otherwise false
  static bool ia_ispsz (const t_byte* buffer) {
    t_quad exp  = ia_get_exp  (buffer);
    if (ia_isint (buffer) == true) return false;
    if (ia_get_sgfd (buffer) == OCTA_0)
    {
      if (exp != QUAD_0)
      {
	if (exp < EXP_BOR)
	{
	  return true;
	}
	else if ((exp == EXP_BOR) && (ia_get_sign (buffer) == true))
	{
	  return true;
	}
      }
    }
    return false;
  }

  // this procedure returns true if ia64 value is natval otherwise false
  static bool ia_isnat (const t_byte* buffer) {
    if (ia_get_sgfd (buffer) == OCTA_0)
      if (ia_get_exp (buffer) == EXP_BOR)
	if (ia_get_sign (buffer) == false) return true;
    return false;
  }

  // this procedure returns true if the ia64 value is normalize
  static bool ia_isnormal (const t_byte* buffer) {
    t_quad exp = ia_get_exp (buffer);
    if (exp == 0) return false;
    if (exp == EXP_MAX) return false;
    return ((buffer[7] & 0x80) == 0x80);
  }

  // this procedure convert an ia representation to a machine one
  static long double ia_to_ma (const t_byte* buffer) {
    long double result = LDI_NAN;
    if ((ia_isnan (buffer) == true) || 
	(ia_ispsz (buffer) == true) || 
	(ia_isnat (buffer) == true))      result = LDI_NAN;
    else if (ia_ispinf (buffer) == true)  result = LDI_PINF;
    else if (ia_isninf (buffer) == true)  result = LDI_NINF;
    else if (ia_ispzero (buffer) == true) result = 0.0L;
    else if (ia_isnzero (buffer) == true) result = -0.0L;
    else if (ia_isint  (buffer) == true)  result = ia_get_sgfd (buffer);
    else {
      // init result with significand msb
      t_octa sgfd = ia_get_sgfd (buffer);
      result = bsetget (sgfd, 63) ? 1.0L : 0.0L;
      // loop with other bits
      for (int i = 0; i < 63; i++) {
	if (bsetget (sgfd, 63-i-1) == true) result += pow (2.0L, -i-1);
      }
      // adjust with exponent
      t_quad exp = ia_get_exp (buffer);
      if (exp == QUAD_0) 
	result = result * pow (2.0L, (int) -Z_BIAS);
      else 
	result = result * pow (2.0L, (int) (exp - N_BIAS));
      if (ia_get_sign (buffer) == true) result = -result;
    }
    return result;
  }

  // this procedure return an ia representation from a machine value
  static void ma_to_ia (t_byte* buf, const long double value) {
    if (ma_isnan (value) == true)       copy_ia_buffer (buf, IA_QNAN);
    else if (ma_isninf (value) == true) copy_ia_buffer (buf, IA_NINF);
    else if (ma_ispinf (value) == true) copy_ia_buffer (buf, IA_PINF);
    else if (value == 0.0L) {
      copy_ia_buffer (buf, IA_ZERO);
      if ((2.0L / value) == LDI_NINF) {
	buf[t_real::TR_IASZ-1] = 0x02;
      }
     } else {
      // adjust sign and operating value
      long double  val = value;
      bool        sign = false;
      if (val < 0) {
	sign = true;
	val  = -val;
      }
      // first compute the exponent - non biased - the algorithm operates
      // by searching the exponent in reference to numbers above 1.0 or less
      // than 1.0.
      t_quad exp = QUAD_0;
      if (val >= 1.0L) {
	while (exp < EXP_BOR) {
	  if (pow (2.0L, (int) exp+1) > val) break;
	  exp++;
	}
      } else {
	while (exp < EXP_MAX) {
	  if (val / pow (2.0L, (int) -exp) >= 1.0L) break;
	  exp++;
	}
      }
      // readjust the value to compute the significand
      if (val >= 1.0L) {
	// readjust value
	val /= pow (2.0L, (int) exp);
	// adjust exponent
	exp += N_BIAS;
      } else {
	val /= pow (2.0L, (int) -exp);
	// adjust exponent
	exp = N_BIAS - exp;
      }
      // compute the significand - adjust msb bit if we are above 1.0
      t_octa sgfd = (val >= 1.0L) ? 0x8000000000000000ULL : OCTA_0;
      if (val >= 1.0L) 	val -= 1.0L;
      long double sv = 0.0L;
      for (int i = 0; i < 63; i++) {
	long double ev = sv + pow (2.0L, -i-1);
	if (ev <= val) {
	  sv = ev;
	  sgfd = bsetocta (sgfd, 63-i-1, true);
	}
      }
      // initialize ia buffer
      init_ia_buffer (buf, sign, exp, sgfd);
    }
  }

  // create a default real with a nan value.

  t_real::t_real (void) {
    copy_ia_buffer (d_bval, IA_QNAN);
  }

  // create a real with a machine value

  t_real::t_real (const long double value) {
    ma_to_ia (d_bval, value);
  }

  // create a real with a real

  t_real::t_real (const t_real& that) {
    that.getbval (d_bval);
  }

  // assigment operator

  t_real& t_real::operator = (const long double value) {
    ma_to_ia (d_bval, value);
    return *this;
  }

  // assigment operator

  t_real& t_real::operator = (const t_real& that) {
    if (this == &that) return *this;
    that.getbval (d_bval);
    return *this;
  }

  // add a real with another

  t_real t_real::operator + (const t_real& x) const {
    t_real result = x;
    return (result += *this);
  }

  // add a real with a long double

  t_real operator + (const t_real& x, const long double y) {
    t_real result = y;
    return (result += x);
  }

  // add-assign operator

  t_real& t_real::operator += (const t_real& that) {
    long double rval = ia_to_ma (d_bval);
    rval += ia_to_ma (that.d_bval);
    ma_to_ia (d_bval, rval);
    return *this;
  }

  // sub a real from another

  t_real t_real::operator - (const t_real& x) const {
    t_real result = *this;
    return (result -= x);
  }

  // sub a long double from a real

  t_real operator - (const t_real& x, const long double y) {
    t_real result = x;
    return (result -= y);
  }

  // sub-assign operator

  t_real& t_real::operator -= (const t_real& that) {
    long double rval = ia_to_ma (d_bval);
    rval -= ia_to_ma (that.d_bval);
    ma_to_ia(d_bval, rval);
    return *this;
  }

  // mul a real with another

  t_real t_real::operator * (const t_real& x) const {
    t_real result = x;
    return (result *= *this);
  }

  // mul a real with a long double

  t_real operator * (const t_real& x, const long double y) {
    t_real result = y;
    return (result *= x);
  }

  // mul-assign operator

  t_real& t_real::operator *= (const t_real& x) {
    long double rval = ia_to_ma (d_bval);
    rval *= ia_to_ma (x.d_bval);
    ma_to_ia(d_bval, rval);
    return *this;
  }
 
  // div one real with another

  t_real t_real::operator / (const t_real& x) const {
    t_real result = *this;
    return (result /= x);
  }

  // div one real with a long double

  t_real operator / (const t_real& x, const long double y) {
    t_real result = x;
    return (result /= y);
  }

  // div-assign operator

  t_real& t_real::operator /= (const t_real& x) {
    long double rval = ia_to_ma (d_bval);
    rval /= ia_to_ma (x.d_bval);
    ma_to_ia(d_bval, rval);
    return *this;
  }

  // compare operator for a long double value
  // equal operator

  bool t_real::operator == (const long double value) const {
    return (ia_to_ma (d_bval) == value);
  }

  // equal operator

  bool t_real::operator == (const t_real value) const {
    // For IEEE 754 -0 = +0
    if ((value.getexp () == QUAD_0) && (getexp () == QUAD_0)) return true;
    if ((value.d_bval[0] & 0xF0) != (d_bval[0] & 0xF0)) return false;
    for (long i = 1; i < TR_IASZ; i++)
      if (value.d_bval[i] != d_bval[i]) return false;
    if ((value.isnan () == true) & (isnan () == true)) return false;
    if ((value.isnat () == true) & (isnat () == true)) return false;
    // Is +inf = +inf ??????????? To check in IEEE 754 norm
    return true;
  }

  // less than operator

  bool t_real::operator < (const t_real value) const {
    return (ia_to_ma (d_bval) < ia_to_ma (value.d_bval));
  }

  // less than or equal operator
 
  bool t_real::operator <= (const t_real value) const {
    return ( ia_to_ma (d_bval) <= ia_to_ma (value.d_bval));
  }

  // greater than operator

  bool t_real::operator > (const t_real value) const {
    return (ia_to_ma (d_bval) > ia_to_ma (value.d_bval));
  }

  // greater than or equal operator

  bool t_real::operator >= (const t_real value) const {
    return ( ia_to_ma (d_bval) >= ia_to_ma (value.d_bval));
  }

  // get long double value

  t_real::operator long double (void) const {
    return ia_to_ma (d_bval);
  }

  // print out a real

  std::ostream& operator << (std::ostream& s, const t_real& x) {
    if (x.isnat () == true)       s << "nat";
    else if (x.ispsz  () == true) s << "psz";
    else if (x.isnan  () == true) s << "nan";
    else if (x.ispinf () == true) s << "inf";
    else if (x.isninf () == true) s << "-inf";
    else {
      s << ia_to_ma(x.d_bval);
    }
    return s;
  }

  // get absolute value
  t_real t_real::getabs (void) const {
    t_real result = *this;
    result.setsign (false);
    return result;
  }

  // print a raw representation of that real

  string t_real::repr (void) const {
    ostringstream os;
    os << "0x";
    for (long i = 10; i >= 0; i--)
      os << hex << setw(2) << setfill ('0') << (t_octa) d_bval[i];
    return os.str ();
  }

  // get ia representation

  void t_real::getbval(t_byte* buf) const{
    copy_ia_buffer (buf, d_bval);
  }

  // set real to nat value

  void t_real::setnat (void) {
    copy_ia_buffer (d_bval, IA_NATV);
  }

  // set the real to positive infinity

  void t_real::setpinf (void) {
    copy_ia_buffer (d_bval, IA_PINF);
  }

  // set the real to negative infinity

  void t_real::setninf (void) {
    copy_ia_buffer (d_bval, IA_NINF);
  }

  // set the real to quiet nan indefinite

  void t_real::setnanindefinite (void) {
    copy_ia_buffer (d_bval, IA_NAN_INDEFINITE);
  }

  // return true if ia is nan

  bool t_real::isnan (void) const {
    return ia_isnan (d_bval);
  }

  // return true if ia is pseudo zero
  
  bool t_real::ispsz (void) const {
    return ia_ispsz (d_bval);
  }

  // return true if ia is natval
  
  bool t_real::isnat (void) const {
    return ia_isnat (d_bval);
  }

  // return true if ia is pinf

  bool t_real::ispinf (void) const {
    return ia_ispinf (d_bval);
  }

  // return true if ia is ninf

  bool t_real::isninf (void) const {
    return ia_isninf (d_bval);
  }

  // return true if ia is inf

  bool t_real::isinf (void) const {
    return ia_ispinf (d_bval) || ia_isninf (d_bval);
  }

  // return true if the number is finite

  bool t_real::isfinite (void) const {
    if (isnan () || isinf () || isnat ()) return false;
    return true;
  }

  // return true if the number is an integer

  bool t_real::isint (void) const {
    return ia_isint (d_bval);
  }

  // return true if the number is unnormal
  
  bool t_real::isunorm (void) const {
    return (ia_isnormal (d_bval) == false);
  }

  // return true if the number is normal

  bool t_real::isnorm (void) const {
    return (ia_isnormal (d_bval) == true);
  }

  // return true if the reals are unordered

  bool t_real::unordered (t_real value) const{
    if ((isnan () == true) ||(value.isnan () == true)) return true;
    return false;
  }

  // return the normalized representation

  void t_real::normalize (void) {
    if ((isinf () == true) || (isnan () == true)) return;
    while (ia_isnormal (d_bval) == false){
      t_quad exp = getexp () - 1;
      setexp  (exp);
      if (exp == QUAD_0) {
	setsgfd (OCTA_0);
	break;
      }
      setsgfd (getsgfd () << 1);      
    }
  }

  // return the sign value of the ia representation

  bool t_real::getsign (void) const {
    return ia_get_sign (d_bval);
  }

  // return the exponent value of the ia representation

  t_quad t_real::getexp (void) const {
    return ia_get_exp (d_bval);
  }

  // return the significand value of the ia representation

  t_octa t_real::getsgfd (void) const {
    return ia_get_sgfd (d_bval);
  }

  // set the sign field

  void t_real::setsign (const bool sign) {
    t_octa sgfd = getsgfd ();
    t_quad exp  = getexp  ();
    init_ia_buffer (d_bval, sign, exp, sgfd);
  }

  // set the exponent field

  void t_real::setexp (const t_quad exp) {
    bool   sign = getsign ();
    t_octa sgfd = getsgfd ();
    init_ia_buffer (d_bval, sign, exp, sgfd);
  }

  // set the significand field

  void t_real::setsgfd (const t_octa sgfd) {
    bool   sign = getsign ();
    t_quad exp  = getexp  ();
    init_ia_buffer (d_bval, sign, exp, sgfd);
  }

  // set the significand with a integer machine value

  void t_real::setinteger (const t_octa value) {
    init_ia_buffer (d_bval, false, EXP_INT, value);
  }

  // single precision memory to floating-point register data translation

  void t_real::singleld (const t_byte* buf){
    const t_octa SGFD_MASK   = ~SGF_BOR;
    const t_quad SI_MASK     = 0x00000080;
    const t_quad SPE_EXP     = 0x0000FF81;
    const t_quad LOC_EXP_MAX = 0x000000FF;
    bool   sign = ((buf[TR_SISZ-1] & 0x80) == 0x80);
    t_quad exp  = (((t_quad) (buf[TR_SISZ-1] << 1)) |
		   (((t_quad) buf[2]) >> 7)) & LOC_EXP_MAX;
    t_octa sgfd = ((((t_octa) buf[0]) << 40) | (((t_octa) buf[1]) << 48) |
		   (((t_octa) buf[2]) << 56)) & SGFD_MASK;
    if (exp == LOC_EXP_MAX) {
      exp  = EXP_MAX;
      sgfd = ~SGFD_MASK | sgfd;
    }
    else if (exp == QUAD_0) {
      if (sgfd != OCTA_0) exp = SPE_EXP;
    }
    else {
      sgfd = ~SGFD_MASK | sgfd;
      if ((exp & SI_MASK) == SI_MASK)
	exp = (exp & (~SI_MASK)) | 0x00010000;
      else exp = exp | 0x0000FF80;
    }
    init_ia_buffer (d_bval, sign, exp, sgfd);
  }

  // double precision memory to floating-point register data translation

  void t_real::doubleld (const t_byte* buf) {
    const t_octa SGFD_MASK   = ~SGF_BOR;
    const t_quad DO_MASK     = 0x00000400;
    const t_quad SPE_EXP     = 0x0000FC01;
    const t_quad LOC_EXP_MAX = 0x000007FF;
    bool   sign = ((buf[TR_DOSZ-1] & 0x80) == 0x80);
    t_quad exp  = ((((t_quad) buf[TR_DOSZ-1]) << 4) |
		   (((t_quad) buf[TR_DOSZ-2]) >> 4)) & LOC_EXP_MAX;
    t_octa sgfd = ((((t_octa) buf[0]) << 11) |(((t_octa) buf[1]) << 19) |
		   (((t_octa) buf[2]) << 27) |(((t_octa) buf[3]) << 35) |
		   (((t_octa) buf[4]) << 43) |(((t_octa) buf[5]) << 51) |
		   (((t_octa) buf[6]) << 59) ) & SGFD_MASK;
    if (exp == LOC_EXP_MAX) {
      exp  = EXP_MAX;
      sgfd = SGF_BOR | sgfd;
    }
    else if (exp == QUAD_0) {
      if (sgfd != OCTA_0) exp = SPE_EXP;
    }
    else {
      sgfd = SGF_BOR | sgfd;
      if ((exp & DO_MASK) == DO_MASK)
	exp = (exp & (~DO_MASK)) | 0x00010000;
      else exp = exp | 0x0000FC00;
    }
    init_ia_buffer (d_bval, sign, exp, sgfd);
  }

  // double extended precision memory to floating-point register translation

  void t_real::extendedld (const t_byte* buf) {
    const t_quad EX_MASK     = 0x00004000;
    const t_quad LOC_EXP_MAX = 0x00007FFF;
    bool   sign = ((buf[TR_DESZ-1] & 0x80) == 0x80);
    t_quad exp  = ((((t_quad) buf[TR_DESZ-1]) << 8) |
		   (((t_quad) buf[TR_DESZ-2]))) & LOC_EXP_MAX;
    t_octa sgfd = OCTA_0;
    for (long i = 0; i < TR_DOSZ; i++) sgfd |= ((t_octa) buf[i]) << (8*i);
    if (exp == LOC_EXP_MAX) exp = EXP_MAX;
    else if (exp != QUAD_0) {
      if ((exp & EX_MASK) == EX_MASK)
	exp = (exp & (~EX_MASK)) | 0x00010000;
      else exp = exp | 0x0000C000;
    }
    init_ia_buffer (d_bval, sign, exp, sgfd);
  }

  // integer memory to floating point transfer

  void t_real::integerld (const t_byte* buf) {
    union{
      t_octa sgfd;
      t_byte bval[8];
    };
    for (long i = 0; i < TR_DOSZ; i++) bval[i]= buf[i];
    init_ia_buffer (d_bval, false, EXP_INT, lfixocta (sgfd));
  }

  // fill floating point transfer

  void t_real::fill (const t_byte* buf) {
    for (long i = 0; i < TR_IASZ; i++) d_bval[i] = buf[i];
  }

  // floating point register to single data memory transfer

  void t_real::singlest (t_byte* buf) const {
    const t_byte B_MASK = 0x80;
    buf[0] = d_bval[5];
    buf[1] = d_bval[6];
    buf[2] = d_bval[7] & (~B_MASK);
    if ( (d_bval[7] & B_MASK) == B_MASK) {
      t_byte x;
      t_quad exp = getexp ();
      if ((exp & QUAD_1) == QUAD_1) buf[2] |= B_MASK;
      x = exp;
      if ((exp & 0x00010000) == 0x00010000) x |= B_MASK;
      else x &= (~B_MASK);
      buf[3] = x >> 1;
    }
    else buf[3] = 0x00;
    if (getsign () == true) buf[3] |= B_MASK;
  }

  // floating point register to double data memory transfer

  void t_real::doublest (t_byte* buf) const {
    const t_byte B_MASK = 0x80;
    for (long i=0; i < (TR_DOSZ - 2); i++) {
      buf[i] = (d_bval[1+i] >> 3) | (d_bval[2+i] << 5);
    }
    buf[TR_DOSZ - 2] = (d_bval[7] & (~B_MASK)) >> 3;
    if ( (d_bval[7] & B_MASK) == B_MASK) {
      t_byte x;
      t_quad exp = getexp ();
      buf [TR_DOSZ - 2] |= (t_byte) exp << 4;
      x = exp >> 3;
      if ((exp & 0x00010000) == 0x00010000) x |= B_MASK;
      else x &= (~B_MASK);
      buf[TR_DOSZ - 1] = (x >> 1);
    }
    else buf[TR_DOSZ - 1] = 0x00;
    if (getsign () == true) buf[TR_DOSZ - 1] |= B_MASK;
  }

  // floating point register to double extended data memory transfer

  void t_real::extendedst (t_byte* buf) const {
    for (long i = 0; i < TR_DESZ; i++) {
      buf[i] = d_bval[i];
    }
    buf[TR_DESZ - 1] &= 0x3F;
    buf[TR_DESZ - 1] |= d_bval[TR_IASZ - 1] << 6;
  }

  // floating point register to integer memory transfer

  void t_real::integerst (t_byte* buf) const {
     for (long i = 0; i < TR_DOSZ; i++) {
      buf[i] = d_bval[i];
    }
  }

  // floating point register to memory spilling

  void t_real::spill (t_byte* buf) const {
    for (long i = 0; i < TR_IASZ; i++) buf[i] = d_bval[i];
    for (long i = TR_IASZ; i < TR_SFSZ; i++) buf[i] = 0x00;
  }

  // compute the ieee reciprocal

  t_real t_real::rcpa (void) const {
    // get reciprocal index
    t_byte index = (t_byte) (getsgfd () >> 55);
    // compute new significand and exponent
    t_octa sgfd = (OCTA_1 << 63) | (((t_octa) IEEE_RCPA[index]) << 53);
    t_quad exp  = 0x0001FFFF - 2 - getexp ();
    // build result
    t_real result;
    result.setsgfd (sgfd);
    result.setexp  (exp);
    result.setsign (getsign ());
    return result;
  }

  // compute the ieee reciprocal square root

  t_real t_real::rsqrt (void) const {
    // get reciprocal index
    t_byte index = (((t_byte) (getsgfd () >> 56)) & 0x7F) | 
      ((t_byte) getexp () << 7);
    // compute new significand and exponent
    t_octa sgfd = SIGN_8 | (((t_octa) IEEE_SQRTA[index]) << 53);
    t_quad exp  = 0x0000FFFE - ((getexp () - 0x0000FFFF) >> 1);
    // build result
    t_real result;
    result.setsgfd (sgfd);
    result.setexp  (exp);
    result.setsign (false);
    return result;
  }

  /// convert the infinite precision value into the selected model
  void t_real::convert (const t_sgfdp sp, const t_expr er, const t_byte rc) {
    if (isnan () == true) return;
    if (isnat () == true) return;
    if (isinf () == true) return;
    t_quad exp = getexp ();
    switch (er) {
    case EXP8B:
      if (exp < SIN_MIN) {
	setexp  (QUAD_0);
	setsgfd (OCTA_0);
	return;
      }
      else if (exp > SIN_MAX) {
	setexp  (EXP_MAX);
	setsgfd (SGF_BOR);
	return;
      }
      break;

    case EXP11B:
      if (exp < DOU_MIN) {
	setexp  (QUAD_0);
	setsgfd (OCTA_0);
	return;
      }
      else if (exp > DOU_MAX) {
	setexp  (EXP_MAX);
	setsgfd (SGF_BOR);
	return;
      }
      break;

    case EXP15B:
      if (exp < EXT_MIN) {
	setexp  (QUAD_0);
	setsgfd (OCTA_0);
	return;
      }
      else if (exp > EXT_MAX) {
	setexp  (EXP_MAX);
	setsgfd (SGF_BOR);
	return;
      }
      break;
    case EXP17B:
      break;
    }
    if (sp == SGFD24B) tosingle (rc, er);      
    if (sp == SGFD53B) todouble (rc, er);
  }

  /// round the real to single according to rounding mode

  void t_real::tosingle (const t_byte rc, const t_expr er) {
    t_quad exp = getexp  ();
    // calculate round value of the mantissa according to rc
    // and eventualy the new exponent
    if (((rc == 0x00) & ((d_bval[4] & 0x80) == 0x80)) || (rc == 0x02)) {
      union{
	t_octa oval;
	t_byte bval[8];
      };
      oval = OCTA_0;
      for (long i = 5; i < 8; i++) bval[i] = d_bval[i];
      oval = lfixocta (oval);
      if (oval == 0xFFFFFF0000000000ULL){
	exp = exp + 1;
	if ((exp > SIN_MAX) && (er == EXP8B)) setexp (EXP_MAX);
	else setexp (exp);
	setsgfd (SGF_BOR);
	return;
      } else oval = lfixocta (oval + 0x0000010000000000ULL);
      for (long i = 5; i < 8; i++) d_bval[i] = bval[i];
      }
    // truncate the mantissa
    for (long i = 0; i < 5; i++) d_bval[i] = BYTE_0;
  }

  /// round the real to single according to rounding mode
  
  void t_real::todouble (const t_byte rc, const t_expr er) {
    t_quad exp = getexp  ();
    // calculate round value of the mantissa  according to rc
    if (((rc == 0x00) & ((d_bval[1] & 0x04) == (0x04))) || (rc == 0x02)) {
      union{
	t_octa oval;
	t_byte bval[8];
      };
      oval = OCTA_0;
      bval[1] = d_bval[1] & 0xF8;
      for (long i = 2; i < 8; i++) bval[i] = d_bval[i];
      oval = lfixocta (oval);
      if (oval == 0xFFFFFFFFFFFFF800ULL){
	exp = exp + 1;
	if ((exp > DOU_MAX) && (er == EXP11B)) setexp  (EXP_MAX);
	else setexp (exp);
	setsgfd (SGF_BOR);
	return;
      } else oval = lfixocta (oval + 0x0000000000000800ULL);
      for (long i = 1; i < 8; i++) d_bval[i] = bval[i];
    }
    // truncate the mantissa
    d_bval[0] = BYTE_0;
    d_bval[1] &= 0xF8;
  }
}
