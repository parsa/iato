// ---------------------------------------------------------------------------
// - I08.s                                                                   -
// - iato isa test suite : I08 group instruction test                        -
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
	movl r5 = 0x1234567890abcdef
	;; 
	pshl2 r20 = r5, 10
	pshl2 r22 = r5, 19
	pshl4 r21 = r5, 17
	pshl4 r23 = r5, 31
	.endp
	
	.section ".iato.checker", "a", "progbits"
	// check r20 =0xd000e000ac00bc00 
  	data8	0x1401			// gr[20]
  	data8	0xd000e000ac00bc00	// value to check
	// check r21 = 0xacf000009bde0000 
  	data8	0x1501			// gr[21]
  	data8	0xacf000009bde0000	// value to check
	// check r22 = 0x0000000000000000 
  	data8	0x1601			// gr[22]
  	data8	0x0000000000000000 	// value to check
	// check r23 = 0x0000000080000000  
  	data8	0x1701			// gr[23]
  	data8	0x0000000080000000 	// value to check
