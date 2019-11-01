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

#define FORTRAN_FUNC(a,A,body)  \
void dfftss_ ## a       body \
void dfftss_ ## a ## _  body \
void DFFTSS_ ## A       body \
void DFFTSS_ ## A ## _  body \
void dfftss_ ## a ## __ body \
void  fftss_ ## a ## _  body \
void  FFTSS_ ## A       body \
void  FFTSS_ ## A ## _  body \
void  fftss_ ## a ## __ body 


FORTRAN_FUNC(plan_dft_1d,PLAN_DFT_1D,
	     (fftss_plan *p, long *n, double *in, double *out,
	      long *sign, long *flags) {
  *p = fftss_plan_dft_1d(*n, in, out, *sign, *flags);
})
     
FORTRAN_FUNC(plan_dft_2d,PLAN_DFT_2D,
	     (fftss_plan *p, long *nx, long *ny, long *py,
	      double *in, double *out, long *sign, long *flags) {
  *p = fftss_plan_dft_2d(*nx, *ny, *py, in, out, *sign, *flags);
})

FORTRAN_FUNC(plan_dft_3d,PLAN_DFT_3D,
	     (fftss_plan *p, long *nx, long *ny, long *nz,
	      long *py, long *pz, double *in, double *out,
	      long *sign, long *flags) {
  *p = fftss_plan_dft_3d(*nx, *ny, *nz, *py, *pz, in, out, *sign, *flags);
})

FORTRAN_FUNC(destroy_plan,DESTROY_PLAN,
	     (fftss_plan *p) {
  fftss_destroy_plan(*p);
})

FORTRAN_FUNC(set,SET,
	     (fftss_plan *p, double *in, double *out) {
  fftss_set(*p, in, out);
})

FORTRAN_FUNC(execute,EXECUTE,
	     (fftss_plan *p) {
  fftss_execute(*p);
})

FORTRAN_FUNC(execute_dft,EXECUTE_DFT,
	     (fftss_plan *p, double *in, double *out) {
  fftss_execute_dft(*p, in, out);
})

FORTRAN_FUNC(get_wtime,GET_WTIME,
	     (double *t) {
  *t = fftss_get_wtime();
})

FORTRAN_FUNC(init_threads,INIT_THREADS,
	     (void) {
  fftss_init_threads();
})

FORTRAN_FUNC(cleanup_threads,CLEANUP_THREADS,
	     (void) {
  fftss_cleanup_threads();
})

FORTRAN_FUNC(plan_with_nthreads,PLAN_WITH_NTHREADS,
	     (int *nthreads) {
  fftss_plan_with_nthreads(*nthreads);
})


