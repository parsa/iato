// ---------------------------------------------------------------------------
// - A02.s                                                                 -
// - iato isa test suite : A2 group instruction test                         -
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
	mov      r14 = 8
	movl     r9  = 0xC0000000
	movl     r20 = 0x600010001fffb418
	movl     r21 = 0xffffffffffffffff
	mov      r15 = 3;;

	shladd   r16 = r15,1,r14   // r16 = 0xE
	shladd   r19 = r21,3,r20   // r19 = 0x600010001fffb410
	shladdp4 r17 = r9,1,r14    // r17 = 0x0000000080000008
	shladdp4 r18 = r15,1,r9    // r18 = 0x60000000C0000006	
	.endp _start#
	
	.section ".iato.checker", "a", "progbits"
  	// check r16 = 0xE
  	data8	0x1001			// gr[16]
  	data8	0x000000000000000E	// value to check
  	// check r17 = 0x0000000080000008
  	data8	0x1101			// gr[17]
  	data8	0x0000000080000008 	// value to check
  	// check r18 = 0x60000000C0000006
  	data8	0x1201			// gr[18]
  	data8	0x60000000C0000006	// value to check
 	// check r19 =600010001fffb410 
  	data8	0x1301			// gr[19]
  	data8	0x600010001fffb410	// value to check
