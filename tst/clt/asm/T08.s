// ---------------------------------------------------------------------------
// - T08.s                                                                   -
// - iato asm test suite : simple load test case                             -
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
        movl r1 = _data#;;
        ld1  r2 = [r1]
        ld2  r3 = [r1]
        ld4  r4 = [r1]
        ld8  r5 = [r1]
	.endp _start#

        .section ".iato.data", "wa", "progbits"
_data:  data8 0x123456789abcdef0
