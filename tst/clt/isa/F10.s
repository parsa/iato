// ---------------------------------------------------------------------------
// - F10.s                                                                   -
// - iato isa test suite : F10 group instruction test                        -
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
	movl r4 = 0x000b806C0270033f
	movl r5 = _data4
	movl r6 = _data5
	;;
	mov ar.fpsr = r4
	;; 
	ldf.fill f3  = [r1]
	ldf.fill f4  = [r2]
	ldf.fill f2  = [r3]
	ldf.fill f15 = [r5]
	ldf.fill f20 = [r6]
	;;
	fcvt.fx.s2     f5  = f4	
	fcvt.fx        f6  = f2	
	fcvt.fx        f7  = f4	
	fcvt.fx.trunc  f8  = f4	
	fcvt.fx.s3     f9  = f4	
	fcvt.fxu.trunc f10 = f3	
	fcvt.fxu.s2    f11 = f3	
	fcvt.fxu.s1    f12 = f4
	fcvt.fxu.trunc f13 = f2	
	fcvt.fxu.trunc f14 = f3
	fcvt.fxu.s1    f16 = f15
	fcvt.fx.s2     f17 = f15
	fcvt.fx.s3     f18 = f15
	fcvt.fx.trunc  f19 = f15
	fcvt.fx        f21 = f20
	.endp

	.section ".iato.data", "wa", "progbits"
	_data1:	data8 0x923456789abcdef1
	_dat1:	data8 0x000000000001003e
	_data2:	data8 0x05345435ddcf5ef0
	_dat2:	data8 0x0000000000010030
	_data3:	data8 0x05345435ddcf5ef0
	_dat3:	data8 0x000000000001f030
	_data4:	data8 0x05345435ddcf5ef0
	_dat4:	data8 0x0000000000030030
	_data5:	data8 0x8000000000000000
	_dat5:  data8 0x000000000001003e
	
	.section ".iato.checker", "a", "progbits"
	// check r1 = _data1
  	data8	0x0101				// gr[1]
  	data8	_data1#				// value to check
	// check ar.fpsr = 0x0x000b806C0270033f
	data8   0x2806				// ar[40]
	data8   0x000b806C0270033f		// value to check
	// check f2 = 0x1f030 05345435ddcf5ef0
  	data8	0x0203				// f[2]
	data8   0x1f030
  	data8	0x05345435ddcf5ef0	         // value to check
	// check f3 = 0x1003e 923456789abcdef1
  	data8	0x0303				// f[3]
	data8   0x1003e
  	data8	0x923456789abcdef1		// value to check
	// check f4 = 0x10030 05345435ddcf5ef0
  	data8	0x0403				// f[4]
	data8   0x10030
  	data8	0x05345435ddcf5ef0		// value to check
	// check f5 = 0x1003e
  	data8	0x0503				// f[5]
	data8   0x1003e
  	data8	0x000014d150d7773e              // value to check
	// check f6 = 0x1003e 8000000000000000
  	data8	0x0603				// f[6]
	data8   0x1003e
  	data8	0x8000000000000000              // value to check
	// check f7 = 0x1003e 000014d150d7773d
  	data8	0x0703				// f[7]
	data8   0x1003e
  	data8	0x000014d150d7773d              // value to check
	// check f8 = 0x1003e 000014d150d7773d
  	data8	0x0803				// f[8]
	data8   0x1003e
  	data8	0x000014d150d7773d              // value to check
	// check f9 = 0x1003e 000014d150d7773d
  	data8	0x0903				// f[9]
	data8   0x1003e
  	data8	0x000014d150d7773d              // value to check
	// check f10 = 0x1003e 923456789abcdef1
  	data8	0x0a03				// f[10]
	data8   0x1003e
  	data8	0x923456789abcdef1              // value to check
	// check f11 = 0x1003e 923456789abcdef1
  	data8	0x0b03				// f[11]
	data8   0x1003e
  	data8	0x923456789abcdef1              // value to check
	// check f12 = 0x1003e 000014d150d7773d
  	data8	0x0c03				// f[12]
	data8   0x1003e
  	data8	0x000014d150d7773d              // value to check
	// check f13 = 0x1003e 8000000000000000
  	data8	0x0d03				// f[13]
	data8   0x1003e
  	data8	0x8000000000000000              // value to check
	// check f14 = 0x1003e 923456789abcdef1
  	data8	0x0e03				// f[14]
	data8   0x1003e
  	data8	0x923456789abcdef1              // value to check
	// check f15 = 0x30030 05345435ddcf5ef0              
  	data8	0x0f03				// f[15]
	data8   0x30030    
  	data8	0x05345435ddcf5ef0              // value to check
	// check f16 = 0x1003e 8000000000000000
  	data8	0x1003				// f[16]
	data8   0x1003e
  	data8	0x8000000000000000              // value to check
	// check f17 = 0x1003e ffffeb2eaf2888c3
  	data8	0x1103				// f[17]
	data8   0x1003e
  	data8	0xffffeb2eaf2888c3              // value to check
	// check f18 = 0x1003e ffffeb2eaf2888c2
  	data8	0x1203	 			// f[18]
	data8   0x1003e
  	data8	0xffffeb2eaf2888c2              // value to check
	// check f19 = 0x1003e ffffeb2eaf2888c3
  	data8	0x1303				// f[19]
	data8   0x1003e
  	data8	0xffffeb2eaf2888c3             // value to check
	// check f20 = 0x1003e 8000000000000000
  	data8	0x1403				// f[20]
	data8   0x1003e
  	data8	0x8000000000000000              // value to check
	// check f21 = 0x1003e 8000000000000000
  	data8	0x1403				// f[21]
	data8   0x1003e
  	data8	0x8000000000000000              // value to check
