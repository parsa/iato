// ---------------------------------------------------------------------------
// - M06s                                                                   -
// - iato isa test suite : M06 group instruction test                        -
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
	movl r1 = _data1;;
	movl r2 = _data2
	;;
	ldfs f2 = [r1]
	ldfd f3 = [r1]
  	ldf8 f4 = [r1]
	ldfe f5 = [r2]
	ldf.fill f6 = [r2]
	.endp _start#

	.section ".iato.data", "wa", "progbits"
	_data:	data8 0x000000789abcdef0
	_data1:	data8 0x123456789abcdef0
  	_data2:	data8 0x123456789abcdef0
	_data3:	data8 0x0000000000034456

	.section ".iato.checker", "a", "progbits"
	// check r1 = _data
  	data8	0x0101				// gr[1]
  	data8	_data1#				// value to check	
	// check f2 = 0x2ffb5 bcdef00000000000
  	data8	0x0203				// f[2]
	data8   0x2ffb5
  	data8	0xbcdef00000000000		// value to check
	// check f3 = 0xfd23 a2b3c4d5e6f78000 
  	data8	0x0303				// f[3]
	data8   0xfd23
  	data8	0xa2b3c4d5e6f78000 		// value to check  
	// check f4 = 0x1003E 123456789abcdef0
  	data8	0x0403				// f[4]
	data8   0x1003E
  	data8	0x123456789abcdef0		// value to check
	// check f5 = 0x10456 123456789abcdef0
  	data8	0x0503				// f[5]
	data8   0x10456
  	data8	0x123456789abcdef0		// value to check
	// check f6 = 0x34456 123456789abcdef0
  	data8	0x0603				// f[6]
	data8   0x34456				// value to check
	data8   0x123456789abcdef0		// value to check
