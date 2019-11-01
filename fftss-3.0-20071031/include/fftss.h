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
#ifndef _FFTSS_H
#define _FFTSS_H      

#define FFTSS_VERSION 20071013

typedef void *fftss_plan;
typedef double fftss_complex[2];

#define FFTSS_FORWARD          -1
#define FFTSS_BACKWARD         1

#define FFTSS_MEASURE          0
#define FFTSS_DESTROY_INPUT    (1<<0)
#define FFTSS_UNALIGNED        (1<<1)
#define FFTSS_CONSERVE_MEMORY  (1<<2)
#define FFTSS_EXHAUSTIVE       (1<<3)
#define FFTSS_PRESERVE_INPUT   (1<<4)
#define FFTSS_PATIENT          (1<<5)
#define FFTSS_ESTIMATE         (1<<6)

#define FFTSS_NO_SIMD         (1<<17)

#define FFTSS_VERBOSE         (1<<20)
#define FFTSS_INOUT           (1<<21)

#ifdef __cplusplus
extern "C" {
#endif

  extern void *fftss_malloc(long);
  extern void fftss_free(void *);
  
  extern fftss_plan fftss_plan_dft_1d(long, double *, double *, long, long);
  extern fftss_plan fftss_plan_dft_2d(long, long, long, 
				      double *, double *, long, long);
  extern fftss_plan fftss_plan_dft_3d(long, long, long, long, long,
				      double *, double *, long, long);
  extern void fftss_destroy_plan(fftss_plan);

  extern void fftss_set(fftss_plan, double *, double *);
  extern void fftss_execute(fftss_plan);
  extern void fftss_execute_dft(fftss_plan, double *, double *);
  extern double fftss_get_wtime(void);

  extern int fftss_init_threads(void);
  extern void fftss_cleanup_threads(void);
  extern void fftss_plan_with_nthreads(int);
  
  extern int fftss_test_dft_1d(long, double *, double *, long, long);
  
#ifdef __cplusplus
}
#endif

#endif

