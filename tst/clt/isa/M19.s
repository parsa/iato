// ---------------------------------------------------------------------------
// - M19.s                                                                   -
// - iato isa test suite : M19 group instruction test                        -
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
	movl r2 = _data
	;;
	ldf.fill f2 = [r2]
	;; 
	getf.sig r1 = f2
	getf.exp r2 = f2
	getf.s   r3 = f2
	getf.d   r4 = f2
	.endp _start#

	.section ".iato.data", "wa", "progbits"
	_data:  data8 0x123456789abcdef0
	_data1:	data8 0x000000000003def0

	.section ".iato.checker", "a", "progbits"	
	// check f2 = 0x3def0 123456789abcdef0
	data8	0x0203				// fr[2]
	data8   0x000000000003def0		// value to check
	data8	0x123456789abcdef0		// value to check
 	// check r1 = 0x123456789abcdef0
  	data8	0x0101				// gr[1]
  	data8	0x123456789abcdef0		// value to check
	// check r2 = 0x000000000003def0
  	data8	0x0201				// gr[2]
  	data8	0x000000000003def0		// value to check
	// check r3 = 0x0000000080123456
  	data8	0x0301				// gr[3]
  	data8	0x0000000080123456		// value to check
	// check r4 = 0x8002468acf13579b
  	data8	0x0401				// gr[4]
  	data8	0x8002468acf13579b		// value to check
