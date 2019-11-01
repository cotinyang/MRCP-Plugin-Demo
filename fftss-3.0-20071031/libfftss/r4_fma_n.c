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

static inline void kern_r4_fma_n_f(double *restrict o0,
				    double *restrict o1,
				    double *restrict o2,
				    double *restrict o3,
				    double *i0, 
				    double *i1, 
				    double *i2, 
				    double *i3, 
				    double *w,
				    double *w2,
				    long bsize, long blocks)
{
  long i, j;

  for (i = 0; i < blocks; i++) {
    double wr1, wi1;
    double wr2, wi2;
    double wr3, wi13;
    
    wr1 = w[2 * i * bsize]; wi1 = w[2 * i * bsize + 1];
    wr2 = w[4 * i * bsize]; wi2 = w[4 * i * bsize + 1];
    wr3 = w2[2 * i * bsize]; wi13 = w2[2 * i * bsize + 1];
    
    for (j = 0; j < bsize; j++) {
      double pr1, pi1;
      double pr2, pi2;
      double pr3, pi3;
      double qr0, qi0;
      double qr1, qi1;
      double qr2, qi2;
      double qr3, qi3;
      
      pr1 = i1[2 * j] * wr1 + i1[2 * j + 1];
      pi1 = i1[2 * j + 1] * wr1 - i1[2 * j];
      pr2 = i2[2 * j] * wr2 + i2[2 * j + 1];
      pi2 = i2[2 * j + 1] * wr2 - i2[2 * j];
      pr3 = i3[2 * j] * wr3 + i3[2 * j + 1];
      pi3 = i3[2 * j + 1] * wr3 - i3[2 * j];
      
      qr0 = i0[2 * j] + pr2 * wi2; qr2 = i0[2 * j] - pr2 * wi2;
      qi0 = i0[2 * j + 1] + pi2 * wi2; qi2 = i0[2 * j + 1] - pi2 * wi2; 
      qr1 = pr1 + pr3 * wi13; qr3 = pr1 - pr3 * wi13;
      qi1 = pi1 + pi3 * wi13; qi3 = pi1 - pi3 * wi13;
      
      o0[2 * j] = qr0 + qr1 * wi1; o0[2 * j + 1] = qi0 + qi1 * wi1;
      o2[2 * j] = qr0 - qr1 * wi1; o2[2 * j + 1] = qi0 - qi1 * wi1;
      o1[2 * j] = qr2 + qi3 * wi1; o1[2 * j + 1] = qi2 - qr3 * wi1;
      o3[2 * j] = qr2 - qi3 * wi1; o3[2 * j + 1] = qi2 + qr3 * wi1;
    }
    i0 += 8 * bsize; i1 += 8 * bsize;
    i2 += 8 * bsize; i3 += 8 * bsize;
    o0 += 2 * bsize; o1 += 2 * bsize;
    o2 += 2 * bsize; o3 += 2 * bsize;
  }
}

static inline void kern_r4_fma_n_b(double *restrict o0,
				    double *restrict o1,
				    double *restrict o2,
				    double *restrict o3,
				    double *i0, 
				    double *i1, 
				    double *i2, 
				    double *i3, 
				    double *w,
				    double *w2,
				    long bsize, long blocks)
{
  long i, j;

  for (i = 0; i < blocks; i++) {
    double wr1, wi1;
    double wr2, wi2;
    double wr3, wi13;
    
    wr1 = w[2 * i * bsize]; wi1 = w[2 * i * bsize + 1];
    wr2 = w[4 * i * bsize]; wi2 = w[4 * i * bsize + 1];
    wr3 = w2[2 * i * bsize]; wi13 = w2[2 * i * bsize + 1];
    
    for (j = 0; j < bsize; j++) {
      double pr1, pi1;
      double pr2, pi2;
      double pr3, pi3;
      double qr0, qi0;
      double qr1, qi1;
      double qr2, qi2;
      double qr3, qi3;
      
      pr1 = i1[2 * j] * wr1 - i1[2 * j + 1];
      pi1 = i1[2 * j] + i1[2 * j + 1] * wr1;
      pr2 = i2[2 * j] * wr2 - i2[2 * j + 1];
      pi2 = i2[2 * j] + i2[2 * j + 1] * wr2;
      pr3 = i3[2 * j] * wr3 - i3[2 * j + 1];
      pi3 = i3[2 * j] + i3[2 * j + 1] * wr3;
      
      qr0 = i0[2 * j] + pr2 * wi2; qr2 = i0[2 * j] - pr2 * wi2;
      qi0 = i0[2 * j + 1] + pi2 * wi2; qi2 = i0[2 * j + 1] - pi2 * wi2; 
      qr1 = pr1 + pr3 * wi13; qr3 = pr1 - pr3 * wi13;
      qi1 = pi1 + pi3 * wi13; qi3 = pi1 - pi3 * wi13;
      
      o0[2 * j] = qr0 + qr1 * wi1; o0[2 * j + 1] = qi0 + qi1 * wi1;
      o2[2 * j] = qr0 - qr1 * wi1; o2[2 * j + 1] = qi0 - qi1 * wi1;
      o1[2 * j] = qr2 - qi3 * wi1; o1[2 * j + 1] = qi2 + qr3 * wi1;
      o3[2 * j] = qr2 + qi3 * wi1; o3[2 * j + 1] = qi2 - qr3 * wi1;
    }
    i0 += 8 * bsize; i1 += 8 * bsize;
    i2 += 8 * bsize; i3 += 8 * bsize;
    o0 += 2 * bsize; o1 += 2 * bsize;
    o2 += 2 * bsize; o3 += 2 * bsize;
  }
}

void fftss_r4_fma_n_f(double *in, double *out, double *w,
			 long bsize, long blocks)
{
  kern_r4_fma_n_f(out, out + bsize * blocks * 2, 
			   out + bsize * blocks * 4, out + bsize * blocks * 6,
			   in, in + bsize * 2,
			   in + bsize * 4, in + bsize * 6,
			   w, w + bsize * blocks * 6, bsize, blocks);
}

void fftss_r4_fma_n_b(double *in, double *out, double *w,
		     long bsize, long blocks)
{
  kern_r4_fma_n_b(out, out + bsize * blocks * 2, 
			   out + bsize * blocks * 4, out + bsize * blocks * 6,
			   in, in + bsize * 2,
			   in + bsize * 4, in + bsize * 6,
			   w, w + bsize * blocks * 6, bsize, blocks);
}
		       
