// ---------------------------------------------------------------------------
// - M01.s                                                                   -
// - iato isa test suite : M01 group instruction test                        -
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
	movl r10 = 0xffffffffffffffff
	;;
	mov ar.unat = r10
	;; 
	ld1  r2 = [r1]
	ld2  r3 = [r1]
	ld4  r4 = [r1]
	ld8  r5 = [r1]
	ld8.fill r8 = [r1]
	// check store for fun
	movl r6 = _area#;;
	st1  [r6] = r2
	ld1  r7   = [r6]
	.endp _start#

	.section ".iato.data", "wa", "progbits"
_data:	data8 0x123456789abcdef0
_area:	data8 0x0000000000000000
	
	.section ".iato.checker", "a", "progbits"
	// check r1 = _data
  	data8	0x0101			// gr[1]
  	data8	_data#			// value to check
  	// check r2 = f0
  	data8	0x0201			// gr[2]
  	data8	0x00000000000000f0	// value to check
	// check r3 = def0
  	data8	0x0301			// gr[3]
  	data8	0x000000000000def0	// value to check
	// check r4 = 9abcdef0
  	data8	0x0401			// gr[4]
  	data8	0x000000009abcdef0	// value to check
	// check r5 = 123456789abcdef0
  	data8	0x0501			// gr[5]
  	data8	0x123456789abcdef0	// value to check
	// ckeck nat8
  	data8	0x0802			// nat[8]
  	data8	0x01			// value to check
