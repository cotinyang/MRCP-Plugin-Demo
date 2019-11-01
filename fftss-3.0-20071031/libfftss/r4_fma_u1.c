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

static inline void kern_r4_fma_u1_f(double *restrict o0,
				    double *restrict o1,
				    double *restrict o2,
				    double *restrict o3,
				    const double *i0, 
				    const double *w,
				    const double *w2,
				    long bsize, long blocks)
{
  long i;

  for (i = 0; i < blocks; i++) {
    double wr1, wi1;
    double wr2, wi2;
    double wr3, wi13;
    double pr1, pi1;
    double pr2, pi2;
    double pr3, pi3;
    double qr0, qi0;
    double qr1, qi1;
    double qr2, qi2;
    double qr3, qi3;

    wr1 = w[2 * i]; wi1 = w[2 * i + 1];
    pr1 = i0[8 * i + 2] * wr1 + i0[8 * i + 3];
    pi1 = i0[8 * i + 3] * wr1 - i0[8 * i + 2];

    wr3 = w2[2 * i]; wi13 = w2[2 * i + 1];
    pr3 = i0[8 * i + 6] * wr3 + i0[8 * i + 7];
    pi3 = i0[8 * i + 7] * wr3 - i0[8 * i + 6];

    wr2 = w[4 * i]; wi2 = w[4 * i + 1];
    pr2 = i0[8 * i + 4] * wr2 + i0[8 * i + 5];
    pi2 = i0[8 * i + 5] * wr2 - i0[8 * i + 4];

    qr1 = pr1 + pr3 * wi13; qr3 = pr1 - pr3 * wi13;
    qi1 = pi1 + pi3 * wi13; qi3 = pi1 - pi3 * wi13;

    qr0 = i0[8 * i] + pr2 * wi2; qr2 = i0[8 * i] - pr2 * wi2;
    qi0 = i0[8 * i + 1] + pi2 * wi2; qi2 = i0[8 * i + 1] - pi2 * wi2; 

    o0[2 * i] = qr0 + qr1 * wi1; o0[2 * i + 1] = qi0 + qi1 * wi1;
    o2[2 * i] = qr0 - qr1 * wi1; o2[2 * i + 1] = qi0 - qi1 * wi1;
    o1[2 * i] = qr2 + qi3 * wi1; o1[2 * i + 1] = qi2 - qr3 * wi1;
    o3[2 * i] = qr2 - qi3 * wi1; o3[2 * i + 1] = qi2 + qr3 * wi1;
  }
}

static inline void kern_r4_fma_u1_b(double *restrict o0,
				    double *restrict o1,
				    double *restrict o2,
				    double *restrict o3,
				    const double *i0, 
				    const double *w,
				    const double *w2,
				    long bsize, long blocks)
{
  long i;

  for (i = 0; i < blocks; i++) {
    double wr1, wi1;
    double wr2, wi2;
    double wr3, wi13;
    double pr1, pi1;
    double pr2, pi2;
    double pr3, pi3;
    double qr0, qi0;
    double qr1, qi1;
    double qr2, qi2;
    double qr3, qi3;

    wr1 = w[2 * i]; wi1 = w[2 * i + 1];
    wr2 = w[4 * i]; wi2 = w[4 * i + 1];
    wr3 = w2[2 * i]; wi13 = w2[2 * i + 1];
      
    pr1 = i0[8 * i + 2] * wr1 - i0[8 * i + 3];
    pi1 = i0[8 * i + 2] + i0[8 * i + 3] * wr1;
    pr2 = i0[8 * i + 4] * wr2 - i0[8 * i + 5];
    pi2 = i0[8 * i + 4] + i0[8 * i + 5] * wr2;
    pr3 = i0[8 * i + 6] * wr3 - i0[8 * i + 7];
    pi3 = i0[8 * i + 6] + i0[8 * i + 7] * wr3;
      
    qr0 = i0[8 * i] + pr2 * wi2; qr2 = i0[8 * i] - pr2 * wi2;
    qi0 = i0[8 * i + 1] + pi2 * wi2; qi2 = i0[8 * i + 1] - pi2 * wi2; 
    qr1 = pr1 + pr3 * wi13; qr3 = pr1 - pr3 * wi13;
    qi1 = pi1 + pi3 * wi13; qi3 = pi1 - pi3 * wi13;

    o0[2 * i] = qr0 + qr1 * wi1; o0[2 * i + 1] = qi0 + qi1 * wi1;
    o2[2 * i] = qr0 - qr1 * wi1; o2[2 * i + 1] = qi0 - qi1 * wi1;
    o1[2 * i] = qr2 - qi3 * wi1; o1[2 * i + 1] = qi2 + qr3 * wi1;
    o3[2 * i] = qr2 + qi3 * wi1; o3[2 * i + 1] = qi2 - qr3 * wi1;
  }
}

void fftss_r4_fma_u1_f(double *in, double *out, double *w,
		      long bsize, long blocks)
{
  kern_r4_fma_u1_f(out, 
		   out + blocks * 2, 
		   out + blocks * 4, 
		   out + blocks * 6,
		   in, 
		   w, w + blocks * 6, 1, blocks);
}

void fftss_r4_fma_u1_b(double *in, double *out, double *w,
		      long bsize, long blocks)
{
  kern_r4_fma_u1_b(out, 
		   out + blocks * 2, 
		   out + blocks * 4,
		   out + blocks * 6,
		   in, 
		   w, w + blocks * 6, 1, blocks);
}
		       
