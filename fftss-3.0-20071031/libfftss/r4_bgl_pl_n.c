/*
 * Copyright (C) 2002-2007 Akira Nukada. All rights reserved.
 * Copyright (C) 2002-2007 The SSI Project. All rights reserved. 
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
#include "libfftss.h"
#ifdef USE_WRAP440D
#include "wrap440d.h"
#endif

void fftss_r4_bgl_pl_o_f(double *vi, double *vo, double *wn,
			 long bsize, long blocks)
{
  fftss_r4_fma_o_f(vi, vo, wn, bsize, blocks);
}

void fftss_r4_bgl_pl_o_b(double *vi, double *vo, double *wn,
			 long bsize, long blocks)
{
  fftss_r4_fma_o_b(vi, vo, wn, bsize, blocks);
}

void fftss_r4_bgl_pl_n_f(const double *vi, double *restrict vo, 
			const double *wn,
			long bsize, long blocks)
{
  double _Complex f0, f1, f2, f3, f4, f5, f6, f7;
  double _Complex f8, f9, f10, f11, f12, f13, f14, f15;
  double _Complex f16, f17, f18, f19, f20, f21;
  const double *vi0, *vi1, *vi2, *vi3;
  double *vo0, *vo1, *vo2, *vo3;
  const double *w1, *w2, *w3;
  long i;
  long n;
  long mask0, mask1;
  long step, count;

#pragma disjoint(*vo0,*vo1,*vo2,*vo3,*vi0,*vi1,*vi2,*vi3,*w1,*w2,*w3)

  n = ((bsize * blocks) >> 2) - 1;
  mask0 = (bsize >> 2);
  mask1 = mask0 - 1;

  vo0 = vo;
  vo1 = vo0 + 2 * blocks * bsize;
  vo2 = vo1 + 2 * blocks * bsize;
  vo3 = vo2 + 2 * blocks * bsize;
  vi0 = vi;
  vi1 = vi0 + 2 * bsize;
  vi2 = vi1 + 2 * bsize;
  vi3 = vi2 + 2 * bsize;

  w1 = wn; w2 = wn; w3 = wn + blocks * bsize * 6;
  
  f2 = __lfpd(vi2);
  f17 = __lfpd(w2);
  f3 = __lfpd(vi3);
  f18 = __lfpd(w3);
  f1 = __lfpd(vi1);        
  f16 = __lfpd(w1);              f6 = __fxcxnpma(f2,f2,__creal(f17));

  f0 = __lfpd(vi0);              f7 = __fxcxnpma(f3,f3,__creal(f18));
  f2 = __lfpd(vi2 + 2);          f5 = __fxcxnpma(f1,f1,__creal(f16));
   
  f3 = __lfpd(vi3 + 2);

  f1 = __lfpd(vi1 + 2);
                                 f8 = __fxcxnsma(f0,f6,__cimag(f17));
                                 f10 = __fxcxnpma(f0,f6,__cimag(f17));
			         f9 = __fxcpmadd(f5,f7,__cimag(f18));
			         f11 = __fxcpnmsub(f5,f7,__cimag(f18));
                                 f6 = __fxcxnpma(f2,f2,__creal(f17));

  f0 = __lfpd(vi0 + 2);          f7  = __fxcxnpma(f3,f3,__creal(f18));
  f2 = __lfpd(vi2 + 4);          f5  = __fxcxnpma(f1,f1,__creal(f16));
                                 f12 = __fxcxnsma(f8,f9,__cimag(f16));
  f3 = __lfpd(vi3 + 4);          f13 = __fxcxnpma(f8,f9,__cimag(f16));
                                 f14 = __fxcsnmsub(f10,f11,__cimag(f16));
  f1 = __lfpd(vi1 + 4);          f15 = __fxcsmadd(f10,f11,__cimag(f16));
                                 f8  = __fxcxnsma(f0,f6,__cimag(f17));
  __stfpd(vo0, f12);             f10 = __fxcxnpma(f0,f6,__cimag(f17));
  __stfpd(vo2, f13);             f9  = __fxcpmadd(f5,f7,__cimag(f18));
  __stfpd(vo1, f14);             f11 = __fxcpnmsub(f5,f7,__cimag(f18));
  __stfpd(vo3, f15);             f6  = __fxcxnpma(f2,f2,__creal(f17));

  f0 = __lfpd(vi0 + 4);          f7  = __fxcxnpma(f3,f3,__creal(f18));
  f2 = __lfpd(vi2 + 6);          f5  = __fxcxnpma(f1,f1,__creal(f16));
  f20 = __lfpd(w2);              f12 = __fxcxnsma(f8,f9,__cimag(f16));
  f3 = __lfpd(vi3 + 6);          f13 = __fxcxnpma(f8,f9,__cimag(f16));
  f21 = __lfpd(w3);              f14 = __fxcsnmsub(f10,f11,__cimag(f16));
  f1 = __lfpd(vi1 + 6);          f15 = __fxcsmadd(f10,f11,__cimag(f16));
  f19 = __lfpd(w1);              f8  = __fxcxnsma(f0,f6,__cimag(f17));
  __stfpd(vo0 + 2, f12);         f10 = __fxcxnpma(f0,f6,__cimag(f17));
  __stfpd(vo2 + 2, f13);         f9  = __fxcpmadd(f5,f7,__cimag(f18));
  __stfpd(vo1 + 2, f14);         f11 = __fxcpnmsub(f5,f7,__cimag(f18));
  __stfpd(vo3 + 2, f15);         f6  = __fxcxnpma(f2,f2,__creal(f20));

  vi0 += 8; vi1 += 8; vi2 += 8; vi3 += 8;
  vo0 += 4; vo1 += 4; vo2 += 4; vo3 += 4;
  count = 1;
  i = 0;
  
  while (i < n) {
    step = count & mask0;
    f0 = __lfpd(vi0 + 8 * i - 2);  f7 = __fxcxnpma(f3,f3,__creal(f21));
    vi0 += 24 * step; 
    vi1 += 24 * step; 
    vi2 += 24 * step;
    vi3 += 24 * step;
    w1 += 8 * step; 
    w2 += 16 * step; 
    w3 += 8 * step;
    f2 = __lfpd(vi2 + 8 * i);      f5  = __fxcxnpma(f1,f1,__creal(f19));
    f17 = __lfpd(w2);              f12 = __fxcxnsma(f8,f9,__cimag(f16));
    f3 = __lfpd(vi3 + 8 * i);      f13 = __fxcxnpma(f8,f9,__cimag(f16));
    f18 = __lfpd(w3);              f14 = __fxcsnmsub(f10,f11,__cimag(f16));
    f1 = __lfpd(vi1 + 8 * i);      f15 = __fxcsmadd(f10,f11,__cimag(f16));
    f16 = __lfpd(w1);              f8  = __fxcxnsma(f0,f6,__cimag(f20));
    __stfpd(vo0 + 8 * i, f12);     f10 = __fxcxnpma(f0,f6,__cimag(f20));
    __stfpd(vo2 + 8 * i, f13);     f9  = __fxcpmadd(f5,f7,__cimag(f21));
    __stfpd(vo1 + 8 * i, f14);     f11 = __fxcpnmsub(f5,f7,__cimag(f21));
    __stfpd(vo3 + 8 * i, f15);     f6  = __fxcxnpma(f2,f2,__creal(f17));

    f0 = __lfpd(vi0 + 8 * i);      f7  = __fxcxnpma(f3,f3,__creal(f18));
    f2 = __lfpd(vi2 + 8 * i + 2);  f5  = __fxcxnpma(f1,f1,__creal(f16));
    count &= mask1;                f12 = __fxcxnsma(f8,f9,__cimag(f19));
    f3 = __lfpd(vi3 + 8 * i + 2);  f13 = __fxcxnpma(f8,f9,__cimag(f19));
    count++;                       f14 = __fxcsnmsub(f10,f11,__cimag(f19));
    f1 = __lfpd(vi1 + 8 * i + 2);  f15 = __fxcsmadd(f10,f11,__cimag(f19));
                                   f8  = __fxcxnsma(f0,f6,__cimag(f17));
    __stfpd(vo0 + 8 * i + 2, f12); f10 = __fxcxnpma(f0,f6,__cimag(f17));
    __stfpd(vo2 + 8 * i + 2, f13); f9  = __fxcpmadd(f5,f7,__cimag(f18));
    __stfpd(vo1 + 8 * i + 2, f14); f11 = __fxcpnmsub(f5,f7,__cimag(f18));
    __stfpd(vo3 + 8 * i + 2, f15); f6  = __fxcxnpma(f2,f2,__creal(f17));

    f0 = __lfpd(vi0 + 8 * i + 2);  f7  = __fxcxnpma(f3,f3,__creal(f18));
    f2 = __lfpd(vi2 + 8 * i + 4);  f5  = __fxcxnpma(f1,f1,__creal(f16));
                                   f12 = __fxcxnsma(f8,f9,__cimag(f16));
    f3 = __lfpd(vi3 + 8 * i + 4);  f13 = __fxcxnpma(f8,f9,__cimag(f16));
                                   f14 = __fxcsnmsub(f10,f11,__cimag(f16));
    f1 = __lfpd(vi1 + 8 * i + 4);  f15 = __fxcsmadd(f10,f11,__cimag(f16));
                                   f8  = __fxcxnsma(f0,f6,__cimag(f17));
    __stfpd(vo0 + 8 * i + 4, f12); f10 = __fxcxnpma(f0,f6,__cimag(f17));
    __stfpd(vo2 + 8 * i + 4, f13); f9  = __fxcpmadd(f5,f7,__cimag(f18));
    __stfpd(vo1 + 8 * i + 4, f14); f11 = __fxcpnmsub(f5,f7,__cimag(f18));
    __stfpd(vo3 + 8 * i + 4, f15); f6  = __fxcxnpma(f2,f2,__creal(f17));

    f0 = __lfpd(vi0 + 8 * i + 4);  f7  = __fxcxnpma(f3,f3,__creal(f18));
    f2 = __lfpd(vi2 + 8 * i + 6);  f5  = __fxcxnpma(f1,f1,__creal(f16));
    f20 = __lfpd(w2);              f12 = __fxcxnsma(f8,f9,__cimag(f16));
    f3 = __lfpd(vi3 + 8 * i + 6);  f13 = __fxcxnpma(f8,f9,__cimag(f16));
    f21 = __lfpd(w3);              f14 = __fxcsnmsub(f10,f11,__cimag(f16));
    f1 = __lfpd(vi1 + 8 * i + 6);  f15 = __fxcsmadd(f10,f11,__cimag(f16));
    f19 = __lfpd(w1);              f8  = __fxcxnsma(f0,f6,__cimag(f17));
    __stfpd(vo0 + 8 * i + 6, f12); f10 = __fxcxnpma(f0,f6,__cimag(f17));
    __stfpd(vo2 + 8 * i + 6, f13); f9  = __fxcpmadd(f5,f7,__cimag(f18));
    __stfpd(vo1 + 8 * i + 6, f14); f11 = __fxcpnmsub(f5,f7,__cimag(f18));
    __stfpd(vo3 + 8 * i + 6, f15); f6  = __fxcxnpma(f2,f2,__creal(f20));
    i++;

  }


  vi0 += 8 * n;
  vo0 += 8 * n; vo1 += 8 * n; vo2 += 8 * n; vo3 += 8 * n;

  w1 += 4 * n; w2 += 8 * n; w3 += 4 * n;
      
  f0 = __lfpd(vi0 - 2);           f7 = __fxcxnpma(f3,f3,__creal(f21));
                                  f5 = __fxcxnpma(f1,f1,__creal(f19));
                                  f12 = __fxcxnsma(f8,f9,__cimag(f16));
                                  f13 = __fxcxnpma(f8,f9,__cimag(f16));
                                  f14 = __fxcsnmsub(f10,f11,__cimag(f16));
                                  f15 = __fxcsmadd(f10,f11,__cimag(f16));
                                  f8 = __fxcxnsma(f0,f6,__cimag(f20));
  __stfpd(vo0, f12);              f10 = __fxcxnpma(f0,f6,__cimag(f20));
  __stfpd(vo2, f13);              f9 = __fxcpmadd(f5,f7,__cimag(f21));
  __stfpd(vo1, f14);              f11 = __fxcpnmsub(f5,f7,__cimag(f21));
  __stfpd(vo3, f15);   

                               
                               
                                  f12 = __fxcxnsma(f8,f9,__cimag(f19));
                                  f13 = __fxcxnpma(f8,f9,__cimag(f19));
                                  f14 = __fxcsnmsub(f10,f11,__cimag(f19));
                                  f15 = __fxcsmadd(f10,f11,__cimag(f19));
   
  __stfpd(vo0 + 2, f12);
  __stfpd(vo2 + 2, f13);
  __stfpd(vo1 + 2, f14);
  __stfpd(vo3 + 2, f15);
}

void fftss_r4_bgl_pl_n_b(const double *vi, double *restrict vo, 
			const double *wn,
			long bsize, long blocks)
{
  double _Complex f0, f1, f2, f3, f4, f5, f6, f7;
  double _Complex f8, f9, f10, f11, f12, f13, f14, f15;
  double _Complex f16, f17, f18, f19, f20, f21;
  const double *vi0, *vi1, *vi2, *vi3;
  double *vo0, *vo1, *vo2, *vo3;
  const double *w1, *w2, *w3;
  long i;
  long n;
  long mask0, mask1;
  long step, count;

#pragma disjoint(*vo0,*vo1,*vo2,*vo3,*vi0,*vi1,*vi2,*vi3,*w1,*w2,*w3)


  n = ((bsize * blocks) >> 2) - 1;
  mask0 = (bsize >> 2);
  mask1 = mask0 - 1;

  vo0 = vo;
  vo1 = vo0 + 2 * blocks * bsize;
  vo2 = vo1 + 2 * blocks * bsize;
  vo3 = vo2 + 2 * blocks * bsize;
  vi0 = vi;
  vi1 = vi0 + 2 * bsize;
  vi2 = vi1 + 2 * bsize;
  vi3 = vi2 + 2 * bsize;

  w1 = wn; w2 = wn; w3 = wn + blocks * bsize * 6;
  
  f2 = __lfpd(vi2);
  f17 = __lfpd(w2);
  f3 = __lfpd(vi3);
  f18 = __lfpd(w3);
  f1 = __lfpd(vi1);        
  f16 = __lfpd(w1);              f6 = __fxcxnsma(f2,f2,__creal(f17));

  f0 = __lfpd(vi0);              f7 = __fxcxnsma(f3,f3,__creal(f18));
  f2 = __lfpd(vi2 + 2);          f5 = __fxcxnsma(f1,f1,__creal(f16));
   
  f3 = __lfpd(vi3 + 2);

  f1 = __lfpd(vi1 + 2);
                                 f8 = __fxcxnpma(f0,f6,__cimag(f17));
                                 f10 = __fxcxnsma(f0,f6,__cimag(f17));
			         f9 = __fxcpmadd(f5,f7,__cimag(f18));
			         f11 = __fxcpnmsub(f5,f7,__cimag(f18));
                                 f6 = __fxcxnsma(f2,f2,__creal(f17));

  f0 = __lfpd(vi0 + 2);          f7  = __fxcxnsma(f3,f3,__creal(f18));
  f2 = __lfpd(vi2 + 4);          f5  = __fxcxnsma(f1,f1,__creal(f16));
                                 f12 = __fxcxnpma(f8,f9,__cimag(f16));
  f3 = __lfpd(vi3 + 4);          f13 = __fxcxnsma(f8,f9,__cimag(f16));
                                 f14 = __fxcsnmsub(f10,f11,__cimag(f16));
  f1 = __lfpd(vi1 + 4);          f15 = __fxcsmadd(f10,f11,__cimag(f16));
                                 f8  = __fxcxnpma(f0,f6,__cimag(f17));
  __stfpd(vo0, f12);             f10 = __fxcxnsma(f0,f6,__cimag(f17));
  __stfpd(vo2, f13);             f9  = __fxcpmadd(f5,f7,__cimag(f18));
  __stfpd(vo1, f14);             f11 = __fxcpnmsub(f5,f7,__cimag(f18));
  __stfpd(vo3, f15);             f6  = __fxcxnsma(f2,f2,__creal(f17));

  f0 = __lfpd(vi0 + 4);          f7  = __fxcxnsma(f3,f3,__creal(f18));
  f2 = __lfpd(vi2 + 6);          f5  = __fxcxnsma(f1,f1,__creal(f16));
  f20 = __lfpd(w2);              f12 = __fxcxnpma(f8,f9,__cimag(f16));
  f3 = __lfpd(vi3 + 6);          f13 = __fxcxnsma(f8,f9,__cimag(f16));
  f21 = __lfpd(w3);              f14 = __fxcsnmsub(f10,f11,__cimag(f16));
  f1 = __lfpd(vi1 + 6);          f15 = __fxcsmadd(f10,f11,__cimag(f16));
  f19 = __lfpd(w1);              f8  = __fxcxnpma(f0,f6,__cimag(f17));
  __stfpd(vo0 + 2, f12);         f10 = __fxcxnsma(f0,f6,__cimag(f17));
  __stfpd(vo2 + 2, f13);         f9  = __fxcpmadd(f5,f7,__cimag(f18));
  __stfpd(vo1 + 2, f14);         f11 = __fxcpnmsub(f5,f7,__cimag(f18));
  __stfpd(vo3 + 2, f15);         f6  = __fxcxnsma(f2,f2,__creal(f20));

  vi0 += 8; vi1 += 8; vi2 += 8; vi3 += 8;
  vo0 += 4; vo1 += 4; vo2 += 4; vo3 += 4;
  count = 1;
  i = 0;

  
  while (i < n) {
    step = count & mask0;
    f0 = __lfpd(vi0 + 8 * i - 2);  f7  = __fxcxnsma(f3,f3,__creal(f21));
    vi0 += 24 * step; 
    vi1 += 24 * step; 
    vi2 += 24 * step;
    vi3 += 24 * step;
    w1 += 8 * step; 
    w2 += 16 * step; 
    w3 += 8 * step;
    f2 = __lfpd(vi2 + 8 * i);      f5  = __fxcxnsma(f1,f1,__creal(f19));
    f17 = __lfpd(w2);              f12 = __fxcxnpma(f8,f9,__cimag(f16));
    f3 = __lfpd(vi3 + 8 * i);      f13 = __fxcxnsma(f8,f9,__cimag(f16));
    f18 = __lfpd(w3);              f14 = __fxcsnmsub(f10,f11,__cimag(f16));
    f1 = __lfpd(vi1 + 8 * i);      f15 = __fxcsmadd(f10,f11,__cimag(f16));
    f16 = __lfpd(w1);              f8  = __fxcxnpma(f0,f6,__cimag(f20));
    __stfpd(vo0 + 8 * i, f12);     f10 = __fxcxnsma(f0,f6,__cimag(f20));
    __stfpd(vo2 + 8 * i, f13);     f9  = __fxcpmadd(f5,f7,__cimag(f21));
    __stfpd(vo1 + 8 * i, f14);     f11 = __fxcpnmsub(f5,f7,__cimag(f21));
    __stfpd(vo3 + 8 * i, f15);     f6  = __fxcxnsma(f2,f2,__creal(f17));

    f0 = __lfpd(vi0 + 8 * i);      f7  = __fxcxnsma(f3,f3,__creal(f18));
    f2 = __lfpd(vi2 + 8 * i + 2);  f5  = __fxcxnsma(f1,f1,__creal(f16));
    count &= mask1;                f12 = __fxcxnpma(f8,f9,__cimag(f19));
    f3 = __lfpd(vi3 + 8 * i + 2);  f13 = __fxcxnsma(f8,f9,__cimag(f19));
    count++;                       f14 = __fxcsnmsub(f10,f11,__cimag(f19));
    f1 = __lfpd(vi1 + 8 * i + 2);  f15 = __fxcsmadd(f10,f11,__cimag(f19));
                                   f8  = __fxcxnpma(f0,f6,__cimag(f17));
    __stfpd(vo0 + 8 * i + 2, f12); f10 = __fxcxnsma(f0,f6,__cimag(f17));
    __stfpd(vo2 + 8 * i + 2, f13); f9  = __fxcpmadd(f5,f7,__cimag(f18));
    __stfpd(vo1 + 8 * i + 2, f14); f11 = __fxcpnmsub(f5,f7,__cimag(f18));
    __stfpd(vo3 + 8 * i + 2, f15); f6  = __fxcxnsma(f2,f2,__creal(f17));

    f0 = __lfpd(vi0 + 8 * i + 2);  f7  = __fxcxnsma(f3,f3,__creal(f18));
    f2 = __lfpd(vi2 + 8 * i + 4);  f5  = __fxcxnsma(f1,f1,__creal(f16));
                                   f12 = __fxcxnpma(f8,f9,__cimag(f16));
    f3 = __lfpd(vi3 + 8 * i + 4);  f13 = __fxcxnsma(f8,f9,__cimag(f16));
                                   f14 = __fxcsnmsub(f10,f11,__cimag(f16));
    f1 = __lfpd(vi1 + 8 * i + 4);  f15 = __fxcsmadd(f10,f11,__cimag(f16));
                                   f8  = __fxcxnpma(f0,f6,__cimag(f17));
    __stfpd(vo0 + 8 * i + 4, f12); f10 = __fxcxnsma(f0,f6,__cimag(f17));
    __stfpd(vo2 + 8 * i + 4, f13); f9  = __fxcpmadd(f5,f7,__cimag(f18));
    __stfpd(vo1 + 8 * i + 4, f14); f11 = __fxcpnmsub(f5,f7,__cimag(f18));
    __stfpd(vo3 + 8 * i + 4, f15); f6  = __fxcxnsma(f2,f2,__creal(f17));

    f0 = __lfpd(vi0 + 8 * i + 4);  f7  = __fxcxnsma(f3,f3,__creal(f18));
    f2 = __lfpd(vi2 + 8 * i + 6);  f5  = __fxcxnsma(f1,f1,__creal(f16));
    f20 = __lfpd(w2);              f12 = __fxcxnpma(f8,f9,__cimag(f16));
    f3 = __lfpd(vi3 + 8 * i + 6);  f13 = __fxcxnsma(f8,f9,__cimag(f16));
    f21 = __lfpd(w3);              f14 = __fxcsnmsub(f10,f11,__cimag(f16));
    f1 = __lfpd(vi1 + 8 * i + 6);  f15 = __fxcsmadd(f10,f11,__cimag(f16));
    f19 = __lfpd(w1);              f8  = __fxcxnpma(f0,f6,__cimag(f17));
    __stfpd(vo0 + 8 * i + 6, f12); f10 = __fxcxnsma(f0,f6,__cimag(f17));
    __stfpd(vo2 + 8 * i + 6, f13); f9  = __fxcpmadd(f5,f7,__cimag(f18));
    __stfpd(vo1 + 8 * i + 6, f14); f11 = __fxcpnmsub(f5,f7,__cimag(f18));
    __stfpd(vo3 + 8 * i + 6, f15); f6  = __fxcxnsma(f2,f2,__creal(f20));
    i++;

  };


  vi0 += 8 * n;
  vo0 += 8 * n; vo1 += 8 * n; vo2 += 8 * n; vo3 += 8 * n;

  w1 += 4 * n; w2 += 8 * n; w3 += 4 * n;
      
  f0 = __lfpd(vi0 - 2);           f7 = __fxcxnsma(f3,f3,__creal(f21));
                                  f5 = __fxcxnsma(f1,f1,__creal(f19));
                                  f12 = __fxcxnpma(f8,f9,__cimag(f16));
                                  f13 = __fxcxnsma(f8,f9,__cimag(f16));
                                  f14 = __fxcsnmsub(f10,f11,__cimag(f16));
                                  f15 = __fxcsmadd(f10,f11,__cimag(f16));
                                  f8 = __fxcxnpma(f0,f6,__cimag(f20));
  __stfpd(vo0, f12);              f10 = __fxcxnsma(f0,f6,__cimag(f20));
  __stfpd(vo2, f13);              f9 = __fxcpmadd(f5,f7,__cimag(f21));
  __stfpd(vo1, f14);              f11 = __fxcpnmsub(f5,f7,__cimag(f21));
  __stfpd(vo3, f15);   

                               
                               
                                  f12 = __fxcxnpma(f8,f9,__cimag(f19));
                                  f13 = __fxcxnsma(f8,f9,__cimag(f19));
                                  f14 = __fxcsnmsub(f10,f11,__cimag(f19));
                                  f15 = __fxcsmadd(f10,f11,__cimag(f19));
   
  __stfpd(vo0 + 2, f12);
  __stfpd(vo2 + 2, f13);
  __stfpd(vo1 + 2, f14);
  __stfpd(vo3 + 2, f15);
}

void fftss_r4_bgl_pl_u4_f(const double *vi, double *restrict vo, 
			  const double *wn,
			  long bsize, long blocks)
{
  double _Complex f0, f1, f2, f3, f4, f5, f6, f7;
  double _Complex f8, f9, f10, f11, f12, f13, f14, f15;
  double _Complex f16, f17, f18, f19, f20, f21;
  const double *vi0, *vi1, *vi2, *vi3;
  double *vo0, *vo1, *vo2, *vo3;
  const double *w1, *w2, *w3;
  long i;
  long n;

#pragma disjoint(*vo0,*vo1,*vo2,*vo3,*vi0,*vi1,*vi2,*vi3,*w1,*w2,*w3)

  n = ((bsize * blocks) >> 2) - 1;

  vo0 = vo;
  vo1 = vo0 + 2 * blocks * bsize;
  vo2 = vo1 + 2 * blocks * bsize;
  vo3 = vo2 + 2 * blocks * bsize;
  vi0 = vi;
  vi1 = vi0 + 2 * bsize;
  vi2 = vi1 + 2 * bsize;
  vi3 = vi2 + 2 * bsize;

  w1 = wn; w2 = wn; w3 = wn + blocks * bsize * 6;
  
  f2 = __lfpd(vi2);
  f17 = __lfpd(w2);
  f3 = __lfpd(vi3);
  f18 = __lfpd(w3);
  f1 = __lfpd(vi1);        
  f16 = __lfpd(w1);              f6 = __fxcxnpma(f2,f2,__creal(f17));

  f0 = __lfpd(vi0);              f7 = __fxcxnpma(f3,f3,__creal(f18));
  f2 = __lfpd(vi2 + 2);          f5 = __fxcxnpma(f1,f1,__creal(f16));
   
  f3 = __lfpd(vi3 + 2);

  f1 = __lfpd(vi1 + 2);
                                 f8 = __fxcxnsma(f0,f6,__cimag(f17));
                                 f10 = __fxcxnpma(f0,f6,__cimag(f17));
			         f9 = __fxcpmadd(f5,f7,__cimag(f18));
			         f11 = __fxcpnmsub(f5,f7,__cimag(f18));
                                 f6 = __fxcxnpma(f2,f2,__creal(f17));

  f0 = __lfpd(vi0 + 2);          f7  = __fxcxnpma(f3,f3,__creal(f18));
  f2 = __lfpd(vi2 + 4);          f5  = __fxcxnpma(f1,f1,__creal(f16));
                                 f12 = __fxcxnsma(f8,f9,__cimag(f16));
  f3 = __lfpd(vi3 + 4);          f13 = __fxcxnpma(f8,f9,__cimag(f16));
                                 f14 = __fxcsnmsub(f10,f11,__cimag(f16));
  f1 = __lfpd(vi1 + 4);          f15 = __fxcsmadd(f10,f11,__cimag(f16));
                                 f8  = __fxcxnsma(f0,f6,__cimag(f17));
  __stfpd(vo0, f12);             f10 = __fxcxnpma(f0,f6,__cimag(f17));
  __stfpd(vo2, f13);             f9  = __fxcpmadd(f5,f7,__cimag(f18));
  __stfpd(vo1, f14);             f11 = __fxcpnmsub(f5,f7,__cimag(f18));
  __stfpd(vo3, f15);             f6  = __fxcxnpma(f2,f2,__creal(f17));

  f0 = __lfpd(vi0 + 4);          f7  = __fxcxnpma(f3,f3,__creal(f18));
  f2 = __lfpd(vi2 + 6);          f5  = __fxcxnpma(f1,f1,__creal(f16));
  f20 = __lfpd(w2);              f12 = __fxcxnsma(f8,f9,__cimag(f16));
  f3 = __lfpd(vi3 + 6);          f13 = __fxcxnpma(f8,f9,__cimag(f16));
  f21 = __lfpd(w3);              f14 = __fxcsnmsub(f10,f11,__cimag(f16));
  f1 = __lfpd(vi1 + 6);          f15 = __fxcsmadd(f10,f11,__cimag(f16));
  f19 = __lfpd(w1);              f8  = __fxcxnsma(f0,f6,__cimag(f17));
  __stfpd(vo0 + 2, f12);         f10 = __fxcxnpma(f0,f6,__cimag(f17));
  __stfpd(vo2 + 2, f13);         f9  = __fxcpmadd(f5,f7,__cimag(f18));
  __stfpd(vo1 + 2, f14);         f11 = __fxcpnmsub(f5,f7,__cimag(f18));
  __stfpd(vo3 + 2, f15);         f6  = __fxcxnpma(f2,f2,__creal(f20));

  vi0 += 8; vi1 += 8; vi2 += 8; vi3 += 8;
  vo0 += 4; vo1 += 4; vo2 += 4; vo3 += 4;

  i = 0;
  
  while (i < n) {
    f0 = __lfpd(vi0 + 8 * i - 2);  f7 = __fxcxnpma(f3,f3,__creal(f21));
    vi0 += 24; 
    vi1 += 24; 
    vi2 += 24;
    vi3 += 24;
    w1 += 8; 
    w2 += 16; 
    w3 += 8;
    f2 = __lfpd(vi2 + 8 * i);      f5  = __fxcxnpma(f1,f1,__creal(f19));
    f17 = __lfpd(w2);              f12 = __fxcxnsma(f8,f9,__cimag(f16));
    f3 = __lfpd(vi3 + 8 * i);      f13 = __fxcxnpma(f8,f9,__cimag(f16));
    f18 = __lfpd(w3);              f14 = __fxcsnmsub(f10,f11,__cimag(f16));
    f1 = __lfpd(vi1 + 8 * i);      f15 = __fxcsmadd(f10,f11,__cimag(f16));
    f16 = __lfpd(w1);              f8  = __fxcxnsma(f0,f6,__cimag(f20));
    __stfpd(vo0 + 8 * i, f12);     f10 = __fxcxnpma(f0,f6,__cimag(f20));
    __stfpd(vo2 + 8 * i, f13);     f9  = __fxcpmadd(f5,f7,__cimag(f21));
    __stfpd(vo1 + 8 * i, f14);     f11 = __fxcpnmsub(f5,f7,__cimag(f21));
    __stfpd(vo3 + 8 * i, f15);     f6  = __fxcxnpma(f2,f2,__creal(f17));

    f0 = __lfpd(vi0 + 8 * i);      f7  = __fxcxnpma(f3,f3,__creal(f18));
    f2 = __lfpd(vi2 + 8 * i + 2);  f5  = __fxcxnpma(f1,f1,__creal(f16));
                                   f12 = __fxcxnsma(f8,f9,__cimag(f19));
    f3 = __lfpd(vi3 + 8 * i + 2);  f13 = __fxcxnpma(f8,f9,__cimag(f19));
                                   f14 = __fxcsnmsub(f10,f11,__cimag(f19));
    f1 = __lfpd(vi1 + 8 * i + 2);  f15 = __fxcsmadd(f10,f11,__cimag(f19));
                                   f8  = __fxcxnsma(f0,f6,__cimag(f17));
    __stfpd(vo0 + 8 * i + 2, f12); f10 = __fxcxnpma(f0,f6,__cimag(f17));
    __stfpd(vo2 + 8 * i + 2, f13); f9  = __fxcpmadd(f5,f7,__cimag(f18));
    __stfpd(vo1 + 8 * i + 2, f14); f11 = __fxcpnmsub(f5,f7,__cimag(f18));
    __stfpd(vo3 + 8 * i + 2, f15); f6  = __fxcxnpma(f2,f2,__creal(f17));

    f0 = __lfpd(vi0 + 8 * i + 2);  f7  = __fxcxnpma(f3,f3,__creal(f18));
    f2 = __lfpd(vi2 + 8 * i + 4);  f5  = __fxcxnpma(f1,f1,__creal(f16));
                                   f12 = __fxcxnsma(f8,f9,__cimag(f16));
    f3 = __lfpd(vi3 + 8 * i + 4);  f13 = __fxcxnpma(f8,f9,__cimag(f16));
                                   f14 = __fxcsnmsub(f10,f11,__cimag(f16));
    f1 = __lfpd(vi1 + 8 * i + 4);  f15 = __fxcsmadd(f10,f11,__cimag(f16));
                                   f8  = __fxcxnsma(f0,f6,__cimag(f17));
    __stfpd(vo0 + 8 * i + 4, f12); f10 = __fxcxnpma(f0,f6,__cimag(f17));
    __stfpd(vo2 + 8 * i + 4, f13); f9  = __fxcpmadd(f5,f7,__cimag(f18));
    __stfpd(vo1 + 8 * i + 4, f14); f11 = __fxcpnmsub(f5,f7,__cimag(f18));
    __stfpd(vo3 + 8 * i + 4, f15); f6  = __fxcxnpma(f2,f2,__creal(f17));

    f0 = __lfpd(vi0 + 8 * i + 4);  f7  = __fxcxnpma(f3,f3,__creal(f18));
    f2 = __lfpd(vi2 + 8 * i + 6);  f5  = __fxcxnpma(f1,f1,__creal(f16));
    f20 = __lfpd(w2);              f12 = __fxcxnsma(f8,f9,__cimag(f16));
    f3 = __lfpd(vi3 + 8 * i + 6);  f13 = __fxcxnpma(f8,f9,__cimag(f16));
    f21 = __lfpd(w3);              f14 = __fxcsnmsub(f10,f11,__cimag(f16));
    f1 = __lfpd(vi1 + 8 * i + 6);  f15 = __fxcsmadd(f10,f11,__cimag(f16));
    f19 = __lfpd(w1);              f8  = __fxcxnsma(f0,f6,__cimag(f17));
    __stfpd(vo0 + 8 * i + 6, f12); f10 = __fxcxnpma(f0,f6,__cimag(f17));
    __stfpd(vo2 + 8 * i + 6, f13); f9  = __fxcpmadd(f5,f7,__cimag(f18));
    __stfpd(vo1 + 8 * i + 6, f14); f11 = __fxcpnmsub(f5,f7,__cimag(f18));
    __stfpd(vo3 + 8 * i + 6, f15); f6  = __fxcxnpma(f2,f2,__creal(f20));
    i++;

  }


  vi0 += 8 * n;
  vo0 += 8 * n; vo1 += 8 * n; vo2 += 8 * n; vo3 += 8 * n;

  w1 += 4 * n; w2 += 8 * n; w3 += 4 * n;
      
  f0 = __lfpd(vi0 - 2);           f7 = __fxcxnpma(f3,f3,__creal(f21));
                                  f5 = __fxcxnpma(f1,f1,__creal(f19));
                                  f12 = __fxcxnsma(f8,f9,__cimag(f16));
                                  f13 = __fxcxnpma(f8,f9,__cimag(f16));
                                  f14 = __fxcsnmsub(f10,f11,__cimag(f16));
                                  f15 = __fxcsmadd(f10,f11,__cimag(f16));
                                  f8 = __fxcxnsma(f0,f6,__cimag(f20));
  __stfpd(vo0, f12);              f10 = __fxcxnpma(f0,f6,__cimag(f20));
  __stfpd(vo2, f13);              f9 = __fxcpmadd(f5,f7,__cimag(f21));
  __stfpd(vo1, f14);              f11 = __fxcpnmsub(f5,f7,__cimag(f21));
  __stfpd(vo3, f15);   

                               
                               
                                  f12 = __fxcxnsma(f8,f9,__cimag(f19));
                                  f13 = __fxcxnpma(f8,f9,__cimag(f19));
                                  f14 = __fxcsnmsub(f10,f11,__cimag(f19));
                                  f15 = __fxcsmadd(f10,f11,__cimag(f19));
   
  __stfpd(vo0 + 2, f12);
  __stfpd(vo2 + 2, f13);
  __stfpd(vo1 + 2, f14);
  __stfpd(vo3 + 2, f15);
}

void fftss_r4_bgl_pl_u4_b(const double *vi, double *restrict vo, 
			const double *wn,
			long bsize, long blocks)
{
  double _Complex f0, f1, f2, f3, f4, f5, f6, f7;
  double _Complex f8, f9, f10, f11, f12, f13, f14, f15;
  double _Complex f16, f17, f18, f19, f20, f21;
  const double *vi0, *vi1, *vi2, *vi3;
  double *vo0, *vo1, *vo2, *vo3;
  const double *w1, *w2, *w3;
  long i;
  long n;

#pragma disjoint(*vo0,*vo1,*vo2,*vo3,*vi0,*vi1,*vi2,*vi3,*w1,*w2,*w3)


  n = ((bsize * blocks) >> 2) - 1;

  vo0 = vo;
  vo1 = vo0 + 2 * blocks * bsize;
  vo2 = vo1 + 2 * blocks * bsize;
  vo3 = vo2 + 2 * blocks * bsize;
  vi0 = vi;
  vi1 = vi0 + 2 * bsize;
  vi2 = vi1 + 2 * bsize;
  vi3 = vi2 + 2 * bsize;

  w1 = wn; w2 = wn; w3 = wn + blocks * bsize * 6;
  
  f2 = __lfpd(vi2);
  f17 = __lfpd(w2);
  f3 = __lfpd(vi3);
  f18 = __lfpd(w3);
  f1 = __lfpd(vi1);        
  f16 = __lfpd(w1);              f6 = __fxcxnsma(f2,f2,__creal(f17));

  f0 = __lfpd(vi0);              f7 = __fxcxnsma(f3,f3,__creal(f18));
  f2 = __lfpd(vi2 + 2);          f5 = __fxcxnsma(f1,f1,__creal(f16));
   
  f3 = __lfpd(vi3 + 2);

  f1 = __lfpd(vi1 + 2);
                                 f8 = __fxcxnpma(f0,f6,__cimag(f17));
                                 f10 = __fxcxnsma(f0,f6,__cimag(f17));
			         f9 = __fxcpmadd(f5,f7,__cimag(f18));
			         f11 = __fxcpnmsub(f5,f7,__cimag(f18));
                                 f6 = __fxcxnsma(f2,f2,__creal(f17));

  f0 = __lfpd(vi0 + 2);          f7  = __fxcxnsma(f3,f3,__creal(f18));
  f2 = __lfpd(vi2 + 4);          f5  = __fxcxnsma(f1,f1,__creal(f16));
                                 f12 = __fxcxnpma(f8,f9,__cimag(f16));
  f3 = __lfpd(vi3 + 4);          f13 = __fxcxnsma(f8,f9,__cimag(f16));
                                 f14 = __fxcsnmsub(f10,f11,__cimag(f16));
  f1 = __lfpd(vi1 + 4);          f15 = __fxcsmadd(f10,f11,__cimag(f16));
                                 f8  = __fxcxnpma(f0,f6,__cimag(f17));
  __stfpd(vo0, f12);             f10 = __fxcxnsma(f0,f6,__cimag(f17));
  __stfpd(vo2, f13);             f9  = __fxcpmadd(f5,f7,__cimag(f18));
  __stfpd(vo1, f14);             f11 = __fxcpnmsub(f5,f7,__cimag(f18));
  __stfpd(vo3, f15);             f6  = __fxcxnsma(f2,f2,__creal(f17));

  f0 = __lfpd(vi0 + 4);          f7  = __fxcxnsma(f3,f3,__creal(f18));
  f2 = __lfpd(vi2 + 6);          f5  = __fxcxnsma(f1,f1,__creal(f16));
  f20 = __lfpd(w2);              f12 = __fxcxnpma(f8,f9,__cimag(f16));
  f3 = __lfpd(vi3 + 6);          f13 = __fxcxnsma(f8,f9,__cimag(f16));
  f21 = __lfpd(w3);              f14 = __fxcsnmsub(f10,f11,__cimag(f16));
  f1 = __lfpd(vi1 + 6);          f15 = __fxcsmadd(f10,f11,__cimag(f16));
  f19 = __lfpd(w1);              f8  = __fxcxnpma(f0,f6,__cimag(f17));
  __stfpd(vo0 + 2, f12);         f10 = __fxcxnsma(f0,f6,__cimag(f17));
  __stfpd(vo2 + 2, f13);         f9  = __fxcpmadd(f5,f7,__cimag(f18));
  __stfpd(vo1 + 2, f14);         f11 = __fxcpnmsub(f5,f7,__cimag(f18));
  __stfpd(vo3 + 2, f15);         f6  = __fxcxnsma(f2,f2,__creal(f20));

  vi0 += 8; vi1 += 8; vi2 += 8; vi3 += 8;
  vo0 += 4; vo1 += 4; vo2 += 4; vo3 += 4;

  i = 0;
  
  while (i < n) {
    f0 = __lfpd(vi0 + 8 * i - 2);  f7  = __fxcxnsma(f3,f3,__creal(f21));
    vi0 += 24; 
    vi1 += 24; 
    vi2 += 24;
    vi3 += 24;
    w1 += 8; 
    w2 += 16; 
    w3 += 8;
    f2 = __lfpd(vi2 + 8 * i);      f5  = __fxcxnsma(f1,f1,__creal(f19));
    f17 = __lfpd(w2);              f12 = __fxcxnpma(f8,f9,__cimag(f16));
    f3 = __lfpd(vi3 + 8 * i);      f13 = __fxcxnsma(f8,f9,__cimag(f16));
    f18 = __lfpd(w3);              f14 = __fxcsnmsub(f10,f11,__cimag(f16));
    f1 = __lfpd(vi1 + 8 * i);      f15 = __fxcsmadd(f10,f11,__cimag(f16));
    f16 = __lfpd(w1);              f8  = __fxcxnpma(f0,f6,__cimag(f20));
    __stfpd(vo0 + 8 * i, f12);     f10 = __fxcxnsma(f0,f6,__cimag(f20));
    __stfpd(vo2 + 8 * i, f13);     f9  = __fxcpmadd(f5,f7,__cimag(f21));
    __stfpd(vo1 + 8 * i, f14);     f11 = __fxcpnmsub(f5,f7,__cimag(f21));
    __stfpd(vo3 + 8 * i, f15);     f6  = __fxcxnsma(f2,f2,__creal(f17));

    f0 = __lfpd(vi0 + 8 * i);      f7  = __fxcxnsma(f3,f3,__creal(f18));
    f2 = __lfpd(vi2 + 8 * i + 2);  f5  = __fxcxnsma(f1,f1,__creal(f16));
                                   f12 = __fxcxnpma(f8,f9,__cimag(f19));
    f3 = __lfpd(vi3 + 8 * i + 2);  f13 = __fxcxnsma(f8,f9,__cimag(f19));
                                   f14 = __fxcsnmsub(f10,f11,__cimag(f19));
    f1 = __lfpd(vi1 + 8 * i + 2);  f15 = __fxcsmadd(f10,f11,__cimag(f19));
                                   f8  = __fxcxnpma(f0,f6,__cimag(f17));
    __stfpd(vo0 + 8 * i + 2, f12); f10 = __fxcxnsma(f0,f6,__cimag(f17));
    __stfpd(vo2 + 8 * i + 2, f13); f9  = __fxcpmadd(f5,f7,__cimag(f18));
    __stfpd(vo1 + 8 * i + 2, f14); f11 = __fxcpnmsub(f5,f7,__cimag(f18));
    __stfpd(vo3 + 8 * i + 2, f15); f6  = __fxcxnsma(f2,f2,__creal(f17));

    f0 = __lfpd(vi0 + 8 * i + 2);  f7  = __fxcxnsma(f3,f3,__creal(f18));
    f2 = __lfpd(vi2 + 8 * i + 4);  f5  = __fxcxnsma(f1,f1,__creal(f16));
                                   f12 = __fxcxnpma(f8,f9,__cimag(f16));
    f3 = __lfpd(vi3 + 8 * i + 4);  f13 = __fxcxnsma(f8,f9,__cimag(f16));
                                   f14 = __fxcsnmsub(f10,f11,__cimag(f16));
    f1 = __lfpd(vi1 + 8 * i + 4);  f15 = __fxcsmadd(f10,f11,__cimag(f16));
                                   f8  = __fxcxnpma(f0,f6,__cimag(f17));
    __stfpd(vo0 + 8 * i + 4, f12); f10 = __fxcxnsma(f0,f6,__cimag(f17));
    __stfpd(vo2 + 8 * i + 4, f13); f9  = __fxcpmadd(f5,f7,__cimag(f18));
    __stfpd(vo1 + 8 * i + 4, f14); f11 = __fxcpnmsub(f5,f7,__cimag(f18));
    __stfpd(vo3 + 8 * i + 4, f15); f6  = __fxcxnsma(f2,f2,__creal(f17));

    f0 = __lfpd(vi0 + 8 * i + 4);  f7  = __fxcxnsma(f3,f3,__creal(f18));
    f2 = __lfpd(vi2 + 8 * i + 6);  f5  = __fxcxnsma(f1,f1,__creal(f16));
    f20 = __lfpd(w2);              f12 = __fxcxnpma(f8,f9,__cimag(f16));
    f3 = __lfpd(vi3 + 8 * i + 6);  f13 = __fxcxnsma(f8,f9,__cimag(f16));
    f21 = __lfpd(w3);              f14 = __fxcsnmsub(f10,f11,__cimag(f16));
    f1 = __lfpd(vi1 + 8 * i + 6);  f15 = __fxcsmadd(f10,f11,__cimag(f16));
    f19 = __lfpd(w1);              f8  = __fxcxnpma(f0,f6,__cimag(f17));
    __stfpd(vo0 + 8 * i + 6, f12); f10 = __fxcxnsma(f0,f6,__cimag(f17));
    __stfpd(vo2 + 8 * i + 6, f13); f9  = __fxcpmadd(f5,f7,__cimag(f18));
    __stfpd(vo1 + 8 * i + 6, f14); f11 = __fxcpnmsub(f5,f7,__cimag(f18));
    __stfpd(vo3 + 8 * i + 6, f15); f6  = __fxcxnsma(f2,f2,__creal(f20));
    i++;

  };


  vi0 += 8 * n;
  vo0 += 8 * n; vo1 += 8 * n; vo2 += 8 * n; vo3 += 8 * n;

  w1 += 4 * n; w2 += 8 * n; w3 += 4 * n;
      
  f0 = __lfpd(vi0 - 2);           f7 = __fxcxnsma(f3,f3,__creal(f21));
                                  f5 = __fxcxnsma(f1,f1,__creal(f19));
                                  f12 = __fxcxnpma(f8,f9,__cimag(f16));
                                  f13 = __fxcxnsma(f8,f9,__cimag(f16));
                                  f14 = __fxcsnmsub(f10,f11,__cimag(f16));
                                  f15 = __fxcsmadd(f10,f11,__cimag(f16));
                                  f8 = __fxcxnpma(f0,f6,__cimag(f20));
  __stfpd(vo0, f12);              f10 = __fxcxnsma(f0,f6,__cimag(f20));
  __stfpd(vo2, f13);              f9 = __fxcpmadd(f5,f7,__cimag(f21));
  __stfpd(vo1, f14);              f11 = __fxcpnmsub(f5,f7,__cimag(f21));
  __stfpd(vo3, f15);   

                               
                               
                                  f12 = __fxcxnpma(f8,f9,__cimag(f19));
                                  f13 = __fxcxnsma(f8,f9,__cimag(f19));
                                  f14 = __fxcsnmsub(f10,f11,__cimag(f19));
                                  f15 = __fxcsmadd(f10,f11,__cimag(f19));
   
  __stfpd(vo0 + 2, f12);
  __stfpd(vo2 + 2, f13);
  __stfpd(vo1 + 2, f14);
  __stfpd(vo3 + 2, f15);
}


void fftss_r4_bgl_pl_u1_f(const double *vi, double *restrict vo, 
			 const double *wn,
			 long bsize, long blocks)
{
  double _Complex f0, f1, f2, f3, f4, f5, f6, f7;
  double _Complex f8, f9, f10, f11, f12, f13, f14, f15;
  double _Complex f16, f17, f18, f19, f20, f21;
  double *vo0, *vo1, *vo2, *vo3;
  const double *w1, *w2, *w3;
  long i;
  long n;
  extern void fftss_r4_fma_u1_f(const double *, double *, const double *,
    long, long);

#pragma disjoint(*vo0,*vo1,*vo2,*vo3,*vi,*w1,*w2,*w3)


  if (blocks == 1) {
    fftss_r4_fma_u1_f(vi, vo, wn, bsize, blocks);
    return;
  }

  n = (blocks >> 1) - 1;
  
  vo0 = vo;
  vo1 = vo0 + 2 * blocks;
  vo2 = vo1 + 2 * blocks;
  vo3 = vo2 + 2 * blocks;
  w1 = wn; w2 = wn; w3 = wn + blocks * 6;
  
  f2 = __lfpd(vi + 4);
  f17 = __lfpd(w2); w2 += 4;
  f3 = __lfpd(vi + 6);
  f18 = __lfpd(w3); w3 += 2;
  f1 = __lfpd(vi + 2);        
  f16 = __lfpd(w1); w1 += 2;  f6 = __fxcxnpma(f2,f2,__creal(f17));

  f0 = __lfpd(vi);            f7 = __fxcxnpma(f3,f3,__creal(f18));
  f2 = __lfpd(vi + 12);       f5 = __fxcxnpma(f1,f1,__creal(f16));
  f20 = __lfpd(w2); w2 += 4;  
  f3 = __lfpd(vi + 14);
  f21 = __lfpd(w3); w3 += 2;
  f1 = __lfpd(vi + 10);
  f19 = __lfpd(w1); w1 += 2;  f8 = __fxcxnsma(f0,f6,__cimag(f17));
                              f10 = __fxcxnpma(f0,f6,__cimag(f17));
			      f9 = __fxcpmadd(f5,f7,__cimag(f18));
			      f11 = __fxcpnmsub(f5,f7,__cimag(f18));
                              f6 = __fxcxnpma(f2,f2,__creal(f20));
  vi += 16;

  i = 0;
  while (i < n) {
    f0 = __lfpd(vi + 16 * i - 8); f7 = __fxcxnpma(f3,f3,__creal(f21));
    f2 = __lfpd(vi + 16 * i + 4); f5 = __fxcxnpma(f1,f1,__creal(f19));
    f17 = __lfpd(w2 + 8 * i);     f12 = __fxcxnsma(f8,f9,__cimag(f16));
    f3 = __lfpd(vi + 16 * i + 6); f13 = __fxcxnpma(f8,f9,__cimag(f16));
    f18 = __lfpd(w3 + 4 * i);     f14 = __fxcsnmsub(f10,f11,__cimag(f16));
    f1 = __lfpd(vi + 16 * i + 2); f15 = __fxcsmadd(f10,f11,__cimag(f16));
    f16 = __lfpd(w1 + 4 * i);     f8 = __fxcxnsma(f0,f6,__cimag(f20));
    __stfpd(vo0 + 4 * i, f12);    f10 = __fxcxnpma(f0,f6,__cimag(f20));
    __stfpd(vo2 + 4 * i, f13);    f9 = __fxcpmadd(f5,f7,__cimag(f21));
    __stfpd(vo1 + 4 * i, f14);    f11 = __fxcpnmsub(f5,f7,__cimag(f21));
    __stfpd(vo3 + 4 * i, f15);    f6 = __fxcxnpma(f2,f2,__creal(f17));

    f0 = __lfpd(vi + 16 * i); f7 = __fxcxnpma(f3,f3,__creal(f18));
    f2 = __lfpd(vi + 16 * i + 12); f5 = __fxcxnpma(f1,f1,__creal(f16));
    f20 = __lfpd(w2 + 8 * i + 4);  f12 = __fxcxnsma(f8,f9,__cimag(f19));
    f3 = __lfpd(vi + 16 * i + 14); f13 = __fxcxnpma(f8,f9,__cimag(f19));
    f21 = __lfpd(w3 + 4 * i + 2);  f14 = __fxcsnmsub(f10,f11,__cimag(f19));
    f1 = __lfpd(vi + 16 * i + 10); f15 = __fxcsmadd(f10,f11,__cimag(f19));
    f19 = __lfpd(w1 + 4 * i + 2);  f8 = __fxcxnsma(f0,f6,__cimag(f17));
    __stfpd(vo0 + 4 * i + 2, f12); f10 = __fxcxnpma(f0,f6,__cimag(f17));
    __stfpd(vo2 + 4 * i + 2, f13); f9 = __fxcpmadd(f5,f7,__cimag(f18));
    __stfpd(vo1 + 4 * i + 2, f14); f11 = __fxcpnmsub(f5,f7,__cimag(f18));
    __stfpd(vo3 + 4 * i + 2, f15); f6 = __fxcxnpma(f2,f2,__creal(f20));
    i++;
  }

  vo0 += 4 * n; vo1 += 4 * n; vo2 += 4 * n; vo3 += 4 * n;
  vi += 16 * n;
  w1 += 4 * n; w2 += 8 * n; w3 += 4 * n;
      
    f0 = __lfpd(vi - 8);          f7 = __fxcxnpma(f3,f3,__creal(f21));
                                  f5 = __fxcxnpma(f1,f1,__creal(f19));
                                  f12 = __fxcxnsma(f8,f9,__cimag(f16));
                                  f13 = __fxcxnpma(f8,f9,__cimag(f16));
                                  f14 = __fxcsnmsub(f10,f11,__cimag(f16));
                                  f15 = __fxcsmadd(f10,f11,__cimag(f16));
                                  f8 = __fxcxnsma(f0,f6,__cimag(f20));
    __stfpd(vo0, f12);            f10 = __fxcxnpma(f0,f6,__cimag(f20));
    __stfpd(vo2, f13);            f9 = __fxcpmadd(f5,f7,__cimag(f21));
    __stfpd(vo1, f14);            f11 = __fxcpnmsub(f5,f7,__cimag(f21));
    __stfpd(vo3, f15);   

                               
                               
                                  f12 = __fxcxnsma(f8,f9,__cimag(f19));
                                  f13 = __fxcxnpma(f8,f9,__cimag(f19));
                                  f14 = __fxcsnmsub(f10,f11,__cimag(f19));
                                  f15 = __fxcsmadd(f10,f11,__cimag(f19));
   
    __stfpd(vo0 + 2, f12);
    __stfpd(vo2 + 2, f13);
    __stfpd(vo1 + 2, f14);
    __stfpd(vo3 + 2, f15);
}

void fftss_r4_bgl_pl_u1_b(const double *vi, double *restrict vo, 
			 const double *wn,
			 long bsize, long blocks)
{
  double _Complex f0, f1, f2, f3, f4, f5, f6, f7;
  double _Complex f8, f9, f10, f11, f12, f13, f14, f15;
  double _Complex f16, f17, f18, f19, f20, f21;
  double *vo0, *vo1, *vo2, *vo3;
  const double *w1, *w2, *w3;
  long i;
  long n;
  extern void fftss_r4_fma_u1_b(const double *, double *, const double *,
    long, long);

#pragma disjoint(*vo0,*vo1,*vo2,*vo3,*vi,*w1,*w2,*w3)


  if (blocks == 1) {
    fftss_r4_fma_u1_b(vi, vo, wn, bsize, blocks);
    return;
  }

  n = (blocks >> 1) - 1;
  
  vo0 = vo;
  vo1 = vo0 + 2 * blocks;
  vo2 = vo1 + 2 * blocks;
  vo3 = vo2 + 2 * blocks;
  w1 = wn; w2 = wn; w3 = wn + blocks * 6;
  
  f2 = __lfpd(vi + 4);
  f17 = __lfpd(w2); w2 += 4;
  f3 = __lfpd(vi + 6);
  f18 = __lfpd(w3); w3 += 2;
  f1 = __lfpd(vi + 2);        
  f16 = __lfpd(w1); w1 += 2;  f6 = __fxcxnsma(f2,f2,__creal(f17));

  f0 = __lfpd(vi);            f7 = __fxcxnsma(f3,f3,__creal(f18));
  f2 = __lfpd(vi + 12);       f5 = __fxcxnsma(f1,f1,__creal(f16));
  f20 = __lfpd(w2); w2 += 4;  
  f3 = __lfpd(vi + 14);
  f21 = __lfpd(w3); w3 += 2;
  f1 = __lfpd(vi + 10);
  f19 = __lfpd(w1); w1 += 2;  f8 = __fxcxnpma(f0,f6,__cimag(f17));
                              f10 = __fxcxnsma(f0,f6,__cimag(f17));
			      f9 = __fxcpmadd(f5,f7,__cimag(f18));
			      f11 = __fxcpnmsub(f5,f7,__cimag(f18));
                              f6 = __fxcxnsma(f2,f2,__creal(f20));
  vi += 16;

  i = 0;
  while (i < n) {
    f0 = __lfpd(vi + 16 * i - 8); f7 = __fxcxnsma(f3,f3,__creal(f21));
    f2 = __lfpd(vi + 16 * i + 4); f5 = __fxcxnsma(f1,f1,__creal(f19));
    f17 = __lfpd(w2 + 8 * i);     f12 = __fxcxnpma(f8,f9,__cimag(f16));
    f3 = __lfpd(vi + 16 * i + 6); f13 = __fxcxnsma(f8,f9,__cimag(f16));
    f18 = __lfpd(w3 + 4 * i);     f14 = __fxcsnmsub(f10,f11,__cimag(f16));
    f1 = __lfpd(vi + 16 * i + 2); f15 = __fxcsmadd(f10,f11,__cimag(f16));
    f16 = __lfpd(w1 + 4 * i);     f8 = __fxcxnpma(f0,f6,__cimag(f20));
    __stfpd(vo0 + 4 * i, f12);    f10 = __fxcxnsma(f0,f6,__cimag(f20));
    __stfpd(vo2 + 4 * i, f13);    f9 = __fxcpmadd(f5,f7,__cimag(f21));
    __stfpd(vo1 + 4 * i, f14);    f11 = __fxcpnmsub(f5,f7,__cimag(f21));
    __stfpd(vo3 + 4 * i, f15);    f6 = __fxcxnsma(f2,f2,__creal(f17));

    f0 = __lfpd(vi + 16 * i); f7 = __fxcxnsma(f3,f3,__creal(f18));
    f2 = __lfpd(vi + 16 * i + 12); f5 = __fxcxnsma(f1,f1,__creal(f16));
    f20 = __lfpd(w2 + 8 * i + 4);  f12 = __fxcxnpma(f8,f9,__cimag(f19));
    f3 = __lfpd(vi + 16 * i + 14); f13 = __fxcxnsma(f8,f9,__cimag(f19));
    f21 = __lfpd(w3 + 4 * i + 2);  f14 = __fxcsnmsub(f10,f11,__cimag(f19));
    f1 = __lfpd(vi + 16 * i + 10); f15 = __fxcsmadd(f10,f11,__cimag(f19));
    f19 = __lfpd(w1 + 4 * i + 2);  f8 = __fxcxnpma(f0,f6,__cimag(f17));
    __stfpd(vo0 + 4 * i + 2, f12); f10 = __fxcxnsma(f0,f6,__cimag(f17));
    __stfpd(vo2 + 4 * i + 2, f13); f9 = __fxcpmadd(f5,f7,__cimag(f18));
    __stfpd(vo1 + 4 * i + 2, f14); f11 = __fxcpnmsub(f5,f7,__cimag(f18));
    __stfpd(vo3 + 4 * i + 2, f15); f6 = __fxcxnsma(f2,f2,__creal(f20));
    i++;
  };
  vo0 += 4 * n; vo1 += 4 * n; vo2 += 4 * n; vo3 += 4 * n;
  vi += 16 * n;
  w1 += 4 * n; w2 += 8 * n; w3 += 4 * n;
      
    f0 = __lfpd(vi - 8);          f7 = __fxcxnsma(f3,f3,__creal(f21));
                                  f5 = __fxcxnsma(f1,f1,__creal(f19));
                                  f12 = __fxcxnpma(f8,f9,__cimag(f16));
                                  f13 = __fxcxnsma(f8,f9,__cimag(f16));
                                  f14 = __fxcsnmsub(f10,f11,__cimag(f16));
                                  f15 = __fxcsmadd(f10,f11,__cimag(f16));
                                  f8 = __fxcxnpma(f0,f6,__cimag(f20));
    __stfpd(vo0, f12);            f10 = __fxcxnsma(f0,f6,__cimag(f20));
    __stfpd(vo2, f13);            f9 = __fxcpmadd(f5,f7,__cimag(f21));
    __stfpd(vo1, f14);            f11 = __fxcpnmsub(f5,f7,__cimag(f21));
    __stfpd(vo3, f15);   

                               
                               
                                  f12 = __fxcxnpma(f8,f9,__cimag(f19));
                                  f13 = __fxcxnsma(f8,f9,__cimag(f19));
                                  f14 = __fxcsnmsub(f10,f11,__cimag(f19));
                                  f15 = __fxcsmadd(f10,f11,__cimag(f19));
   
    __stfpd(vo0 + 2, f12);
    __stfpd(vo2 + 2, f13);
    __stfpd(vo1 + 2, f14);
    __stfpd(vo3 + 2, f15);
}

