// ---------------------------------------------------------------------------
// - I10.s                                                                   -
// - iato isa test suite : I10 group instruction test                        -
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
	movl r3 = 0xf1ff1f1f1f1f454f
	movl r4 = 0x7654321765432166
	;;
	shrp r2 = r3, r4, 16
	shrp r5 = r3, r4, 63
	.endp _start

	.section ".iato.checker", "a", "progbits"
	// check r2 = 0x454f765432176543
  	data8	0x0201			// gr[2]
  	data8	0x454f765432176543	// value to check
	// check r5 = 0xe3fe3e3e3e3e8a9e
  	data8	0x0501			// gr[5]
  	data8	0xe3fe3e3e3e3e8a9e	// value to check
