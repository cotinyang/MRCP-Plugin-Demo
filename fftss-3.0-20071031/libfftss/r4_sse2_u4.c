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
kern_r4_sse2_u4_f(double *restrict o0,
		  double *restrict o1,
		  double *restrict o2,
		  double *restrict o3,
		  const double *i0, 
		  const double *w,
		  long blocks)
{
  long i;
  __m128d zm, mz;
  zm = _mm_loadu_pd((double *)fftss_sse2_czm);
  mz = _mm_loadu_pd((double *)fftss_sse2_cmz);

  for (i = 0; i < blocks; i++) {
    __m128d w1, w2, w3;
    __m128d w1p, w2p, w3p;
    __m128d p0, p1, p2, p3;
    __m128d d0, d1, d2, d3;
    
    w1p = w1 = _mm_load_pd(w + 8 * i);
/*    w1 = _mm_xor_pd(w1, _mm_set_pd(-0.0, 0.0));  */
	w1 = _mm_xor_pd(w1, zm);
    w1p = _mm_shuffle_pd(w1p, w1p, 1);

    w2p = w2 = _mm_load_pd(w + 16 * i);
/*    w2 = _mm_xor_pd(w2, _mm_set_pd(-0.0, 0.0));  */
	w2 = _mm_xor_pd(w2, zm);
    w2p = _mm_shuffle_pd(w2p, w2p, 1);

    w3p = w3 = _mm_load_pd(w + 24 * i);
/*    w3 = _mm_xor_pd(w3, _mm_set_pd(-0.0, 0.0));  */
	w3 = _mm_xor_pd(w3, zm);
    w3p = _mm_shuffle_pd(w3p, w3p, 1);


    d2 = _mm_load_pd(i0 + 16);
    p2 = _mm_unpacklo_pd(d2, d2);
    d2 = _mm_unpackhi_pd(d2, d2);
    p2 = _mm_mul_pd(p2, w2); 
    d2 = _mm_mul_pd(d2, w2p);
    d2 = _mm_add_pd(d2, p2);
    
    d1 = _mm_load_pd(i0 + 8);
    p1 = _mm_unpacklo_pd(d1, d1);
    d1 = _mm_unpackhi_pd(d1, d1);
    p1 = _mm_mul_pd(p1, w1); 
    d1 = _mm_mul_pd(d1, w1p);
    d1 = _mm_add_pd(d1, p1);

    d3 = _mm_load_pd(i0 + 24);
    p3 = _mm_unpacklo_pd(d3, d3);
    d3 = _mm_unpackhi_pd(d3, d3);
    p3 = _mm_mul_pd(p3, w3); 
    d3 = _mm_mul_pd(d3, w3p);
    d3 = _mm_add_pd(d3, p3);
    
    d0 = _mm_load_pd(i0);
    p0 = _mm_add_pd(d0, d2);
    p2 = _mm_sub_pd(d0, d2);
    p1 = _mm_add_pd(d1, d3);
    p3 = _mm_sub_pd(d1, d3);
    _mm_store_pd(o0, _mm_add_pd(p0, p1));
    _mm_store_pd(o2, _mm_sub_pd(p0, p1));
/*    p3 = _mm_xor_pd(p3, _mm_set_sd(-0.0));  */
	p3 = _mm_xor_pd(p3, mz);
    p3 = _mm_shuffle_pd(p3, p3, 1);
    
    _mm_store_pd(o1, _mm_add_pd(p2, p3));
    _mm_store_pd(o3, _mm_sub_pd(p2, p3));

     d2 = _mm_load_pd(i0 + 18);
    p2 = _mm_unpacklo_pd(d2, d2);
    d2 = _mm_unpackhi_pd(d2, d2);
    p2 = _mm_mul_pd(p2, w2); 
    d2 = _mm_mul_pd(d2, w2p);
    d2 = _mm_add_pd(d2, p2);
    
    d1 = _mm_load_pd(i0 + 10);
    p1 = _mm_unpacklo_pd(d1, d1);
    d1 = _mm_unpackhi_pd(d1, d1);
    p1 = _mm_mul_pd(p1, w1); 
    d1 = _mm_mul_pd(d1, w1p);
    d1 = _mm_add_pd(d1, p1);

    d3 = _mm_load_pd(i0 + 26);
    p3 = _mm_unpacklo_pd(d3, d3);
    d3 = _mm_unpackhi_pd(d3, d3);
    p3 = _mm_mul_pd(p3, w3); 
    d3 = _mm_mul_pd(d3, w3p);
    d3 = _mm_add_pd(d3, p3);
    
    d0 = _mm_load_pd(i0 + 2);
    p0 = _mm_add_pd(d0, d2);
    p2 = _mm_sub_pd(d0, d2);
    p1 = _mm_add_pd(d1, d3);
    p3 = _mm_sub_pd(d1, d3);
    _mm_store_pd(o0 + 2, _mm_add_pd(p0, p1));
    _mm_store_pd(o2 + 2, _mm_sub_pd(p0, p1));
/*    p3 = _mm_xor_pd(p3, _mm_set_sd(-0.0));  */
	p3 = _mm_xor_pd(p3, mz);
    p3 = _mm_shuffle_pd(p3, p3, 1);
    
    _mm_store_pd(o1 + 2, _mm_add_pd(p2, p3));
    _mm_store_pd(o3 + 2, _mm_sub_pd(p2, p3));

     d2 = _mm_load_pd(i0 + 20);
    p2 = _mm_unpacklo_pd(d2, d2);
    d2 = _mm_unpackhi_pd(d2, d2);
    p2 = _mm_mul_pd(p2, w2); 
    d2 = _mm_mul_pd(d2, w2p);
    d2 = _mm_add_pd(d2, p2);
    
    d1 = _mm_load_pd(i0 + 12);
    p1 = _mm_unpacklo_pd(d1, d1);
    d1 = _mm_unpackhi_pd(d1, d1);
    p1 = _mm_mul_pd(p1, w1); 
    d1 = _mm_mul_pd(d1, w1p);
    d1 = _mm_add_pd(d1, p1);

    d3 = _mm_load_pd(i0 + 28);
    p3 = _mm_unpacklo_pd(d3, d3);
    d3 = _mm_unpackhi_pd(d3, d3);
    p3 = _mm_mul_pd(p3, w3); 
    d3 = _mm_mul_pd(d3, w3p);
    d3 = _mm_add_pd(d3, p3);
    
    d0 = _mm_load_pd(i0 + 4);
    p0 = _mm_add_pd(d0, d2);
    p2 = _mm_sub_pd(d0, d2);
    p1 = _mm_add_pd(d1, d3);
    p3 = _mm_sub_pd(d1, d3);
    _mm_store_pd(o0 + 4, _mm_add_pd(p0, p1));
    _mm_store_pd(o2 + 4, _mm_sub_pd(p0, p1));
/*    p3 = _mm_xor_pd(p3, _mm_set_sd(-0.0));  */
	p3 = _mm_xor_pd(p3, mz);
    p3 = _mm_shuffle_pd(p3, p3, 1);
    
    _mm_store_pd(o1 + 4, _mm_add_pd(p2, p3));
    _mm_store_pd(o3 + 4, _mm_sub_pd(p2, p3));

     d2 = _mm_load_pd(i0 + 22);
    p2 = _mm_unpacklo_pd(d2, d2);
    d2 = _mm_unpackhi_pd(d2, d2);
    p2 = _mm_mul_pd(p2, w2); 
    d2 = _mm_mul_pd(d2, w2p);
    d2 = _mm_add_pd(d2, p2);
    
    d1 = _mm_load_pd(i0 + 14);
    p1 = _mm_unpacklo_pd(d1, d1);
    d1 = _mm_unpackhi_pd(d1, d1);
    p1 = _mm_mul_pd(p1, w1); 
    d1 = _mm_mul_pd(d1, w1p);
    d1 = _mm_add_pd(d1, p1);

    d3 = _mm_load_pd(i0 + 30);
    p3 = _mm_unpacklo_pd(d3, d3);
    d3 = _mm_unpackhi_pd(d3, d3);
    p3 = _mm_mul_pd(p3, w3); 
    d3 = _mm_mul_pd(d3, w3p);
    d3 = _mm_add_pd(d3, p3);
    
    d0 = _mm_load_pd(i0 + 6);
    p0 = _mm_add_pd(d0, d2);
    p2 = _mm_sub_pd(d0, d2);
    p1 = _mm_add_pd(d1, d3);
    p3 = _mm_sub_pd(d1, d3);
    _mm_store_pd(o0 + 6, _mm_add_pd(p0, p1));
    _mm_store_pd(o2 + 6, _mm_sub_pd(p0, p1));
/*    p3 = _mm_xor_pd(p3, _mm_set_sd(-0.0));  */
	p3 = _mm_xor_pd(p3, mz);
    p3 = _mm_shuffle_pd(p3, p3, 1);
    
    _mm_store_pd(o1 + 6, _mm_add_pd(p2, p3));
    _mm_store_pd(o3 + 6, _mm_sub_pd(p2, p3));

 
    i0 += 32;
    o0 += 8; o1 += 8;
    o2 += 8; o3 += 8;
  }
}

