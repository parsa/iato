// ---------------------------------------------------------------------------
// - M30.s                                                                   -
// - iato isa test suite : M30 group instruction test                        -
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
	.mmi {
	     mov.m ar.k0 = 0
	     mov.m ar.k1 = 1
	     nop.i 0;;
	}
	.mmi {
	     mov.m ar.k2  = 2
	     mov.m ar.rsc = 3
	     nop.i 0;;
	}
	.mmi {
	     mov.m ar.bsp      = 4;;
	     mov.m ar.bspstore = 5
	     nop.i 0;;
	}
	.mmi {
	     mov.m ar.rnat = 6
	     mov.m ar.fcr  = 7
	     nop.i 0;;
	}
	.mmi {
	     mov.m ar.ccv  = 8
	     mov.m ar.unat = 9
	     nop.i 0;;
	}
	.mmi {
	     mov.m ar.fpsr = 10
	     mov.m ar.itc  = 11
	     nop.i 0;;
	}
	.endp _start#

		.section ".iato.checker", "a", "progbits"
	// check ar.k0 = 0
  	data8	0x0006			// ar[0]
  	data8	0x00			// value to check
	// check ar.k1 = 1
  	data8	0x0106			// ar[1]
  	data8	0x01			// value to check
	// check ar.k2 = 2
  	data8	0x0206			// ar[2]
  	data8	0x02			// value to check
	// check ar.rsc = 3
  	data8	0x1006			// ar[16]
  	data8	0x03			// value to check
	// check ar.bsp = 4
  	data8	0x1106			// ar[17]
  	data8	0x04			// value to check
	// check ar.bspstore = 5
  	data8	0x1206			// ar[18]
	data8	0x05			// value to check
  	// check ar.rnat = 6
  	data8	0x1306			// ar[19]
  	data8	0x06			// value to check
	// check ar.fcr  = 7
  	data8	0x1506			// ar[21]
  	data8	0x07			// value to check
	// check ar.ccv  = 8
  	data8	0x2006			// ar[32]
  	data8	0x08			// value to check
	// check ar.unat = 9
  	data8	0x2406			// ar[36]
  	data8	0x09			// value to check
	// check ar.fpsr = 10
  	data8	0x2806			// ar[40]
  	data8	0x0a			// value to check
	// check ar.itc  = 11
  	data8	0x2c06			// ar[44]
  	data8	0x0b			// value to check
