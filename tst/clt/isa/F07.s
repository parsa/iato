// ---------------------------------------------------------------------------
// - F07.s                                                                   -
// - iato isa test suite : F7 group instruction test                         -
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
	movl r3 = _data3
	movl r4 = 0x0009804C0270033f
	movl r5 = _data4
	movl r6 = _data5
	movl r7 = _data6
	;;
	// default fpsr value
	mov ar.fpsr = r4
	;;
	ldf.fill f2  = [r1]
	ldf.fill f3  = [r2]
	ldf.fill f4  = [r3]
	ldf.fill f5  = [r5]
	ldf.fill f11 = [r6]
	ldf.fill f13 = [r7]
	;;
	frsqrta f7, p2  = f2
	frsqrta f8, p3  = f3
	frsqrta f9, p4  = f4
	frsqrta f10, p5 = f5
  	fprsqrta f12, p7 = f11
  	fprsqrta f14, p8 = f13
	.endp _start

	.section ".iato.data", "wa", "progbits"
	_data1:	data8 0x8000000000000000
	_dat1:	data8 0x000000000001ffff
	_data2:	data8 0x95345435ddcf5ef0
	_dat2:	data8 0x00000000000300f0
	_data3:	data8 0xe23456789abcdef0
	_dat3:	data8 0x000000000001060f
	_data4:	data8 0x5908609456095680
	_dat4:	data8 0x0000000000010f3f
	_data5:	data8 0x7f80000000000000
	_dat5:	data8 0x0000000000010f3f
	_data6:	data8 0x8573954703952365
	_dat6:	data8 0x0000000000003f3f
	
	.section ".iato.checker", "a", "progbits"
	// check r1 = _data1
  	data8	0x0101				// gr[1]
  	data8	_data1#				// value to check
	// check f2 = 0x1ffff 800000000000000
  	data8	0x0203				// f[2]
	data8   0x1ffff
  	data8	0x8000000000000000	         // value to check
	// check f3 = 0x300f0 95345435ddcf5ef0
  	data8	0x0303				// f[3]
	data8   0x300f0
  	data8	0x95345435ddcf5ef0		// value to check
	// check f4 = 0x21060f e23456789abcdef0                
  	data8	0x0403				// f[4]
	data8   0x1060f
  	data8	0xe23456789abcdef0		// value to check
	// check f5 = 0x10f3f 5908609456095680
	data8	0x0503				// f[5]
	data8   0x10f3f
	data8	0x5908609456095680  		// value to check
	// check f6 = 0x3ffff 8000000000000000
//	data8	0x0603				// f[6]
//	data8   0x3ffff
//	data8	0x8000000000000000		// value to check
	// check f7 = 0x1ffff 8000000000000000
  	data8	0x0703				// f[7]
	data8   0x1ffff
  	data8	0x8000000000000000		// value to check
	// check f8 = 0x1ffff 8000000000000001
  	data8	0x0803				// f[8]
	data8   0x1ffff
  	data8	0x8000000000000001		// value to check
	// check f9 = 0x00fcf6 c080000000000000
  	data8	0x0903				// f[9]
	data8   0x0fcf6
  	data8	0xc080000000000000		// value to check
	// check f10 = 0x0f85f 9940000000000000
  	data8	0x0a03				// f[10]
	data8   0x0f85f
  	data8	0x9940000000000000		// value to check
	// check f11 = 0x10f3f 7f80000000000000
  	data8	0x0b03				// f[11]
	data8   0x10f3f
  	data8	0x7f80000000000000		// value to check
	// check f12 = 0x1003e 000000007f800000
  	data8	0x0c03				// f[12]
	data8   0x1003e
  	data8	0x000000007f800000		// value to check
	// check f13 = 0x03f3f 8573954703952365                     
  	data8	0x0d03				// f[13]
	data8   0x03f3f     
  	data8	0x8573954703952365     		// value to check
	// check f14 = 0x1003e ffc000005d6ce000                     
  	data8	0x0e03				// f[14]
	data8   0x1003e      
  	data8	0xffc000005d6ce000    		// value to check
  	// check p2 = false
  	data8	0x0204				// p[2]
  	data8   0x00			  	// value to check
 	// check p3 = false
  	data8	0x0304				// p[3]
  	data8   0x00				// value to check
 	// check p4 = true 
  	data8	0x0404				// p[4]
  	data8   0x01				// value to check
 	// check p5 = true	
  	data8	0x0504				// p[5]
  	data8   0x01			  	// value to check
	// check p7 = false	
  	data8	0x0704				// p[7]
  	data8   0x00			  	// value to check
	// check p8 = false	
  	data8	0x0804				// p[8]
  	data8   0x00			  	// value to check
