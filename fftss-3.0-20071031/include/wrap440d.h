/*
 *  Copyright (C) 2005 Yuichiro Hourai. All rights reserved.
 *  Copyright (C) 2005 Akira Nukada. All rights reserved.
 *  Copyright (C) 2002-2007 The SSI Project. All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the project nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software 
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE SSI PROJECT ``AS IS'' AND ANY EXPRESS 
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE SSI PROJECT BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 *  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef _WRAP440D_H
#define _WRAP440D_H
#include <math.h>
#ifdef HAVE_COMPLEX_H
#include <complex.h>
#endif

#if !defined(__xlc__)
#define __creal(a) ((double)(a))
#define __cimag(a) cimag(a)
#define __alignx(a,b) {}
#define __cmplx(a,b) (__creal(a)+__creal(b)*_Complex_I)
#define __fsel(a,b,c) ((a)>=0?(c):(b))
#define __dcbt(a) {}
#endif

#define fxcxnpma(c,b,a) ((__creal(a))*(-__cimag(b)+__creal(b)*_Complex_I)+(c))
#define fxcxnsma(c,b,a) ((__creal(a))*(__cimag(b)-__creal(b)*_Complex_I)+(c))

#define fxcpmsub(c,b,a) ((__creal(a))*(b)-(c))
#define fxcpmadd(c,b,a) ((__creal(a))*(b)+(c))
#define fxcpnmadd(c,b,a) (-((__creal(a))*(b)+(c)))
#define fxcpnmsub(c,b,a) (-((__creal(a))*(b)-(c)))
#define fpmul(a,b) (__creal(a)*__creal(b)+(__cimag(a)*__cimag(b))*_Complex_I)
#define fxpmul(b,a) ((__creal(a))*(b))
#define fpmadd(c,b,a) ((__creal(a)*__creal(b)+__creal(c))+(__cimag(a)*__cimag(b)+__cimag(c))*_Complex_I)
#define fpmsub(c,b,a) ((__creal(a)*__creal(b)-__creal(c))+(__cimag(a)*__cimag(b)-__cimag(c))*_Complex_I)
#define fpnmsub(c,b,a) ((__creal(c)-__creal(a)*__creal(b))+(__cimag(c)-__cimag(a)*__cimag(b))*_Complex_I)
#define fpneg(a) (-__creal(a)-__cimag(a)*_Complex_I)
#define fpabs(a) ( fabs(__creal(a))+fabs(__cimag(a))*_Complex_I)
#define fpsel(a,b,c) __cmplx(__fsel(__creal(a),__creal(b),__creal(c)),__fsel(__cimag(a),__cimag(b),__cimag(c)))
#define fpadd(a,b) ((a)+(b))
#define fpsub(a,b) ((a)-(b))


#define __lfpd lfpd
#define __stfpd stfpd
#define __fxcxnpma fxcxnpma
#define __fxcxnsma fxcxnsma

#define __fxcpmsub fxcpmsub
#define __fxcsmsub fxcpmsub
#define __fxcpmadd fxcpmadd
#define __fxcsmadd fxcpmadd
#define __fxcpnmsub fxcpnmsub
#define __fxcsnmsub fxcpnmsub
#define __fxcpnmadd fxcpnmadd
#define __fxcsnmadd fxcpnmadd
#define __fpmadd fpmadd
#define __fpmsub fpmsub
#define __fpnmsub fpnmsub
#define __fpmul fpmul
#define __fxpmul fxpmul
#define __fpneg fpneg
#define __fpabs fpabs
#define __fpsub fpsub
#define __fpadd fpadd
#define __fpsel fpsel

#define lfpd(x) (((double*)(x))[0]+((double*)(x))[1]*_Complex_I)
#define stfpd(b,a) { \
 double *bp = (b);     \
 bp[0] = (__creal(a)); \
 bp[1] = (__cimag(a)); \
 }

#endif
