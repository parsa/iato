// ---------------------------------------------------------------------------
// - I05.s                                                                   -
// - iato isa test suite : I05 group instruction test                        -
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
	movl r15 = 0x123465efef58856F
	movl r28 = 0x8233566985532445
	mov  r2  = 3
	mov  r3  = 70
	;;
	pshr2   r16 = r15,r2
	pshr2.u r17 = r15,r2
	pshr4   r18 = r15,r2
	pshr4.u r19 = r15,r2
	shr     r20 = r15,r2
	shr.u   r21 = r15,r2
	pshr2   r22 = r15,r3
	pshr2.u r23 = r15,r3
	pshr4   r24 = r15,r3
	pshr4.u r25 = r15,r3
	shr     r26 = r15,r3
	shr.u   r27 = r15,r3
	shr     r29 = r28,r3
	shr     r30 = r28,r2
	.endp _start#

	.section ".iato.checker", "a", "progbits"
	// check r16 = 0x2460cbdfdebf0ad
  	data8	0x1001			// gr[16]
  	data8	0x2460cbdfdebf0ad       // value to check
	// check r17 = 0x2460cbd1deb10ad
  	data8	0x1101			// gr[17]
  	data8	0x2460cbd1deb10ad       // value to check	
	// check r18 = 0x2468cbdfdeb10ad
  	data8	0x1201			// gr[18]
  	data8	0x2468cbdfdeb10ad       // value to check
	// check r19 = 0x2468cbd1deb10ad
  	data8	0x1301			// gr[19]
  	data8	0x2468cbd1deb10ad       // value to check
	// check r20 = 0x2468cbdfdeb10ad
  	data8	0x1401			// gr[20]
  	data8	0x2468cbdfdeb10ad   	// value to check
	// check r21 = 0x2468cbdfdeb10ad
  	data8	0x1501			// gr[21]
  	data8	0x2468cbdfdeb10ad       // value to check
	// check r22 = 0x00000000ffffffff
  	data8	0x1601			// gr[22]
  	data8	0x00000000ffffffff      // value to check
	// check r23 = 0x0000000000000000
  	data8	0x1701			// gr[23]
  	data8	0x0000000000000000      // value to check
	// check r24 = 0x00000000ffffffff
  	data8	0x1801			// gr[24]
  	data8	0x00000000ffffffff      // value to check
	// check r25 = 0x0000000000000000
  	data8	0x1901			// gr[25]
  	data8	0x0000000000000000      // value to check
	// check r26 = 0x0000000000000000
  	data8	0x1a01			// gr[26]
  	data8	0x0000000000000000      // value to check
	// check r27 = 0x0000000000000000
  	data8	0x1b01			// gr[27]
  	data8	0x0000000000000000     // value to check
	// check r29 = 0xffffffffffffffff
  	data8	0x1d01			// gr[29]
  	data8	0xffffffffffffffff      // value to check
	// check r30 = 0xf0466acd30aa6488
  	data8	0x1e01			// gr[30]
  	data8	0xf0466acd30aa6488      // value to check
