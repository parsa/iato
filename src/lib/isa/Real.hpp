// ---------------------------------------------------------------------------
// - Real.hpp                                                                -
// - iato:isa library - base real type class definition                      -
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

#ifndef  IATO_REAL_HPP
#define  IATO_REAL_HPP

#ifndef  IATO_CONFIG_HXX
#include "Config.hxx"
#endif

namespace iato {
  using namespace std;

  /// The t_real class is the implementation of floating point 
  /// representations. Two representations are used : One is the IA64 ISA 
  /// representation, the other is the machine long double representation.
  /// Methods to cast from one representation to another are menber of the 
  /// class.

  class t_real {
  public:
    /// the ia representation size
    static const long TR_IASZ = 11;
    /// the single representation size
    static const long TR_SISZ = 4;
    /// the long double representation size
    static const long TR_DOSZ = 8;
    /// the double extended representation size
    static const long TR_DESZ = 10;
    /// the spill fill representation size
    static const long TR_SFSZ = 16;
    /// the significand precision
    enum t_sgfdp {
      SGFD24B,
      SGFD53B,
      SGFD64B
    };
    /// the exponent range
    enum t_expr {
      EXP8B,
      EXP11B,
      EXP15B,
      EXP17B
    };

  private:
    /// the ia64 isa representation (little endian)
    t_byte d_bval[TR_IASZ];

  public:
    /// create a default real with a quiet nan value. 
    t_real (void);

    /// create a real with a machine value
    /// @param value the value to use
    t_real (const long double value);

    /// copy construct a real with a real
    /// @param that the real to copy
    t_real (const t_real& that);

    /// assign a machine value to this real
    /// @param value the value to assign
    t_real& operator = (const long double value);

    /// assign a real to this one
    /// @param that the real to assign
    t_real& operator = (const t_real& that);

    /// add one real with another one
    /// @param x the real operand
    t_real operator + (const t_real& x) const;

    /// add one real with a machine one
    /// @param x the real operand
    /// @param y the machine operand
    friend t_real operator + (const t_real& x, const long double y);

    /// add this real with another one
    /// @param that the real to add
    t_real& operator += (const t_real& that);

    /// substract one real with another one
    /// @param x the real operand to sub
    t_real operator - (const t_real& x) const;

    /// substract one real with a machine one
    /// @param x the real operand
    /// @param y the machine operand
    friend t_real operator - (const t_real& x, const long double y);

    /// substract this real with another one
    /// @param that the real to substract
    t_real& operator -= (const t_real& that);

    /// multiply one real with another one
    /// @param x the real operand
    t_real operator * (const t_real& x) const;

    /// multiply one real with a machine one
    /// @param x the real operand
    /// @param y the machine operand
    friend t_real operator * (const t_real& x, const long double y);

    /// multiply this real with another one
    /// @param x the real to multiply with
    t_real& operator *= (const t_real& x);

    /// divide one real with another one
    /// @param x the real operand
    t_real operator / (const t_real& x) const;

    /// divide one real with a machine one
    /// @param x the real operand
    /// @param y the machine operand
    friend t_real operator / (const t_real& x, const long double y);

    /// divide this real with another one
    /// @param x the real to divide with
    t_real& operator /= (const t_real& x);

    /// compare a real with a machine value; equal operator
    /// @param value the value to compare
    bool operator == (const long double value) const;

    /// compare a real with another one; equal operator
    /// @param value the real to compare
    bool operator == (const t_real value) const;

    /// compare a real with another one; less than operator
    /// @param value the real to compare
    bool operator < (const t_real value) const;

    /// compare a real with another one; less than or equal operator
    /// @param value the real to compare
    bool operator <= (const t_real value) const;

    /// compare a real with another one; greater than operator
    /// @param value the real to compare
    bool operator > (const t_real value) const;

    /// compare a real with another one; greater than or equal operator
    /// @param value the real to compare
    bool operator >= (const t_real value) const;

    /// @return a machine value representation
    operator long double (void) const;

    /// @return the string representation of the real
    friend ostream& operator << (ostream& s, const t_real& x);

