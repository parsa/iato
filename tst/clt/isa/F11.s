// ---------------------------------------------------------------------------
// - F11.s                                                                   -
// - iato isa test suite : F11 group instruction test                        -
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
	movl r1 = _data1
	movl r2 = _data2
	;; 
	ldf.fill f3  = [r1]
	ldf.fill f4  = [r2]
	;; 
	fcvt.xf f2 = f3
	fcvt.xf f5 = f4	
	.endp
	
	.section ".iato.data", "wa", "progbits"
	_data1:	data8 0x923456789abcdef0
	_dat1:	data8 0x000000000002ffec
	_data2:	data8 0x00000035ddcf5ef0
	_dat2:	data8 0x00000000000100f0

	.section ".iato.checker", "a", "progbits"
	// check r1 = _data1
  	data8	0x0101				// gr[1]
  	data8	_data1#				// value to check
	// check f2 = 0x3003d db97530eca864220 
  	data8	0x0203				// f[2]
	data8   0x3003d     
  	data8	0xdb97530eca864220             // value to check
	// check f3 = 0x2ffec 923456789abcdef0
  	data8	0x0303				// f[3]
	data8   0x2ffec
  	data8	0x923456789abcdef0		// value to check
	// check f4 = 0x100f0 00000035ddcf5ef0
  	data8	0x0403				// f[4]
	data8   0x100f0
  	data8	0x00000035ddcf5ef0		// value to check
	// check f5 = 0x10024 d7773d7bc0000000
  	data8	0x0503				// f[5]
	data8   0x10024     
  	data8	0xd7773d7bc0000000              // value to check
