// ---------------------------------------------------------------------------
// - M16.s                                                                   -
// - iato isa test suite : M16 group instruction test                        -
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
	cmpxchg1.acq r1 = [r3], r2, ar.ccv
	cmpxchg2.acq.nt1 r5 = [r3], r2, ar.ccv
	cmpxchg4.acq.nta r7 = [r3], r2, ar.ccv
	cmpxchg8.acq.nta r9 = [r3], r2, ar.ccv
	cmpxchg1.rel r6 = [r3], r2, ar.ccv
	cmpxchg2.rel r8 = [r3], r2, ar.ccv
	cmpxchg4.rel r10 = [r3], r2, ar.ccv
	cmpxchg8.rel r11 = [r3], r2, ar.ccv
	xchg1.nta r14 = [r15], r16
	;; 
	xchg2.nt1 r14 = [r15], r16
	;; 
	xchg4.nt1 r14 = [r15], r16
	;; 
	xchg8.nta r14 = [r15], r16
	.endp
