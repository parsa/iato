// ---------------------------------------------------------------------------
// - M02.s                                                                   -
// - iato isa test suite : M02 group instruction test                        -
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
	// check load only
	movl r1  = _data#
	movl r14 = _data#
	movl r17 = _data#
	mov  r15 = 4
	mov  r16 = -4
	;;
	ld1  r2 = [r1], r15
	ld2  r3 = [r14], r0
	;; 
	ld4  r4 = [r1], r0
	ld8  r5 = [r14], r16
	ld8.fill r6 = [r17], r16
	.endp _start#

	.section ".iato.data", "wa", "progbits"
_data:	data8 0x123456789abcdef0
_area:	data8 0x0000000000000000
	
	.section ".iato.checker", "a", "progbits"
	// check r1 = _data + 4
  	data8	0x0101			// gr[1]
  	data8	_data# + 4		// value to check
  	// check r2 = f0
  	data8	0x0201			// gr[2]
  	data8	0x00000000000000f0	// value to check
	// check r3 = def0
  	data8	0x0301			// gr[3]
  	data8	0x000000000000def0	// value to check
	// check r4 = 9abcdef0
  	data8	0x0401			// gr[4]
  	data8	0x0000000012345678	// value to check
	// check r5 = 123456789abcdef0
  	data8	0x0501			// gr[5]
  	data8	0x123456789abcdef0	// value to check
	// check r5 = 123456789abcdef0
  	data8	0x0601			// gr[6]
  	data8	0x123456789abcdef0	// value to check
	// check r14 = _data - 4
  	data8	0x0E01			// gr[14]
  	data8	_data# - 4		// value to check
	// check r17 = _data - 4
  	data8	0x1101			// gr[17]
  	data8	_data# - 4		// value to check
