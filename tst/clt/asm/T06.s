// ---------------------------------------------------------------------------
// - T06.s                                                                   -
// - iato asm test suite : simple call cancel test                           -
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
	
_fail:	.body
	mov r1 = 2
	br.ret.sptk.many b0
	.endp _fail#
	
_start:
	.body
	mov    r1 = 1
	cmp.eq p1,p2= r0,r0
   (p2) br.call.sptk.few b0=_fail#;;
	add r1 = 1,r1
	.endp _start#

	.section ".iato.checker", "a", "progbits"
	// check r1 = 2
	data8	0x0101			// gr[1]
	data8	0x0000000000000002	// value to check
