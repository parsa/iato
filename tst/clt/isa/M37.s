// ---------------------------------------------------------------------------
// - M37.s                                                                   -
// - iato isa test suite : M37 group instruction test                        -
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
	     break.m 0xff
	     nop.m   0xff
	     nop.i   0
	}
	// predicated group test
	.mmi {
	(p1) break.m 0xff
	(p2) nop.m   0xff
	(p3) nop.i   0
	}
	.endp _start#