static inline void 
kern_r4_sse2_u4_b(double *restrict o0,
		  double *restrict o1,
		  double *restrict o2,
		  double *restrict o3,
		  const double *i0, 
		  const double *w,
		  long blocks)
{
  long i;
  __m128d zm, mz;
  zm = _mm_loadu_pd((double *)fftss_sse2_czm);
  mz = _mm_loadu_pd((double *)fftss_sse2_cmz);

  for (i = 0; i < blocks; i++) {
    __m128d w1, w2, w3;
    __m128d w1p, w2p, w3p;
    __m128d p0, p1, p2, p3;
    __m128d d0, d1, d2, d3;
    
    w1 = _mm_load_pd(w + 8 * i);
    w2 = _mm_load_pd(w + 16 * i);
    w3 = _mm_load_pd(w + 24 * i);
#if 1
    w1p = _mm_shuffle_pd(w1, w1, 1);
    w2p = _mm_shuffle_pd(w2, w2, 1);
    w3p = _mm_shuffle_pd(w3, w3, 1);
#endif
	/*
    w1p = _mm_xor_pd(w1p, _mm_set_sd(-0.0));
    w2p = _mm_xor_pd(w2p, _mm_set_sd(-0.0));
    w3p = _mm_xor_pd(w3p, _mm_set_sd(-0.0));
    */
	w1p = _mm_xor_pd(w1p, mz);
	w2p = _mm_xor_pd(w2p, mz);
	w3p = _mm_xor_pd(w3p, mz);

    d2 = _mm_load_pd(i0 + 16);
    p2 = _mm_unpacklo_pd(d2, d2);
    d2 = _mm_unpackhi_pd(d2, d2);
    p2 = _mm_mul_pd(p2, w2); 
    d2 = _mm_mul_pd(d2, w2p);
    d2 = _mm_add_pd(d2, p2);

    d1 = _mm_load_pd(i0 + 8);
    p1 = _mm_unpacklo_pd(d1, d1);
    d1 = _mm_unpackhi_pd(d1, d1);
    p1 = _mm_mul_pd(p1, w1); 
    d1 = _mm_mul_pd(d1, w1p);
    d1 = _mm_add_pd(d1, p1);

    d3 = _mm_load_pd(i0 + 24);
    p3 = _mm_unpacklo_pd(d3, d3);
    d3 = _mm_unpackhi_pd(d3, d3);
    p3 = _mm_mul_pd(p3, w3); 
    d3 = _mm_mul_pd(d3, w3p);
    d3 = _mm_add_pd(d3, p3);

    d0 = _mm_load_pd(i0);
    p0 = _mm_add_pd(d0, d2);
    p2 = _mm_sub_pd(d0, d2);
    p1 = _mm_add_pd(d1, d3);
    p3 = _mm_sub_pd(d1, d3);
/*    p3 = _mm_xor_pd(p3, _mm_set_sd(-0.0));  */
	p3 = _mm_xor_pd(p3, mz);
    p3 = _mm_shuffle_pd(p3, p3, 1);
      
    _mm_store_pd(o0, _mm_add_pd(p0, p1));
    _mm_store_pd(o2, _mm_sub_pd(p0, p1));
    _mm_store_pd(o1, _mm_sub_pd(p2, p3));
    _mm_store_pd(o3, _mm_add_pd(p2, p3));

    d2 = _mm_load_pd(i0 + 18);
    p2 = _mm_unpacklo_pd(d2, d2);
    d2 = _mm_unpackhi_pd(d2, d2);
    p2 = _mm_mul_pd(p2, w2); 
    d2 = _mm_mul_pd(d2, w2p);
    d2 = _mm_add_pd(d2, p2);

    d1 = _mm_load_pd(i0 + 10);
    p1 = _mm_unpacklo_pd(d1, d1);
    d1 = _mm_unpackhi_pd(d1, d1);
    p1 = _mm_mul_pd(p1, w1); 
    d1 = _mm_mul_pd(d1, w1p);
    d1 = _mm_add_pd(d1, p1);

    d3 = _mm_load_pd(i0 + 26);
    p3 = _mm_unpacklo_pd(d3, d3);
    d3 = _mm_unpackhi_pd(d3, d3);
    p3 = _mm_mul_pd(p3, w3); 
    d3 = _mm_mul_pd(d3, w3p);
    d3 = _mm_add_pd(d3, p3);

    d0 = _mm_load_pd(i0 + 2);
    p0 = _mm_add_pd(d0, d2);
    p2 = _mm_sub_pd(d0, d2);
    p1 = _mm_add_pd(d1, d3);
    p3 = _mm_sub_pd(d1, d3);
/*    p3 = _mm_xor_pd(p3, _mm_set_sd(-0.0));  */
	p3 = _mm_xor_pd(p3, mz);
    p3 = _mm_shuffle_pd(p3, p3, 1);
      
    _mm_store_pd(o0 + 2, _mm_add_pd(p0, p1));
    _mm_store_pd(o2 + 2, _mm_sub_pd(p0, p1));
    _mm_store_pd(o1 + 2, _mm_sub_pd(p2, p3));
    _mm_store_pd(o3 + 2, _mm_add_pd(p2, p3));

    d2 = _mm_load_pd(i0 + 20);
    p2 = _mm_unpacklo_pd(d2, d2);
    d2 = _mm_unpackhi_pd(d2, d2);
    p2 = _mm_mul_pd(p2, w2); 
    d2 = _mm_mul_pd(d2, w2p);
    d2 = _mm_add_pd(d2, p2);

    d1 = _mm_load_pd(i0 + 12);
    p1 = _mm_unpacklo_pd(d1, d1);
    d1 = _mm_unpackhi_pd(d1, d1);
    p1 = _mm_mul_pd(p1, w1); 
    d1 = _mm_mul_pd(d1, w1p);
    d1 = _mm_add_pd(d1, p1);

    d3 = _mm_load_pd(i0 + 28);
    p3 = _mm_unpacklo_pd(d3, d3);
    d3 = _mm_unpackhi_pd(d3, d3);
    p3 = _mm_mul_pd(p3, w3); 
    d3 = _mm_mul_pd(d3, w3p);
    d3 = _mm_add_pd(d3, p3);

    d0 = _mm_load_pd(i0 + 4);
    p0 = _mm_add_pd(d0, d2);
    p2 = _mm_sub_pd(d0, d2);
    p1 = _mm_add_pd(d1, d3);
    p3 = _mm_sub_pd(d1, d3);
/*    p3 = _mm_xor_pd(p3, _mm_set_sd(-0.0));  */
	p3 = _mm_xor_pd(p3, mz);
    p3 = _mm_shuffle_pd(p3, p3, 1);
      
    _mm_store_pd(o0 + 4, _mm_add_pd(p0, p1));
    _mm_store_pd(o2 + 4, _mm_sub_pd(p0, p1));
    _mm_store_pd(o1 + 4, _mm_sub_pd(p2, p3));
    _mm_store_pd(o3 + 4, _mm_add_pd(p2, p3));

    d2 = _mm_load_pd(i0 + 22);
    p2 = _mm_unpacklo_pd(d2, d2);
    d2 = _mm_unpackhi_pd(d2, d2);
    p2 = _mm_mul_pd(p2, w2); 
    d2 = _mm_mul_pd(d2, w2p);
    d2 = _mm_add_pd(d2, p2);

    d1 = _mm_load_pd(i0 + 14);
    p1 = _mm_unpacklo_pd(d1, d1);
    d1 = _mm_unpackhi_pd(d1, d1);
    p1 = _mm_mul_pd(p1, w1); 
    d1 = _mm_mul_pd(d1, w1p);
    d1 = _mm_add_pd(d1, p1);

    d3 = _mm_load_pd(i0 + 30);
    p3 = _mm_unpacklo_pd(d3, d3);
    d3 = _mm_unpackhi_pd(d3, d3);
    p3 = _mm_mul_pd(p3, w3); 
    d3 = _mm_mul_pd(d3, w3p);
    d3 = _mm_add_pd(d3, p3);

    d0 = _mm_load_pd(i0 + 6);
    p0 = _mm_add_pd(d0, d2);
    p2 = _mm_sub_pd(d0, d2);
    p1 = _mm_add_pd(d1, d3);
    p3 = _mm_sub_pd(d1, d3);
/*    p3 = _mm_xor_pd(p3, _mm_set_sd(-0.0));  */
	p3 = _mm_xor_pd(p3, mz);
    p3 = _mm_shuffle_pd(p3, p3, 1);
      
    _mm_store_pd(o0 + 6, _mm_add_pd(p0, p1));
    _mm_store_pd(o2 + 6, _mm_sub_pd(p0, p1));
    _mm_store_pd(o1 + 6, _mm_sub_pd(p2, p3));
    _mm_store_pd(o3 + 6, _mm_add_pd(p2, p3));

    i0 += 32; 
    o0 += 8; o1 += 8;
    o2 += 8; o3 += 8;
  }
}

void fftss_r4_sse2_u4_f(double *in, double *out, double *w,
		       long bsize, long blocks)
{
  kern_r4_sse2_u4_f(out, out + bsize * blocks * 2, 
		    out + bsize * blocks * 4, out + bsize * blocks * 6,
		    in, w, blocks);
}

void fftss_r4_sse2_u4_b(double *in, double *out, double *w,
		       long bsize, long blocks)
{
  kern_r4_sse2_u4_b(out, out + bsize * blocks * 2, 
		    out + bsize * blocks * 4, out + bsize * blocks * 6,
		    in, w, blocks);
}
		       
