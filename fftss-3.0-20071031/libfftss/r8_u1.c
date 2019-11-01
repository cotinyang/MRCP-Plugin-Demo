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

static inline void kern_r8_u1_f(double *restrict o0,
				double *restrict o1,
				double *restrict o2,
				double *restrict o3,
				double *restrict o4,
				double *restrict o5,
				double *restrict o6,
				double *restrict o7,
				double *i0, 
				double *i1, 
				double *i2, 
				double *i3, 
				double *i4, 
				double *i5, 
				double *i6, 
				double *i7, 
				double *w,
				long bsize, long blocks)
{
  long j;

  for (j = 0; j < blocks; j++) {
    double wr1, wi1;
    double wr2, wi2;
    double wr3, wi3;
    double wr4, wi4;
    double wr5, wi5;
    double wr6, wi6;
    double wr7, wi7;

    double pr2, pi2;
    double pr4, pi4;
    double pr5, pi5;
    double pr6, pi6;
    double pr7, pi7;

    double sr1, si1;
    double sr3, si3;
    double sr5, si5;
    double sr7, si7;
      
    double qr0, qi0;
    double qr1, qi1;
    double qr2, qi2;
    double qr3, qi3;
    double qr4, qi4;
    double qr5, qi5;
    double qr6, qi6;
    double qr7, qi7;
      
    double tr0, ti0;
    double tr1, ti1;
    double tr2, ti2;
    double tr3, ti3;
    double tr4, ti4;
    double tr5, ti5;
    double tr6, ti6;
    double tr7, ti7;
      
    wr4 = w[8 * j]; wi4 = w[8 * j + 1];
    wr2 = w[4 * j]; wi2 = w[4 * j + 1];
    wr6 = w[12 * j]; wi6 = w[12 * j + 1];
    wr1 = w[2 * j]; wi1 = w[2 * j + 1];
    wr3 = w[6 * j]; wi3 = w[6 * j + 1];

    wr5 = w[2 * j + 2 * blocks]; 
    wi5 = w[2 * j + 2 * blocks + 1];
    wr7 = w[6 * j + 6 * blocks]; 
    wi7 = w[6 * j + 6 * blocks + 1];

    pr5 = i5[16 * j] * wr4 + i5[16 * j + 1] * wi4;
    pi5 = i5[16 * j + 1] * wr4 - i5[16 * j] * wi4;
    pr7 = i7[16 * j] * wr4 + i7[16 * j + 1] * wi4;
    pi7 = i7[16 * j + 1] * wr4 - i7[16 * j] * wi4;
    pr2 = i2[16 * j] * wr2 + i2[16 * j + 1] * wi2;
    pi2 = i2[16 * j + 1] * wr2 - i2[16 * j] * wi2;
    pr4 = i4[16 * j] * wr4 + i4[16 * j + 1] * wi4;
    pi4 = i4[16 * j + 1] * wr4 - i4[16 * j] * wi4;
    pr6 = i6[16 * j] * wr6 + i6[16 * j + 1] * wi6;
    pi6 = i6[16 * j + 1] * wr6 - i6[16 * j] * wi6;
    
    sr1 = i1[16 * j] + pr5; sr5 = i1[16 * j] - pr5;
    si1 = i1[16 * j + 1] + pi5; si5 = i1[16 * j + 1] - pi5; 
    sr3 = i3[16 * j] + pr7; sr7 = i3[16 * j] - pr7;
    si3 = i3[16 * j + 1] + pi7; si7 = i3[16 * j + 1] - pi7; 

    qr0 = i0[16 * j] + pr4; qr4 = i0[16 * j] - pr4;
    qi0 = i0[16 * j + 1] + pi4; qi4 = i0[16 * j + 1] - pi4; 
    qr2 = pr2 + pr6; qr6 = pr2 - pr6;
    qi2 = pi2 + pi6; qi6 = pi2 - pi6;
      
    qr1 = sr1 * wr1 + si1 * wi1; qi1 = si1 * wr1 - sr1 * wi1;
    qr3 = sr3 * wr3 + si3 * wi3; qi3 = si3 * wr3 - sr3 * wi3;
    qr5 = sr5 * wr5 + si5 * wi5; qi5 = si5 * wr5 - sr5 * wi5;
    qr7 = sr7 * wr7 + si7 * wi7; qi7 = si7 * wr7 - sr7 * wi7;
    
    tr0 = qr0 + qr2; ti0 = qi0 + qi2;
    tr2 = qr0 - qr2; ti2 = qi0 - qi2;
    tr4 = qr4 + qi6; ti4 = qi4 - qr6;
    tr6 = qr4 - qi6; ti6 = qi4 + qr6;
    
    tr1 = qr1 + qr3; ti1 = qi1 + qi3;
    tr3 = qr1 - qr3; ti3 = qi1 - qi3;
    tr5 = qr5 + qr7; ti5 = qi5 + qi7;
    tr7 = qr5 - qr7; ti7 = qi5 - qi7;

    o0[2 * j] = tr0 + tr1; o0[2 * j + 1] = ti0 + ti1;
    o4[2 * j] = tr0 - tr1; o4[2 * j + 1] = ti0 - ti1;
    o1[2 * j] = tr4 + tr5; o1[2 * j + 1] = ti4 + ti5;
    o5[2 * j] = tr4 - tr5; o5[2 * j + 1] = ti4 - ti5;
    o2[2 * j] = tr2 + ti3; o2[2 * j + 1] = ti2 - tr3;
    o6[2 * j] = tr2 - ti3; o6[2 * j + 1] = ti2 + tr3;
    o3[2 * j] = tr6 + ti7; o3[2 * j + 1] = ti6 - tr7;
    o7[2 * j] = tr6 - ti7; o7[2 * j + 1] = ti6 + tr7;

  }
}

