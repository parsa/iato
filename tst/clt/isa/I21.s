// ---------------------------------------------------------------------------
// - I21.s                                                                   -
// - iato isa test suite : I21 group instruction test                        -
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
	movl r1 = 0x123456789abcdef0
	movl r2 = 0x1234567898765432
	;; 
	mov b1 = r1
	mov b2 = r2, 16;;
	mov.sptk.imp b3 = r1, -16
	mov.ret.dptk b4 = r2, 80;;
	.endp _start#

	.section ".iato.checker", "a", "progbits"
	// check r1 = 0x123456789abcdef0
  	data8	0x0101			// gr[1]
  	data8	0x123456789abcdef0      // value to check
	// check b1 = 0x123456789abcdef0
  	data8	0x0105			// b[1]
  	data8	0x123456789abcdef0      // value to check
	// check b2 = 0x1234567898765432
  	data8	0x0205			// b[2]
  	data8	0x1234567898765432      // value to check
	// check b3 = 0x123456789abcdef0
  	data8	0x0305			// b[3]
  	data8	0x123456789abcdef0      // value to check
	// check b4 = 0x1234567898765432
  	data8	0x0405			// b[4]
  	data8	0x1234567898765432      // value to check
