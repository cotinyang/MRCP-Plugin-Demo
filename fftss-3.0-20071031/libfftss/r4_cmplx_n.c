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

#if defined(HAVE_COMPLEX_H)
#include <complex.h>

static inline void 
kern_r4_cmplx_n_f(double complex *restrict o0,
	      double complex *restrict o1,
	      double complex *restrict o2,
	      double complex *restrict o3,
	      double complex *i0, 
	      double complex *i1, 
	      double complex *i2, 
	      double complex *i3, 
	      double complex *w,
	      long bsize, long blocks)
{
  long i, j;

  for (i = 0; i < blocks; i++) {
    double complex w1, w2, w3;

    w1 = w[i * bsize];
    w2 = w[2 * i * bsize];
    w3 = w[3 * i * bsize];
    
    for (j = 0; j < bsize; j++) {
      double complex p1, p2, p3;
      double complex q0, q1, q2, q3;
      
      p1 = creal(i1[j]) * creal(w1) + cimag(i1[j]) * cimag(w1)
	+ (cimag(i1[j]) * creal(w1) - creal(i1[j]) * cimag(w1)) * I;
      p2 = creal(i2[j]) * creal(w2) + cimag(i2[j]) * cimag(w2)
	+ (cimag(i2[j]) * creal(w2) - creal(i2[j]) * cimag(w2)) * I;
      p3 = creal(i3[j]) * creal(w3) + cimag(i3[j]) * cimag(w3)
	+ (cimag(i3[j]) * creal(w3) - creal(i3[j]) * cimag(w3)) * I;

      q0 = i0[j] + p2;
      q2 = i0[j] - p2;
      q1 = p1 + p3;
      q3 = -I * (p1 - p3);
            
      o0[j] = q0 + q1;
      o2[j] = q0 - q1;
      o1[j] = q2 + q3;
      o3[j] = q2 - q3;
    }
    i0 += 4 * bsize; i1 += 4 * bsize;
    i2 += 4 * bsize; i3 += 4 * bsize;
    o0 += bsize; o1 += bsize;
    o2 += bsize; o3 += bsize;
  }
}

static inline void 
kern_r4_cmplx_n_b(double complex *restrict o0,
	   double complex *restrict o1,
	   double complex *restrict o2,
	   double complex *restrict o3,
	   double complex *i0, 
	   double complex *i1, 
	   double complex *i2, 
	   double complex *i3, 
	   double complex *w,
	   long bsize, long blocks)
{
  long i, j;

  for (i = 0; i < blocks; i++) {
    double complex w1, w2, w3;
    
    w1 = w[i * bsize];
    w2 = w[2 * i * bsize];
    w3 = w[3 * i * bsize];
    
    for (j = 0; j < bsize; j++) {
      double complex p1, p2, p3;
      double complex q0, q1, q2, q3;
      
      p1 = i1[j] * w1;
      p2 = i2[j] * w2;
      p3 = i3[j] * w3;

      q0 = i0[j] + p2;
      q2 = i0[j] - p2;
      q1 = p1 + p3;
      q3 = I * (p1 - p3);
      
      o0[j] = q0 + q1; 
      o2[j] = q0 - q1; 
      o1[j] = q2 + q3; 
      o3[j] = q2 - q3; 
    }
    i0 += 4 * bsize; i1 += 4 * bsize;
    i2 += 4 * bsize; i3 += 4 * bsize;
    o0 += bsize; o1 += bsize;
    o2 += bsize; o3 += bsize;
  }
}

void fftss_r4_cmplx_n_f(double *in, double *out, double *w,
		      long bsize, long blocks)
{
  kern_r4_cmplx_n_f((double complex *)out, 
		(double complex *)out + bsize * blocks, 
		(double complex *)out + bsize * blocks * 2,
		(double complex *)out + bsize * blocks * 3,
		(double complex *)in, 
		(double complex *)in + bsize,
		(double complex *)in + bsize * 2,
		(double complex *)in + bsize * 3,
		(double complex *)w, bsize, blocks);
}

void fftss_r4_cmplx_n_b(double *in, double *out, double *w,
		   long bsize, long blocks)
{
  kern_r4_cmplx_n_b((double complex *)out, 
		(double complex *)out + bsize * blocks, 
		(double complex *)out + bsize * blocks * 2, 
		(double complex *)out + bsize * blocks * 3,
		(double complex *)in, 
		(double complex *)in + bsize,
		(double complex *)in + bsize * 2, 
		(double complex *)in + bsize * 3,
		(double complex *)w, bsize, blocks);
}

#endif
