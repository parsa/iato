// ---------------------------------------------------------------------------
// - M42.s                                                                   -
// - iato isa test suite : M42 group instruction test                        -
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
	mov    dbr[r3]  = r4
	mov    ibr[r5]  = r6
	mov    pkr[r7]  = r8
	mov    pmc[r9]  = r10
	mov    pmd[r11] = r12
	itr.d  dtr[r13] = r14
	;; 
 	itr.i  itr[r15] = r16
	mov     rr[r1]  = r2
 	.endp