static inline void kern_r8_u1_b(double *restrict o0,
				double *restrict o1,
				double *restrict o2,
				double *restrict o3,
				double *restrict o4,
				double *restrict o5,
				double *restrict o6,
				double *restrict o7,
				double *i0, 
				double *i1, 
				double *i2, 
				double *i3, 
				double *i4, 
				double *i5, 
				double *i6, 
				double *i7, 
				double *w,
				long bsize, long blocks)
{
  long i, j;

  for (i = 0; i < blocks; i++) {
    double wr1, wi1;
    double wr2, wi2;
    double wr3, wi3;
    double wr4, wi4;
    double wr5, wi5;
    double wr6, wi6;
    double wr7, wi7;

    wr4 = w[8 * i * bsize]; wi4 = w[8 * i * bsize + 1];
    wr2 = w[4 * i * bsize]; wi2 = w[4 * i * bsize + 1];
    wr6 = w[12 * i * bsize]; wi6 = w[12 * i * bsize + 1];
    wr1 = w[2 * i * bsize]; wi1 = w[2 * i * bsize + 1];
    wr3 = w[6 * i * bsize]; wi3 = w[6 * i * bsize + 1];

    wr5 = w[2 * i * bsize + 2 * blocks * bsize]; 
    wi5 = w[2 * i * bsize + 2 * blocks * bsize + 1];
    wr7 = w[6 * i * bsize + 6 * blocks * bsize]; 
    wi7 = w[6 * i * bsize + 6 * blocks * bsize + 1];
    
    for (j = 0; j < bsize; j++) {
      double pr2, pi2;
      double pr4, pi4;
      double pr5, pi5;
      double pr6, pi6;
      double pr7, pi7;

      double sr1, si1;
      double sr3, si3;
      double sr5, si5;
      double sr7, si7;
      
      double qr0, qi0;
      double qr1, qi1;
      double qr2, qi2;
      double qr3, qi3;
      double qr4, qi4;
      double qr5, qi5;
      double qr6, qi6;
      double qr7, qi7;
      
      double tr0, ti0;
      double tr1, ti1;
      double tr2, ti2;
      double tr3, ti3;
      double tr4, ti4;
      double tr5, ti5;
      double tr6, ti6;
      double tr7, ti7;      

      pr5 = i5[2 * j] * wr4 - i5[2 * j + 1] * wi4;
      pi5 = i5[2 * j + 1] * wr4 + i5[2 * j] * wi4;
      pr7 = i7[2 * j] * wr4 - i7[2 * j + 1] * wi4;
      pi7 = i7[2 * j + 1] * wr4 + i7[2 * j] * wi4;
      pr2 = i2[2 * j] * wr2 - i2[2 * j + 1] * wi2;
      pi2 = i2[2 * j + 1] * wr2 + i2[2 * j] * wi2;
      pr4 = i4[2 * j] * wr4 - i4[2 * j + 1] * wi4;
      pi4 = i4[2 * j + 1] * wr4 + i4[2 * j] * wi4;
      pr6 = i6[2 * j] * wr6 - i6[2 * j + 1] * wi6;
      pi6 = i6[2 * j + 1] * wr6 + i6[2 * j] * wi6;
      
      sr1 = i1[2 * j] + pr5; sr5 = i1[2 * j] - pr5;
      si1 = i1[2 * j + 1] + pi5; si5 = i1[2 * j + 1] - pi5; 
      sr3 = i3[2 * j] + pr7; sr7 = i3[2 * j] - pr7;
      si3 = i3[2 * j + 1] + pi7; si7 = i3[2 * j + 1] - pi7; 

      qr0 = i0[2 * j] + pr4; qr4 = i0[2 * j] - pr4;
      qi0 = i0[2 * j + 1] + pi4; qi4 = i0[2 * j + 1] - pi4; 
      qr2 = pr2 + pr6; qr6 = pr2 - pr6;
      qi2 = pi2 + pi6; qi6 = pi2 - pi6;
      
      qr1 = sr1 * wr1 - si1 * wi1; qi1 = si1 * wr1 + sr1 * wi1;
      qr3 = sr3 * wr3 - si3 * wi3; qi3 = si3 * wr3 + sr3 * wi3;
      qr5 = sr5 * wr5 - si5 * wi5; qi5 = si5 * wr5 + sr5 * wi5;
      qr7 = sr7 * wr7 - si7 * wi7; qi7 = si7 * wr7 + sr7 * wi7;

      tr0 = qr0 + qr2; ti0 = qi0 + qi2;
      tr2 = qr0 - qr2; ti2 = qi0 - qi2;
      tr4 = qr4 - qi6; ti4 = qi4 + qr6;
      tr6 = qr4 + qi6; ti6 = qi4 - qr6;

      tr1 = qr1 + qr3; ti1 = qi1 + qi3;
      tr3 = qr1 - qr3; ti3 = qi1 - qi3;
      tr5 = qr5 + qr7; ti5 = qi5 + qi7;
      tr7 = qr5 - qr7; ti7 = qi5 - qi7;

      o0[2 * j] = tr0 + tr1; o0[2 * j + 1] = ti0 + ti1;
      o4[2 * j] = tr0 - tr1; o4[2 * j + 1] = ti0 - ti1;
      o1[2 * j] = tr4 + tr5; o1[2 * j + 1] = ti4 + ti5;
      o5[2 * j] = tr4 - tr5; o5[2 * j + 1] = ti4 - ti5;
      o2[2 * j] = tr2 - ti3; o2[2 * j + 1] = ti2 + tr3;
      o6[2 * j] = tr2 + ti3; o6[2 * j + 1] = ti2 - tr3;
      o3[2 * j] = tr6 - ti7; o3[2 * j + 1] = ti6 + tr7;
      o7[2 * j] = tr6 + ti7; o7[2 * j + 1] = ti6 - tr7;

    }
    i0 += 16 * bsize; i1 += 16 * bsize;
    i2 += 16 * bsize; i3 += 16 * bsize;
    i4 += 16 * bsize; i5 += 16 * bsize;
    i6 += 16 * bsize; i7 += 16 * bsize;
    o0 += 2 * bsize; o1 += 2 * bsize;
    o2 += 2 * bsize; o3 += 2 * bsize;
    o4 += 2 * bsize; o5 += 2 * bsize;
    o6 += 2 * bsize; o7 += 2 * bsize;
  }
}


void fftss_r8_u1_f(double *in, double *out, double *w,
		      long bsize, long blocks)
{
  kern_r8_u1_f(out, out + bsize * blocks * 2, 
		out + bsize * blocks * 4, out + bsize * blocks * 6,
		out + bsize * blocks * 8, out + bsize * blocks * 10,
		out + bsize * blocks * 12, out + bsize * blocks * 14,
		in, in + bsize * 2,
		in + bsize * 4, in + bsize * 6,
		in + bsize * 8, in + bsize * 10,
		in + bsize * 12, in + bsize * 14,
		w, bsize, blocks);
}

void fftss_r8_u1_b(double *in, double *out, double *w,
	      long bsize, long blocks)
{
  kern_r8_u1_b(out, out + bsize * blocks * 2, 
		out + bsize * blocks * 4, out + bsize * blocks * 6,
		out + bsize * blocks * 8, out + bsize * blocks * 10,
		out + bsize * blocks * 12, out + bsize * blocks * 14,
		in, in + bsize * 2,
		in + bsize * 4, in + bsize * 6,
		in + bsize * 8, in + bsize * 10,
		in + bsize * 12, in + bsize * 14,
		w, bsize, blocks);
}
		       
