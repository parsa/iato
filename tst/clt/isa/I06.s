// ---------------------------------------------------------------------------
// - I06.s                                                                   -
// - iato isa test suite : I06 group instruction test                        -
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
	movl r3 = 0xc43b34d57e64dfe4
	;;
	pshr2 r20   = r3, 24
	pshr4 r21   = r3, 9
	pshr2.u r22 = r3, 5
	pshr4.u r23 = r3, 12
	pshr2 r25   = r3, 16
	pshr2.u r27 = r3, 23
	;;
	.endp _start#

	.section ".iato.checker", "a", "progbits"
	// check r20 = 0xffff00000000ffff
  	data8	0x1401			// gr[20]
  	data8	0xffff00000000ffff	// value to check
	// check r21 = 0xffe21d9a003f326f
  	data8	0x1501			// gr[21]
  	data8	0xffe21d9a003f326f	// value to check
	// check r22 = 0x62101a603f306ff
  	data8	0x1601			// gr[22]
  	data8	0x062101a603f306ff	// value to check
	// check r23 = 0x000c43b30007e64d
  	data8	0x1701			// gr[23]
  	data8	0x000c43b30007e64d	// value to check
	// check r25 = 0xffff00000000ffff
  	data8	0x1901			// gr[25]
  	data8	0xffff00000000ffff	// value to check
	// check r27 = 0x0000000000000000
  	data8	0x1b01			// gr[27]
  	data8	0x0000000000000000	// value to check
