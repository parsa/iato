// ---------------------------------------------------------------------------
// - t_alat.cpp                                                              -
// - iato:isa testers - alat class tester                                    -
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

#include "Alat.hpp"
#include "Ctx.hpp"

int main () {
  using namespace std;
  using namespace iato;

  Alat alat;
  t_octa addr1 = 0x0000000000080000ULL;
  t_octa addr4 = 0x0000080000000000ULL;
  t_octa addr6 = 0x0000000000000010ULL;
  t_octa addr7 = 0x0000000000000017ULL;
  Rid r1; r1.setreg   (GREG, 10, 10);
  Rid r2; r2.setreg   (GREG, 12, 12);
  Rid r3; r3.setreg   (GREG, 19, 19);
  Rid r4; r4.setreg   (GREG, 8,   8);
  Rid r5; r5.setreg   (GREG, 9,   9);
  Rid r6; r6.setreg   (GREG, 45, 45);
  Rid r7; r7.setreg   (GREG, 23, 23);
  Rid r8; r8.setreg   (GREG, 42, 42);
  Rid r9; r9.setreg   (FREG, 42, 42);
  Rid r10; r10.setreg (GREG, 0, 11);
  Rid r11; r11.setreg (GREG, 0, 13);
  Rid r12; r12.setreg (GREG, 0, 14);
  Rid r13; r13.setreg (GREG, 0, 15);
  Rid r14; r14.setreg (GREG, 0, 16);
  Rid r15; r15.setreg (GREG, 0, 17);
  Rid r16; r16.setreg (FREG, 0, 41); 
  Rid r17; r17.setreg (GREG, 0, 18);
  Rid r18; r18.setreg (GREG, 0, 20);
  Rid r19; r19.setreg (GREG, 0, 21);
  Rid r20; r20.setreg (GREG, 0, 22);
  Rid r21; r21.setreg (GREG, 0, 24);
  Rid r22; r22.setreg (GREG, 0, 25);
  Rid r23; r23.setreg (FREG, 0, 40);
  Rid r24; r24.setreg (GREG, 0, 26);
  Rid r25; r25.setreg (GREG, 0, 27);
  Rid r26; r26.setreg (GREG, 0, 28);
  Rid r27; r27.setreg (GREG, 0, 29);
  Rid r28; r28.setreg (GREG, 0, 30);
  Rid r29; r29.setreg (GREG, 0, 31);
  Rid r30; r30.setreg (FREG, 0, 43);
  Rid r31; r31.setreg (GREG, 0, 32);
  Rid r32; r32.setreg (FREG, 0, 44);
  
  // test randomly if not well initialize
  assert (alat.check (r8) == false);
  assert (alat.check (r7) == false);
  assert (alat.check (r3) == false);
  assert (alat.check (r2) == false);

  // add entries to alat and test if present
  alat.add (addr1, r1, 8);
  alat.add (addr1, r8, 8);
  alat.add (addr4, r7, 4);
  assert (alat.check (r8) == true);
  assert (alat.check (r7) == true);
  assert (alat.check (r1) == true);

  // add 2 new entries
  alat.add (addr7, r2, 1);
  alat.add (addr6, r3, 8);
  alat.add (addr1, r9, 8);
  assert (alat.check (r9) == true);
  assert (alat.check (r8) == true);
  assert (alat.check (r7) == true);
  assert (alat.check (r3) == true);
  assert (alat.check (r2) == true);
  assert (alat.check (r1) == true);

  // direct address conflict
  alat.memupd (addr6, 1);
  alat.memupd (addr6, 2);
  alat.memupd (addr6, 4);
  assert (alat.check (r8) == true);
  assert (alat.check (r7) == true);
  assert (alat.check (r3) == false);
  assert (alat.check (r2) == true);
  assert (alat.check (r1) == true);

  // address conflict by side effet
  alat.memupd (addr6, 8);
  assert (alat.check (r8) == true);
  assert (alat.check (r7) == true);
  assert (alat.check (r3) == false);
  assert (alat.check (r2) == false);
  assert (alat.check (r1) == true);

  // remove one entry
  alat.remove (r8);
  assert (alat.check (r8) == false);
  assert (alat.check (r7) == true);
  assert (alat.check (r3) == false);
  assert (alat.check (r2) == false);
  assert (alat.check (r1) == true);

  // test report the alat and test if alat is full
  assert(alat.isfull() == false);

  // add entries to alat and test if present
  alat.add (addr1, r1, 8);
  alat.add (addr1, r2 , 8);
  alat.add (addr4, r3 , 4);
  assert (alat.check (r1) == true);
  assert (alat.check (r2) == true);
  assert (alat.check (r3) == true); 

  alat.add (addr1, r4, 8);
  alat.add (addr1, r5, 8);
  alat.add (addr4, r6, 4);
  assert (alat.check (r4) == true);
  assert (alat.check (r5) == true);
  assert (alat.check (r6) == true); 
  
  alat.add (addr1, r7, 8);
  alat.add (addr1, r8, 8);
  alat.add (addr4, r9, 4);
  assert (alat.check (r7) == true);
  assert (alat.check (r8) == true);
  assert (alat.check (r9) == true); 

  alat.add (addr1, r10, 8);
  alat.add (addr1, r11, 8);
  alat.add (addr4, r12, 4);
  assert (alat.check (r10) == true);
  assert (alat.check (r11) == true);
  assert (alat.check (r12) == true); 

  alat.add (addr1, r13, 8);
  alat.add (addr1, r14, 8);
  alat.add (addr4, r15, 4);
  assert (alat.check (r13) == true);
  assert (alat.check (r14) == true);
  assert (alat.check (r15) == true); 

  alat.add (addr1, r16, 8);
  alat.add (addr1, r17, 8);
  alat.add (addr4, r18, 4);
  assert (alat.check (r16) == true);
  assert (alat.check (r17) == true);
  assert (alat.check (r18) == true); 

  alat.add (addr1, r19, 8);
  alat.add (addr1, r20, 8);
  alat.add (addr4, r21, 4);
  assert (alat.check (r19) == true);
  assert (alat.check (r20) == true);
  assert (alat.check (r21) == true); 
  
  alat.add (addr1, r22, 8);
  alat.add (addr1, r23, 8);
  alat.add (addr4, r24, 4);
  assert (alat.check (r22) == true);
  assert (alat.check (r23) == true);
  assert (alat.check (r24) == true); 

  alat.add (addr1, r25, 8);
  alat.add (addr1, r26, 8);
  alat.add (addr4, r27, 4);
  assert (alat.check (r25) == true);
  assert (alat.check (r26) == true);
  assert (alat.check (r27) == true); 

  alat.add (addr1, r28, 8);
  alat.add (addr1, r29, 8);
  alat.add (addr4, r30, 4);
  assert (alat.check (r28) == true);
  assert (alat.check (r29) == true);
  assert (alat.check (r30) == true); 

  alat.add (addr1, r31, 8);
  alat.add (addr4, r32, 4);
  assert (alat.check (r31) == true);
  assert (alat.check (r32) == true);
  
  assert(alat.isfull() == true);

  // fill and reset
  alat.add (addr1, r8, 8);
  alat.add (addr7, r2, 1);
  alat.add (addr6, r3, 8);
  assert (alat.check (r8) == true);
  assert (alat.check (r7) == true);
  assert (alat.check (r3) == true);
  assert (alat.check (r2) == true);
  alat.reset ();
  assert (alat.check (r8) == false);
  assert (alat.check (r7) == false);
  assert (alat.check (r3) == false);
  assert (alat.check (r2) == false);

  // test create an alat with a context
  Ctx ctx;
  ctx.setlong ("value", 4);
  Alat alat2 (&ctx);
  assert (ctx.getlong ("value") == 4);
  
  // test create an alat with a context and name
  Alat alat3 (&ctx, "context2");
  assert (alat3.getname () == "context2");

  // build result for check
  alat3.add (addr7, r2, 1);
  assert (alat3.check (r2) == true);
  Result result;
  result.setrid (1, r2);
  result.setoval (1, Result::REG_CHK, 0);
  result.setaclr (1, true);
  Rid ip; ip.setreg (IPRG, 0, 0);
  result.setrid (0, ip);
  result.setoval (0, 1231654);

  // test check
  alat3.check (result, 1);
  assert (alat3.check (r2) == false);  
  assert (result.getrid (1).isvalid () == false);
  assert (result.getrid (0).isvalid () == false);

  // init alat for test
  alat3.add (addr7, r1, 1);
  assert (alat3.check (r1) == true);
  
  // create ld.c.nc result
  Result result2;
  result2.setrid  (2, r1);
  result2.setachk (2, true);
  result2.setaddr (2, Result::REG_LD1, addr7);
  // test alate updated state
  assert (alat3.load(result2, 2) == false);
  assert (alat3.check (r1) == true);
  
  // create ld.c.clr result
  result2.reset ();
  result2.setrid  (2, r1);
  result2.setachk (2, true);
  result2.setaclr (2, true);
  result2.setaddr (2, Result::REG_LD1, addr7);
  // test alate updated state
  assert (alat3.load(result2, 2) == false);
  assert (alat3.check (r1) == false);

  // create ld.c.nc result
  result2.reset ();
  result2.setrid  (2, r1);
  result2.setachk (2, true);
  result2.setaclr (2, false);
  result2.setaddr (2, Result::REG_LD1, addr7);
  // test alate updated state
  assert (alat3.load(result2, 2) == true);
  assert (alat3.check (r1) == true);

  // remove alat entrie for test
  alat3.memupd (addr7, 4); 
  assert (alat3.check (r1) == false);

  // create ld.c.clr result
  result2.reset ();
  result2.setrid  (2, r1);
  result2.setachk (2, true);
  result2.setaclr (2, true);
  result2.setaddr (2, Result::REG_LD1, addr7);
  // test alate updated state
  assert (alat3.load(result2, 2) == true);
  assert (alat3.check (r1) == false);
  
  // success 
  return 0;
}
