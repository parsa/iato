// ---------------------------------------------------------------------------
// - F01.s                                                                   -
// - iato isa test suite : F1 group instruction test                         -
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
	movl r8 = _data7
	;;
	// default fpsr value
	mov ar.fpsr = r4
	;;
	ldf.fill f3  = [r1]
	ldf.fill f4  = [r2]
	ldf.fill f5  = [r3]
	ldf.fill f18 = [r5]
	ldf.fill f20 = [r6]
	ldf.fill f22 = [r7]
	ldf.fill f24 = [r8]
	;;
	fma.s0    f2  = f3, f4, f5
	fma.s     f6  = f3, f4, f5
	fma.d     f7  = f3, f4, f5
	fma.d     f8  = f5, f4, f3
	fnma.s1   f9  = f3, f3, f3
	fnma.s2   f10 = f4, f3, f5
	fnma.s.s3 f11 = f5, f4, f3
	fma.s0    f19 = f18, f1, f0
	fma.s0    f21 = f20, f1, f0	
   	fma.s0	  f23 = f22, f1, f0
	fma.s0    f32 = f1, f1, f0
	;;
	fnma.s    f12 = f9, f3, f9
	fnma.d    f13 = f5, f4, f3
	;;
	fpma      f14 = f3, f4, f5
	fpma      f15 = f3, f4, f0
	fpnma     f16 = f5, f4, f5
	fpnma     f17 = f3, f5, f0
	;;
	fms.s1    f24 = f3, f4, f5
	fms       f25 = f3, f4, f5
	fms.s     f26 = f3, f4, f5
	fms.d     f27 = f3, f4, f5
	fms.d     f28 = f8, f4, f5
	fpms      f29 = f5, f4, f5
	fpms      f30 = f3, f5, f0
	.endp _start

	.section ".iato.data", "wa", "progbits"
	_data1:	data8 0x923456789abcdef0
	_dat1:	data8 0x000000000002ffec
	_data2:	data8 0x95345435ddcf5ef0
	_dat2:	data8 0x00000000000100f0
	_data3:	data8 0xe23456789abcdef0
	_dat3:	data8 0x000000000003000f
	_data4:	data8 0x0000000000000001
	_dat4:	data8 0x0000000000020003
	_data5:	data8 0xffffffffffffffff
	_dat5:	data8 0x000000000001003e
	_data6:	data8 0x0000000000000004
	_dat6:	data8 0x000000000001003e
	_data7:	data8 0xfffffffffffffffe
	_dat7:	data8 0x000000000001003e
	
	.section ".iato.checker", "a", "progbits"
	// check r1 = _data1
  	data8	0x0101				// gr[1]
  	data8	_data1#				// value to check
	// check ar.fpsr = 0x0009804C0270033f
	data8   0x2806				// ar[40]
	data8   0x0009804C0270033f		// value to check
	// check f2 = 0x300dd aa6cb21b64e4e61b
  	data8	0x0203				// f[2]
	data8   0x300dd
  	data8	0xaa6cb21b64e4e61b	         // value to check
	// check f3 = 0x2ffec 923456789abcdef0
  	data8	0x0303				// f[3]
	data8   0x2ffec
  	data8	0x923456789abcdef0		// value to check
	// check f4 = 0x100f0 95345435ddcf5ef0
  	data8	0x0403				// f[4]
	data8   0x100f0
  	data8	0x95345435ddcf5ef0		// value to check
	// check f5 = 0x3000f e23456789abcdef0
  	data8	0x0503				// f[5]
	data8   0x3000f
  	data8	0xe23456789abcdef0		// value to check
	// check f6 = 0x3ffff 8000000000000000
  	data8	0x0603				// f[6]
	data8   0x3ffff
  	data8	0x8000000000000000		// value to check
	// check f7 = 0x300dd aa6cb21b64e4e800
  	data8	0x0703				// f[7]
	data8   0x300dd
  	data8	0xaa6cb21b64e4e800		// value to check
	// check f8 = 0x30101 83d6b35e87c34000
  	data8	0x0803				// f[8]
	data8   0x30101
  	data8	0x83d6b35e87c34000		// value to check
	// check f9 = 0x2ffec 92346b588a12129d
  	data8	0x0903				// f[9]
	data8   0x2ffec
  	data8	0x92346b588a12129d		// value to check
	// check f10 = 0x100dd aa6cb21b64e4e61b
  	data8	0x0a03				// f[10]
	data8   0x100dd
  	data8	0xaa6cb21b64e4e61b		// value to check
	// check f11 = 0x1ffff 8000000000000000
  	data8	0x0b03				// f[11]
	data8   0x1ffff
  	data8	0x8000000000000000		// value to check
	// check f12 = 0x2ffec 9234800000000000
  	data8	0x0c03				// f[12]
	data8   0x2ffec
  	data8	0x9234800000000000		// value to check
	// check f13 = 0x10101 83d6b35e87c34000
  	data8	0x0d03				// f[13]
	data8   0x10101
  	data8	0x83d6b35e87c34000		// value to check
	// check f14 = 0x1003e e23456783918f24f
  	data8	0x0e03				// f[14]
	data8   0x1003e
  	data8	0xe23456783918fe4f		// value to check
	// check f15 = 0x1003e 000000003918f24f
  	data8	0x0f03				// f[15]
	data8   0x1003e
  	data8	0x000000003918fe4f		// value to check
	// check f16 = 0x1003e e2345678b918fe4f
  	data8	0x1003				// f[16]
	data8   0x1003e
  	data8	0xe2345678b918fe4f		// value to check
	// check f17 = 0x1003e b4fe136c80000000
  	data8	0x1103				// f[17]
	data8   0x1003e
  	data8	0xb4fe136c80000000              // value to check
	// check f18 = 0x20003 0000000000000001
  	data8	0x1203				// f[18]
	data8   0x20003
  	data8	0x0000000000000001              // value to check
	// check f19 = 0x1003e b4fe136c80000000
  	data8	0x1303				// f[19]
	data8   0x20000
  	data8	0x0000000000000000              // value to check
	// check f20 = 0x1003e ffffffffffffffff
  	data8	0x1403				// f[20]
	data8   0x1003e
  	data8	0xffffffffffffffff              // value to check
	// check f21 = 0x1003e ffffffffffffffff
  	data8	0x1503				// f[21]
	data8   0x1003e
  	data8	0xffffffffffffffff              // value to check
	// check f22 = 0x1003e 0000000000000004                     
  	data8	0x1603				// f[22]
	data8   0x1003e
  	data8	0x0000000000000004             // value to check
	// check f23 = 0x10001 8000000000000000
  	data8	0x1703				// f[23]
	data8   0x10001     
  	data8	0x8000000000000000              // value to check
	// check f24 = 0x300dd aa6cb21b64e4e61b                     
  	data8	0x1803				// f[24]
	data8   0x300dd     
  	data8	0xaa6cb21b64e4e61b              // value to check
	// check f25 = 0x300dd aa6cb21b64e4e61b                   
  	data8	0x1903				// f[25]
	data8   0x300dd     
  	data8	0xaa6cb21b64e4e61b              // value to check
	// check f26 = 0x3ffff 8000000000000000                    
  	data8	0x1a03				// f[26]
	data8   0x3ffff     
  	data8	0x8000000000000000              // value to check
	// check f27 = 0x300dd aa6cb21b64e4e800                    
  	data8	0x1b03				// f[27]
	data8   0x300dd    
  	data8	0xaa6cb21b64e4e800              // value to check
	// check f28 = 0x301f2 99add2c149830800                    
  	data8	0x1c03				// f[28]
	data8   0x301f2     
  	data8	0x99add2c149830800              // value to check
	// check f29 = 0x1003e 623456783918fe4f                    
  	data8	0x1d03				// f[29]
	data8   0x1003e     
  	data8	0x623456783918fe4f              // value to check
	// check f30 = 0x1003e 34fe136c00000004                    
  	data8	0x1e03				// f[30]
	data8   0x1003e     
  	data8	0x34fe136c00000004              // value to check
