// ---------------------------------------------------------------------------
// - I22.s                                                                   -
// - iato isa test suite : I22 group instruction test                        -
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
	movl r1 = 0x1234567898765432
	;; 
	mov b5 = r1
	;; 
	mov r4 = b5
	.endp _start#

	.section ".iato.checker", "a", "progbits"
	// check b5 = 0x1234567898765432
  	data8	0x0505			// b[5]
  	data8	0x1234567898765432      // value to check
	// check r4 = 0x1234567898765432
  	data8	0x0401			// gr[4]
  	data8	0x1234567898765432      // value to check
