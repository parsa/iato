// ---------------------------------------------------------------------------
// - B01.s                                                                   -
// - iato isa test suite : branch unit br.wtop test module                   -
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
	alloc r14 = ar.pfs, 8, 0, 0, 8;; 
        mov pr.rot=0x10000
        mov.i ar.ec = 7
	mov r15 = 10
	clrrrb;; 
	mov r16 = 0
        mov r32 = 0
        mov r33 = 0
        mov r34 = 0
	mov r35 = 0
        mov r36 = 0
        mov r37 = 0
        mov r38 = 0
	mov r39 = 0
	;;
.loop:
	cmp.eq p16, p17 = r35, r15
	adds r32 = 1, r33
	adds r16 = 1, r16
        (p16) br.wtop.dptk.few .loop#
	clrrrb
	;; 
	.endp  _start#

 	.section ".iato.checker", "a", "progbits"
	// check r16 = 7
 	data8	0x1001			// gr[32]
  	data8	0x0000000000000007	// value to check
	// check r33 = 0
 	data8	0x2101			// gr[33]
  	data8	0x0000000000000000	// value to check
	// check r34 = 7
 	data8	0x2201			// gr[34]
  	data8	0x0000000000000007	// value to check
	// check r35 = 6
 	data8	0x2301			// gr[35]
  	data8	0x0000000000000006	// value to check
	// check r36 = 5
 	data8	0x2401			// gr[36]
  	data8	0x0000000000000005	// value to check
	// check r37 = 4
 	data8	0x2501			// gr[37]
  	data8	0x0000000000000004	// value to check
	// check r38 = 3
 	data8	0x2601			// gr[38]
  	data8	0x0000000000000003	// value to check
	// check r39 = 2
 	data8	0x2701			// gr[39]
  	data8	0x0000000000000002	// value to check
