// ---------------------------------------------------------------------------
// - M34.s                                                                   -
// - iato isa test suite : M34 group instruction test                        -
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
	// register initialization
	.mmi {
	     alloc r34 = ar.pfs,9,3,4,8;;
	     nop.m 0
	     nop.i 0
	}
	// predicated group test
	.mmi {
	(p0) alloc r34 = ar.pfs,9,3,4,8;;
	(p2) nop.m 0
	(p3) nop.i 0
	}
	.endp _start#
