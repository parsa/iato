// ---------------------------------------------------------------------------
// - I01.s                                                                   -
// - iato isa test suite : I01 group instruction test                        -
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
	movl r14 = 0x345678674321feda
	movl r15 = 0xab42372983636bbb
	;; 
	pmpyshr2 r4 = r14, r15, 7
	pmpyshr2.u r17 = r14, r15, 15
	;; 
	.endp _start#
	
	.section ".iato.checker", "a", "progbits"
	// check r4 = 0x59e0e2d2a5b1088e
  	data8	0x0401			// gr[1]
  	data8	0x59e0e2d2a5b1088e	// value to check
	// check r17 = 0x460533e244e7d67e
  	data8	0x1101			// gr[1]
  	data8	0x460533e244e7d67e	// value to check
