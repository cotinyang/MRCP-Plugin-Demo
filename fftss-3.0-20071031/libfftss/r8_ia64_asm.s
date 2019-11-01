//
// Copyright (C) 2002 Akira Nukada
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
.global	fftss_kern_r8_ia64_f
.type	fftss_kern_r8_ia64_f,@function
.proc	fftss_kern_r8_ia64_f
fftss_kern_r8_ia64_f:
	alloc	r9=ar.pfs,6,18,0,0
	mov	r8=ar.itc
	mov	r10=ar.lc
	mov	r11=pr;;
	add	r14=-1,r36
	mov	ar.ec=3
	mov	r48=r33
	shladd	r49=r36,4,r33;;
	shladd	r50=r36,4,r49;;
	shladd	r51=r36,4,r50;;
	shladd	r52=r36,4,r51;;
	shladd	r53=r36,4,r52;;
	shladd	r54=r36,4,r53;;
	shladd	r55=r36,4,r54
	mov	ar.lc=r14
	mov	r40=r32
	add	r35=8,r35
	shladd	r41=r37,4,r32;;
	shladd	r42=r37,4,r41;;
	shladd	r43=r37,4,r42;;
	shladd	r44=r37,4,r43;;
	shladd	r45=r37,4,r44;;
	shladd	r46=r37,4,r45;;
	shladd	r47=r37,4,r46
	mov	pr.rot=1<<16
	mov	r21=r34
	mov	r22=r34
	mov	r23=r34
	mov	r24=r34
	mov	r26=r34
	mov	r25=r35
	shladd	r27=r36,4,r34;;
	shladd	r28=r36,4,r27;;
	shladd	r28=r36,4,r28
	mov	r29=r35
	shladd	r30=r36,4,r35
	add	r15=1,r0
	shladd	r16=r37,1,r0	// 2 * r37.
	shladd	r17=r37,1,r37	// 3 * r37
	shladd	r18=r37,2,r0;;	// 4 * r37
	shladd	r19=r37,1,r18	// 6 * r37
	shladd	r20=r37,2,r17	// 7 * r37

