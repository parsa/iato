// ---------------------------------------------------------------------------
// - t_bits.cpp                                                              -
// - iato:isa testers - bits class tester                                    -
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


#include "Bits.hpp"

int main () {
  using namespace std;
  using namespace iato;
  
  // test swap bites in a word
  union {
    t_byte bval[2];
    t_word value;
  };
  value = 0x0001U;
  value = swapword (value);
  assert ( (bval[1] == 0x01) && (bval[0] == 0x00) );
  
  // test swap bites in a quad
  union {
    t_byte qval[4];
    t_quad value2;
  };
  value2 =  0x00000001U;
  value2 = swapquad (value2);
  assert ((qval[3] == 0x01) && (qval[2] == 0x00) && 
	  (qval[1] == 0x00) && (qval[0] == 0x00) == true );
  
  // test swap bites in a octa
  union {
    t_byte oval[8];
    t_octa value3;
  };
  value3 = 0x0000000000000001ULL;
  value3 = swapocta (value3);
  assert ((oval[7] == 0x01) && (oval[6] == 0x00) && 
	  (oval[5] == 0x00) && (oval[4] == 0x00) && 
	  (oval[3] == 0x00) && (oval[2] == 0x00) && 
	  (oval[1] == 0x00) && (oval[0] == 0x00) == true) ;
  

  // test fix a word in little endian form
  t_word value4 = 0x0001U;
  value4 = lfixword ( value4 );
  assert ( value4 == value4 );

  // test fix a quad in little endian form
  t_quad value5 = 0x00000001U;
  value5 = lfixquad ( value5 );
  assert ( value5 == value5 );

 // test fix an octa  in little endian form
  t_octa value6 = 0x0000000000000001ULL;
  value6 = lfixocta ( value6 );
  assert ( value6 == value6 );

  // test set a bit in an octa at a certain position
  value6 = bsetocta ( 0x0000000000000000ULL,  0, 1 );
  assert ( (value6 & OCTA_1) == OCTA_1 );

  // test get a bit at a certain position in an octa
  assert ( bsetget ( value6, 0) == true );

  // test get a bit at a certain position in a byte
  t_byte value7 = 0x01U;
  assert ( bsetget ( value7, 0) == true );

  // success
  return 0;
}
