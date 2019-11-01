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
#include <stdio.h>
#include <malloc.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "libfftss.h"

void fftss_execute_dft_2d_bb(fftss_plan p, double *in, double *out)
{
  long i;

#ifdef _OPENMP
#pragma omp parallel for schedule(static)
#endif
  for (i = 0; i < p->ny; i++)
    fftss_execute_inplace_dft_1d(p->p1, in + i * p->py * 2,
			 p->b[2 * omp_get_thread_num()]);
  
#ifdef _OPENMP
#pragma omp parallel for schedule(static)
#endif
  for (i = 0; i < p->nx; i++) {
    double *b0, *b1;

    b0 = p->b[2 * omp_get_thread_num()];
    b1 = p->b[2 * omp_get_thread_num() + 1];
    fftss_copyin(b0, in + i * 2, p->py, p->ny);
    fftss_execute_inplace_dft_1d(p->p2, b0, b1);
    fftss_copyout(out + i * 2, b0, p->py, p->ny);
  }

  return;
}

void fftss_execute_dft_2d_bc(fftss_plan p, double *in, double *out)
{
  long i;

#ifdef _OPENMP
#pragma omp parallel for schedule(static)
#endif
  for (i = 0; i < p->ny; i++)
    fftss_execute_inplace_dft_1d(p->p1, in + i * p->py * 2,
			 p->b[2 * omp_get_thread_num()]);
  
#ifdef _OPENMP
#pragma omp parallel for schedule(static,1)
#endif
  for (i = 0; i < p->nx; i++) {
    double *b0, *b1;

    b0 = p->b[2 * omp_get_thread_num()];
    b1 = p->b[2 * omp_get_thread_num() + 1];
    fftss_copyin(b0, in + i * 2, p->py, p->ny);
    fftss_execute_inplace_dft_1d(p->p2, b0, b1);
    fftss_copyout(out + i * 2, b0, p->py, p->ny);
  }

  return;
}

void fftss_execute_dft_2d_cc(fftss_plan p, double *in, double *out)
{
  long i;

#ifdef _OPENMP
#pragma omp parallel for schedule(static,1)
#endif
  for (i = 0; i < p->ny; i++)
    fftss_execute_inplace_dft_1d(p->p1, in + i * p->py * 2,
			 p->b[2 * omp_get_thread_num()]);
  
#ifdef _OPENMP
#pragma omp parallel for schedule(static,1)
#endif
  for (i = 0; i < p->nx; i++) {
    double *b0, *b1;

    b0 = p->b[2 * omp_get_thread_num()];
    b1 = p->b[2 * omp_get_thread_num() + 1];
    fftss_copyin(b0, in + i * 2, p->py, p->ny);
    fftss_execute_inplace_dft_1d(p->p2, b0, b1);
    fftss_copyout(out + i * 2, b0, p->py, p->ny);
  }

  return;
}

void fftss_execute_dft_2d_cb(fftss_plan p, double *in, double *out)
{
  long i;

#ifdef _OPENMP
#pragma omp parallel for schedule(static,1)
#endif
  for (i = 0; i < p->ny; i++)
    fftss_execute_inplace_dft_1d(p->p1, in + i * p->py * 2,
			 p->b[2 * omp_get_thread_num()]);
  
#ifdef _OPENMP
#pragma omp parallel for schedule(static)
#endif
  for (i = 0; i < p->nx; i++) {
    double *b0, *b1;

    b0 = p->b[2 * omp_get_thread_num()];
    b1 = p->b[2 * omp_get_thread_num() + 1];
    fftss_copyin(b0, in + i * 2, p->py, p->ny);
    fftss_execute_inplace_dft_1d(p->p2, b0, b1);
    fftss_copyout(out + i * 2, b0, p->py, p->ny);
  }

  return;
}

void fftss_execute_dft_2d_preserve(fftss_plan p, double *in, double *out)
{
  long i;

#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (i = 0; i < p->nx; i++) {
    double *b0, *b1;

    b0 = p->b[2 * omp_get_thread_num()];
    b1 = p->b[2 * omp_get_thread_num() + 1];
    fftss_copyin(b0, in + i * 2, p->py, p->ny);
    fftss_execute_inplace_dft_1d(p->p2, b0, b1);
    fftss_copyout(out + i * 2, b0, p->py, p->ny);
  }

#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (i = 0; i < p->ny; i++)
    fftss_execute_inplace_dft_1d(p->p1, out + i * p->py * 2, 
				 p->b[2 * omp_get_thread_num()]);
  
  return;
}


extern void fftss_destroy_plan_1d(fftss_plan_1d);

