// ---------------------------------------------------------------------------
// - A9b.s                                                                   -
// - iato isa test suite : A09 group instruction test                        -
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
	movl  r3 = 0x1234567891234567
	movl  r4 = 0x987FF65432198767
	movl  r5 = 0x7fff523698f5efff
	;;
	pavg1     r9 = r3, r4	// r09 = 0x5559a666611e6667
	pavg2     r14 = r3, r4	// r14 = 0x5559a666619e6667
	pavg1.raz r25 = r3, r4	// r25 = 0x555aa666621e6667
	pavg2.raz r16 = r3, r4	// r16 = 0x555aa666619e6667
	pavgsub1  r17 = r3, r4	// r17 = 0xbddbb0122f05df00
	pavgsub2  r18 = r3, r4	// r18 = 0xbcdbb0122f85df00
	pcmp1.eq  r19 = r3, r4	// r19 = 0xff
	pcmp2.eq  r20 = r3, r4	// r20 = 0x0
	pcmp4.eq  r21 = r3, r4	// r21 = 0x0
	pcmp1.gt  r22 = r3, r4	// r22 = 0xff00ffff00ffff00
	pcmp2.gt  r23 = r3, r4	// r23 = 0xffffffff0000ffff
	pcmp4.gt  r24 = r3, r4	// r24 = 0xffffffff00000000
	padd1.uus r26 = r5, r3
	padd2.uus r27 = r5, r3
	psub1.uus r28 = r5, r3
	psub2.uus r29 = r5, r3
	.endp _start#

	.section ".iato.checker", "a", "progbits"
	// check r09 = 0x5559a666611e6667
  	data8	0x0901			// gr[09]
  	data8	0x5559a666611e6667	// value to check
  	// check r14 = 0x5559a666619e6667
  	data8	0x0E01			// gr[14]
  	data8	0x5559a666619e6667 	// value to check
	// check r16 = 0x555aa666619e6667
  	data8	0x1001			// gr[16]
  	data8	0x555aa666619e6667	// value to check	
	// check r17 = 0xbddbb0122f05df00 
  	data8	0x1101			// gr[17]
  	data8	0xbddbb0122f05df00 	// value to check	
	// check r18 = 0xbcdbb0122f85df00
  	data8	0x1201			// gr[18]
  	data8	0xbcdbb0122f85df00	// value to check	
	// check r19 = 0xff
  	data8	0x1301			// gr[19]
  	data8	0xff			// value to check	
	// check r20 = 0x0
  	data8	0x1401			// gr[20]
  	data8	0x0			// value to check	
	// check r21 = 0x0
  	data8	0x1501			// gr[21]
  	data8	0x0			// value to check	
	// check r22 = 0xff00ffff00ffff00
  	data8	0x1601			// gr[22]
  	data8	0xff00ffff00ffff00	// value to check	
	// check r23 = 0xffffffff0000ffff
  	data8	0x1701			// gr[23]
  	data8	0xffffffff0000ffff	// value to check	
	// check r24 = 0xffffffff00000000
  	data8	0x1801			// gr[24]
  	data8	0xffffffff00000000	// value to check	
	// check r25 = 0x555aa666621e6667
  	data8	0x1901			// gr[25]
  	data8	0x555aa666621e6667	// value to check
	// check r26 = 0x91ffa8ae29ffffff
  	data8	0x1a01			// gr[26]
  	data8	0x91ffa8ae29ffffff	// value to check
	// check r27 = 0x9233a8ae2a18ffff
  	data8	0x1b01			// gr[27]
  	data8	0x9233a8ae2a18ffff	// value to check
	// check r28 = 0x6dcb0000ffd2aa98
  	data8	0x1c01			// gr[28]
  	data8	0x6dcb0000ffd2aa98	// value to check
	// check r29 = 0x6dcb0000ffffaa98
  	data8	0x1d01			// gr[29]
  	data8	0x6dcb0000ffffaa98	// value to check
