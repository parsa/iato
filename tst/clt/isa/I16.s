// ---------------------------------------------------------------------------
// - I16.s                                                                   -
// - iato isa test suite : I16 group instruction test                        -
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
	movl r6 = 0xFFFFFFFFFFFFFFFF
	movl r5 = 0x9876512340fcdeab
	;; 
	mov pr = r6, 0x1FFFF
	;;
	tbit.z p4, p5 = r5, 6
	tbit.z.unc p6, p7 = r5, 6
	tbit.z.and p8, p9 = r5, 6
	tbit.nz.and p10, p11 = r5, 6
	tbit.z.or p12, p13 = r5, 6  
	tbit.nz.or p14, p15 = r5, 6
	tbit.z.or.andcm p16, p17 = r5, 6
	tbit.nz.or.andcm p18, p19 = r5, 6
	.endp _start#

	.section ".iato.checker", "a", "progbits"
	// check p4 = true 
  	data8	0x0404			// p[4]
  	data8   0x01	          	// value to check
 	// check p5 = false
  	data8	0x0504			// p[5]
  	data8   0x0	          	// value to check
	// check p6 = true 
  	data8	0x0604			// p[6]
  	data8   0x1	          	// value to check
 	// check p7 = false
  	data8	0x0704			// p[7]
  	data8   0x0	          	// value to check	         
 	// check p8 = true 
  	data8	0x0804			// p[8]
  	data8   0x1	          	// value to check
 	// check p9 = true
  	data8	0x0904			// p[9]
  	data8   0x1	          	// value to check
 	// check p10 = false
  	data8	0x0A04			// p[10]
  	data8   0x0	          	// value to check
	// check p11 = false
  	data8	0x0B04			// p[11]
  	data8   0x0	          	// value to check
	// check p12 = true 
  	data8	0x0C04			// p[12]
  	data8   0x1	          	// value to check
	// check p13 = true 
  	data8	0x0D04			// p[13]
  	data8   0x1	          	// value to check
	// check p14 = true 
  	data8	0x0E04			// p[14]
  	data8   0x1	          	// value to check
	// check p15 = true
  	data8	0x0F04			// p[15]
  	data8   0x1	          	// value to check
	// check p16 = true 
  	data8	0x1004			// p[16]
  	data8   0x1	          	// value to check
	// check p17 = false 
  	data8	0x1104			// p[17]
  	data8   0x0	          	// value to check
	// check p18 = true 
  	data8	0x1204			// p[18]
  	data8   0x1	          	// value to check
	// check p19 = true 
  	data8	0x1304			// p[19]
  	data8   0x1	          	// value to check