    /// @return the absolute value
    t_real getabs (void) const;

    /// @return a raw representation of that real
    string repr (void) const;

    /// get the ia byte representation
    /// @param buf the destination buffer 
    void getbval(t_byte* buf) const;

    /// set the real value to nat
    void setnat (void);

    /// set the real value to positive infinity
    void setpinf (void);

    /// set the real value to negative infinity
    void setninf (void);

    /// set the real to quiet nan indefinite
    void setnanindefinite (void);

    /// @return true if the number is nan
    bool isnan (void) const;

    /// @return true if the number is psz
    bool ispsz (void) const;

    /// @return true if the number is nat
    bool isnat (void) const;

    /// @return true if the number if positive infinity
    bool ispinf (void) const;

    /// @return true if the number if negative infinity
    bool isninf (void) const;

    /// @return true if the number if positive or negative infinity
    bool isinf (void) const;

    /// @return true if the number is finite
    bool isfinite (void) const;

    /// @return true if the number is an integer
    bool isint (void) const;

    /// @return true if the number is unnormal
    bool isunorm (void) const;

    /// @return true if the number is normal
    bool isnorm (void) const;

    /// @return true if the reals are unordered
    bool unordered (const t_real value) const;

    /// normalize this real
    void normalize (void);

    /// @return the ia representation sign value
    bool getsign  (void) const;

    /// @return the ia representation exponent value
    t_quad getexp (void) const;

    /// @return the ia representation significand value
    t_octa getsgfd (void) const;

    /// set the sign field
    /// @param sign the sign value
    void setsign (const bool sign);

    /// set the exponent field
    /// @param exp the exponent value
    void setexp (const t_quad exp);

    /// set the significand field
    /// @param sgfd the significand value
    void setsgfd (const t_octa sgfd);

    /// set the ia representation with an octa value
    /// @param value the octa value to use
    void setinteger (const t_octa value);

    /// single precision memory to floating-point register data translation
    /// @param buf the 4 bytes single precision source buffer, 
    void singleld (const t_byte* buf);

    /// double precision memory to floating-point register data translation
    /// @param buf the 8 bytes double precision source buffer, 
    void doubleld (const t_byte* buf);

    /// double extended precision memory to floating-point register data 
    /// @param buf the 10 bytes double precision source buffer, 
    void extendedld (const t_byte* buf);

    /// integer memory to floating-point register data translation
    /// @param buf the 8 bytes double precision source buffer, 
    void integerld (const t_byte* buf);

    /// fill floating-point register data translation.
    /// @param buf the 16 bytes double precision source buffer, 
    void fill (const t_byte* buf);

    /// floating-point register to single data memory translation
    /// @param dst the 4 bytes memory to store in
    void singlest (t_byte* dst) const;

    /// floating-point register to double data memory translation
    /// @param dst the 8 bytes memory to store in
    void doublest (t_byte* dst) const;

    /// floating-point register to double extended data memory translation
    /// @param dst the 10 bytes memory to store in
    void extendedst (t_byte* dst) const;

    /// floating-point register to integer data memory translation
    /// @param dst the 8 bytes memory to store in
    void integerst (t_byte* dst) const;

    /// floating-point register to data memory translation
    /// @param dst the 16 bytes memory to store in
    void spill (t_byte* dst) const;

    /// @return the ieee reciprocal
    t_real rcpa (void) const;

    /// @return the ieee reciprocal square root approximation
    t_real rsqrt (void) const;

    /// convert the infinite precision value into the selected model
    /// @param sp the significand precision
    /// @param er the exponent range
    /// @param rc the rounding control mode
    void convert (const t_sgfdp sp, const t_expr er, const t_byte rc);

  private:
    /// round the real to single according to rounding mode
    /// @param rc the rounding control mode
    /// @param er the exponent range
    void tosingle (const t_byte rc, const t_expr er);

    /// round the real to double according to rounding mode
    /// @param rc the rounding control mode
    /// @param er the exponent range
    void todouble (const t_byte rc, const t_expr er);
  };
}

#endif
