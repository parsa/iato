// ---------------------------------------------------------------------------
// - M18.s                                                                   -
// - iato isa test suite : M18 group instruction test                        -
// ---------------------------------------------------------------------------
// - (c) inria 2002-2004       
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
	// register initialization
	movl r1 = 0x000000000123456789
	;; 
	setf.sig f2 = r1
	setf.exp f3 = r1
	setf.s   f4 = r1
	setf.d   f5 = r1
	.endp _start#

	.section ".iato.checker", "a", "progbits"
  	// check r1 = 0x123456789
  	data8	0x0101				// gr[1]
  	data8	0x123456789			// value to check
	// check f2 = 0x1003E 0000000123456789
	data8	0x0203				// fr[2]
	data8   0x000000000001003E		// value to check
	data8	0x0000000123456789		// value to check
	// check f3 = 0x16789 8000000000000000
	data8	0x0303				// fr[3]
	data8   0x16789				// value to check
	data8	0x8000000000000000		// value to check
	// check f4 = 0xffc6  c567890000000000
	data8	0x0403				// fr[3]
	data8   0x000000000000ffc6		// value to check
	data8	0xc567890000000000		// value to check
	// check f5 = 0x00fc01 0000091a2b3c4800
	data8	0x0503				// fr[3]
	data8   0x000000000000fc01		// value to check
	data8	0x0000091a2b3c4800		// value to check
