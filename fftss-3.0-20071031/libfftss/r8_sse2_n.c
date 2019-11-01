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

#include <emmintrin.h>

extern unsigned int fftss_sse2_cmz[];
extern unsigned int fftss_sse2_czm[];

static inline void 
kern_r8_sse2_n_f(double *restrict o0,
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
  __m128d zm, mz;
  zm = _mm_loadu_pd((double *)fftss_sse2_czm);
  mz = _mm_loadu_pd((double *)fftss_sse2_cmz);


  for (i = 0; i < blocks; i++) {
#if 0
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
#endif

    __m128d w1, w2, w3, w4, w5, w6, w7;
    __m128d w1p, w2p, w3p, w4p, w5p, w6p, w7p;

    w1p = w1 = _mm_load_pd(w + 2 * i * bsize);
/*    w1 = _mm_xor_pd(w1, _mm_set_pd(-0.0, 0.0));  */
	w1 = _mm_xor_pd(w1, zm);
    w1p = _mm_shuffle_pd(w1p, w1p, 1);
    w2p = w2 = _mm_load_pd(w + 4 * i * bsize);
/*    w2 = _mm_xor_pd(w2, _mm_set_pd(-0.0, 0.0));  */
	w2 = _mm_xor_pd(w2, zm);
    w2p = _mm_shuffle_pd(w2p, w2p, 1);
    w3p = w3 = _mm_load_pd(w + 6 * i * bsize);
/*    w3 = _mm_xor_pd(w3, _mm_set_pd(-0.0, 0.0));  */
	w3 = _mm_xor_pd(w3, zm);
	w3p = _mm_shuffle_pd(w3p, w3p, 1);
    w4p = w4 = _mm_load_pd(w + 8 * i * bsize);
/*    w4 = _mm_xor_pd(w4, _mm_set_pd(-0.0, 0.0));  */
	w4 = _mm_xor_pd(w4, zm);
    w4p = _mm_shuffle_pd(w4p, w4p, 1);
    w5p = w5 = _mm_load_pd(w + 2 * i * bsize + 2 * blocks * bsize);
/*    w5 = _mm_xor_pd(w5, _mm_set_pd(-0.0, 0.0));  */
	w5 = _mm_xor_pd(w5, zm);
    w5p = _mm_shuffle_pd(w5p, w5p, 1);
    w6p = w6 = _mm_load_pd(w + 12 * i * bsize);
/*    w6 = _mm_xor_pd(w6, _mm_set_pd(-0.0, 0.0));  */
	w6 = _mm_xor_pd(w6, zm);
    w6p = _mm_shuffle_pd(w6p, w6p, 1);
    w7p = w7 = _mm_load_pd(w + 6 * i * bsize + 6 * blocks * bsize);
/*    w7 = _mm_xor_pd(w7, _mm_set_pd(-0.0, 0.0));  */
	w7 = _mm_xor_pd(w7, zm);
    w7p = _mm_shuffle_pd(w7p, w7p, 1);

    for (j = 0; j < bsize; j++) {
#if 0
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

      pr5 = i5[2 * j] * wr4 + i5[2 * j + 1] * wi4;
      pi5 = i5[2 * j + 1] * wr4 - i5[2 * j] * wi4;
      pr7 = i7[2 * j] * wr4 + i7[2 * j + 1] * wi4;
      pi7 = i7[2 * j + 1] * wr4 - i7[2 * j] * wi4;
      pr2 = i2[2 * j] * wr2 + i2[2 * j + 1] * wi2;
      pi2 = i2[2 * j + 1] * wr2 - i2[2 * j] * wi2;
      pr4 = i4[2 * j] * wr4 + i4[2 * j + 1] * wi4;
      pi4 = i4[2 * j + 1] * wr4 - i4[2 * j] * wi4;
      pr6 = i6[2 * j] * wr6 + i6[2 * j + 1] * wi6;
      pi6 = i6[2 * j + 1] * wr6 - i6[2 * j] * wi6;

      sr1 = i1[2 * j] + pr5; sr5 = i1[2 * j] - pr5;
      si1 = i1[2 * j + 1] + pi5; si5 = i1[2 * j + 1] - pi5; 
      sr3 = i3[2 * j] + pr7; sr7 = i3[2 * j] - pr7;
      si3 = i3[2 * j + 1] + pi7; si7 = i3[2 * j + 1] - pi7; 

      qr0 = i0[2 * j] + pr4; qr4 = i0[2 * j] - pr4;
      qi0 = i0[2 * j + 1] + pi4; qi4 = i0[2 * j + 1] - pi4; 
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
#endif
      __m128d p0, p1, p2, p3, p4, p5, p6, p7;
      __m128d s1, s3, s5, s7;
      __m128d q0, q1, q2, q3, q4, q5, q6, q7;
      __m128d t0, t1, t2, t3, t4, t5, t6, t7;

      q5 = p5 = _mm_load_pd(i5 + 2 * j);
      q5 = _mm_unpacklo_pd(q5, q5);
      p5 = _mm_unpackhi_pd(p5, p5);
      q5 = _mm_mul_pd(q5, w4);
      p5 = _mm_mul_pd(p5, w4p);
      p5 = _mm_add_pd(p5, q5);

      p1 = _mm_load_pd(i1 + 2 * j);
      s1 = _mm_add_pd(p1, p5);
      s5 = _mm_sub_pd(p1, p5);

      q1 = s1;
      s1 = _mm_unpacklo_pd(s1, s1);
      q1 = _mm_unpackhi_pd(q1, q1);
      s1 = _mm_mul_pd(s1, w1);
      q1 = _mm_mul_pd(q1, w1p);
      q1 = _mm_add_pd(q1, s1);

      q5 = s5;
      s5 = _mm_unpacklo_pd(s5, s5);
      q5 = _mm_unpackhi_pd(q5, q5);
      s5 = _mm_mul_pd(s5, w5);
      q5 = _mm_mul_pd(q5, w5p);
      q5 = _mm_add_pd(q5, s5);

      q7 = p7 = _mm_load_pd(i7 + 2 * j);
      q7 = _mm_unpacklo_pd(q7, q7);
      p7 = _mm_unpackhi_pd(p7, p7);
      q7 = _mm_mul_pd(q7, w4);
      p7 = _mm_mul_pd(p7, w4p);
      p7 = _mm_add_pd(p7, q7);

      p3 = _mm_load_pd(i3 + 2 * j);
      s3 = _mm_add_pd(p3, p7);
      s7 = _mm_sub_pd(p3, p7);

      q3 = s3;
      s3 = _mm_unpacklo_pd(s3, s3);
      q3 = _mm_unpackhi_pd(q3, q3);
      s3 = _mm_mul_pd(s3, w3);
      q3 = _mm_mul_pd(q3, w3p);
      q3 = _mm_add_pd(q3, s3);

      t1 = _mm_add_pd(q1, q3);
      t3 = _mm_sub_pd(q1, q3);
/*      t3 = _mm_xor_pd(t3, _mm_set_sd(-0.0));  */
	  t3 = _mm_xor_pd(t3, mz);
      t3 = _mm_shuffle_pd(t3, t3, 1);

      q7 = s7;
      s7 = _mm_unpacklo_pd(s7, s7);
      q7 = _mm_unpackhi_pd(q7, q7);
      s7 = _mm_mul_pd(s7, w7);
      q7 = _mm_mul_pd(q7, w7p);
      q7 = _mm_add_pd(q7, s7);

      t5 = _mm_add_pd(q5, q7);
      t7 = _mm_sub_pd(q5, q7);
/*      t7 = _mm_xor_pd(t7, _mm_set_sd(-0.0));  */
	  t7 = _mm_xor_pd(t7, mz);
      t7 = _mm_shuffle_pd(t7, t7, 1);

      q4 = p4 = _mm_load_pd(i4 + 2 * j);
      q4 = _mm_unpacklo_pd(q4, q4);
      p4 = _mm_unpackhi_pd(p4, p4);
      q4 = _mm_mul_pd(q4, w4);
      p4 = _mm_mul_pd(p4, w4p);
      p4 = _mm_add_pd(p4, q4);

      p0 = _mm_load_pd(i0 + 2 * j);
      q0 = _mm_add_pd(p0, p4);
      q4 = _mm_sub_pd(p0, p4);

      q2 = p2 = _mm_load_pd(i2 + 2 * j);
      q2 = _mm_unpacklo_pd(q2, q2);
      p2 = _mm_unpackhi_pd(p2, p2);
      q2 = _mm_mul_pd(q2, w2);
      p2 = _mm_mul_pd(p2, w2p);
      p2 = _mm_add_pd(p2, q2);

      q6 = p6 = _mm_load_pd(i6 + 2 * j);
      q6 = _mm_unpacklo_pd(q6, q6);
      p6 = _mm_unpackhi_pd(p6, p6);
      q6 = _mm_mul_pd(q6, w6);
      p6 = _mm_mul_pd(p6, w6p);
      p6 = _mm_add_pd(p6, q6);

      q2 = _mm_add_pd(p2, p6);
      q6 = _mm_sub_pd(p2, p6);
/*      q6 = _mm_xor_pd(q6, _mm_set_sd(-0.0));  */
	  q6 = _mm_xor_pd(q6, mz);
      q6 = _mm_shuffle_pd(q6, q6, 1);

      t0 = _mm_add_pd(q0, q2);
      _mm_store_pd(o0 + 2 * j, _mm_add_pd(t0, t1));
      _mm_store_pd(o4 + 2 * j, _mm_sub_pd(t0, t1));
      t2 = _mm_sub_pd(q0, q2);
      _mm_store_pd(o2 + 2 * j, _mm_add_pd(t2, t3));
      _mm_store_pd(o6 + 2 * j, _mm_sub_pd(t2, t3));
      t4 = _mm_add_pd(q4, q6);
      _mm_store_pd(o1 + 2 * j, _mm_add_pd(t4, t5));
      _mm_store_pd(o5 + 2 * j, _mm_sub_pd(t4, t5));
      t6 = _mm_sub_pd(q4, q6);
      _mm_store_pd(o3 + 2 * j, _mm_add_pd(t6, t7));
      _mm_store_pd(o7 + 2 * j, _mm_sub_pd(t6, t7));

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

static inline void 
kern_r8_sse2_n_b(double *restrict o0,
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
  __m128d zm, mz;
  zm = _mm_loadu_pd((double *)fftss_sse2_czm);
  mz = _mm_loadu_pd((double *)fftss_sse2_cmz);


  for (i = 0; i < blocks; i++) {
#if 0
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
#endif
    __m128d w1, w2, w3, w4, w5, w6, w7;
    __m128d w1p, w2p, w3p, w4p, w5p, w6p, w7p;

    w1p = w1 = _mm_load_pd(w + 2 * i * bsize);
    w1p = _mm_shuffle_pd(w1p, w1p, 1);
/*    w1p = _mm_xor_pd(w1p, _mm_set_sd(-0.0));  */
	w1p = _mm_xor_pd(w1p, mz);
    w2p = w2 = _mm_load_pd(w + 4 * i * bsize);
    w2p = _mm_shuffle_pd(w2p, w2p, 1);
/*    w2p = _mm_xor_pd(w2p, _mm_set_sd(-0.0));  */
	w2p = _mm_xor_pd(w2p, mz);
    w3p = w3 = _mm_load_pd(w + 6 * i * bsize);
    w3p = _mm_shuffle_pd(w3p, w3p, 1);
/*    w3p = _mm_xor_pd(w3p, _mm_set_sd(-0.0));  */
	w3p = _mm_xor_pd(w3p, mz);
    w4p = w4 = _mm_load_pd(w + 8 * i * bsize);
    w4p = _mm_shuffle_pd(w4p, w4p, 1);
/*    w4p = _mm_xor_pd(w4p, _mm_set_sd(-0.0));  */
	w4p = _mm_xor_pd(w4p, mz);
    w5p = w5 = _mm_load_pd(w + 2 * i * bsize + 2 * blocks * bsize);
    w5p = _mm_shuffle_pd(w5p, w5p, 1);
/*    w5p = _mm_xor_pd(w5p, _mm_set_sd(-0.0));  */
	w5p = _mm_xor_pd(w5p, mz);
    w6p = w6 = _mm_load_pd(w + 12 * i * bsize);
    w6p = _mm_shuffle_pd(w6p, w6p, 1);
/*    w6p = _mm_xor_pd(w6p, _mm_set_sd(-0.0));  */
	w6p = _mm_xor_pd(w6p, mz);
    w7p = w7 = _mm_load_pd(w + 6 * i * bsize + 6 * blocks * bsize);
    w7p = _mm_shuffle_pd(w7p, w7p, 1);
/*    w7p = _mm_xor_pd(w7p, _mm_set_sd(-0.0));  */
	w7p = _mm_xor_pd(w7p, mz);


    
    for (j = 0; j < bsize; j++) {
#if 0
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
#endif
      __m128d p0, p1, p2, p3, p4, p5, p6, p7;
      __m128d s1, s3, s5, s7;
      __m128d q0, q1, q2, q3, q4, q5, q6, q7;
      __m128d t0, t1, t2, t3, t4, t5, t6, t7;

      q5 = p5 = _mm_load_pd(i5 + 2 * j);
      q5 = _mm_unpacklo_pd(q5, q5);
      p5 = _mm_unpackhi_pd(p5, p5);
      q5 = _mm_mul_pd(q5, w4);
      p5 = _mm_mul_pd(p5, w4p);
      p5 = _mm_add_pd(p5, q5);

      p1 = _mm_load_pd(i1 + 2 * j);
      s1 = _mm_add_pd(p1, p5);
      s5 = _mm_sub_pd(p1, p5);

      q1 = s1;
      s1 = _mm_unpacklo_pd(s1, s1);
      q1 = _mm_unpackhi_pd(q1, q1);
      s1 = _mm_mul_pd(s1, w1);
      q1 = _mm_mul_pd(q1, w1p);
      q1 = _mm_add_pd(q1, s1);

      q5 = s5;
      s5 = _mm_unpacklo_pd(s5, s5);
      q5 = _mm_unpackhi_pd(q5, q5);
      s5 = _mm_mul_pd(s5, w5);
      q5 = _mm_mul_pd(q5, w5p);
      q5 = _mm_add_pd(q5, s5);

      q7 = p7 = _mm_load_pd(i7 + 2 * j);
      q7 = _mm_unpacklo_pd(q7, q7);
      p7 = _mm_unpackhi_pd(p7, p7);
      q7 = _mm_mul_pd(q7, w4);
      p7 = _mm_mul_pd(p7, w4p);
      p7 = _mm_add_pd(p7, q7);

      p3 = _mm_load_pd(i3 + 2 * j);
      s3 = _mm_add_pd(p3, p7);
      s7 = _mm_sub_pd(p3, p7);

      q3 = s3;
      s3 = _mm_unpacklo_pd(s3, s3);
      q3 = _mm_unpackhi_pd(q3, q3);
      s3 = _mm_mul_pd(s3, w3);
      q3 = _mm_mul_pd(q3, w3p);
      q3 = _mm_add_pd(q3, s3);

      t1 = _mm_add_pd(q1, q3);
      t3 = _mm_sub_pd(q1, q3);
/*      t3 = _mm_xor_pd(t3, _mm_set_sd(-0.0));  */
	  t3 = _mm_xor_pd(t3, mz);
      t3 = _mm_shuffle_pd(t3, t3, 1);

      q7 = s7;
      s7 = _mm_unpacklo_pd(s7, s7);
      q7 = _mm_unpackhi_pd(q7, q7);
      s7 = _mm_mul_pd(s7, w7);
      q7 = _mm_mul_pd(q7, w7p);
      q7 = _mm_add_pd(q7, s7);

      t5 = _mm_add_pd(q5, q7);
      t7 = _mm_sub_pd(q5, q7);
/*      t7 = _mm_xor_pd(t7, _mm_set_sd(-0.0));  */
	  t7 = _mm_xor_pd(t7, mz);
      t7 = _mm_shuffle_pd(t7, t7, 1);

      q2 = p2 = _mm_load_pd(i2 + 2 * j);
      q2 = _mm_unpacklo_pd(q2, q2);
      p2 = _mm_unpackhi_pd(p2, p2);
      q2 = _mm_mul_pd(q2, w2);
      p2 = _mm_mul_pd(p2, w2p);
      p2 = _mm_add_pd(p2, q2);

      q6 = p6 = _mm_load_pd(i6 + 2 * j);
      q6 = _mm_unpacklo_pd(q6, q6);
      p6 = _mm_unpackhi_pd(p6, p6);
      q6 = _mm_mul_pd(q6, w6);
      p6 = _mm_mul_pd(p6, w6p);
      p6 = _mm_add_pd(p6, q6);

      q2 = _mm_add_pd(p2, p6);
      q6 = _mm_sub_pd(p2, p6);
/*      q6 = _mm_xor_pd(q6, _mm_set_sd(-0.0));  */
	  q6 = _mm_xor_pd(q6, mz);
      q6 = _mm_shuffle_pd(q6, q6, 1);

      q4 = p4 = _mm_load_pd(i4 + 2 * j);
      q4 = _mm_unpacklo_pd(q4, q4);
      p4 = _mm_unpackhi_pd(p4, p4);
      q4 = _mm_mul_pd(q4, w4);
      p4 = _mm_mul_pd(p4, w4p);
      p4 = _mm_add_pd(p4, q4);

      p0 = _mm_load_pd(i0 + 2 * j);
      q0 = _mm_add_pd(p0, p4);
      q4 = _mm_sub_pd(p0, p4);

      t0 = _mm_add_pd(q0, q2);
      _mm_store_pd(o0 + 2 * j, _mm_add_pd(t0, t1));
      _mm_store_pd(o4 + 2 * j, _mm_sub_pd(t0, t1));
      t2 = _mm_sub_pd(q0, q2);
      _mm_store_pd(o2 + 2 * j, _mm_sub_pd(t2, t3));
      _mm_store_pd(o6 + 2 * j, _mm_add_pd(t2, t3));
      t4 = _mm_sub_pd(q4, q6);
      _mm_store_pd(o1 + 2 * j, _mm_add_pd(t4, t5));
      _mm_store_pd(o5 + 2 * j, _mm_sub_pd(t4, t5));
      t6 = _mm_add_pd(q4, q6);
      _mm_store_pd(o3 + 2 * j, _mm_sub_pd(t6, t7));
      _mm_store_pd(o7 + 2 * j, _mm_add_pd(t6, t7));

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


void fftss_r8_sse2_n_f(double *in, double *out, double *w,
		     long bsize, long blocks)
{
  kern_r8_sse2_n_f(out, out + bsize * blocks * 2, 
			    out + bsize * blocks * 4, 
			    out + bsize * blocks * 6,
			    out + bsize * blocks * 8,
			    out + bsize * blocks * 10,
			    out + bsize * blocks * 12, 
			    out + bsize * blocks * 14,
			    in, in + bsize * 2,
			    in + bsize * 4, in + bsize * 6,
			    in + bsize * 8, in + bsize * 10,
			    in + bsize * 12, in + bsize * 14,
			    w, bsize, blocks);
}

void fftss_r8_sse2_n_b(double *in, double *out, double *w,
		     long bsize, long blocks)
{
  kern_r8_sse2_n_b(out, out + bsize * blocks * 2, 
			    out + bsize * blocks * 4,
			    out + bsize * blocks * 6,
			    out + bsize * blocks * 8, 
			    out + bsize * blocks * 10,
			    out + bsize * blocks * 12, 
			    out + bsize * blocks * 14,
			    in, in + bsize * 2,
			    in + bsize * 4,
			    in + bsize * 6,
			    in + bsize * 8,
			    in + bsize * 10,
			    in + bsize * 12,
			    in + bsize * 14,
			    w, bsize, blocks);
}
		       
