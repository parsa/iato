// ---------------------------------------------------------------------------
// - I04.s                                                                   -
// - iato isa test suite : I04 group instruction test                        -
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
	movl r3 = 0x5432187690bbcdef
	;; 
	mux2 r20 = r3, 0xFF
	mux2 r21 = r3, 0xcd
	mux2 r22 = r3, 0x10
	mux2 r23 = r3, 0x38
	mux2 r24 = r3, 0x56
	;;
	.endp
	
	.section ".iato.checker", "a", "progbits"
	// check r20 = 0x5432543254325432
  	data8	0x1401			// gr[20]
  	data8	0x5432543254325432	// value to check
	// check r21 = 0x5432cdef543290bb
  	data8	0x1501			// gr[21]
  	data8	0x5432cdef543290bb	// value to check
	// check r22 = 0xcdef90bbcdefcdef
  	data8	0x1601			// gr[22]
  	data8	0xcdef90bbcdefcdef	// value to check
	// check r23 = 0xcdef54321876cdef
  	data8	0x1701			// gr[23]
  	data8	0xcdef54321876cdef	// value to check
	// check r24 = 0x90bb90bb90bb1876
  	data8	0x1801			// gr[24]
  	data8	0x90bb90bb90bb1876	// value to check
