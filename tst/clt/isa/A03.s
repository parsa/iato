// ---------------------------------------------------------------------------
// - A03.s                                                                 -
// - iato isa test suite : A3 group instruction test                         -
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
	.body
	mov   r2 = 2
	mov   r3 = 1;;
	sub   r16 = 1, r2;;   // r16 = 0xffffffffffffffff
	andcm r20 = 2, r3     // r20 = 0x2
	or    r21 = 2, r3     // r21 = 0x3
	xor   r22 = 2, r3     // r22 = 0x3
	.endp _start#
	
	.section ".iato.checker", "a", "progbits"
  	// check r16 = 0xE
  	data8	0x1001			// gr[16]
  	data8	0xFFFFFFFFFFFFFFFF	// value to check
  	// check r20 = 0x2
  	data8	0x1401			// gr[20]
  	data8	0x0000000000000002	// value to check
  	// check r21 = 0x3
  	data8	0x1501			// gr[21]
  	data8	0x0000000000000003	// value to check
	// check r22 = 0x3
  	data8	0x1601			// gr[22]
  	data8	0x0000000000000003	// value to check
