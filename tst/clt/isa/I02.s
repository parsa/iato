// ---------------------------------------------------------------------------
// - I02.s                                                                   -
// - iato isa test suite : I02 group instruction test                        -
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
	movl r14 = 0x8521609636048701
	movl r15 = 0x123456987fdaecb0
	;;
	pmpy2.r r16 = r14,r15  
	pmpy2.l r17 = r14,r15  
	mix1.r  r18 = r14,r15  
	mix2.r  r19 = r14,r15  
	mix4.r  r20 = r14,r15  
	mix1.l  r21 = r14,r15  
	mix2.l  r22 = r14,r15  
	mix4.l  r23 = r14,r15  
	pack2.uss r24 = r14,r15
	pack2.sss r25 = r14,r15
	pack4.sss r26 = r14,r15
	unpack1.h r27 = r14,r15
	unpack2.h r28 = r14,r15
	unpack4.h r29 = r14,r15
	unpack1.l r30 = r14,r15
	unpack2.l r31 = r14,r15
	unpack4.l r1  = r14,r15
	pmin1.u   r2  = r14,r15
	pmax1.u   r3  = r14,r15
	pmin2     r4  = r14,r15
	pmax2     r5  = r14,r15
	psad1     r6  = r14,r15
	.endp _start#

	.section ".iato.checker", "a", "progbits"
	// check r1 = 0x360487017fdaecb0
  	data8	0x0101			// gr[1]
  	data8	0x360487017fdaecb0	// value to check
	// check r2 = 0x1221569636048701
  	data8	0x0201			// gr[2]
  	data8	0x1221569636048701	// value to check
	// check r3 = 0x853460987fdaecb0
  	data8	0x0301			// gr[3]
  	data8	0x853460987fdaecb0	// value to check
	// check r4 = 0x8521569836048701
  	data8	0x0401			// gr[4]
  	data8	0x8521569836048701	// value to check
	// check r5 = 0x123460967fdaecb0
  	data8	0x0501			// gr[5]
  	data8   0x123460967fdaecb0	// value to check
	// check r6 = 0x2c5
  	data8	0x0601			// gr[6]
  	data8	0x2c5	// value to check
	// check r16 = 0x20abbd100920bcb0
  	data8	0x1001			// gr[16]
  	data8	0x20abbd100920bcb0	// value to check
	// check r17 = 0xf7435cb41af9fb68
  	data8	0x1101			// gr[17]
  	data8	0xf7435cb41af9fb68	// value to check
	// check r18 = 0x2134969804da01b0
  	data8	0x1201			// gr[18]
  	data8	0x2134969804da01b0	// value to check
	// check r19 = 0x609656988701ecb0
  	data8	0x1301			// gr[19]
  	data8	0x609656988701ecb0	// value to check
	// check r20 = 0x360487017fdaecb0
  	data8	0x1401			// gr[20]
  	data8	0x360487017fdaecb0	// value to check
	// check r21 = 0x85126056367f87ec
  	data8	0x1501			// gr[21]
  	data8	0x85126056367f87ec 	// value to check
	// check r22 = 0x8521123436047fda
  	data8	0x1601			// gr[22]
  	data8	0x8521123436047fda	// value to check
	// check r23 = 0x8521609612345698
  	data8	0x1701			// gr[23]
  	data8	0x8521609612345698	// value to check
	// check r24 = 0xffffff0000ffff00
  	data8	0x1801			// gr[24]
  	data8	0xffffff0000ffff00	// value to check
	// check r25 = 0x7f7f7f80807f7f80
  	data8	0x1901			// gr[25]
  	data8	0x7f7f7f80807f7f80	// value to check
	// check r26 = 0x7fff7fff80007fff
  	data8	0x1a01			// gr[26]
  	data8	0x7fff7fff80007fff	// value to check
	// check r27 = 0x8512213460569698
  	data8	0x1b01			// gr[27]
  	data8   0x8512213460569698	// value to check
	// check r28 = 0x8521123460965698
  	data8	0x1c01			// gr[28]
  	data8	0x8521123460965698	// value to check
	// check r29 = 0x8521609612345698
  	data8	0x1d01			// gr[29]
  	data8	0x8521609612345698	// value to check
	// check r30 = 0x367f04da87ec01b0
  	data8	0x1e01			// gr[30]
  	data8	0x367f04da87ec01b0	// value to check
	// check r31 = 0x36047fda8701ecb0
  	data8	0x1f01			// gr[31]
  	data8	0x36047fda8701ecb0	// value to check

