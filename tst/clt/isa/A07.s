// ---------------------------------------------------------------------------
// - A07.s                                                                   -
// - iato isa test suite : A7 group instruction test                         -
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
	mov   r5  = -6;;

	cmp.gt.and p2, p3 = r0, r5               // p2  = false p3  = false 
	cmp.gt.or p4, p5 = r0, r5                // p4  = true  p5  = true 
	cmp.gt.or.andcm p6, p7 = r0, r5;;        // p6  = true  p7  = false
  (p7)	cmp.ltu.unc p24, p25 = r3, r5;;	         // p24 = false p25 = false

	cmp.le.and p8, p9 = r0, r5               // p8  = false p9  = false 
	cmp.le.or p10, p11 = r0, r5              // p10 = false p11 = false
	cmp.le.or.andcm p12, p13 = r0, r5        // p12 = false p13 = false

	cmp.ge.and p4, p5 = r0, r5               // p4  = true  p5  = true  
	cmp.ge.or p14, p15 = r0, r0              // p14 = true  p15 = true 
	cmp.ge.or.andcm p16, p17 = r0, r5        // p16 = true  p17 = false

	cmp.lt.and p18, p19 = r0, r3             // p18 = false p19 = false 
	cmp.lt.or p20, p21 = r0, r3              // p20 = true  p21 = true
	cmp.lt.or.andcm p22, p23 = r0, r3        // p22 = true  p23 = false

	cmp4.gt.and p32, p33 = r0, r5            // p32 = false p33 = false 
	cmp4.gt.or p34, p35 = r0, r5             // p34 = true  p35 = true 
	cmp4.gt.or.andcm p36, p37 = r0, r5;;     // p36 = true  p37 = false

	cmp4.le.and p38, p39 = r0, r5            // p38 = false p39 = false 
	cmp4.le.or p40, p41 = r0, r5             // p40 = false p41 = false
	cmp4.le.or.andcm p42, p43 = r0, r5       // p42 = false p43 = false

	cmp4.ge.and p34, p35 = r0, r5            // p34 = true  p35 = true  
	cmp4.ge.or p44, p45 = r0, r0             // p44 = true  p45 = true 
	cmp4.ge.or.andcm p46, p47 = r0, r5       // p46 = true  p47 = false

	cmp4.lt.and p48, p49 = r0, r3            // p48 = false p59 = false 
	cmp4.lt.or p50, p51 = r0, r3             // p50 = true  p51 = true
	cmp4.lt.or.andcm p52, p53 = r0, r3       // p52 = true  p53 = false
	.endp _start#

	.section ".iato.checker", "a", "progbits"
  	// check p2 = false
  	data8	0x0204			// p[2]
  	data8   0x00	          	// value to check
 	// check p3 = false
  	data8	0x0304			// p[3]
  	data8   0x00 	          	// value to check
 	// check p4 = true 
  	data8	0x0404			// p[4]
  	data8   0x01	          	// value to check
 	// check p5 = true 
  	data8	0x0504			// p[5]
  	data8   0x01	          	// value to check
	// check p6 = true 
  	data8	0x0604			// p[6]
  	data8   0x01	          	// value to check
 	// check p7 = false
  	data8	0x0704			// p[7]
  	data8   0x00	          	// value to check	         
 	// check p8 = false
  	data8	0x0804			// p[8]
  	data8   0x00	          	// value to check
 	// check p9 = false
  	data8	0x0904			// p[9]
  	data8   0x00	          	// value to check
 	// check p10 = false
  	data8	0x0A04			// p[10]
  	data8   0x00	          	// value to check
	// check p11 = false
  	data8	0x0B04			// p[11]
  	data8   0x00	          	// value to check
	// check p12 = false
  	data8	0x0C04			// p[12]
  	data8   0x00	          	// value to check
	// check p13 = false
  	data8	0x0D04			// p[13]
  	data8   0x00	          	// value to check
	// check p14 = true 
  	data8	0x0E04			// p[14]
  	data8   0x01	          	// value to check
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
	// check p20 = true 
  	data8	0x1404			// p[20]
  	data8   0x01	          	// value to check
	// check p21 = true 
  	data8	0x1504			// p[21]
  	data8   0x01	          	// value to check
	// check p22 = true 
  	data8	0x1604			// p[22]
  	data8   0x01	          	// value to check
	// check p23 = false
  	data8	0x1704			// p[23]
  	data8   0x00	          	// value to check
	// check p24 = false
  	data8	0x1804			// p[24]
  	data8   0x00	          	// value to check
	// check p25 = false
  	data8	0x1904			// p[25]
  	data8   0x00	          	// value to check
	// check p32 = false
  	data8	0x2004			// p[32]
  	data8   0x00	          	// value to check
	// check p33 = false
  	data8	0x2104			// p[33]
  	data8   0x00	          	// value to check
	// check p34 = true
  	data8	0x2204			// p[34]
  	data8   0x01	          	// value to check
	// check p35 = true 
  	data8	0x2304			// p[35]
  	data8   0x01	          	// value to check	
	// check p36 = true 
  	data8	0x2404			// p[36]
  	data8   0x01	          	// value to check
	// check p37 = false
  	data8	0x2504			// p[37]
  	data8   0x00	          	// value to check
	// check p38 = false
  	data8	0x2604			// p[38]
  	data8   0x00	          	// value to check
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
	// check p44 = true
  	data8	0x2c04			// p[44]
  	data8   0x01 	          	// value to check
	// check p45 = true        
  	data8	0x2d04			// p[45]
  	data8   0x01	          	// value to check
	// check p46 = true 
  	data8	0x2e04			// p[46]
  	data8   0x01	          	// value to check
	// check p47 = false
  	data8	0x2f04			// p[47]
  	data8   0x00	          	// value to check
	// check p48 = false
  	data8	0x3004			// p[48]
  	data8   0x00	          	// value to check
	// check p49 = false
  	data8	0x3104			// p[49]
  	data8   0x00	          	// value to check
	// check p50 = true 
  	data8	0x3204			// p[50]
  	data8   0x01	          	// value to check
	// check p51 = true 
  	data8	0x3304			// p[51]
  	data8   0x01	          	// value to check
	// check p52 = true 
  	data8	0x3404			// p[52]
  	data8   0x01	          	// value to check
	// check p53 = false
  	data8	0x3504			// p[53]
  	data8   0x00	          	// value to check
