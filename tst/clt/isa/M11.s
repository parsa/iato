// ---------------------------------------------------------------------------
// - M11s                                                                    -
// - iato isa test suite : M11 group instruction test                        -
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
	movl r1 = _data#
	;;
	ldfps f2, f3 = [r1]
	ldfpd f4, f5 = [r1]
	ldfp8 f6, f7 = [r1]
	.endp _start#

	.section ".iato.data", "wa", "progbits"
	_data:	data8 0x123456789abcdef0
	_data1:	data8 0x000000789abcdef0

	.section ".iato.checker", "a", "progbits"
	// check f2 = 0x2ffb5 bcdef00000000000
  	data8	0x0203				// f[2]
	data8   0x2ffb5
  	data8	0xbcdef00000000000		// value to check
	// check f3 = 0x0ffa4 b456780000000000
  	data8	0x0303				// f[3]
	data8   0x0ffa4
  	data8	0xb456780000000000     		// value to check
	// check f4 = 0xfd23 a2b3c4d5e6f78000 
  	data8	0x0403				// f[4]
	data8   0xfd23
  	data8	0xa2b3c4d5e6f78000 		// value to check  	
	// check f5 = 0xfc01 0003c4d5e6f78000       
  	data8	0x0503				// f[5]
	data8   0xfc01				// value to check
	data8   0x0003c4d5e6f78000              // value to check
	// check f6 = 0x1003e 123456789abcdef0                
  	data8	0x0603				// f[6]
	data8   0x1003e				// value to check
	data8   0x123456789abcdef0              // value to check
	// check f7 = 0x1003e 000000789abcdef0
  	data8	0x0703				// f[7]
	data8   0x1003e				// value to check
	data8   0x000000789abcdef0		// value to check
