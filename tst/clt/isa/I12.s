// ---------------------------------------------------------------------------
// - I12.s                                                                   -
// - iato isa test suite : I12 group instruction test                        -
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
	movl r2 = 0x123456789abcdef
	;; 
	dep.z  r1 = r2, 35, 64
	.endp _start#

	.section ".iato.checker", "a", "progbits"
	// check r1 = 0x4d5e6f7800000000
  	data8	0x0101			// gr[01]
  	data8   0x4d5e6f7800000000	// value to check
