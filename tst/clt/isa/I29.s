// ---------------------------------------------------------------------------
// - I29.s                                                                   -
// - iato isa test suite : I29 group instruction test                        -
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
	movl r2 = 0x765432198bb
	;; 
	zxt1     r3  = r2
        zxt2     r4  = r2
	zxt4     r5  = r2
        sxt1     r6  = r2
	sxt2     r7  = r2
        sxt4     r8  = r2  
	czx1.l   r9  = r2
        czx2.l   r10 = r2  
	czx1.r   r11 = r2
        czx2.r   r12 = r2  
	.endp _start#
	
	.section ".iato.checker", "a", "progbits"
	// check r2 = 0x765432198bb
  	data8	0x0201			// gr[2]
  	data8	0x765432198bb		// value to check
	// check r3 = 0xbb
  	data8	0x0301			// gr[3]
  	data8	0xbb			// value to check
	// check r4 = 0x98bb 
  	data8	0x0401			// gr[4]
  	data8	0x98bb			// value to check
	// check r5 = 0x432198bb
  	data8	0x0501			// gr[5]
  	data8   0x432198bb		// value to check
	// check r6 = 0xffffffffffffffbb
  	data8	0x0601			// gr[6]
  	data8	0xffffffffffffffbb	// value to check
	// check r7 = 0xffffffffffff98bb
  	data8	0x0701			// gr[7]
  	data8	0xffffffffffff98bb	// value to check
	// check r8 = 0x432198bb 
  	data8	0x0801			// gr[8]
  	data8	0x432198bb		// value to check
	// check r9 = 0x0 
  	data8	0x0901			// gr[9]
  	data8	0x0			// value to chec
	// check r10 = 0x0 
  	data8	0x0a01			// gr[10]
  	data8	0x0			// value to check
	// check r11 = 0x6 
  	data8	0x0b01			// gr[11]
  	data8	0x6			// value to check
	// check r12 = 0x3
  	data8	0x0c01			// gr[12]
  	data8	0x3			// value to check
