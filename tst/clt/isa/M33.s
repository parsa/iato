// ---------------------------------------------------------------------------
// - M33.s                                                                   -
// - iato isa test suite : M33 group instruction test                        -
// ---------------------------------------------------------------------------
// - (c) inria 2002-2004       
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
	movl r10 = 0x0192837465bc
	;; 
	mov cr5 = r10
	;; 
	mov r4  = cr5
	.endp _start#

	.section ".iato.checker", "a", "progbits"
  	// check r10 = 0x0192837465bc
  	data8	0x0a01			// gr[10]
  	data8	0x0192837465bc		// value to check  	
	// check cr5 = 0x0192837465bc 
  	data8	0x050D			// cr[5]
  	data8	0x0192837465bc		// value to check
  	// check r4 = 0x0192837465bc
  	data8	0x0401			// gr[4]
  	data8	0x0192837465bc		// value to check  	
