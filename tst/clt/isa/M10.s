// ---------------------------------------------------------------------------
// - M10.s                                                                   -
// - iato isa test suite : M10 group instruction test                        -
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
	movl r3 = _data2
	movl r4 = _data4
	movl r5 = _data6	
	;;
	mov r21 = r1
	mov r22 = r2
	mov r23 = r3
	mov r24 = r4
	mov r25 = r5
	ldf.fill f5 = [r3]
	;;
	stfs [r1] = f5, 8
	stfd [r2] = f5, 8
	stf8 [r3] = f5, 8 
	stfe [r4] = f5, 10
	stf.spill [r5] = f5, 16
	;; 
	ld4 r7 = [r21]
	ld8 r8 = [r22]
  	ldf8 f4 = [r23]
	ld8 r14 = [r24], 8
	;;
	ld8 r16 = [r24]
	ldf.fill f6 = [r25]
	.endp _start#
	
	.section ".iato.data", "wa", "progbits"
	_data:	data8 0x000000789abcdef0
	_data1:	data8 0x123456789abcdef0
	_data2:	data8 0x123456789abcdef0
	_data3:	data8 0x0000000000034456
	_data4:	data8 0x0
	_data5:	data8 0x0
	_data6:	data8 0x0
	_data7:	data8 0x0

	.section ".iato.checker", "a", "progbits"
	// check r1 = _data + 8
  	data8	0x0101				// gr[1]
  	data8	_data# + 8			// value to check
	// check r2 = _data1 + 8
  	data8	0x0201				// gr[2]
  	data8	_data1# + 8			// value to check
	// check r3 = _data2 + 8
  	data8	0x0301				// gr[3]
  	data8	_data2# + 8			// value to check
	// check r7 = 0x80123456
  	data8	0x0701				// gr[7]
  	data8	0x80123456			// value to check
	// check r8 = 0x8002468acf13579b
  	data8	0x0801				// gr[8]
  	data8	0x8002468acf13579b		// value to check
	// check r14 = 0123456789abcdef0
  	data8	0x0e01				// gr[14]
  	data8	0x123456789abcdef0		// value to check
	// check r16 = 0x000000000000c456
  	data8	0x1001				// gr[16]
	data8   0x000000000000c456
	// check f4 = 0x1003E 123456789abcdef0
  	data8	0x0403				// f[4]
	data8   0x1003E
  	data8	0x123456789abcdef0		// value to check
	// check f6 = 0x34456 123456789abcdef0
  	data8	0x0603				// f[6]
	data8   0x34456				// value to check
	data8   0x123456789abcdef0		// value to check
