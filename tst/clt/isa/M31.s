// ---------------------------------------------------------------------------
// - M31.s                                                                   -
// - iato isa test suite : M31 group instruction test                        -
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
	// first init ar registers with M30 instruction
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
	// Now mov ar to gr registers
	.mmi {
	     mov.m r12 = ar.k0
	     mov.m r1 = ar.k1
	     nop.i 0;;
	}
	.mmi {
	     mov.m r2 = ar.k2
	     mov.m r3 = ar.rsc
	     nop.i 0;;
	}
	.mmi {
	     mov.m r4 = ar.bsp
	     mov.m r5 = ar.bspstore
	     nop.i 0;;
	}
	.mmi {
	     mov.m r6 = ar.rnat
	     mov.m r7 = ar.fcr
	     nop.i 0;;
	}
	.mmi {
	     mov.m r8 = ar.ccv
	     mov.m r9 = ar.unat
	     nop.i 0;;
	}
	.mmi {
	     mov.m r10 = ar.fpsr
	     mov.m r11 = ar.itc
	     nop.i 0;;
	}
	.endp _start#

	.section ".iato.checker", "a", "progbits"
	// check r1 = 1
  	data8	0x0101			// gr[1]
  	data8	0x01   			// value to check
  	// check r2 = 2
  	data8	0x0201			// gr[2]
  	data8	0x02              	// value to check
	// check r3 = 3
  	data8	0x0301			// gr[3]
  	data8	0x03              	// value to check
	// check r4 = 4
  	data8	0x0401			// gr[4]
  	data8	0x04              	// value to check
	// check r5 = 5
  	data8	0x0501			// gr[5]
  	data8	0x05               	// value to check
	// check r6 = 6
  	data8	0x0601			// gr[6]
  	data8	0x06   			// value to check
  	// check r7 = 7
  	data8	0x0701			// gr[7]
  	data8	0x07              	// value to check
	// check r8 = 8
  	data8	0x0801			// gr[8]
  	data8	0x08              	// value to check
	// check r9 = 9
  	data8	0x0901			// gr[9]
  	data8	0x09              	// value to check
	// check r10 = 10
  	data8	0x0a01			// gr[10]
  	data8	0x0a               	// value to check
	// check r11 = 11
  	data8	0x0b01			// gr[11]
  	data8	0x0b              	// value to check
	// check r12 = 0 
  	data8	0x0c01			// gr[12]
  	data8	0x00               	// value to check
