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

#ifndef _OPENMP
#define omp_get_max_threads()   1
#define omp_get_thread_num()    0
#endif


void fftss_execute_dft_3d_xy(fftss_plan p, double *in, double *out)
{
  long i, j;

#ifdef _OPENMP
#pragma omp parallel for private(i)
#endif
  for (j = 0; j < p->nz; j++) {
    double *b0, *b1;
    
    b0 = p->b[2 * omp_get_thread_num()];
    b1 = p->b[2 * omp_get_thread_num() + 1];

    for (i = 0; i < p->ny; i++)
      fftss_execute_inplace_dft_1d(p->p1, 
				   in + i * p->py * 2 + j * p->pz * 2, 
				   b0);

    for (i = 0; i < p->nx; i++) {
      fftss_copyin(b0, in + i * 2 + j * p->pz * 2, p->py, p->ny);
      fftss_execute_inplace_dft_1d(p->p2, b0, b1);
      fftss_copyout(out + i * 2 + j * p->pz * 2, b0, p->py, p->ny);
    }
  }
  
#ifdef _OPENMP
#pragma omp parallel for private(i)
#endif
  for (j = 0; j < p->ny; j++) {
    double *b0, *b1;
    
    b0 = p->b[2 * omp_get_thread_num()];
    b1 = p->b[2 * omp_get_thread_num() + 1];
    for (i = 0; i < p->nx; i++) {
      fftss_copyin(b0, out + i * 2 + j * p->py * 2, p->pz, p->nz);
      fftss_execute_inplace_dft_1d(p->p3, b0, b1);
      fftss_copyout(out + i * 2 + j * p->py * 2, b0, p->pz, p->nz);
    }
  }

  return;
}

void fftss_execute_dft_3d_xy2(fftss_plan p, double *in, double *out)
{
  long i, j;

  for (j = 0; j < p->nz; j++) {

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (i = 0; i < p->ny; i++)
      fftss_execute_inplace_dft_1d(p->p1, 
				   in + i * p->py * 2 + j * p->pz * 2, 
				   p->b[2 * omp_get_thread_num()]);

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (i = 0; i < p->nx; i++) {
      double *b0, *b1;
    
      b0 = p->b[2 * omp_get_thread_num()];
      b1 = p->b[2 * omp_get_thread_num() + 1];

      fftss_copyin(b0, in + i * 2 + j * p->pz * 2, p->py, p->ny);
      fftss_execute_inplace_dft_1d(p->p2, b0, b1);
      fftss_copyout(out + i * 2 + j * p->pz * 2, b0, p->py, p->ny);
    }
  }
  
#ifdef _OPENMP
#pragma omp parallel for private(i)
#endif
  for (j = 0; j < p->ny; j++) {
    double *b0, *b1;
    
    b0 = p->b[2 * omp_get_thread_num()];
    b1 = p->b[2 * omp_get_thread_num() + 1];
    for (i = 0; i < p->nx; i++) {
      fftss_copyin(b0, out + i * 2 + j * p->py * 2, p->pz, p->nz);
      fftss_execute_inplace_dft_1d(p->p3, b0, b1);
      fftss_copyout(out + i * 2 + j * p->py * 2, b0, p->pz, p->nz);
    }
  }

  return;
}


void fftss_execute_dft_3d(fftss_plan p, double *in, double *out)
{
  long i, j;

#ifdef _OPENMP
#pragma omp parallel for private(i)
#endif
  for (j = 0; j < p->nz; j++)
    for (i = 0; i < p->ny; i++)
      fftss_execute_inplace_dft_1d(p->p1, 
				   in + i * p->py * 2 + j * p->pz * 2, 
				   p->b[2 * omp_get_thread_num()]);
  
#ifdef _OPENMP
#pragma omp parallel for private(i)
#endif
  for (j = 0; j < p->nz; j++) {
    double *b0, *b1;
    
    b0 = p->b[2 * omp_get_thread_num()];
    b1 = p->b[2 * omp_get_thread_num() + 1];
    for (i = 0; i < p->nx; i++) {
      fftss_copyin(b0, in + i * 2 + j * p->pz * 2, p->py, p->ny);
      fftss_execute_inplace_dft_1d(p->p2, b0, b1);
      fftss_copyout(out + i * 2 + j * p->pz * 2, b0, p->py, p->ny);
    }
  }

#ifdef _OPENMP
#pragma omp parallel for private(i)
#endif
  for (j = 0; j < p->ny; j++) {
    double *b0, *b1;
    
    b0 = p->b[2 * omp_get_thread_num()];
    b1 = p->b[2 * omp_get_thread_num() + 1];
    for (i = 0; i < p->nx; i++) {
      fftss_copyin(b0, out + i * 2 + j * p->py * 2, p->pz, p->nz);
      fftss_execute_inplace_dft_1d(p->p3, b0, b1);
      fftss_copyout(out + i * 2 + j * p->py * 2, b0, p->pz, p->nz);
    }
  }

  return;
}

