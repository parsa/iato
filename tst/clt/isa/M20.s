// ---------------------------------------------------------------------------
// - M20.s                                                                   -
// - iato isa test suite : M20 group instruction test                        -
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
	.align  16
	.global _start#
	.proc   _start#

.recover1:	
	mov r1 = 1
	br  .fin1
.recover2:	
	mov r2 = 1
	br  .fin2
	
_start:
	mov     r1 = r0
	mov     r2 = r0;;
	chk.s.m r1,.recover1#
.fin1:  nop.b   0x00
	ld8.s   r2=[r0];;
	chk.s.m r2,.recover2#
.fin2:  nop.i   0x00
	.endp _start#

  	.section ".iato.checker", "a", "progbits"
  	// check r1 = 0
  	data8	0x0101			// gr[01]
  	data8	0x0000000000000000	// value to check
  	// check r2 = 1
  	data8	0x0201			// gr[02]
  	data8	0x0000000000000001	// value to check
