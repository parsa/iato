// ---------------------------------------------------------------------------
// - T02.s                                                                   -
// - iato asm test suite : simple register load                              -
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
	.align  16
	.global _start#
	.proc   _start#

_start:
	.body
	adds r1 = 1,  r0;;
	adds r2 = 1,  r1;;
	add  r3 = r1, r2;;
	add  r4 = r3, r2;;
	add  r5 = r4, r3
	.endp _start#

	.section ".iato.checker", "a", "progbits"
	// check r1 = 1
	data8	0x0101			// gr[1]
	data8	0x0000000000000001	// value to check
	// check r2 = 2
	data8	0x0201			// gr[2]
	data8	0x0000000000000002	// value to check
	// check r3 = 3
	data8	0x0301			// gr[3]
	data8	0x0000000000000003	// value to check
	// check r4 = 5
	data8	0x0401			// gr[4]
	data8	0x0000000000000005	// value to check
	// check r5 = 8
	data8	0x0501			// gr[5]
	data8	0x0000000000000008	// value to check
	
