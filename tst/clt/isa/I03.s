// ---------------------------------------------------------------------------
// - I03.s                                                                   -
// - iato isa test suite : I03 group instruction test                        -
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
	movl r3 = 0x1234567890abcdef
	;; 
	mux1 r20 = r3, @rev
	mux1 r21 = r3, @shuf
	mux1 r22 = r3, @mix
	mux1 r23 = r3, @alt
	mux1 r24 = r3, @brcst
	.endp
	
	.section ".iato.checker", "a", "progbits"
	// check r20 = 0xefcdab9078563412
  	data8	0x1401			// gr[20]
  	data8	0xefcdab9078563412	// value to check
	// check r21 = 0x129034ab56cd78ef
  	data8	0x1501			// gr[21]
  	data8	0x129034ab56cd78ef	// value to check
	// check r22 = 0x129056cd34ab78ef
  	data8	0x1601			// gr[22]
  	data8	0x129056cd34ab78ef	// value to check
	// check r23 = 0x125690cd3478abef
  	data8	0x1701			// gr[23]
  	data8	0x125690cd3478abef	// value to check
	// check r24 = 0xefefefefefefefef
  	data8	0x1801			// gr[24]
  	data8	0xefefefefefefefef	// value to check
