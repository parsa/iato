// ---------------------------------------------------------------------------
// - I23.s                                                                   -
// - iato isa test suite : I23 group instruction test                        -
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
	movl r6 = 0x1234512345123456
	;;
	// reset pr
	mov pr = r0, 0x1FFFF
	;; 
	mov pr = r6, 0x1FFF0
	.endp _start#
	
	.section ".iato.checker", "a", "progbits"
  	// check p1 = false
  	data8	0x0104			// p[1]
  	data8   0x00	          	// value to check
  	// check p2 = false 
  	data8	0x0204			// p[2]
  	data8   0x00	          	// value to check
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
 	// check p9 = false
  	data8	0x0904			// p[9]
  	data8   0x00	          	// value to check
 	// check p10 = true 
  	data8	0x0A04			// p[10]
  	data8   0x01	          	// value to check
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
	// check p16 = false
  	data8	0x1004			// p[16]
  	data8   0x00	          	// value to check
	// check p17 = true 
  	data8	0x1104			// p[17]
  	data8   0x01	          	// value to check
	// check p18 = false
  	data8	0x1204			// p[18]
  	data8   0x00	          	// value to check
	// check p19 = false
  	data8	0x1304			// p[19]
  	data8   0x00	          	// value to check
	// check p20 = true 
  	data8	0x1404			// p[20]
  	data8   0x01	          	// value to check
	// check p21 = false
  	data8	0x1504			// p[21]
  	data8   0x00	          	// value to check
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
	// check p26 = true 
  	data8	0x1a04			// p[26]
  	data8   0x01	          	// value to check
	// check p27 = false
  	data8	0x1b04			// p[27]
  	data8   0x00	          	// value to check
	// check p28 = false
  	data8	0x1c04			// p[28]
  	data8   0x00	          	// value to check
	// check p29 = false
  	data8	0x1d04			// p[29]
  	data8   0x00	          	// value to check
	// check p30 = true 
  	data8	0x1e04			// p[30]
  	data8   0x01	          	// value to check
	// check p31 = false
  	data8	0x1f04			// p[31]
  	data8   0x00	
 	// check p32 = true 
  	data8	0x2004			// p[32]
  	data8   0x01	          	// value to check
	// check p33 = true 
  	data8	0x2104			// p[33]
  	data8   0x01	          	// value to check
	// check p34 = false
  	data8	0x2204			// p[34]
  	data8   0x00	          	// value to check
	// check p35 = false
  	data8	0x2304			// p[35]
  	data8   0x00	          	// value to check	
	// check p36 = false
  	data8	0x2404			// p[36]
  	data8   0x00	          	// value to check
	// check p37 = true 
  	data8	0x2504			// p[37]
  	data8   0x01	          	// value to check
	// check p38 = false
  	data8	0x2604			// p[38]
  	data8   0x00	          	// value to check
	// check p39 = false
  	data8	0x2704			// p[39]
  	data8   0x00	          	// value to check
	// check p40 = true 
  	data8	0x2804			// p[40]
  	data8   0x01 	          	// value to check
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
	// check p45 = false       
  	data8	0x2d04			// p[45]
  	data8   0x00	          	// value to check
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
	// check p51 = false
  	data8	0x3304			// p[51]
  	data8   0x00	          	// value to check
	// check p52 = true 
  	data8	0x3404			// p[52]
  	data8   0x01	          	// value to check
	// check p53 = true 
  	data8	0x3504			// p[53]
  	data8   0x01	          	// value to check
	// check p54 = false
  	data8	0x3604			// p[49]
  	data8   0x00	          	// value to check
	// check p55 = false
  	data8	0x3704			// p[49]
  	data8   0x00	          	// value to check
	// check p56 = false
  	data8	0x3804			// p[49]
  	data8   0x00	          	// value to check
	// check p57 = true
  	data8	0x3904			// p[49]
  	data8   0x01	          	// value to check
	// check p58 = false
  	data8	0x3a04			// p[49]
  	data8   0x00	          	// value to check
	// check p59 = false
  	data8	0x3b04			// p[49]
  	data8   0x00	          	// value to check
	// check p60 = true
  	data8	0x3c04			// p[49]
  	data8   0x01	          	// value to check	
	// check p61 = false
  	data8	0x3d04			// p[49]
  	data8   0x00	          	// value to check
	// check p62 = false
  	data8	0x3e04			// p[49]
  	data8   0x00	          	// value to check
	// check p63 = false
  	data8	0x3f04			// p[49]
  	data8   0x00	          	// value to check