L0:
(p16)	ldfpd	f48,f85=[r24]
(p17)	fma	f8=f49,f43,f80
(p16)	cmp.eq.unc	p6,p0=r15,r37
(p17)	fms	f9=f49,f80,f43;;
(p16)	ldfpd	f42,f79=[r45],16
(p17)	fma	f10=f49,f47,f84
(p6)	shladd	r24=r18,4,r24
(p6)	add	r15=0,r0
(p17)	fms	f11=f49,f84,f47;;
(p16)	ldfpd	f46,f83=[r47],16
(p17)	fma	f12=f49,f41,f78
(p6)	shladd	r45=r20,4,r45
(p17)	fms	f13=f49,f78,f41
(p16)	add	r15=1,r15;;
(p16)	ldfpd	f34,f71=[r41],16
(p17)	fma	f14=f51,f37,f74
(p6)	shladd	r47=r20,4,r47
(p17)	fms	f15=f51,f74,f37;;
(p16)	ldfpd	f38,f75=[r43],16
(p17)	fma	f6=f53,f45,f82
(p6)	shladd	r41=r20,4,r41
(p17)	fms	f7=f53,f82,f45;;
(p16)	ldfpd	f40,f77=[r44],16
(p17)	fnma	f43=f86,f8,f35
(p6)	shladd	r43=r20,4,r43
(p17)	fnma	f80=f86,f9,f72;;
(p16)	ldfpd	f50,f87=[r22]
(p17)	fma	f35=f86,f8,f35
(p6)	shladd	r44=r20,4,r44
(p17)	fma	f72=f86,f9,f72;;
(p16)	ldfpd	f36,f73=[r42],16
(p17)	fnma	f47=f86,f10,f39
(p6)	shladd	r22=r16,4,r22
(p17)	fnma	f84=f86,f11,f76;;
(p16)	ldfd	f52=[r26]
(p17)	fma	f39=f86,f10,f39
(p6)	shladd	r42=r20,4,r42
(p18)	stfd	[r54]=f99,8
(p17)	fma	f76=f86,f11,f76;;
//(p17)	stfd	[r48]=f39,8		// debug
(p16)	ldfpd	f44,f81=[r46],16
(p17)	fnma	f41=f86,f12,f33
(p6)	shladd	r26=r19,4,r26
(p17)	fnma	f78=f86,f13,f70;;
(p16)	ldfpd	f32,f69=[r40],16
(p17)	fma	f33=f86,f12,f33
(p6)	shladd	r46=r20,4,r46
(p17)	fma	f70=f86,f13,f70;;
(p16)	ldfd	f54=[r25]
(p17)	fnma	f45=f55,f6,f14
(p6)	shladd	r40=r20,4,r40
(p17)	fnma	f82=f55,f7,f15;;
(p16)	ldfpd	f56,f89=[r21]
(p17)	fma	f37=f55,f6,f14
(p6)	shladd	r25=r16,4,r25
(p17)	fma	f74=f55,f7,f15;;
(p16)	ldfpd	f58,f91=[r23]
(p17)	fma	f8=f57,f35,f72
(p6)	shladd	r21=r37,4,r21
(p17)	fms	f9=f57,f72,f35;;
(p16)	ldfpd	f60,f93=[r27]
(p17)	fma	f10=f61,f43,f80
(p6)	shladd	r23=r17,4,r23
(p17)	fms	f11=f61,f80,f43;;
(p16)	ldfpd	f62,f95=[r28]
(p17)	fma	f12=f59,f39,f76
(p6)	shladd	r27=r37,4,r27
(p17)	fms	f13=f59,f76,f39;;
(p16)	ldfd	f64=[r29]
(p16)	ldfd	f66=[r30]
(p17)	fma	f14=f63,f47,f84
(p18)	stfd	[r49]=f101,8
(p18)	stfd	[r53]=f109,8
(p17)	fms	f15=f63,f84,f47;;
(p18)	stfd	[r49]=f103,8
(p17)	fma	f100=f88,f37,f33
(p6)	shladd	r28=r17,4,r28
(p18)	stfd	[r53]=f111,8
(p17)	fma	f101=f88,f74,f70
(p6)	shladd	r29=r37,4,r29;;
(p18)	stfd	[r51]=f113,8
(p17)	fnma	f102=f88,f37,f33
(p6)	shladd	r30=r37,4,r30
(p18)	stfd	[r55]=f117,8
(p17)	fnma	f103=f88,f74,f70;;
(p18)	stfd	[r51]=f115,8
(p17)	fma	f104=f88,f82,f41
(p18)	stfd	[r55]=f119,8
(p17)	fnma	f105=f88,f45,f78;;
(p17)	fnma	f106=f88,f82,f41
(p17)	fma	f107=f88,f45,f78;;
(p17)	fma	f35=f65,f12,f8
(p17)	fma	f72=f65,f13,f9;;
(p17)	fnma	f39=f65,f12,f8
(p17)	fnma	f76=f65,f13,f9;;
(p17)	fma	f43=f67,f14,f10
(p17)	fma	f80=f67,f15,f11;;
(p17)	fnma	f47=f67,f14,f10
(p17)	fnma	f84=f67,f15,f11;;
(p17)	fma	f8=f90,f35,f100
(p17)	fma	f9=f90,f72,f101;;
(p17)	fnma	f10=f90,f35,f100
(p17)	fnma	f11=f90,f72,f101;;
(p17)	fma	f12=f90,f76,f102
(p17)	fnma	f13=f90,f39,f103;;
(p17)	fnma	f14=f90,f76,f102
(p17)	fma	f98=f90,f39,f103;;
(p17)	stfd	[r48]=f8,8		// should be f8
(p17)	fma	f100=f94,f43,f104
(p17)	fma	f102=f94,f80,f105;;
(p17)	stfd	[r52]=f10,8
(p17)	fnma	f108=f94,f43,f104
(p17)	stfd	[r48]=f9,8
(p17)	fnma	f110=f94,f80,f105;;
(p17)	stfd	[r50]=f12,8
(p17)	fma	f112=f94,f84,f106
(p17)	stfd	[r52]=f11,8
(p17)	fnma	f114=f94,f47,f107;;
(p17)	stfd	[r54]=f14,8
(p17)	fnma	f116=f94,f84,f106
(p17)	stfd	[r50]=f13,8
(p17)	fma	f118=f94,f47,f107
	br.ctop.sptk.many	L0;;

	mov	ar.pfs=r9
	mov	ar.lc=r10
	mov	pr=r11,-1
	mov	r9=ar.itc;;
	sub	r8=r9,r8
	br.ret.sptk.many	rp;;
