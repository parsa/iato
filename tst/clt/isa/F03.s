// ---------------------------------------------------------------------------
// - F03.s                                                                   -
// - iato isa test suite : F3 group instruction test                         -
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
	movl r4 = _data4
	;;
	ldf8 f3 = [r1]
	ldf8 f4 = [r2]
	ldf8 f5 = [r4]
	;;
        fselect f25 = f5, f4, f3
	.endp _start

	.section ".iato.data", "wa", "progbits"
	_data:	data8 0xf00000789abcdef0
	_data1:	data8 0x123456789abcdef0
	_data4:	data8 0x5465769436545854

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
	// check f5 = 0x1003e 5465769436545854
  	data8	0x0503				// f[5]
	data8   0x1003e
  	data8	0x5465769436545854		// value to check
	// check f25 = 0x1003e 5234561012145850
  	data8	0x1903				// f[25]
	data8   0x1003e
  	data8	0x5234561012145850		// value to check
