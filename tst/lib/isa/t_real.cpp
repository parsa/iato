// ---------------------------------------------------------------------------
// - t_real.cpp                                                              -
// - iato:isa testers - real class tester                                    -
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

#include "Types.hpp"
#include <Limits.hxx>

int main (int, char**) {
  using namespace std;
  using namespace iato;

  // the magic constant
  const long double PI = 3.1415926535L;
  long double PZERO = 0.0L;
  long double MZERO = -0.0L;

  // create a default real
  t_real r0;
  assert (r0.getexp() == 0x0001FFFFU);
  assert (r0.isnan () == true);

  // test value 0.0
  t_real z = 0.0L;
  assert (z == 0.0L);
  assert (z.isfinite () == true);

  // check for formating
  ostringstream os;
  os << r0;
  assert (os.str () == "nan");
  
  // create a real with a simple number
  t_real r1 = 12.0L;
  assert (r1 == 12.0L);
  assert (r1.repr() == "0x010002c000000000000000");

  // create a real with a simple number
  r1 = 0.75L;
  assert (r1 == 0.75L);
  assert (r1.repr() == "0x00fffec000000000000000");

  // create a real with a double
  t_real r2 = PI;
  assert (r2.getsign () == false);
  assert (r2 == PI);

  // create a real with a real
  t_real r3 = r2;
  assert (r3 == r2);

  // assign a value to a real
  t_real r4; r4 = PI;
  assert (r4 == r2);

  // assign a real to a real
  r4 = r3;
  assert (r4 == r3);

  // cast a real to a double
  long double dl = r3;
  assert (dl == PI);

  // check add
  dl = dl + dl;
  r4 = r3 + r2;
  t_real r5 = 0.0L;
  r5 += r3;
  assert (r4 == dl);
  assert (r5 == PI);
  r5 = r3 + PI;
  assert (r5 == 2 * PI);

  // check sub
  r5 = r5 - r3;
  assert (r5 == PI);
  r5 -= PI;
  assert (r5 == 0.0L);
  r5 = r5 - PI;
  assert (r5.getsign () == true);

  // check mul
  dl = PI*PI;
  r5 = r3 * r2;
  assert (r5 == dl);
  r5 = r3 * PI;
  assert (r5 == dl);
  r5 = PI * r3;
  assert (r5 == dl);

  // ckeck div
  dl = PI / 2.0;
  r5 = PI;
  r5 /= 2.0;
  assert (r5 == dl);

  // change exp field
  r3.setexp (0x00010FF0);
  assert (r3.getexp () == 0x00010FF0U);

  // change sign field
  r3.setsign (true);
  assert (r3.getsign () == true);

  // change significand field
  r3.setsgfd  (0x8000000000010FF0ULL);
  assert (r3.getsgfd () == 0x8000000000010FF0ULL);

  // store and load single float
  r3 = 1.0L;
  assert (r3.getexp  () == 0x0000FFFF);
  assert (r3.getsgfd () == 0x8000000000000000ULL);
  t_byte buf2[t_real::TR_SISZ];
  r3.singlest (buf2);
  r3.singleld (buf2);
  assert  (r3 == 1.0L);

  // store and load double float
  r3 = 1.0L;
  t_byte buf3[t_real::TR_DOSZ];
  r3.doublest (buf3);
  r3.doubleld (buf3);
  assert (r3 == 1.0L);

  // store and load double extended float
  r3 = 1.0L;
  t_byte buf4[t_real::TR_DESZ];
  r3.extendedst (buf4);
  r3.extendedld (buf4);
  assert  (r3 == 1.0L);

  // spill and fill ia reg
  r3 = 1.0L;
  t_byte buf5[t_real::TR_SFSZ];
  r3.spill (buf5);
  r3.fill  (buf5);
  assert   (r3 == 1.0L);

  // test some special result
  r1 = LDI_NINF / PZERO;
  r2 = MZERO / LDI_PINF;
  r3 = iato::LDI_NINF + iato::LDI_PINF;
  r4 = MZERO / LDI_NINF;
  r5 = MZERO / PZERO;
  assert (r1.isninf ());
  assert (r2 == MZERO);
  assert (r3.isnan ());
  assert (r4 == PZERO);
  assert (r5.isnan ());

  // more special operation which have well defined result
  r1 = 1.0L;
  r2 = MZERO;
  r3 = PZERO;
  r1 /= r2;
  assert (r1.isninf ());
  r1 /= r3;
  assert (r1.isinf ());
  r1 = PZERO;
  r2 = iato::LDI_NINF;
  r3 = iato::LDI_PINF;
  r2 += r2;
  r3 += r3;
  r1 *= r3;
  assert (r2.isninf ());
  assert (r3.ispinf ());
  assert (r1.isnan ());

  // success
  return 0;
}
