// ---------------------------------------------------------------------------
// - A10.s                                                                   -
// - iato isa test suite : A10 group instruction test                        -
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
	mov    r16 = 1
	movl   r4  = 0x8000405060708087
	movl   r2  = 0xFEDC232123212321
	movl   r9  = 0x8000405060708087
	movl   r17 = 0x6fff5e5fff456789
	;; 
	cmp.eq p6, p7 = r16, r0
	;; 
	pshladd2 r14 = r17, 2, r9 
	pshladd2 r18 = r9,  2, r0
	pshradd2 r19 = r9,  2, r17
	pshradd2 r25 = r2,  3, r4
   (p6)	pshradd2 r16 = r2,  3, r4
	.endp _start# 
	
	.section ".iato.checker", "a", "progbits"
	// check r14 = 0x7fff7fff5d847fff
  	data8	0x0E01			// gr[14]
  	data8	0x7fff7fff5d847fff	// value to check
	// check r16 = 0x1
  	data8	0x1001			// gr[16]
  	data8	0x1			// value to check
	// check r18 = 0x80007fff7fff8000
  	data8	0x1201			// gr[18]
  	data8	0x80007fff7fff8000	// value to check
	// check r19 = 0x4fff6e73176147aa
  	data8	0x1301			// gr[19]
  	data8	0x4fff6e73176147aa	// value to check
	// check r25 = 0x800044b464d484eb
  	data8	0x1901			// gr[25]
  	data8	0x800044b464d484eb	// value to check
	// check p6 = false
  	data8	0x0604			// p[6]
  	data8   0x00	          	// value to check
	// check p7 = true
  	//data8	0x0704			// p[7]
  	//data8   0x01	          	// value to check
