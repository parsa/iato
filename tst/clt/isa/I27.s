// ---------------------------------------------------------------------------
// - I27.s                                                                   -
// - iato isa test suite : I27 group instruction test                        -
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
        mov.i ar.pfs = 48
        mov.i ar.lc  = -23;;
	mov.i ar.ec  = 10
	.endp  _start#
	
	.section ".iato.checker", "a", "progbits"
	// check ar.pfs = 48
  	data8	0x4006			// ar[64]
  	data8	48			// value to check
	// check ar.lc = -23
  	data8	0x4106			// ar[65]
  	data8	-23			// value to check
	// check ar.ec = 10
  	data8	0x4206			// ar[66]
  	data8	10
