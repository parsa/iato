// ---------------------------------------------------------------------------
// - A05.s                                                                 -
// - iato isa test suite : A5 group instruction test                         -
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
	mov   r5 = 0x19234
	mov   r3 = 1;;
	addl  r14 = 0xABC0, r3      // r14 = 0x000000000000ABC1
	.endp _start#

	.section ".iato.checker", "a", "progbits"
  	// check r14 = 0x000000000000ABC1
  	data8	0x0E01			// gr[14]
  	data8	0x000000000000ABC1	// value to check

