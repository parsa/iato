// ---------------------------------------------------------------------------
// - F04.s                                                                   -
// - iato isa test suite : F4 group instruction test                         -
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
	;;
	ldf.fill f8 = [r1]
	ldf.fill f9 = [r1]
	ldf.fill f4 = [r2]
	ldf.fill f5 = [r3]
	;; 
	fcmp.eq p3, p4 = f8, f9
	fcmp.lt.s2 p5, p6 = f4, f5
	fcmp.le p7, p8 = f5, f9
	//fcmp.unord.s1 p9, p10 = f4, f9
	fcmp.eq.unc p11, p12 = f8, f4
	fcmp.lt.unc.s2 p13, p14 = f5, f4
	fcmp.le.unc p15, p16 = f8, f5
	//fcmp.unord.unc.s1 p17, p18 = f8, f9
	;; 
	(p15) fcmp.eq p19, p20 = f8, f9
	(p8)  fcmp.lt.s2 p21, p22 = f8, f9
	(p7)  fcmp.le p23, p24 = f8, f9
	//(p6)  fcmp.unord.s1 p25, p26 = f5, f4
	(p4)  fcmp.eq.unc p27, p28 = f8, f4
	(p4)  fcmp.lt.unc.s2 p29, p30 = f8, f5
	(p3)  fcmp.le.unc p31, p32 = f5, f9
	//(p8)  fcmp.unord.unc.s1 p33, p34 = f4, f9
	.endp

	.section ".iato.data", "wa", "progbits"
	_data1:	data8 0x923456789abcdef0
	_dat1:	data8 0x000000000002ffec
	_data2:	data8 0x95345435ddcf5ef0
	_dat2:	data8 0x00000000000100f0
	_data3:	data8 0xe23456789abcdef0
	_dat3:	data8 0x000000000003000f

	.section ".iato.checker", "a", "progbits"
	// check f4 = 0x100f0 95345435ddcf5ef0
  	data8	0x0403				// f[4]
	data8   0x100f0
  	data8	0x95345435ddcf5ef0		// value to check
	// check f5 = 0x3000f e23456789abcdef0
  	data8	0x0503				// f[5]
	data8   0x3000f
  	data8	0xe23456789abcdef0		// value to check
	// check f8 = 0x2ffec 923456789abcdef0
  	data8	0x0803				// f[8]
	data8   0x2ffec
  	data8	0x923456789abcdef0		// value to check
	// check f9 = 0x2ffec 923456789abcdef0
  	data8	0x0903				// f[9]
	data8   0x2ffec
  	data8	0x923456789abcdef0		// value to check
 	// check p3 = true
  	data8	0x0304			// p[3]
  	data8   0x01 	          	// value to check
 	// check p4 = false
  	data8	0x0404			// p[4]
  	data8   0x00	          	// value to check
 	// check p5 = false
  	data8	0x0504			// p[5]
  	data8   0x00	          	// value to check
 	// check p6 = true 
  	data8	0x0604			// p[6]
  	data8   0x01	          	// value to check
 	// check p7 = true 
  	data8	0x0704			// p[7]
  	data8   0x01	          	// value to check	
 	// check p8 = false
  	data8	0x0804			// p[8]
  	data8   0x00	          	// value to check
 	// check p9 = false
  	//data8	0x0904			// p[9]
  	//data8   0x00	          	// value to check
 	// check p10 = true 
  	//data8	0x0A04			// p[10]
  	//data8   0x01	          	// value to check
	// check p11 = false
  	data8	0x0B04			// p[11]
  	data8   0x00	          	// value to check
	// check p12 = true 
  	data8	0x0C04			// p[12]
  	data8   0x01	          	// value to check
	// check p13 = true 
  	data8	0x0D04			// p[13]
  	data8   0x01	          	// value to check
	// check p14 = false
  	data8	0x0E04			// p[14]
  	data8   0x00	          	// value to check
	// check p15 = false
  	data8	0x0F04			// p[15]
  	data8   0x00	          	// value to check
	// check p16 = true 
  	data8	0x1004			// p[16]
  	data8   0x01	          	// value to check
	// check p17 = true 
  	//data8	0x1104			// p[17]
  	//data8   0x01	          	// value to check
	// check p18 = false
  	//data8	0x1204			// p[18]
  	//data8   0x00	          	// value to check
	// check p19 = false
  	data8	0x1304			// p[19]
  	data8   0x00	          	// value to check
	// check p20 = false
  	data8	0x1404			// p[20]
  	data8   0x00	          	// value to check
	// check p21 = false
  	data8	0x1504			// p[21]
  	data8   0x00	          	// value to check
	// check p22 = false
  	data8	0x1604			// p[22]
  	data8   0x00	          	// value to check
	// check p23 = true 
  	data8	0x1704			// p[23]
  	data8   0x01	          	// value to check
	// check p24 = false
  	data8	0x1804			// p[24]
  	data8   0x00	          	// value to check
	// check p25 = false
  	//data8	0x1904			// p[25]
  	//data8   0x00	          	// value to check
	// check p26 = false
  	//data8	0x1a04			// p[26]
  	//data8   0x00	          	// value to check
	// check p27 = false
  	data8	0x1b04			// p[27]
  	data8   0x00	          	// value to check
	// check p28 = false
  	data8	0x1c04			// p[28]
  	data8   0x00	          	// value to check
	// check p29 = false
  	data8	0x1d04			// p[29]
  	data8   0x00	          	// value to check
	// check p30 = false
  	data8	0x1e04			// p[30]
  	data8   0x00	          	// value to check
	// check p31 = true 
  	data8	0x1F04			// p[31]
  	data8   0x01	          	// value to check
	// check p32 = false
  	data8	0x2004			// p[32]
  	data8   0x00	          	// value to check
	// check p33 = false
  	//data8	0x2104			// p[33]
  	//data8   0x00	          	// value to check	
	// check p34 = false
  	//data8	0x2204			// p[34]
  	//data8   0x00	          	// value to check
