// ---------------------------------------------------------------------------
// - I14.s                                                                   -
// - iato isa test suite : I14 group instruction test                        -
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
	movl r2 = 0x123456789abcdef0
	;; 
	dep    r1 = -1, r2, 35, 25
	dep    r3 = -1, r2, 35, 64
	.endp _start#

	.section ".iato.checker", "a", "progbits"
	// check r1 = 0x1ffffff89abcdef0
  	data8	0x0101			// gr[1]
  	data8	0x1ffffff89abcdef0	// value to check
	// check r3 = 0xfffffff89abcdef0
  	data8	0x0301			// gr[3]
  	data8	0xfffffff89abcdef0	// value to check
