// ---------------------------------------------------------------------------
// - T11.s                                                                   -
// - iato asm test suite : simple branch taken test with short bundle        -
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
	nop.m  0x00
	nop.m  0x00
	cmp.eq p1,p2= r0,r0;;
	nop.m  0x00
	nop.f  0x00
   (p1) br.cond.dpnt.few _cont#
	nop.m     0x00
	nop.m     0x00
	break.i   0x00
_cont:	nop.m     0x00
	nop.m     0x00
	nop.i     0x00
	.endp _start#

