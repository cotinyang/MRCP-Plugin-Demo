//
// Copyright (C) 2002 Akira Nukada. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the project nor the names of its contributors 
//       may be used to endorse or promote products derived from this software 
//       without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE SSI PROJECT ``AS IS'' AND ANY EXPRESS 
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE SSI PROJECT BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
//
.text
.align	128
.global	fftss_kern_r4_ia64_f
.type	fftss_kern_r4_ia64_f,@function
.proc	fftss_kern_r4_ia64_f
fftss_kern_r4_ia64_f:
{ .mmi
	alloc	r9=ar.pfs,8,16,0,0
	mov	r8=ar.itc
	mov	r10=ar.lc
}
{ .mmi
	add	r15=1,r0
	add	r14=-1,r36
	mov	r11=pr;;
}
{ .mmi
	add	r44=r0,r34
	add	r45=r0,r34
	cmp.ge	p7,p8=r37,r38
}
{ .mmi
//	add	r46=r0,r34
	add	r46=r0,r35
	shladd	r41=r37,4,r32
	shladd	r49=r36,4,r33;;
}
{ .mmi
	shladd	r52=r37,1,r37
	shladd	r54=r37,1,r0
//	add	r47=r0,r35
	nop.i	0x0
}
{ .mmi
	add	r40=r0,r32
	shladd	r42=r37,4,r41
	shladd	r50=r36,4,r49;;
}
{ .mmi
	add	r48=r0,r33
	shladd	r43=r37,4,r42
  	shladd	r51=r36,4,r50
}
{ .mmb
	add	r16=-1,r37
(p7)	sub	r14=r14,r37
(p8)	br.cond.dpnt.many	.L3;;
}
{ .mii
	mov	ar.lc=r16
	mov	ar.ec=6
	
}
{ .mmi
	cmp.ge	p6,p0=r0,r14

	mov	pr.rot=1<<16;;

}
.L0:
{ .mfi
(p16)	ldfpd	f32,f41=[r40],16
(p19)	fma	f36=f35,f1,f71
	nop.i	0x0
}
{ .mfi
	nop.m	0x0
(p19)	fms	f72=f35,f1,f71
	nop.i	0x0;;
}
{ .mfi
(p16)	ldfpd	f50,f59=[r41],16
(p19)	fma	f45=f44,f1,f80
	nop.i	0x0
}
{ .mfi
	nop.m	0x0
(p19)	fms	f81=f44,f1,f80
	nop.i	0x0;;
}
{ .mfi
(p16)	ldfpd	f68,f77=[r42],16
(p19)	fma	f54=f53,f1,f89
	nop.i	0x0
}
{ .mfi
	nop.m	0x0
(p19)	fms	f90=f53,f1,f89
	nop.i	0x0;;
}
{ .mfi
(p16)	ldfpd	f86,f95=[r43],16
(p19)	fma	f63=f62,f1,f98
	nop.i	0x0
}
{ .mfi
	nop.m	0x0
(p19)	fms	f99=f62,f1,f98
	nop.i	0x0;;
}
{ .mfi
(p21)	stfd	[r48]=f39,8
(p20)	fma	f38=f37,f1,f55
	nop.i	0x0
}
{ .mfi
(p21)	stfd	[r50]=f57,8
(p20)	fms	f56=f37,f1,f55
	nop.i	0x0;;
}
{ .mfi
(p21)	stfd	[r48]=f48,8
(p20)	fma	f47=f46,f1,f64
	nop.i	0x0
}
{ .mfi
(p21)	stfd	[r50]=f66,8
(p20)	fms	f65=f46,f1,f64
	nop.i	0x0;;
}
{ .mfi
(p21)	stfd	[r49]=f75,8
(p20)	fma	f74=f73,f1,f100
	nop.i	0x0
}
{ .mfi
(p21)	stfd	[r51]=f93,8
(p20)	fms	f92=f73,f1,f100
	nop.i	0x0;;
}
{ .mfi
(p21)	stfd	[r49]=f84,8
(p20)	fms	f83=f82,f1,f91
	nop.i	0x0
}
{ .mfb
(p21)	stfd	[r51]=f102,8
(p20)	fma	f101=f82,f1,f91
	br.ctop.sptk.many	.L0;;
}
{ .mmi
	shladd	r44=r37,4,r44
	shladd	r45=r54,4,r45
//	shladd	r46=r52,4,r46
	shladd	r46=r37,4,r46
}
{ .mbb
//	shladd	r47=r37,3,r47
	nop.m	0x0
(p6)	br.cond.spnt.many	.L2
	clrrrb.pr;;
}
{ .mmi
	shladd	r40=r52,4,r40
	shladd	r41=r52,4,r41
	shladd	r42=r52,4,r42
}
{ .mmi
	shladd	r43=r52,4,r43
}
.L3:
{ .mii
	nop.m	0x0
	mov	pr.rot=1<<16
	mov	ar.lc=r14
}
{ .mmi
	nop.m	0x0
	nop.m	0x0
	mov	ar.ec=5;;
}
.L1:
{ .mfi
(p16)	ldfpd	f32,f45=[r40],16
(p19)	fma	f91=f71,f84,f35
(p16)	cmp.eq.unc	p6,p0=r15,r37
}
{ .mfi
	nop.m	0x0
(p19)	fnma	f95=f71,f84,f35
(p16)	add	r15=1,r15;;
}
{ .mfi
(p16)	ldfpd	f36,f49=[r41],16
(p19)	fma	f93=f78,f88,f80
(p6)	add	r15=1,r0
}
{ .mfi
(p6)	shladd	r40=r52,4,r40
(p19)	fnma	f97=f78,f88,f80
	nop.i	0x0;;
}
{ .mfi
(p16)	ldfpd	f39,f52=[r42],16
(p19)	fma	f92=f71,f86,f48
	nop.i	0x0
}
{ .mfi
(p6)	shladd	r41=r52,4,r41
(p19)	fnma	f96=f71,f86,f48
	nop.i	0x0;;
}
{ .mfi
(p16)	ldfpd	f42,f55=[r43],16
(p19)	fma	f94=f78,f90,f82
	nop.i	0x0
}
{ .mfi
(p6)	shladd	r42=r52,4,r42
(p19)	fnma	f98=f78,f90,f82
	nop.i	0x0;;
}
{ .mfi
(p16)	ldfpd	f58,f61=[r44]
(p18)	fma	f79=f60,f38,f51
	nop.i	0x0
}
{ .mfi
(p6)	shladd	r43=r52,4,r43
(p18)	fms	f81=f60,f51,f38
	nop.i	0x0;;
}
{ .mfi
(p16)	ldfpd	f65,f68=[r45]
(p18)	fma	f83=f67,f41,f54
	nop.i	0x0
}
{ .mfi
(p6)	shladd	r44=r37,4,r44
(p18)	fms	f85=f67,f54,f41
	nop.i	0x0;;
}
{ .mfi
(p16)	ldfpd	f72,f75=[r46]
(p18)	fma	f87=f74,f44,f57
(p6)	shladd	r45=r54,4,r45
}
{ .mfi
//(p16)	ldfd	f75=[r47]
	nop.m	0x0
(p18)	fms	f89=f74,f57,f44
	nop.i	0x0;;
}
{ .mfi
(p20)	stfd	[r48]=f100,8
(p19)	fma	f99=f64,f93,f91
//(p6)	shladd	r46=r52,4,r46
(p6)	shladd	r46=r37,4,r46
}
{ .mfi
(p20)	stfd	[r50]=f104,8
(p19)	fnma	f103=f64,f93,f91
(p6)	shladd	r47=r37,3,r47;;
}
{ .mfi
(p20)	stfd	[r48]=f102,8
(p19)	fma	f101=f64,f94,f92
	nop.i	0x0
}
{ .mfi
(p20)	stfd	[r50]=f106,8
(p19)	fnma	f105=f64,f94,f92
	nop.i	0x0;;
}
{ .mfi
(p20)	stfd	[r49]=f108,8
(p19)	fma	f107=f64,f98,f95
	nop.i	0x0
}
{ .mfi
(p20)	stfd	[r51]=f112,8
(p19)	fnma	f111=f64,f98,f95
	nop.i	0x0;;
}
{ .mfi
(p20)	stfd	[r49]=f110,8
(p19)	fnma	f109=f64,f97,f96
	nop.i	0x0
}
{ .mfb
(p20)	stfd	[r51]=f114,8
(p19)	fma	f113=f64,f97,f96
	br.ctop.sptk.many	.L1;;
}
.L2:

	mov	ar.pfs=r9
	mov	ar.lc=r10
	mov	pr=r11,-1
	mov	r9=ar.itc;;
	sub	r8=r9,r8
	br.ret.sptk.many	rp;;
