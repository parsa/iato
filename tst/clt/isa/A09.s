// ---------------------------------------------------------------------------
// - A09.s                                                                   -
// - iato isa test suite : A09 group instruction test                         -
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
	
	.text
	.align 16
	.global _start#
	.proc   _start#

_start:
	movl  r3 = 0x1345689134567812
	movl  r4 = 0x9876FFFF54321987
	;; 
	padd1 r14 = r3, r4	// r14 = 0xabbb679088889199
	padd2 r2 = r3, r4	// r2  = 0xabbb689088889199 
	padd4 r16 = r3, r4	// r16 = 0xabbc689088889199
	padd1.sss r17 = r3, r4	// r17 = 0xab7f67907f7f7f99
	padd2.sss r18 = r3, r4	// r18 = 0xabbb68907fff7fff
	padd1.uuu r19 = r3, r4	// r19 = 0xabbbffff88889199
	padd2.uuu r20 = r3, r4	// r20 = 0xabbbffff88889199
	padd1.uus r21 = r3, r4	// r21 = 0x00bb679088889100
	padd2.uus r22 = r3, r4	// r22 = 0x0000689088889199 
	psub1 r23 = r3, r4	// r23 = 0x7bcf6992e0245f8b
	psub2 r24 = r3, r4	// r24 = 0x7acf6892e0245e8b
	psub4 r25 = r3, r4	// r25 = 0x7ace6892e0245e8b
	psub1.sss r26 = r3, r4	// r26 = 0x7bcf6992e0245f7f
	psub2.sss r27 = r3, r4	// r27 = 0x7acf6892e0245e8b 
	psub1.uuu r28 = r3, r4	// r28 = 0x0000000000245f00
	psub2.uuu r29 = r3, r4	// r29 = 0x0000000000005e8b
	psub1.uus r30 = r3, r4	// r30 = 0x7b00699200245f8b 
	psub2.uus r9 = r3, r4	// r09 = 0x7acf689200005e8b
	.endp _start#

	.section ".iato.checker", "a", "progbits"
  	// check r14 = 0xabbb679088889199
  	data8	0x0E01			// gr[14]
  	data8	0xabbb679088889199	// value to check
	// check r2 = 0xabbb689088889199
  	data8	0x0201			// gr[02]
  	data8	0xabbb689088889199	// value to check
	// check r16 = 0xabbc689088889199
  	data8	0x1001			// gr[16]
  	data8	0xabbc689088889199	// value to check	
	// check r17 = 0xab7f67907f7f7f99
  	data8	0x1101			// gr[17]
  	data8	0xab7f67907f7f7f99	// value to check	
	// check r18 = 0xabbb68907fff7fff 
  	data8	0x1201			// gr[18]
  	data8	0xabbb68907fff7fff	// value to check	
	// check r19 = 0xabbbffff88889199
  	data8	0x1301			// gr[19]
  	data8	0xabbbffff88889199	// value to check	
	// check r20 = 0xabbbffff88889199
  	data8	0x1401			// gr[20]
  	data8	0xabbbffff88889199	// value to check	
	// check r21 = 0x00bb679088889100
  	data8	0x1501			// gr[21]
  	data8	0x00bb679088889100	// value to check	
	// check r22 = 0x0000689088889199
  	data8	0x1601			// gr[22]
  	data8	0x0000689088889199	// value to check	
	// check r23 = 0x7bcf6992e0245f8b
  	data8	0x1701			// gr[23]
  	data8	0x7bcf6992e0245f8b	// value to check	
	// check r24 = 0x7acf6892e0245e8b 
  	data8	0x1801			// gr[24]
  	data8	 0x7acf6892e0245e8b	// value to check	
	// check r25 = 0x7ace6892e0245e8b
  	data8	0x1901			// gr[25]
  	data8	0x7ace6892e0245e8b	// value to check	
	// check r26 = 0x7bcf6992e0245f7f
  	data8	0x1A01			// gr[26]
  	data8	0x7bcf6992e0245f7f	// value to check	
	// check r27 = 0x7acf6892e0245e8b	
  	data8	0x1B01			// gr[27]
  	data8	0x7acf6892e0245e8b	// value to check	
	// check r28 = 0x0000000000245f00
  	data8	0x1C01			// gr[28]
  	data8	0x0000000000245f00	// value to check	
	// check r29 = 0x0000000000005e8b
  	data8	0x1D01			// gr[29]
  	data8	0x0000000000005e8b	// value to check	
	// check r30 = 0x7b00699200245f8b
  	data8	0x1E01			// gr[30]
  	data8	 0x7b00699200245f8b	// value to check	
	// check r09 = 0x7acf689200005e8b
  	data8	0x0901			// gr[09]
  	data8	0x7acf689200005e8b	// value to check
