// ---------------------------------------------------------------------------
// - xit.s                                                                   -
// - iato:isa test suite : xit handler for all test module                   -
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
	
	.section ".fini", "a", "progbits"
	.align  16
	.global _exit#
	.proc   _exit#

_exit:
	.mii {
	mov	r15=1025
	mov	r32=0
	break.i 0x100000
	}
	// we should never get here - so break miserably
	.mii {
	nop.m   0x00
	nop.i   0x00
	break.i 0x00
	}
	.endp   _exit#

