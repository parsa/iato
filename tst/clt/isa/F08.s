// ---------------------------------------------------------------------------
// - F08.s                                                                   -
// - iato isa test suite : F8 group instruction test                         -
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

	ldf.fill f3 = [r1]
	ldf.fill f4 = [r2]
	ldf.fill f5 = [r3]
	ldf.fill f6 = [r5]
	ldf.fill f7 = [r6]
	ldf.fill f8 = [r7]
	ldf.fill f9 = [r8]
	;;
	fmin   f2  = f3, f4
	fmax   f10 = f3, f5
	famin  f11 = f7, f8
	famax  f12 = f9, f6
	famax  f13 = f6, f9
	fpmin  f14 = f9, f3
	fpmin  f15 = f3, f9
	fpmax  f16 = f7, f8
	fpamin f17 = f4, f3
	fpamax f18 = f4, f3
	;;
	fpcmp.eq    f19 = f7, f8
	fpcmp.lt    f20 = f9, f8
	fpcmp.le    f21 = f6, f5
	fpcmp.unord f22 = f7, f8
	fpcmp.neq   f23 = f5, f6
	fpcmp.nlt   f24 = f7, f9
	fpcmp.nle   f25 = f3, f8
	fpcmp.ord   f26 = f7, f3
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
	_data5:	data8 0xffffff1234567890
	_dat5:	data8 0x0000000000010f3e
	_data6:	data8 0x9230000000000004
	_dat6:	data8 0x0000000000030e3e
	_data7:	data8 0xfffffffffffffffe
	_dat7:	data8 0x000000000001ffff
	
	
	.section ".iato.checker", "a", "progbits"
	// check r1 = _data1
  	data8	0x0101				// gr[1]
  	data8	_data1#				// value to check
	// check ar.fpsr = 0x0009804C0270033f
	data8   0x2806				// ar[40]
	data8   0x0009804C0270033f		// value to check
	// check f2 = 0x2ffec 923456789abcdef0
  	data8	0x0203				// f[2]
	data8   0x2ffec
  	data8	0x923456789abcdef0	        // value to check
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
	// check f6 = 0x20003 0000000000000001
  	data8	0x0603				// f[06]
	data8   0x20003
  	data8	0x0000000000000001              // value to check
	// check f7 = 0x10f3e ffffff1234567890
  	data8	0x0703				// f[07]
	data8   0x10f3e
  	data8	0xffffff1234567890              // value to check
	// check f08 = 0x10e3e 9230000000000004                     
  	data8	0x0803				// f[08]
	data8   0x30e3e
  	data8	0x9230000000000004              // value to check
	// check f09 = 0x1ffff fffffffffffffffe                     
  	data8	0x0903				// f[09]
	data8   0x1ffff     
  	data8	0xfffffffffffffffe              // value to check
	// check f10 = 0x2ffec 923456789abcdef0
  	data8	0x0a03				// f[10]
	data8   0x2ffec
  	data8	0x923456789abcdef0	        // value to check
	// check f11 = 0x10e3e 9230000000000004                     
  	data8	0x0b03				// f[11]
	data8   0x30e3e
  	data8	0x9230000000000004              // value to check
	// check f12 = 0x20003 0000000000000001
  	data8	0x0c03				// f[12]
	data8   0x20003
  	data8	0x0000000000000001              // value to check
	// check f13 = 0x1ffff fffffffffffffffe                     
  	data8	0x0d03				// f[13]
	data8   0x1ffff     
  	data8	0xfffffffffffffffe              // value to check
	// check f14 = 0x1003e 923456789abcdef0
  	data8	0x0e03				// f[14]
	data8   0x1003e
  	data8	0x923456789abcdef0		// value to check
	// check f15 = 0x1003e 923456789abcdef0
  	data8	0x0f03				// f[15]
	data8   0x1003e
  	data8	0xfffffffffffffffe		// value to check
	// check f16 = 0x1003e 9230000034567890
  	data8	0x1003				// f[16]
	data8   0x1003e
  	data8	0x9230000034567890		// value to check	
	// check f17 = 0x1003e 923456789abcdef0
  	data8	0x1103				// f[17]
	data8   0x1003e
  	data8	0x923456789abcdef0		// value to check
	// check f18 = 0x1003e 95345435ddcf5ef0
  	data8	0x1203				// f[18]
	data8   0x1003e
  	data8	0x95345435ddcf5ef0		// value to check
	// check f19 = 0x1003e 0000000000000000
  	data8	0x1303				// f[19]
	data8   0x1003e
  	data8	0x0000000000000000    		// value to check
	// check f20 = 0x1003e 0000000000000000
  	data8	0x1403				// f[20]
	data8   0x1003e
  	data8	0x0000000000000000     		// value to check
	// check f21 = 0x1003e 0000000000000000
  	data8	0x1503				// f[21]
	data8   0x1003e
  	data8	0x0000000000000000   		// value to check
	// check f22 = 0x1003e ffffffff00000000
  	data8	0x1603				// f[22]
	data8   0x1003e
  	data8	0xffffffff00000000   		// value to check
	// check f23 = 0x1003e ffffffffffffffff
  	data8	0x1703				// f[23]
	data8   0x1003e
  	data8	0xffffffffffffffff     		// value to check
	// check f24 = 0x1003e ffffffffffffffff
  	data8	0x1803				// f[24]
	data8   0x1003e
  	data8	0xffffffffffffffff    		// value to check
	// check f25 = 0x1003e 0000000000000000
  	data8	0x1903				// f[25]
	data8   0x1003e
  	data8	0x0000000000000000     		// value to check
	// check f26 = 0x1003e 00000000ffffffff
  	data8	0x1a03				// f[26]
	data8   0x1003e
  	data8	0x00000000ffffffff		// value to check
