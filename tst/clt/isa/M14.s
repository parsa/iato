// ---------------------------------------------------------------------------
// - M14.s                                                                   -
// - iato isa test suite : M14 group instruction test                        -
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

_start:	.body
	movl r5 = 0x00000000000000F
	movl r6 = 0x000000100000000
	movl r7 = 0x000000000000000
	movl r8 = 0x000000000000000
	adds r9 = 8, r0
	;; 
	lfetch.nt2	  [r5], r9
	lfetch.nta        [r6], r9
	lfetch.excl.nt1   [r7], r9
	lfetch.fault      [r8], r9
	.endp _start#

	.section ".iato.checker", "a", "progbits"
	// check r5 = 0x000000000000017
  	data8	0x0501				// gr[5]
  	data8	0x000000000000017		// value to check
 	// check r6 = 0x000000100000008
  	data8	0x0601				// gr[6]
  	data8	0x000000100000008		// value to check
	// check r7 = 0x000000000000008
  	data8	0x0701				// gr[7]
  	data8	0x000000000000008		// value to check
	// check r8 =0x000000000000008
  	data8	0x0801				// gr[8]
  	data8	0x000000000000008		// value to check
