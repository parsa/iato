// ---------------------------------------------------------------------------
// - A04.s                                                                 -
// - iato isa test suite : A4 group instruction test                         -
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
	movl  r9 = 0xC0000000    
	mov   r3 = 1;;
	adds  r14 = -3,r3      // r14 = 0xFFFFFFFFFFFFFFFE
	addp4 r15 = 3,r9       // r15 = 0x60000000C0000003
	.endp _start#

	.section ".iato.checker", "a", "progbits"
  	// check r14 = 0xFFFFFFFFFFFFFFFE
  	data8	0x0E01			// gr[14]
  	data8	0xFFFFFFFFFFFFFFFE	// value to check
  	// check r15 = 0x60000000C0000003 
  	data8	0x0F01			// gr[15]
  	data8	0x60000000C0000003	// value to check