void fftss_execute_dft_3d_xy_preserve(fftss_plan p, double *in, double *out)
{
  long i, j;

#ifdef _OPENMP
#pragma omp parallel for private(i)
#endif
  for (j = 0; j < p->nz; j++) {
    double *b0, *b1;
    
    b0 = p->b[2 * omp_get_thread_num()];
    b1 = p->b[2 * omp_get_thread_num() + 1];

    for (i = 0; i < p->nx; i++) {
      fftss_copyin(b0, in + i * 2 + j * p->pz * 2, p->py, p->ny);
      fftss_execute_inplace_dft_1d(p->p2, b0, b1);
      fftss_copyout(out + i * 2 + j * p->pz * 2, b0, p->py, p->ny);
    }

    for (i = 0; i < p->ny; i++)
      fftss_execute_inplace_dft_1d(p->p1, 
				   out + i * p->py * 2 + j * p->pz * 2, 
				   b0);

  }
  
#ifdef _OPENMP
#pragma omp parallel for private(i)
#endif
  for (j = 0; j < p->ny; j++) {
    double *b0, *b1;
    
    b0 = p->b[2 * omp_get_thread_num()];
    b1 = p->b[2 * omp_get_thread_num() + 1];
    for (i = 0; i < p->nx; i++) {
      fftss_copyin(b0, out + i * 2 + j * p->py * 2, p->pz, p->nz);
      fftss_execute_inplace_dft_1d(p->p3, b0, b1);
      fftss_copyout(out + i * 2 + j * p->py * 2, b0, p->pz, p->nz);
    }
  }

  return;
}

void fftss_execute_dft_3d_xy2_preserve(fftss_plan p, double *in, double *out)
{
  long i, j;

  for (j = 0; j < p->nz; j++) {

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (i = 0; i < p->nx; i++) {
      double *b0, *b1;
    
      b0 = p->b[2 * omp_get_thread_num()];
      b1 = p->b[2 * omp_get_thread_num() + 1];

      fftss_copyin(b0, in + i * 2 + j * p->pz * 2, p->py, p->ny);
      fftss_execute_inplace_dft_1d(p->p2, b0, b1);
      fftss_copyout(out + i * 2 + j * p->pz * 2, b0, p->py, p->ny);
    }

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (i = 0; i < p->ny; i++)
      fftss_execute_inplace_dft_1d(p->p1, 
				   out + i * p->py * 2 + j * p->pz * 2, 
				   p->b[2 * omp_get_thread_num()]);

  }
  
  for (j = 0; j < p->ny; j++) {

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (i = 0; i < p->nx; i++) {
      double *b0, *b1;
    
      b0 = p->b[2 * omp_get_thread_num()];
      b1 = p->b[2 * omp_get_thread_num() + 1];

      fftss_copyin(b0, out + i * 2 + j * p->py * 2, p->pz, p->nz);
      fftss_execute_inplace_dft_1d(p->p3, b0, b1);
      fftss_copyout(out + i * 2 + j * p->py * 2, b0, p->pz, p->nz);
    }
  }

  return;
}


