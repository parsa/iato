// ---------------------------------------------------------------------------
// - F09.s                                                                   -
// - iato isa test suite : F9 group instruction test                         -
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
        fmerge.s  f5 = f3, f4	
        fmerge.ns f6 = f3, f4	
        fmerge.se f7 = f3, f4
	;;
	fsxt.l    f8 = f7, f6
	fsxt.r    f9 = f3, f4
	.endp

	.section ".iato.data", "wa", "progbits"
	_data1:	data8 0x923456789abcdef0
	_dat1:	data8 0x000000000002ffec
	_data2:	data8 0x12345635ddcf5ef0
	_dat2:	data8 0x00000000000100f0

	.section ".iato.checker", "a", "progbits"
	// check r1 = _data1
  	data8	0x0101				// gr[1]
  	data8	_data1#				// value to check
	// check f3 = 0x2ffec 923456789abcdef0
  	data8	0x0303				// f[3]
	data8   0x2ffec
  	data8	0x923456789abcdef0		// value to check
	// check f4 = 0x100f0 12345635ddcf5ef0
  	data8	0x0403				// f[4]
	data8   0x100f0
  	data8	0x12345635ddcf5ef0		// value to check
	// check f5 = 0x300f0 12345635ddcf5ef0
  	data8	0x0503				// f[5]
	data8   0x300f0          
  	data8	0x12345635ddcf5ef0              // value to check
	// check f6 = 0x100f0 12345635ddcf5ef0
  	data8	0x0603				// f[6]
	data8   0x100f0          
  	data8	0x12345635ddcf5ef0              // value to check
	// check f7 = 0x2ffec 12345635ddcf5ef0
  	data8	0x0703				// f[7]
	data8   0x2ffec         
  	data8	0x12345635ddcf5ef0              // value to check
	// check f8 = 0x1003e 0000000012345635
  	data8	0x0803				// f[8]
	data8   0x1003e       
  	data8	0x0000000012345635              // value to check
	// check f9 = 0x1003e ffffffffddcf5ef0
  	data8	0x0903				// f[9]
	data8   0x1003e       
  	data8	0xffffffffddcf5ef0              // value to check
