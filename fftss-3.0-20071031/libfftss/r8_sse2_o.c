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
kern_r8_sse2_o_f(double *restrict o0,
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
  __m128d w1, w2, w3, w4, w5, w6, w7;
  __m128d w1p, w2p, w3p, w4p, w5p, w6p, w7p;
  __m128d zm, mz;
  zm = _mm_loadu_pd((double *)fftss_sse2_czm);
  mz = _mm_loadu_pd((double *)fftss_sse2_cmz);


  w5p = w5 = _mm_load_pd(w + 2 * blocks * bsize);
/*  w5 = _mm_xor_pd(w5, _mm_set_pd(-0.0, 0.0));  */
  w5 = _mm_xor_pd(w5, zm);
  w5p = _mm_shuffle_pd(w5p, w5p, 1);
  w7p = w7 = _mm_load_pd(w + 6 * blocks * bsize);
/*  w7 = _mm_xor_pd(w7, _mm_set_pd(-0.0, 0.0));  */
  w7 = _mm_xor_pd(w7, zm);
  w7p = _mm_shuffle_pd(w7p, w7p, 1);

  for (j = 0; j < bsize; j++) {
    __m128d p0, p1, p2, p3, p4, p5, p6, p7;
    __m128d s5, s7;
    __m128d q0, q1, q2, q3, q4, q5, q6, q7;
    __m128d t0, t1, t2, t3, t4, t5, t6, t7;

    p1 = _mm_load_pd(i1 + 2 * j);
    p5 = _mm_load_pd(i5 + 2 * j);
    q1 = _mm_add_pd(p1, p5);
    s5 = _mm_sub_pd(p1, p5);
    q5 = s5;
    s5 = _mm_unpacklo_pd(s5, s5);
    q5 = _mm_unpackhi_pd(q5, q5);
    s5 = _mm_mul_pd(s5, w5);
    q5 = _mm_mul_pd(q5, w5p);
    q5 = _mm_add_pd(q5, s5);

    p3 = _mm_load_pd(i3 + 2 * j);
    p7 = _mm_load_pd(i7 + 2 * j);
    q3 = _mm_add_pd(p3, p7);
    s7 = _mm_sub_pd(p3, p7);
    q7 = s7;
    s7 = _mm_unpacklo_pd(s7, s7);
    q7 = _mm_unpackhi_pd(q7, q7);
    s7 = _mm_mul_pd(s7, w7);
    q7 = _mm_mul_pd(q7, w7p);
    q7 = _mm_add_pd(q7, s7);

    t1 = _mm_add_pd(q1, q3);
    t3 = _mm_sub_pd(q1, q3);
/*    t3 = _mm_xor_pd(t3, _mm_set_sd(-0.0));  */
	t3 = _mm_xor_pd(t3, mz);
    t3 = _mm_shuffle_pd(t3, t3, 1);
    t5 = _mm_add_pd(q5, q7);
    t7 = _mm_sub_pd(q5, q7);
/*    t7 = _mm_xor_pd(t7, _mm_set_sd(-0.0));  */
	t7 = _mm_xor_pd(t7, mz);
    t7 = _mm_shuffle_pd(t7, t7, 1);

    p0 = _mm_load_pd(i0 + 2 * j);
    p4 = _mm_load_pd(i4 + 2 * j);
    q0 = _mm_add_pd(p0, p4);
    q4 = _mm_sub_pd(p0, p4);

    p2 = _mm_load_pd(i2 + 2 * j);
    p6 = _mm_load_pd(i6 + 2 * j);
    q2 = _mm_add_pd(p2, p6);
    q6 = _mm_sub_pd(p2, p6);
/*    q6 = _mm_xor_pd(q6, _mm_set_sd(-0.0));  */
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

  for (i = 1; i < blocks; i++) {

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

      q7 = s7;
      s7 = _mm_unpacklo_pd(s7, s7);
      q7 = _mm_unpackhi_pd(q7, q7);
      s7 = _mm_mul_pd(s7, w7);
      q7 = _mm_mul_pd(q7, w7p);
      q7 = _mm_add_pd(q7, s7);

      t1 = _mm_add_pd(q1, q3);
      t3 = _mm_sub_pd(q1, q3);
/*      t3 = _mm_xor_pd(t3, _mm_set_sd(-0.0));  */
	  t3 = _mm_xor_pd(t3, mz);
      t3 = _mm_shuffle_pd(t3, t3, 1);
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
      t2 = _mm_sub_pd(q0, q2);
      _mm_store_pd(o0 + 2 * j, _mm_add_pd(t0, t1));
      _mm_store_pd(o4 + 2 * j, _mm_sub_pd(t0, t1));
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
kern_r8_sse2_o_b(double *restrict o0,
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
  __m128d w1, w2, w3, w4, w5, w6, w7;
  __m128d w1p, w2p, w3p, w4p, w5p, w6p, w7p;
  __m128d zm, mz;
  zm = _mm_loadu_pd((double *)fftss_sse2_czm);
  mz = _mm_loadu_pd((double *)fftss_sse2_cmz);


  w5p = w5 = _mm_load_pd(w + 2 * blocks * bsize);
  w5p = _mm_shuffle_pd(w5p, w5p, 1);
/*  w5p = _mm_xor_pd(w5p, _mm_set_sd(-0.0));  */
  w5p = _mm_xor_pd(w5p, mz);
  w7p = w7 = _mm_load_pd(w + 6 * blocks * bsize);
  w7p = _mm_shuffle_pd(w7p, w7p, 1);
/*  w7p = _mm_xor_pd(w7p, _mm_set_sd(-0.0));  */
  w7p = _mm_xor_pd(w7p, mz);

  for (j = 0; j < bsize; j++) {
    __m128d p0, p1, p2, p3, p4, p5, p6, p7;
    __m128d s5, s7;
    __m128d q0, q1, q2, q3, q4, q5, q6, q7;
    __m128d t0, t1, t2, t3, t4, t5, t6, t7;


    p1 = _mm_load_pd(i1 + 2 * j);
    p5 = _mm_load_pd(i5 + 2 * j);
    q1 = _mm_add_pd(p1, p5);
    s5 = _mm_sub_pd(p1, p5);
    q5 = s5;
    s5 = _mm_unpacklo_pd(s5, s5);
    q5 = _mm_unpackhi_pd(q5, q5);
    s5 = _mm_mul_pd(s5, w5);
    q5 = _mm_mul_pd(q5, w5p);
    q5 = _mm_add_pd(q5, s5);

    p3 = _mm_load_pd(i3 + 2 * j);
    p7 = _mm_load_pd(i7 + 2 * j);
    q3 = _mm_add_pd(p3, p7);
    s7 = _mm_sub_pd(p3, p7);
    q7 = s7;
    s7 = _mm_unpacklo_pd(s7, s7);
    q7 = _mm_unpackhi_pd(q7, q7);
    s7 = _mm_mul_pd(s7, w7);
    q7 = _mm_mul_pd(q7, w7p);
    q7 = _mm_add_pd(q7, s7);

    t1 = _mm_add_pd(q1, q3);
    t3 = _mm_sub_pd(q1, q3);
/*    t3 = _mm_xor_pd(t3, _mm_set_sd(-0.0));  */
	t3 = _mm_xor_pd(t3, mz);
    t3 = _mm_shuffle_pd(t3, t3, 1);
    t5 = _mm_add_pd(q5, q7);
    t7 = _mm_sub_pd(q5, q7);
/*    t7 = _mm_xor_pd(t7, _mm_set_sd(-0.0));  */
	t7 = _mm_xor_pd(t7, mz);
    t7 = _mm_shuffle_pd(t7, t7, 1);

    p0 = _mm_load_pd(i0 + 2 * j);
    p4 = _mm_load_pd(i4 + 2 * j);
    q0 = _mm_add_pd(p0, p4);
    q4 = _mm_sub_pd(p0, p4);

    p2 = _mm_load_pd(i2 + 2 * j);
    p6 = _mm_load_pd(i6 + 2 * j);
    q2 = _mm_add_pd(p2, p6);
    q6 = _mm_sub_pd(p2, p6);
/*    q6 = _mm_xor_pd(q6, _mm_set_sd(-0.0));  */
	q6 = _mm_xor_pd(q6, mz);
    q6 = _mm_shuffle_pd(q6, q6, 1);

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

  for (i = 1; i < blocks; i++) {
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

      q7 = s7;
      s7 = _mm_unpacklo_pd(s7, s7);
      q7 = _mm_unpackhi_pd(q7, q7);
      s7 = _mm_mul_pd(s7, w7);
      q7 = _mm_mul_pd(q7, w7p);
      q7 = _mm_add_pd(q7, s7);

      t1 = _mm_add_pd(q1, q3);
      t3 = _mm_sub_pd(q1, q3);
/*      t3 = _mm_xor_pd(t3, _mm_set_sd(-0.0));  */
	  t3 = _mm_xor_pd(t3, mz);
      t3 = _mm_shuffle_pd(t3, t3, 1);
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


void fftss_r8_sse2_o_f(double *in, double *out, double *w,
		       long bsize, long blocks)
{
  kern_r8_sse2_o_f(out, out + bsize * blocks * 2, 
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

void fftss_r8_sse2_o_b(double *in, double *out, double *w,
		       long bsize, long blocks)
{
  kern_r8_sse2_o_b(out, out + bsize * blocks * 2, 
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
		       
