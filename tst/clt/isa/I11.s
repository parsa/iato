// ---------------------------------------------------------------------------
// - I11.s                                                                   -
// - iato isa test suite : I11 group instruction test                        -
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
	movl r14 = 0x50610901600b0c01
	movl r15 = 0xFfFF0e010b08FF63
	;;
	extr     r18 = r15, 16, 33
	extr.u   r16 = r15, 26, 21
	extr     r19 = r15, 36, 64
	extr.u   r20 = r15, 55, 5
	extr     r21 = r14, 18, 33
	extr.u   r22 = r14, 25, 21
	extr     r23 = r14, 51, 64
	extr.u   r24 = r14, 55, 5
	shr      r25 = r14, 11
	extr     r26 = r14, 0,  12
	extr     r27 = r15, 0,  8
	.endp _start#

	.section ".iato.checker", "a", "progbits"
	// check r16 = 0x0000000000038042
  	data8	0x1001			// gr[16]
  	data8	0x0000000000038042	// value to check
	// check r18 = 0xffffffff0e010b08
  	data8	0x1201			// gr[18]
  	data8   0xffffffff0e010b08	// value to check
	// check r19 = 0xfffffffffffff0e0
  	data8	0x1301			// gr[19]
  	data8   0xfffffffffffff0e0	// value to check
	// check r20 = 0x000000000000001f
  	data8	0x1401			// gr[20]
  	data8   0x000000000000001f    	// value to check
	// check r21 = 0x0000000042405802
  	data8	0x1501			// gr[21]
  	data8   0x0000000042405802    	// value to check
	// check r22 = 0x00000000000480b0
  	data8	0x1601			// gr[22]
  	data8   0x00000000000480b0    	// value to check
	// check r23 = 0x0000000000000a0c
  	data8	0x1701			// gr[23]
  	data8   0x0000000000000a0c    	// value to check
	// check r24 = 0x0000000000000000
  	data8	0x1801			// gr[24]
  	data8   0x0000000000000000    	// value to check
	// check r25 = 0x000a0c21202c0161
  	data8	0x1901			// gr[25]
  	data8   0x000a0c21202c0161    	// value to check
	// check r26 = 0xfffffffffffffc01
  	data8	0x1a01			// gr[26]
  	data8   0xfffffffffffffc01    	// value to check
	// check r27 = 0x0000000000000063
  	data8	0x1b01			// gr[27]
  	data8   0x0000000000000063    	// value to check
