// ---------------------------------------------------------------------------
// - F10.s                                                                   -
// - iato asm test suite : floating-point test with umoddi3                  -
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

_umoddi3:	
	setf.sig f14=r32
	setf.sig f9=r33
	nop.f 0x0;;
	nop.m 0x0
	fnorm.s1 f8=f14
	nop.b 0x0
	nop.m 0x0
	fnorm.s1 f9=f9
	nop.b 0x0;;
	nop.m 0x0
	frcpa.s1 f10,p6=f8,f9
	nop.b 0x0;;
	nop.m 0x0
   (p6) fmpy.s1 f12=f8,f10
	nop.b 0x0
	nop.m 0x0
   (p6) fnma.s1 f11=f9,f10,f1
	nop.b 0x0;;
	nop.m 0x0
   (p6) fma.s1 f12=f11,f12,f12
	nop.b 0x0
	nop.m 0x0
   (p6) fmpy.s1 f13=f11,f11
	nop.b 0x0;;
	nop.m 0x0
   (p6) fma.s1 f10=f11,f10,f10
	nop.b 0x0
	nop.m 0x0
   (p6) fma.s1 f11=f13,f12,f12
	nop.i 0x0;;
	sub r33=r0,r33
   (p6) fma.s1 f10=f13,f10,f10
        nop.b 0x0
        nop.m 0x0
   (p6) fnma.s1 f12=f9,f11,f8
        nop.b 0x0;;
	setf.sig f9=r33
   (p6) fma.s1 f10=f12,f10,f11
        nop.b 0x0;;
        nop.m 0x0
        fcvt.fxu.trunc.s1 f10=f10
        nop.b 0x0;;
        nop.m 0x0
        xma.l f10=f10,f9,f14
        nop.b 0x0;;
        getf.sig r8=f10
        nop.f 0x0
        br.ret.sptk.few b0;;

_start:
	.body
	alloc r32 = ar.pfs,0,1,2,0
	mov   r33 = 0x0000000000000000
	mov   r34 = 0x0000000000002000;;
	br.call.sptk.few b0=_umoddi3#
	.endp _start#
