// ---------------------------------------------------------------------------
// - I24.s                                                                   -
// - iato isa test suite : I24 group instruction test                        -
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
	mov pr.rot = 0xFCFCF4F0000
	.endp _start#

	.section ".iato.checker", "a", "progbits"
	// check p16 = true 
  	data8	0x1004			// p[16]
  	data8   0x01	          	// value to check
	// check p17 = true 
  	data8	0x1104			// p[17]
  	data8   0x01	          	// value to check
	// check p18 = true 
  	data8	0x1204			// p[18]
  	data8   0x01	          	// value to check
	// check p19 = true 
  	data8	0x1304			// p[19]
  	data8   0x01	          	// value to check
	// check p20 = false
  	data8	0x1404			// p[20]
  	data8   0x00	          	// value to check
	// check p21 = false
  	data8	0x1504			// p[21]
  	data8   0x00	          	// value to check
	// check p22 = true 
  	data8	0x1604			// p[22]
  	data8   0x01	          	// value to check
	// check p23 = false
  	data8	0x1704			// p[23]
  	data8   0x00	          	// value to check
	// check p24 = true 
  	data8	0x1804			// p[24]
  	data8   0x01	          	// value to check
	// check p25 = true 
  	data8	0x1904			// p[25]
  	data8   0x01	          	// value to check
	// check p26 = true 
  	data8	0x1a04			// p[26]
  	data8   0x01	          	// value to check
	// check p27 = true 
  	data8	0x1b04			// p[27]
  	data8   0x01	          	// value to check
	// check p28 = false
  	data8	0x1c04			// p[28]
  	data8   0x00	          	// value to check
	// check p29 = false
  	data8	0x1d04			// p[29]
  	data8   0x00	          	// value to check
	// check p30 = true 
  	data8	0x1e04			// p[30]
  	data8   0x01	          	// value to check
	// check p31 = true 
  	data8	0x1f04			// p[31]
  	data8   0x01	
 	// check p32 = true 
  	data8	0x2004			// p[32]
  	data8   0x01	          	// value to check
	// check p33 = true 
  	data8	0x2104			// p[33]
  	data8   0x01	          	// value to check
	// check p34 = true 
  	data8	0x2204			// p[34]
  	data8   0x01	          	// value to check
	// check p35 = true 
  	data8	0x2304			// p[35]
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
	// check p39 = true 
  	data8	0x2704			// p[39]
  	data8   0x01	          	// value to check
	// check p40 = true 
  	data8	0x2804			// p[40]
  	data8   0x01 	          	// value to check
	// check p41 = true 
  	data8	0x2904			// p[41]
  	data8   0x01	          	// value to check
	// check p42 = true 
  	data8	0x2a04			// p[42]
  	data8   0x01	          	// value to check
	// check p43 = true 
  	data8	0x2b04			// p[43]
  	data8   0x01	          	// value to check
	// check p44 = true
  	data8	0x2c04			// p[44]
  	data8   0x01 	          	// value to check
	// check p45 = true        
  	data8	0x2d04			// p[45]
  	data8   0x01	          	// value to check
	// check p46 = true 
  	data8	0x2e04			// p[46]
  	data8   0x01	          	// value to check
	// check p47 = true 
  	data8	0x2f04			// p[47]
  	data8   0x01	          	// value to check
	// check p48 = true 
  	data8	0x3004			// p[48]
  	data8   0x01	          	// value to check
	// check p49 = true 
  	data8	0x3104			// p[49]
  	data8   0x01	          	// value to check
	// check p50 = true 
  	data8	0x3204			// p[50]
  	data8   0x01	          	// value to check
	// check p51 = true 
  	data8	0x3304			// p[51]
  	data8   0x01	          	// value to check
	// check p52 = true 
  	data8	0x3404			// p[52]
  	data8   0x01	          	// value to check
	// check p53 = true 
  	data8	0x3504			// p[53]
  	data8   0x01	          	// value to check
	// check p54 = true 
  	data8	0x3604			// p[54]
  	data8   0x01	          	// value to check
	// check p55 = true 
  	data8	0x3704			// p[55]
  	data8   0x01	          	// value to check
	// check p56 = true 
  	data8	0x3804			// p[56]
  	data8   0x01	          	// value to check
	// check p57 = true 
  	data8	0x3904			// p[57]
  	data8   0x01	          	// value to check
	// check p58 = true 
  	data8	0x3a04			// p[58]
  	data8   0x01	          	// value to check
	// check p59 = true 
  	data8	0x3b04			// p[59]
  	data8   0x01	          	// value to check
	// check p60 = true 
  	data8	0x3c04			// p[60]
  	data8   0x01	          	// value to check
	// check p61 = true 
  	data8	0x3d04			// p[61]
  	data8   0x01	          	// value to check
	// check p62 = true 
  	data8	0x3e04			// p[62]
  	data8   0x01	          	// value to check
	// check p63 = true 
  	data8	0x3f04			// p[63]
  	data8   0x01	          	// value to check
