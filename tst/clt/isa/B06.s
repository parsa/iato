// ---------------------------------------------------------------------------
// - B06.s                                                                   -
// - iato isa test suite : B06 group instruction test                        -
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
	brp.sptk      0x00000000000000FF , 0x0000000000000F80
	brp.loop.imp  0x0000000000000000 , 0x0000000000000F80
	brp.dptk      0x000000000000008F , 0x0000000000000F80
	brp.exit.imp  0x0000000000000081 , 0x0000000000000F80

 	.endp
