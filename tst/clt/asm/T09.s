// ---------------------------------------------------------------------------
// - T09.s                                                                   -
// - iato asm test suite : simple store test case                            -
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
        movl r1 = _area#
	movl r2 = 0x0123456789abcdef;; 
        st1  [r1] = r2
        st2  [r1] = r2
        st4  [r1] = r2
        st8  [r1] = r2
	.endp _start#

        .section ".iato.data", "wa", "progbits"
_area:  data8 0x0000000000000000
