// ---------------------------------------------------------------------------
// - A01.s                                                                   -
// - iato isa test suite : A1 group instruction test                         -
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
	.align  16
	.global _start#
	.proc   _start#

_start:
	.body
	mov   r8 = 1
	movl  r9 = 0xC0000000    
	mov   r2 = 1
	mov   r3 = 1;;
	add   r14 = r8,r2       // r14 = 0x2
	add   r23 = r8,r3,1;;   // r23 = 0x3
	sub   r16 = r23,r2      // r16 = 0x2
	sub   r17 = r23,r2,1    // r17 = 0x1
	addp4 r18 = r23,r9      // r18 = 0x60000000C0000003
	and   r19 = r14, r23    // r19 = 0x2
	andcm r20 = r14, r23    // r20 = 0x0
	or    r21 = r14, r23    // r21 = 0x3
	xor   r22 = r14, r23    // r22 = 0x1
	.endp _start#

  	.section ".iato.checker", "a", "progbits"
  	// check r14 = 2
  	data8	0x0E01			// gr[14]
  	data8	0x0000000000000002	// value to check
  	// check r23 = 3
  	data8	0x1701			// gr[23]
  	data8	0x0000000000000003	// value to check
  	// check r16 = 2
  	data8	0x1001			// gr[16]
  	data8	0x0000000000000002	// value to check
  	// check r17 = 1
  	data8	0x1101			// gr[17]
  	data8	0x0000000000000001	// value to check
  	// check r18 = 0x60000000C0000003
  	data8	0x1201			// gr[18]
  	data8	0x60000000C0000003	// value to check
  	// check r19 = 2
  	data8	0x1301			// gr[19]
  	data8	0x0000000000000002	// value to check
  	// check r20 = 0
  	data8	0x1401			// gr[20]
  	data8	0x0000000000000000	// value to check
  	// check r21 = 3
  	data8	0x1501			// gr[21]
  	data8	0x0000000000000003	// value to check
  	// check r22 = 1 
 	data8	0x1601			// gr[22]
  	data8	0x0000000000000001	// value to check