.endp

.align	128
.global	fftss_kern_r8_ia64_b
.type	fftss_kern_r8_ia64_b,@function
.proc	fftss_kern_r8_ia64_b
fftss_kern_r8_ia64_b:
	alloc	r9=ar.pfs,6,18,0,0
	mov	r8=ar.itc
	mov	r10=ar.lc
	mov	r11=pr;;
	add	r14=-1,r36
	mov	ar.ec=3
	mov	r48=r33
	add	r35=8,r35
	shladd	r49=r36,4,r33;;
	shladd	r50=r36,4,r49;;
	shladd	r51=r36,4,r50;;
	shladd	r52=r36,4,r51;;
	shladd	r53=r36,4,r52;;
	shladd	r54=r36,4,r53;;
	shladd	r55=r36,4,r54
	mov	ar.lc=r14
	mov	r40=r32
	shladd	r41=r37,4,r32;;
	shladd	r42=r37,4,r41;;
	shladd	r43=r37,4,r42;;
	shladd	r44=r37,4,r43;;
	shladd	r45=r37,4,r44;;
	shladd	r46=r37,4,r45;;
	shladd	r47=r37,4,r46
	mov	pr.rot=1<<16
	mov	r21=r34
	mov	r22=r34
	mov	r23=r34
	mov	r24=r34
	mov	r26=r34
	mov	r25=r35
	shladd	r27=r36,4,r34;;
	shladd	r28=r36,4,r27;;
	shladd	r28=r36,4,r28
	mov	r29=r35
	shladd	r30=r36,4,r35
	add	r15=1,r0
	shladd	r16=r37,1,r0	// 2 * r37.
	shladd	r17=r37,1,r37	// 3 * r37
	shladd	r18=r37,2,r0;;	// 4 * r37
	shladd	r19=r37,1,r18	// 6 * r37
	shladd	r20=r37,2,r17	// 7 * r37

