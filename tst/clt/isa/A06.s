// ---------------------------------------------------------------------------
// - A06.s                                                                 -
// - iato isa test suite : A6 group instruction test                         -
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
	mov   r3  = 1
	mov   r6  = 1
	mov   r14 = -6
	mov   r5  = -6;;

	cmp.eq p2, p3 = r3, r5               // p2  = false p3 = true
	;; 
	(p2) cmp.eq.unc p4, p5 = r3, r5      // p4  = false p5= false
	cmp.lt p8, p9 = r3, r5               // p8  = false p9 = true
	(p2) cmp.lt.unc p10, p11 = r3, r5    // p10 = false p11= false
	cmp.ltu p12, p13 = r3, r5            // p12 = true  p13 = false
	cmp.ltu.unc p14, p15 = r3, r0        // p14 = false p15 = true
	cmp.eq.or  p28, p29 = r3, r6;;       // p28 = true  p29 = true 
	cmp.eq.and p14, p15 = r3, r6         // p14 = false p15 = true
	cmp.eq.or.andcm p30, p31 = r5, r14   // p30 = true  p31 = false
	cmp.ne.or  p40, p41 = r3, r6;;       // p40 = false p41 = false
	cmp.ne.and p42, p43 = r3, r6         // p42 = false p43 = false
	cmp.ne.or.andcm p44, p45 = r5, r14   // p44 = false p45 = false	

	cmp4.eq p16, p17 = r3, r3            // p16 = true  p17 = false 
	(p2) cmp4.eq.unc p18, p19 = r3, r5   // p18 = false p19 = false
	cmp4.lt p20, p21 = r3, r5            // p20 = false p21 = true
	(p2) cmp4.lt.unc p22, p23 = r3, r5   // p22=  false p23 = false
	cmp4.ltu p24, p25 = r3, r5           // p24 = true  p25 = false
	cmp4.ltu.unc p32, p33 = r3, r0       // p32 = false p33 = true
  	cmp4.eq.or  p28, p29 = r3, r5;;      // p28 = true  p29 = true 
	cmp4.eq.and p36, p37 = r3, r5        // p36 = false p37 = false
	cmp4.eq.or.andcm p38, p39 = r5, r14  // p38 = true  p39 = false
	cmp4.ne.or  p46, p47 = r3, r5;;      // p46 = true  p47 = true 
	cmp4.ne.and p48, p49 = r3, r5        // p48 = false p49 = false
	cmp4.ne.or.andcm p50, p51 = r5, r14  // p50 = false p51 = false	
	.endp _start#

	.section ".iato.checker", "a", "progbits"
  	// check p2 = false
  	data8	0x0204			// p[2]
  	data8   0x00	          	// value to check
 	// check p3 = true
  	data8	0x0304			// p[3]
  	data8   0x01 	          	// value to check
 	// check p4 = false
  	data8	0x0404			// p[4]
  	data8   0x00	          	// value to check
 	// check p5 = false
  	data8	0x0504			// p[5]
  	data8   0x00	          	// value to check
 	// check p8 = false
  	data8	0x0804			// p[8]
  	data8   0x00	          	// value to check
 	// check p9 = true
  	data8	0x0904			// p[9]
  	data8   0x01	          	// value to check
 	// check p10 = false
  	data8	0x0A04			// p[10]
  	data8   0x00	          	// value to check
	// check p11 = false
  	data8	0x0B04			// p[11]
  	data8   0x00	          	// value to check
	// check p12 = true 
  	data8	0x0C04			// p[12]
  	data8   0x01	          	// value to check
	// check p13 = false
  	data8	0x0D04			// p[13]
  	data8   0x00	          	// value to check
	// check p14 = false
  	data8	0x0E04			// p[14]
  	data8   0x00	          	// value to check
	// check p15 = true 
  	data8	0x0F04			// p[15]
  	data8   0x01	          	// value to check
	// check p16 = true 
  	data8	0x1004			// p[16]
  	data8   0x01	          	// value to check
	// check p17 = false
  	data8	0x1104			// p[17]
  	data8   0x00	          	// value to check
	// check p18 = false
  	data8	0x1204			// p[18]
  	data8   0x00	          	// value to check
	// check p19 = false
  	data8	0x1304			// p[19]
  	data8   0x00	          	// value to check
	// check p20 = false
  	data8	0x1404			// p[20]
  	data8   0x00	          	// value to check
	// check p21 = true 
  	data8	0x1504			// p[21]
  	data8   0x01	          	// value to check
	// check p22 = false
  	data8	0x1604			// p[22]
  	data8   0x00	          	// value to check
	// check p23 = false
  	data8	0x1704			// p[23]
  	data8   0x00	          	// value to check
	// check p24 = true 
  	data8	0x1804			// p[24]
  	data8   0x01	          	// value to check
	// check p25 = false
  	data8	0x1904			// p[25]
  	data8   0x00	          	// value to check
	// check p28 = true 
  	data8	0x1c04			// p[28]
  	data8   0x01	          	// value to check
	// check p29 = true 
  	data8	0x1d04			// p[29]
  	data8   0x01	          	// value to check
	// check p30 = true 
  	data8	0x1e04			// p[30]
  	data8   0x01	          	// value to check
	// check p31 = false
  	data8	0x1F04			// p[31]
  	data8   0x00	          	// value to check
	// check p32 = false
  	data8	0x2004			// p[32]
  	data8   0x00	          	// value to check
	// check p33 = true 
  	data8	0x2104			// p[33]
  	data8   0x01	          	// value to check	
	// check p36 = false
  	data8	0x2404			// p[36]
  	data8   0x00	          	// value to check
	// check p37 = false
  	data8	0x2504			// p[37]
  	data8   0x00	          	// value to check
	// check p38 = true 
  	data8	0x2604			// p[38]
  	data8   0x01	          	// value to check
	// check p39 = false
  	data8	0x2704			// p[39]
  	data8   0x00	          	// value to check
	// check p40 = false
  	data8	0x2804			// p[40]
  	data8   0x00 	          	// value to check
	// check p41 = false
  	data8	0x2904			// p[41]
  	data8   0x00	          	// value to check
	// check p42 = false
  	data8	0x2a04			// p[42]
  	data8   0x00	          	// value to check
	// check p43 = false
  	data8	0x2b04			// p[43]
  	data8   0x00	          	// value to check
	// check p44 = false
  	data8	0x2c04			// p[44]
  	data8   0x00	          	// value to check
	// check p45 = false
  	data8	0x2d04			// p[45]
  	data8   0x00	          	// value to check
	// check p46 = true 
  	data8	0x2e04			// p[46]
  	data8   0x01	          	// value to check
	// check p47 = true 
  	data8	0x2f04			// p[47]
  	data8   0x01	          	// value to check
	// check p48 = false
  	data8	0x3004			// p[48]
  	data8   0x00	          	// value to check
	// check p49 = false
  	data8	0x3104			// p[49]
  	data8   0x00	          	// value to check
	// check p50 = false
  	data8	0x3204			// p[51]
  	data8   0x00	          	// value to check
	// check p51 = false
  	data8	0x3304			// p[51]
  	data8   0x00	          	// value to check