void fftss_execute_dft_3d_preserve(fftss_plan p, double *in, double *out)
{
  long i, j;

#ifdef _OPENMP
#pragma omp parallel for private(i)
#endif
  for (j = 0; j < p->nz; j++) {
    double *b0, *b1;
    
    b0 = p->b[2 * omp_get_thread_num()];
    b1 = p->b[2 * omp_get_thread_num() + 1];
    for (i = 0; i < p->nx; i++) {
      fftss_copyin(b0, in + i * 2 + j * p->pz * 2, p->py, p->ny);
      fftss_execute_inplace_dft_1d(p->p2, b0, b1);
      fftss_copyout(out + i * 2 + j * p->pz * 2, b0, p->py, p->ny);
    }
  }

#ifdef _OPENMP
#pragma omp parallel for private(i)
#endif
  for (j = 0; j < p->nz; j++)
    for (i = 0; i < p->ny; i++)
      fftss_execute_inplace_dft_1d(p->p1, 
				   out + i * p->py * 2 + j * p->pz * 2, 
				   p->b[2 * omp_get_thread_num()]);
  

#ifdef _OPENMP
#pragma omp parallel for private(i)
#endif
  for (j = 0; j < p->ny; j++) {
    double *b0, *b1;
    
    b0 = p->b[2 * omp_get_thread_num()];
    b1 = p->b[2 * omp_get_thread_num() + 1];
    for (i = 0; i < p->nx; i++) {
      fftss_copyin(b0, out + i * 2 + j * p->py * 2, p->pz, p->nz);
      fftss_execute_inplace_dft_1d(p->p3, b0, b1);
      fftss_copyout(out + i * 2 + j * p->py * 2, b0, p->pz, p->nz);
    }
  }

  return;
}

extern void fftss_destroy_plan_1d(fftss_plan_1d);

fftss_plan 
fftss_plan_dft_3d(long nx, long ny, long nz, long py, long pz,
		  double *in, double *out, long sign, long flags)
{
  fftss_plan p, pp;
  long l;
  long i;
  double t, t0;
  long flags_for_1d;

  flags_for_1d = flags &
    (FFTSS_NO_SIMD|FFTSS_VERBOSE|FFTSS_UNALIGNED|FFTSS_CONSERVE_MEMORY|
     FFTSS_EXHAUSTIVE|FFTSS_PATIENT|FFTSS_ESTIMATE|FFTSS_MEASURE);
  
  p = malloc(sizeof(fftss_plan_s));
  p->nx = nx; p->ny = ny; p->nz = nz;
  p->py = py; p->pz = pz;
  p->in = in;
  p->out = out;
  if (flags & FFTSS_PRESERVE_INPUT)
    p->fp = fftss_execute_dft_3d_preserve;
  else
    p->fp = fftss_execute_dft_3d;
  if (flags & FFTSS_INOUT)
    p->out = p->in;

  l = (nx > ny) ? nx : ny;
  if (l < nz) l = nz;

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
    return NULL;
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
      return NULL;
    }

    p->p2 = pp->p1;
    free(pp);
  } else p->p2 = p->p1;

  if (nx == nz)
    p->p3 = p->p1;
  else if (ny == nz)
    p->p3 = p->p2;
  else {
    pp = fftss_plan_dft_1d(nz, p->b[0], p->b[1], sign, 
			   FFTSS_INOUT | flags_for_1d);

    if (pp == NULL) {
      fftss_destroy_plan_1d(p->p1);
      if (nx != ny) fftss_destroy_plan_1d(p->p2);
      for (i = 0; i < omp_get_max_threads(); i++) {
	free(p->b[i * 2]);
	free(p->b[i * 2 + 1]);
      }
      free(p->b);
      free(p);
      return NULL;
    }

    p->p3 = pp->p1;
    free(pp);
  }
  /*  p->work = fftss_malloc(sizeof(double) * 2 * py * ny); */


  t = fftss_get_wtime();
  if (flags & FFTSS_PRESERVE_INPUT)
    fftss_execute_dft_3d_preserve(p, p->in, p->out);
  else
    fftss_execute_dft_3d(p, p->in, p->out);
  t0 = fftss_get_wtime() - t;

  t = fftss_get_wtime();
  if (flags & FFTSS_PRESERVE_INPUT)
    fftss_execute_dft_3d_xy_preserve(p, p->in, p->out);
  else
    fftss_execute_dft_3d_xy(p, p->in, p->out);
  t = fftss_get_wtime() - t;
  if (t < t0) {
    if (flags & FFTSS_PRESERVE_INPUT)
      p->fp = fftss_execute_dft_3d_xy_preserve;
    else
      p->fp = fftss_execute_dft_3d_xy;
    t0 = t;
  }
#if 0
  t = fftss_get_wtime();
  if (flags & FFTSS_PRESERVE_INPUT)
    fftss_execute_dft_3d_xy2_preserve(p, p->in, p->out);
  else
    fftss_execute_dft_3d_xy2(p, p->in, p->out);
  t = fftss_get_wtime() - t;
  if (t < t0) {
    if (flags & FFTSS_PRESERVE_INPUT)
      p->fp = fftss_execute_dft_3d_xy2_preserve;
    else
      p->fp = fftss_execute_dft_3d_xy2;
    t0 = t;
  }
#endif

  return p;
}
  
