// ---------------------------------------------------------------------------
// - M43.s                                                                   -
// - iato isa test suite : M43 group instruction test                        -
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
	mov r1 =    rr[r8]    
	mov r2 =   dbr[r9]  
	mov r3 =   ibr[r10] 
	mov r4 =   pkr[r11] 
	mov r5 =   pmc[r12] 
	mov r6 =   pmd[r13] 
	mov r7 = cpuid[r14] 
 	.endp