L1:
(p16)	ldfpd	f48,f85=[r24]
(p17)	fms	f8=f49,f43,f80
(p16)	cmp.eq.unc	p6,p0=r15,r37
(p17)	fma	f9=f49,f80,f43;;
(p16)	ldfpd	f42,f79=[r45],16
(p17)	fms	f10=f49,f47,f84
(p6)	shladd	r24=r18,4,r24
(p6)	add	r15=0,r0
(p17)	fma	f11=f49,f84,f47;;
(p16)	ldfpd	f46,f83=[r47],16
(p17)	fms	f12=f49,f41,f78
(p6)	shladd	r45=r20,4,r45
(p17)	fma	f13=f49,f78,f41
(p16)	add	r15=1,r15;;
(p16)	ldfpd	f34,f71=[r41],16
(p17)	fms	f14=f51,f37,f74
(p6)	shladd	r47=r20,4,r47
(p17)	fma	f15=f51,f74,f37;;
(p16)	ldfpd	f38,f75=[r43],16
(p17)	fms	f6=f53,f45,f82
(p6)	shladd	r41=r20,4,r41
(p17)	fma	f7=f53,f82,f45;;
(p16)	ldfpd	f40,f77=[r44],16
(p17)	fnma	f43=f86,f8,f35
(p6)	shladd	r43=r20,4,r43
(p17)	fnma	f80=f86,f9,f72;;
(p16)	ldfpd	f50,f87=[r22]
(p17)	fma	f35=f86,f8,f35
(p6)	shladd	r44=r20,4,r44
(p17)	fma	f72=f86,f9,f72;;
(p16)	ldfpd	f36,f73=[r42],16
(p17)	fnma	f47=f86,f10,f39
(p6)	shladd	r22=r16,4,r22
(p17)	fnma	f84=f86,f11,f76;;
(p16)	ldfd	f52=[r26]
(p17)	fma	f39=f86,f10,f39
(p6)	shladd	r42=r20,4,r42
(p18)	stfd	[r54]=f99,8
(p17)	fma	f76=f86,f11,f76;;
//(p17)	stfd	[r48]=f39,8		// debug
(p16)	ldfpd	f44,f81=[r46],16
(p17)	fnma	f41=f86,f12,f33
(p6)	shladd	r26=r19,4,r26
(p17)	fnma	f78=f86,f13,f70;;
(p16)	ldfpd	f32,f69=[r40],16
(p17)	fma	f33=f86,f12,f33
(p6)	shladd	r46=r20,4,r46
(p17)	fma	f70=f86,f13,f70;;
(p16)	ldfd	f54=[r25]
(p17)	fnma	f45=f55,f6,f14
(p6)	shladd	r40=r20,4,r40
(p17)	fnma	f82=f55,f7,f15;;
(p16)	ldfpd	f56,f89=[r21]
(p17)	fma	f37=f55,f6,f14
(p6)	shladd	r25=r16,4,r25
(p17)	fma	f74=f55,f7,f15;;
(p16)	ldfpd	f58,f91=[r23]
(p17)	fms	f8=f57,f35,f72
(p6)	shladd	r21=r37,4,r21
(p17)	fma	f9=f57,f72,f35;;
(p16)	ldfpd	f60,f93=[r27]
(p17)	fms	f10=f61,f43,f80
(p6)	shladd	r23=r17,4,r23
(p17)	fma	f11=f61,f80,f43;;
(p16)	ldfpd	f62,f95=[r28]
(p17)	fms	f12=f59,f39,f76
(p6)	shladd	r27=r37,4,r27
(p17)	fma	f13=f59,f76,f39;;
(p16)	ldfd	f64=[r29]
(p16)	ldfd	f66=[r30]
(p17)	fms	f14=f63,f47,f84
(p18)	stfd	[r49]=f101,8
(p18)	stfd	[r53]=f109,8
(p17)	fma	f15=f63,f84,f47;;
(p18)	stfd	[r49]=f103,8
(p17)	fma	f100=f88,f37,f33
(p6)	shladd	r28=r17,4,r28
(p18)	stfd	[r53]=f111,8
(p17)	fma	f101=f88,f74,f70
(p6)	shladd	r29=r37,4,r29;;
(p18)	stfd	[r51]=f113,8
(p17)	fnma	f102=f88,f37,f33
(p6)	shladd	r30=r37,4,r30
(p18)	stfd	[r55]=f117,8
(p17)	fnma	f103=f88,f74,f70;;
(p18)	stfd	[r51]=f115,8
(p17)	fnma	f104=f88,f82,f41
(p18)	stfd	[r55]=f119,8
(p17)	fma	f105=f88,f45,f78;;
(p17)	fma	f106=f88,f82,f41
(p17)	fnma	f107=f88,f45,f78;;
(p17)	fma	f35=f65,f12,f8
(p17)	fma	f72=f65,f13,f9;;
(p17)	fnma	f39=f65,f12,f8
(p17)	fnma	f76=f65,f13,f9;;
(p17)	fma	f43=f67,f14,f10
(p17)	fma	f80=f67,f15,f11;;
(p17)	fnma	f47=f67,f14,f10
(p17)	fnma	f84=f67,f15,f11;;
(p17)	fma	f8=f90,f35,f100
(p17)	fma	f9=f90,f72,f101;;
(p17)	fnma	f10=f90,f35,f100
(p17)	fnma	f11=f90,f72,f101;;
(p17)	fnma	f12=f90,f76,f102
(p17)	fma	f13=f90,f39,f103;;
(p17)	fma	f14=f90,f76,f102
(p17)	fnma	f98=f90,f39,f103;;
(p17)	stfd	[r48]=f8,8		// should be f8
(p17)	fma	f100=f94,f43,f104
(p17)	fma	f102=f94,f80,f105;;
(p17)	stfd	[r52]=f10,8
(p17)	fnma	f108=f94,f43,f104
(p17)	stfd	[r48]=f9,8
(p17)	fnma	f110=f94,f80,f105;;
(p17)	stfd	[r50]=f12,8
(p17)	fnma	f112=f94,f84,f106
(p17)	stfd	[r52]=f11,8
(p17)	fma	f114=f94,f47,f107;;
(p17)	stfd	[r54]=f14,8
(p17)	fma	f116=f94,f84,f106
(p17)	stfd	[r50]=f13,8
(p17)	fnma	f118=f94,f47,f107
	br.ctop.sptk.many	L1;;

	mov	ar.pfs=r9
	mov	ar.lc=r10
	mov	pr=r11,-1
	mov	r9=ar.itc;;
	sub	r8=r9,r8
	br.ret.sptk.many	rp;;
.endp

