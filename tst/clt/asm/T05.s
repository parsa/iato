// ---------------------------------------------------------------------------
// - T05.s                                                                   -
// - iato asm test suite : simple call-return test                           -
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
	
_func:	.body
	alloc	  r34 = ar.pfs,1,2,0,0;;
	mov	  r16 = 2
	mov	  r33 = r32
	mov.i     ar.pfs = r34;;
	br.ret.sptk.many b0
	.endp _func#
	
_start:
	.body
	mov	  r16 = 1
	alloc	  r32 = ar.pfs, 0,1,1,0
	mov       r33 = 0xff;;
	br.call.sptk.few b0=_func#
	.endp _start#

	.section ".iato.checker", "a", "progbits"
	// check r16 = 2
	data8	0x1001			// gr[16]
	data8	0x0000000000000002	// value to check

