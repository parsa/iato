// ---------------------------------------------------------------------------
// - M07s                                                                   -
// - iato isa test suite : M07 group instruction test                        -
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

_start:	.body

	movl r1 = _data1;;
	movl r2 = _data1
	movl r3 = _data1
	movl r4 = _data2
	movl r5 = _data2	
	adds r6 = 8, r0 
	;;
	ldfs f2 = [r1], r6
	ldfd f3 = [r2], r6
  	ldf8 f4 = [r3], r6
	ldfe f5 = [r4], r6
	ldf.fill f6 = [r5], r6
	.endp _start#
	
	.section ".iato.data", "wa", "progbits"
	_data:	data8 0x000000789abcdef0
	_data1:	data8 0x123456789abcdef0
	_data2:	data8 0x123456789abcdef0
	_data3:	data8 0x0000000000034456
	_data4:	data8 0x0
	_data5:	data8 0x0
	
	.section ".iato.checker", "a", "progbits"
	// check r1 = _data1
  	data8	0x0101				// gr[1]
  	data8	_data1#	+ 8			// value to check	
	// check r2 = _data1
  	data8	0x0201				// gr[2]
  	data8	_data1#	+ 8			// value to check	
	// check r3 = _data1
  	data8	0x0301				// gr[3]
  	data8	_data1#	+ 8			// value to check	
	// check r4 = _data2
  	data8	0x0401				// gr[4]
  	data8	_data2#	+ 8			// value to check
	// check r5 = _data2
  	data8	0x0501				// gr[5]
  	data8	_data2#	+ 8			// value to check
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
