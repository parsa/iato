// ---------------------------------------------------------------------------
// - F02.s                                                                   -
// - iato isa test suite : F2 group instruction test                         -
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
	movl r1 = _data
	movl r2 = _data1
	;;
	ldf8 f3 = [r1]
	ldf8 f4 = [r2]
	ldf8 f5 = [r2]
	;;
        xma.l  f20 = f3, f4, f5
        xma.h  f21 = f3, f4, f5
        xma.hu f22 = f3, f4, f5
	.endp _start

	.section ".iato.data", "wa", "progbits"
	_data:	data8 0xf00000789abcdef0
	_data1:	data8 0x123456789abcdef0

	.section ".iato.checker", "a", "progbits"
	// check r1 = _data
  	data8	0x0101				// gr[1]
  	data8	_data#				// value to check
	// check f3 = 0x1003e f00000789abcdef0
  	data8	0x0303				// f[3]
	data8   0x1003e
  	data8	0xf00000789abcdef0		// value to check
	// check f4 = 0x1003e 123456789abcdef0
  	data8	0x0403				// f[4]
	data8   0x1003e
  	data8	0x123456789abcdef0		// value to check
	// check f5 = 0x1003e 123456789abcdef0
  	data8	0x0503				// f[5]
	data8   0x1003e
  	data8	0x123456789abcdef0		// value to check
	// check f20 = 0x1003e 1c71bf098d61fff0
  	data8	0x1403				// f[20]
	data8   0x1003e
  	data8	0x1c71bf098d61fff0		// value to check
	// check f21 = 0x1003e fedcbaa109dda4e8
  	data8	0x1503				// f[21]
	data8   0x1003e
  	data8	0xfedcbaa109dda4e8		// value to check  
	// check f22 = 0x1003e 11111119a49a83d8
  	data8	0x1603				// f[22]
	data8   0x1003e
  	data8	0x11111119a49a83d8		// value to check
