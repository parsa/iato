// ---------------------------------------------------------------------------
// - T13.s                                                                   -
// - iato asm test suite : simple predicate test                             -
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
	mov       r1 = 1
	mov       r2 = 1
	mov       r3 = 1;;
	cmp.eq p1,p2 = r1,r2;;
   (p2) add       r3 = r1,r2;;
   (p2) sub       r3 = r3,r2;;
	mov       r4 = r3
	.endp _start#