.endp
	
.align	128
.global	fftss_kern_r4_ia64_b
.type	fftss_kern_r4_ia64_b,@function
.proc	fftss_kern_r4_ia64_b
fftss_kern_r4_ia64_b:
{ .mmi
	alloc	r9=ar.pfs,8,16,0,0
	mov	r8=ar.itc
	mov	r10=ar.lc
}
{ .mmi
	add	r15=1,r0
	add	r14=-1,r36
	mov	r11=pr;;
}
{ .mmi
	add	r44=r0,r34
	add	r45=r0,r34
	cmp.ge	p7,p8=r37,r38
}
{ .mmi
//	add	r46=r0,r34
	add	r46=r0,r35
	shladd	r41=r37,4,r32
	shladd	r49=r36,4,r33;;
}
{ .mmi
	shladd	r52=r37,1,r37
	shladd	r54=r37,1,r0
//	add	r47=r0,r35
	nop.i	0x0
}
{ .mmi
	add	r40=r0,r32
	shladd	r42=r37,4,r41
	shladd	r50=r36,4,r49;;
}
{ .mmi
	add	r48=r0,r33
	shladd	r43=r37,4,r42
  	shladd	r51=r36,4,r50
}
{ .mmb
	add	r16=-1,r37
(p7)	sub	r14=r14,r37
(p8)	br.cond.dpnt.many	.LL3;;
}
{ .mii
	mov	ar.lc=r16
	mov	ar.ec=6
	
}
{ .mmi
	cmp.ge	p6,p0=r0,r14

	mov	pr.rot=1<<16;;

}
.LL0:
{ .mfi
(p16)	ldfpd	f32,f41=[r40],16
(p19)	fma	f36=f35,f1,f71
	nop.i	0x0
}
{ .mfi
	nop.m	0x0
(p19)	fms	f72=f35,f1,f71
	nop.i	0x0;;
}
{ .mfi
(p16)	ldfpd	f50,f59=[r41],16
(p19)	fma	f45=f44,f1,f80
	nop.i	0x0
}
{ .mfi
	nop.m	0x0
(p19)	fms	f81=f44,f1,f80
	nop.i	0x0;;
}
{ .mfi
(p16)	ldfpd	f68,f77=[r42],16
(p19)	fma	f54=f53,f1,f89
	nop.i	0x0
}
{ .mfi
	nop.m	0x0
(p19)	fms	f90=f53,f1,f89
	nop.i	0x0;;
}
{ .mfi
(p16)	ldfpd	f86,f95=[r43],16
(p19)	fma	f63=f62,f1,f98
	nop.i	0x0
}
{ .mfi
	nop.m	0x0
(p19)	fms	f99=f62,f1,f98
	nop.i	0x0;;
}
{ .mfi
(p21)	stfd	[r48]=f39,8
(p20)	fma	f38=f37,f1,f55
	nop.i	0x0
}
{ .mfi
(p21)	stfd	[r50]=f57,8
(p20)	fms	f56=f37,f1,f55
	nop.i	0x0;;
}
{ .mfi
(p21)	stfd	[r48]=f48,8
(p20)	fma	f47=f46,f1,f64
	nop.i	0x0
}
{ .mfi
(p21)	stfd	[r50]=f66,8
(p20)	fms	f65=f46,f1,f64
	nop.i	0x0;;
}
{ .mfi
(p21)	stfd	[r49]=f75,8
(p20)	fms	f74=f73,f1,f100
	nop.i	0x0
}
{ .mfi
(p21)	stfd	[r51]=f93,8
(p20)	fma	f92=f73,f1,f100
	nop.i	0x0;;
}
{ .mfi
(p21)	stfd	[r49]=f84,8
(p20)	fma	f83=f82,f1,f91
	nop.i	0x0
}
{ .mfb
(p21)	stfd	[r51]=f102,8
(p20)	fms	f101=f82,f1,f91
	br.ctop.sptk.many	.LL0;;
}
{ .mmi
	shladd	r44=r37,4,r44
	shladd	r45=r54,4,r45
//	shladd	r46=r52,4,r46
	shladd	r46=r37,4,r46
}
{ .mbb
//	shladd	r47=r37,3,r47
	nop.m	0x0
(p6)	br.cond.spnt.many	.LL2
	clrrrb.pr;;
}
{ .mmi
	shladd	r40=r52,4,r40
	shladd	r41=r52,4,r41
	shladd	r42=r52,4,r42
}
{ .mmi
	shladd	r43=r52,4,r43
}
.LL3:
{ .mii
	nop.m	0x0
	mov	pr.rot=1<<16
	mov	ar.lc=r14
}
{ .mmi
	nop.m	0x0
	nop.m	0x0
	mov	ar.ec=5;;
}
.LL1:
{ .mfi
(p16)	ldfpd	f32,f45=[r40],16
(p19)	fma	f91=f71,f84,f35
(p16)	cmp.eq.unc	p6,p0=r15,r37
}
{ .mfi
	nop.m	0x0
(p19)	fnma	f95=f71,f84,f35
(p16)	add	r15=1,r15;;
}
{ .mfi
(p16)	ldfpd	f36,f49=[r41],16
(p19)	fma	f93=f78,f88,f80
(p6)	add	r15=1,r0
}
{ .mfi
(p6)	shladd	r40=r52,4,r40
(p19)	fnma	f97=f78,f88,f80
	nop.i	0x0;;
}
{ .mfi
(p16)	ldfpd	f39,f52=[r42],16
(p19)	fma	f92=f71,f86,f48
	nop.i	0x0
}
{ .mfi
(p6)	shladd	r41=r52,4,r41
(p19)	fnma	f96=f71,f86,f48
	nop.i	0x0;;
}
{ .mfi
(p16)	ldfpd	f42,f55=[r43],16
(p19)	fma	f94=f78,f90,f82
	nop.i	0x0
}
{ .mfi
(p6)	shladd	r42=r52,4,r42
(p19)	fnma	f98=f78,f90,f82
	nop.i	0x0;;
}
{ .mfi
(p16)	ldfpd	f58,f61=[r44]
(p18)	fms	f79=f60,f38,f51
	nop.i	0x0
}
{ .mfi
(p6)	shladd	r43=r52,4,r43
(p18)	fma	f81=f60,f51,f38
	nop.i	0x0;;
}
{ .mfi
(p16)	ldfpd	f65,f68=[r45]
(p18)	fms	f83=f67,f41,f54
	nop.i	0x0
}
{ .mfi
(p6)	shladd	r44=r37,4,r44
(p18)	fma	f85=f67,f54,f41
	nop.i	0x0;;
}
{ .mfi
(p16)	ldfpd	f72,f75=[r46]
//(p16)	ldfd	f72=[r46]
(p18)	fms	f87=f74,f44,f57
(p6)	shladd	r45=r54,4,r45
}
{ .mfi
//(p16)	ldfd	f75=[r47]
	nop.m	0x0
(p18)	fma	f89=f74,f57,f44
	nop.i	0x0;;
}
{ .mfi
(p20)	stfd	[r48]=f100,8
(p19)	fma	f99=f64,f93,f91
//(p6)	shladd	r46=r52,4,r46
(p6)	shladd	r46=r37,4,r46
}
{ .mfi
(p20)	stfd	[r50]=f104,8
(p19)	fnma	f103=f64,f93,f91
//(p6)	shladd	r47=r37,3,r47;;
	nop.i	0x0;; 
}
{ .mfi
(p20)	stfd	[r48]=f102,8
(p19)	fma	f101=f64,f94,f92
	nop.i	0x0
}
{ .mfi
(p20)	stfd	[r50]=f106,8
(p19)	fnma	f105=f64,f94,f92
	nop.i	0x0;;
}
{ .mfi
(p20)	stfd	[r49]=f108,8
(p19)	fnma	f107=f64,f98,f95
	nop.i	0x0
}
{ .mfi
(p20)	stfd	[r51]=f112,8
(p19)	fma	f111=f64,f98,f95
	nop.i	0x0;;
}
{ .mfi
(p20)	stfd	[r49]=f110,8
(p19)	fma	f109=f64,f97,f96
	nop.i	0x0
}
{ .mfb
(p20)	stfd	[r51]=f114,8
(p19)	fnma	f113=f64,f97,f96
	br.ctop.sptk.many	.LL1;;
}
.LL2:

	mov	ar.pfs=r9
	mov	ar.lc=r10
	mov	pr=r11,-1
	mov	r9=ar.itc;;
	sub	r8=r9,r8
	br.ret.sptk.many	rp;;
.endp