fftss_plan 
fftss_plan_dft_2d(long nx, long ny, long py,
		  double *in, double *out, long sign, long flags)
{
  fftss_plan p, pp;
  long l;
  long i;
  long flags_for_1d;
  double fastest = 1000000000.0;
  double t0, t1;

  p = malloc(sizeof(fftss_plan_s));
  p->nx = nx; p->ny = ny; p->py = py; p->nz = 0;
  p->in = in;
  p->out = out;
  if (flags & FFTSS_INOUT)
    p->out = p->in;

  flags_for_1d = flags &
    (FFTSS_NO_SIMD|FFTSS_VERBOSE|FFTSS_UNALIGNED|FFTSS_CONSERVE_MEMORY|
     FFTSS_EXHAUSTIVE|FFTSS_PATIENT|FFTSS_ESTIMATE|FFTSS_MEASURE);

  l = (nx > ny) ? nx : ny;

  p->b = malloc(sizeof(double *) * 2 * omp_get_max_threads());

  for (i = 0; i < omp_get_max_threads(); i++) {
    p->b[i * 2] = fftss_malloc(sizeof(double) * 2 * l);
    p->b[i * 2 + 1] = fftss_malloc(sizeof(double) * 2 * l);
  }

#ifdef _OPENMP
#pragma omp parallel private(i)
#endif
  {
    double *b0, *b1;
    int id;

    id = omp_get_thread_num();
    b0 = p->b[id * 2];
    b1 = p->b[id * 2 + 1];
    for (i = 0; i < l * 2; i++) b0[i] = 0.0;
    for (i = 0; i < l * 2; i++) b1[i] = 0.0;
  } 
  
  pp = fftss_plan_dft_1d(nx, p->b[0], p->b[1], sign, 
			 FFTSS_INOUT | flags_for_1d);

  if (pp == NULL) {
    for (i = 0; i < omp_get_max_threads(); i++) {
      free(p->b[i * 2]);
      free(p->b[i * 2 + 1]);
    }
    free(p->b);
    free(p);
  }

  p->p1 = pp->p1;
  free(pp);
  if (nx != ny) {
    pp = fftss_plan_dft_1d(ny, p->b[0], p->b[1], sign, 
			   FFTSS_INOUT | flags_for_1d);
    if (pp == NULL) {
      fftss_destroy_plan_1d(p->p1);
      for (i = 0; i < omp_get_max_threads(); i++) {
	free(p->b[i * 2]);
	free(p->b[i * 2 + 1]);
      }
      free(p->b);
      free(p);
    }

    p->p2 = pp->p1;
    free(pp);
  } else p->p2 = p->p1;

  p->p3 = NULL;
  /*  p->work = fftss_malloc(sizeof(double) * 2 * py * ny); */

#define TRY(name, func) \
  {                                   \
    t0 = fftss_get_wtime();           \
    func(p, in,out);                  \
    t1 = fftss_get_wtime() - t0;      \
    if (flags & FFTSS_VERBOSE)        \
      printf("%s : %lf.\n", name, t1);\
    if (t1 < fastest) {               \
      fastest = t1;                   \
      p->fp = func;                   \
    }                                 \
  }

  if (flags & FFTSS_PRESERVE_INPUT)
    p->fp = fftss_execute_dft_2d_preserve;
  else {
    TRY("Block-Block", fftss_execute_dft_2d_bb);
    TRY("Block-Cyclic", fftss_execute_dft_2d_bc);
    TRY("Cyclic-Block", fftss_execute_dft_2d_cb);
    TRY("Cyclic-Cyclic", fftss_execute_dft_2d_cc);
  }


  return p;
}

extern void fftss_execute_dft_3d(fftss_plan, double *, double *);
  
void fftss_destroy_plan(fftss_plan p)
{
  long i;

  if (p->nz != 0) {

    fftss_destroy_plan_1d(p->p1);
    if (p->nx != p->ny) fftss_destroy_plan_1d(p->p2);
    if (p->nx != p->nz && p->ny != p->nz) fftss_destroy_plan_1d(p->p3);
    for (i = 0; i < omp_get_max_threads(); i++) {
      fftss_free(p->b[i * 2]);
      fftss_free(p->b[i * 2 + 1]);
    }
    free(p->b);
    free(p);

  } else if (p->ny != 0) {

    fftss_destroy_plan_1d(p->p1);
    if (p->nx != p->ny) fftss_destroy_plan_1d(p->p2);
    for (i = 0; i < omp_get_max_threads(); i++) {
      fftss_free(p->b[i * 2]);
      fftss_free(p->b[i * 2 + 1]);
    }
    free(p->b);
    free(p);

  } else if (p->nx != 0) {

    fftss_destroy_plan_1d(p->p1);
    free(p);
  }

  return;
}
