// ---------------------------------------------------------------------------
// - F05.s                                                                   -
// - iato isa test suite : F5 group instruction test                         -
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
	fclass.m p3, p4 = f3, 0x131
	fclass.m.unc p5, p6 = f4, 0x131
	;; 
	(p5) fclass.m.unc p7, p8 = f4, 0x131
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
	// check p3 = false
  	data8	0x0304			// p[3]
  	data8   0x00 	          	// value to check
 	// check p4 = true 
  	data8	0x0404			// p[4]
  	data8   0x01	          	// value to check
 	// check p5 = false
  	data8	0x0504			// p[5]
  	data8   0x00	          	// value to check
	// check p6 = true 
  	data8	0x0604			// p[6]
  	data8   0x01	          	// value to check
 	// check p7 = false
  	data8	0x0704			// p[7]
  	data8   0x00	          	// value to check	         
	// check p8 = false
  	data8	0x0804			// p[8]
  	data8   0x00	          	// value to check	         
