// ---------------------------------------------------------------------------
// - T12.s                                                                   -
// - iato isa test suite : branch unit br.cop test module                    -
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
	alloc r14   = ar.pfs, 8, 0, 0, 8;; 
        mov.i ar.lc = 7
        mov.i ar.ec = 0
	mov r15     = 0
	clrrrb;; 
.loop:
	add r15 = r15,r0,1
        br.ctop.dptk.few .loop#
	clrrrb
	;; 
	.endp  _start#
