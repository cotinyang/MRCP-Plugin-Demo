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

#if defined(BlueGene)
#ifdef USE_WRAP440D
#include "wrap440d.h"
#endif

static inline void 
kern_r4_bgl_n_f(double *restrict o0,
		double *restrict o1,
		double *restrict o2,
		double *restrict o3,
		double *i0, 
		double *i1, 
		double *i2, 
		double *i3, 
		double *wn,
		double *wn2,
		long bsize, long blocks)
{
  long i, j;

#pragma disjoint(*o0,*o1,*o2,*o3,*i0,*i1,*i2,*i3,*wn,*wn2)

  __alignx(16,o0);
  __alignx(16,o1);
  __alignx(16,o2);
  __alignx(16,o3);
  __alignx(16,i0);
  __alignx(16,i1);
  __alignx(16,i2);
  __alignx(16,i3);
  __alignx(16,wn);
  __alignx(16,wn2);



  for (i = 0; i < blocks; i++) {
    double _Complex w1, w2, w3;

    w2 = __lfpd(wn + i * bsize * 4);
    w1 = __lfpd(wn + i * bsize * 2);
    w3 = __lfpd(wn2 + i * bsize * 2);
    
    for (j = 0; j < bsize; j++) {
      double _Complex p0, p1, p2, p3;
      double _Complex q0, q1, q2, q3;

      p2 = __lfpd(i2 + 2 * j);
      p1 = __lfpd(i1 + 2 * j);
      p3 = __lfpd(i3 + 2 * j);
      p0 = __lfpd(i0 + 2 * j);

      p2 = __fxcxnpma(p2, p2, __creal(w2)); /* [-pi2,pr2] */
      p1 = __fxcxnpma(p1, p1, __creal(w1)); /* [-pi1,pr1] */
      p3 = __fxcxnpma(p3, p3, __creal(w3));         /* [-pi3,pr3] */
      

      q0 = __fxcxnsma(p0, p2, __cimag(w2));  /* [qr0,qi0] */
      q2 = __fxcxnpma(p0, p2, __cimag(w2));  /* [qr2,qi2] */
      q1 = __fxcpmadd(p1, p3, __cimag(w3));         /* [-qi1,qr1] */
      q3 = __fxcpnmsub(p1, p3, __cimag(w3));        /* [-qi3,qr3] */



      __stfpd(o0 + 2 * j, __fxcxnsma(q0, q1, __cimag(w1)));
      __stfpd(o2 + 2 * j, __fxcxnpma(q0, q1, __cimag(w1)));
      __stfpd(o1 + 2 * j, __fxcsnmsub(q2, q3, __cimag(w1)));
      __stfpd(o3 + 2 * j, __fxcsmadd(q2, q3, __cimag(w1)));
      /*    44flops/26cycle */
    }
    i0 += 8 * bsize; i1 += 8 * bsize;
    i2 += 8 * bsize; i3 += 8 * bsize;
    o0 += 2 * bsize; o1 += 2 * bsize;
    o2 += 2 * bsize; o3 += 2 * bsize;
  }
}

static inline void 
kern_r4_bgl_n_b(double *restrict o0,
		 double *restrict o1,
		 double *restrict o2,
		 double *restrict o3,
		 double *i0, 
		 double *i1, 
		 double *i2, 
		 double *i3, 
		 double *wn,
		 double *wn2,
		 long bsize, long blocks)
{
  long i, j;

  for (i = 0; i < blocks; i++) {
    double _Complex w1, w2, w3;

    w2 = __lfpd(wn + 4 * i * bsize);
    w1 = __lfpd(wn + 2 * i * bsize);
    w3 = __lfpd(wn2 + 2 * i * bsize);

    for (j = 0; j < bsize; j++) {
      double _Complex p0, p1, p2, p3;
      double _Complex q0, q1, q2, q3;

      p2 = __lfpd(i2 + 2 * j);
      p1 = __lfpd(i1 + 2 * j);
      p3 = __lfpd(i3 + 2 * j);
      p0 = __lfpd(i0 + 2 * j);
      
      p2 = __fxcxnsma(p2, p2, __creal(w2));   /* [pi2,-pr2] */
      p1 = __fxcxnsma(p1, p1, __creal(w1));   /* [pi1,-pr1] */
      p3 = __fxcxnsma(p3, p3, __creal(w3));           /* [pi3,-pr3] */

      q0 = __fxcxnpma(p0, p2, __cimag(w2));   /* [qr0,qi0] */
      q2 = __fxcxnsma(p0, p2, __cimag(w2));   /* [qr2,qi2] */
      q1 = __fxcpmadd(p1, p3, __cimag(w3));          /* [qi1,-qr1] */
      q3 = __fxcpnmsub(p1, p3, __cimag(w3));         /* [qi3,-qr3] */
      
      __stfpd(o0 + 2 * j, __fxcxnpma(q0, q1, __cimag(w1)));
      __stfpd(o2 + 2 * j, __fxcxnsma(q0, q1, __cimag(w1)));
      __stfpd(o1 + 2 * j, __fxcsnmsub(q2, q3, __cimag(w1)));
      __stfpd(o3 + 2 * j, __fxcsmadd(q2, q3, __cimag(w1)));

    }
    i0 += 8 * bsize; i1 += 8 * bsize;
    i2 += 8 * bsize; i3 += 8 * bsize;
    o0 += 2 * bsize; o1 += 2 * bsize;
    o2 += 2 * bsize; o3 += 2 * bsize;
  }
}

void fftss_r4_bgl_n_f(double *in, double *out, double *w,
			 long bsize, long blocks)
{
  kern_r4_bgl_n_f(out, out + bsize * blocks * 2, 
		   out + bsize * blocks * 4, out + bsize * blocks * 6,
		   in, in + bsize * 2,
		   in + bsize * 4, in + bsize * 6,
		   w, w + bsize * blocks * 6, bsize, blocks);
}

void fftss_r4_bgl_n_b(double *in, double *out, double *w,
		     long bsize, long blocks)
{
  kern_r4_bgl_n_b(out, out + bsize * blocks * 2, 
		   out + bsize * blocks * 4, out + bsize * blocks * 6,
		   in, in + bsize * 2,
		   in + bsize * 4, in + bsize * 6,
		   w, w + bsize * blocks * 6, bsize, blocks);
}
		       
#endif
