// ---------------------------------------------------------------------------
// - I07.s                                                                   -
// - iato isa test suite : I07 group instruction test                        -
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
	movl r14 = 0x7539512468dda885
	movl r15 = 0xFFFF00010001FFFF
	mov  r2  = 3
	mov  r5  = 55
	mov  r6  = 25
	;;
	pshl2   r16 = r15,r2      // r16 = 0xFFF800080008FFF8
	pshl2   r17 = r15,r5      // r17 = 0x0000000000000000
	pshl4   r18 = r15,r2      // r18 = 0xFFF80008000FFFF8
	pshl4   r19 = r15,r5      // r19 = 0x0000000000000000
	shl     r20 = r15,r2      // r20 = 0xFFF80008000FFFF8
	pshl2   r21 = r15,r6      // r21 = 0x
	pshl4   r22 = r15,r6      // r22 = 0x	
	pshl2   r26 = r14,r2
	pshl2   r27 = r14,r5
	pshl4   r28 = r14,r2
	pshl4   r29 = r14,r5
	shl     r30 = r14,r2
	pshl2   r31 = r14,r6
	pshl4   r23 = r14,r6
	.endp _start#

	.section ".iato.checker", "a", "progbits"
	// check r16 = 0xFFF800080008FFF8
  	data8	0x1001			// gr[16]
  	data8	0xFFF800080008FFF8	// value to check
	// check r17 = 0x0000000000000000
  	data8	0x1101			// gr[17]
  	data8	0x0000000000000000	// value to check
	// check r18 = 0xFFF80008000FFFF8
  	data8	0x1201			// gr[18]
  	data8	0xFFF80008000FFFF8	// value to check
	// check r19 = 0x0000000000000000
  	data8	0x1301			// gr[19]
  	data8	0x0000000000000000	// value to check
	// check r20 = 0xFFF80008000FFFF8
  	data8	0x1401			// gr[20]
  	data8	0xFFF80008000FFFF8 	// value to check
	// check r21 = 0x0000000000000000
  	data8	0x1501			// gr[21]
  	data8	0x0000000000000000 	// value to check
	// check r22 = 0x2000000fe000000
  	data8	0x1601			// gr[22]
  	data8	0x2000000fe000000 	// value to check
	// check r23 = 0x480000000a000000
  	data8	0x1701			// gr[23]
  	data8	0x480000000a000000 	// value to check
	// check r26 = 0xa9c8892046e84428
  	data8	0x1a01			// gr[26]
  	data8	0xa9c8892046e84428	// value to check
	// check r27 = 0x0000000000000000
  	data8	0x1b01			// gr[27]
  	data8	0x0000000000000000	// value to check
	// check r28 = 0xa9ca892046ed4428
  	data8	0x1c01			// gr[28]
  	data8	0xa9ca892046ed4428 	// value to check
	// check r29 = 0x0000000000000000
  	data8	0x1d01			// gr[29]
  	data8	0x0000000000000000	// value to check
	// check r30 = 0xa9ca892346ed4428
  	data8	0x1e01			// gr[30]
  	data8	0xa9ca892346ed4428 	// value to check
	// check r31 = 0x0000000000000000
  	data8	0x1f01			// gr[31]
  	data8	0x0000000000000000	// value to check
