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
#ifdef HAVE_CONFIG_H
#include "config.h"
#elif defined(_WIN32)
#include "win32config.h"
#endif
#include <stdio.h>
#include <malloc.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <mpi.h>
#include <fftss.h>

#ifndef _OPENMP
#define omp_get_max_threads()   1
#define omp_get_thread_num()    0
#endif

typedef struct _pfftss_plan_s {
  void (*fp)(struct _pfftss_plan_s *, double *);
  long nx, ny, oy, ly, ox, lx;
  int id, npe;
  long py, plx;
  double **b;
  double *inout, *sb, *rb;
  long *oxp, *oyp;
  int *soff, *roff;
  int *rsoff, *rroff;
  int *ssize, *rsize;
  MPI_Request *sreq, *rreq;
  MPI_Status *st;
  fftss_plan p1, p2;
  MPI_Comm comm;
#ifdef HAVE_MPI2
  MPI_Win rwin, swin;
#endif
  long flags;
} pfftss_plan_s;

typedef pfftss_plan_s *pfftss_plan;


#if 1
extern void fftss_copyin(double *, double *, long, long);
extern void fftss_copyout(double *, double *, long, long);
#endif

static void trans_x(pfftss_plan p, double *inout)
{
  long i;

#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (i = 0; i < p->ly; i++)
    fftss_execute_dft(p->p1, inout + i * p->py * 2, 
		      p->b[2 * omp_get_thread_num()]);
}

static void trans_y(pfftss_plan p)
{
  long i;

#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (i = 0; i < p->lx; i++) {
    double *b0, *b1;

    b0 = p->b[2 * omp_get_thread_num()];
    b1 = p->b[2 * omp_get_thread_num() + 1];
    fftss_copyin(b0, p->rb + i * 2, p->plx, p->ny);
    fftss_execute_dft(p->p2, b0, b1);
    fftss_copyout(p->rb + i * 2, b0, p->plx, p->ny);
  }
}

static void alltoall0(pfftss_plan p)
{
  MPI_Alltoall(p->sb, p->plx * p->ly * 2, MPI_DOUBLE,
	       p->rb, p->plx * p->ly * 2, MPI_DOUBLE,
	       p->comm);
}

static void alltoalls0(pfftss_plan p)
{
  long i;
  int bsize;

  bsize = p->plx * p->ly * 2;

  for (i = 1; i < p->npe; i++) {
    int d;
    d = (p->id + i) % p->npe;
    MPI_Irecv(p->rb + bsize * d, bsize, MPI_DOUBLE, d, 
	      MPI_ANY_TAG, p->comm, &(p->rreq[i - 1]));
  }
  for (i = 1; i < p->npe; i++) {
    int d;
    d = (p->id + i) % p->npe;
    MPI_Isend(p->sb + bsize * d, bsize, MPI_DOUBLE, d, 
	      0, p->comm, &(p->sreq[i - 1]));
  }

#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (i = 0; i < bsize; i++) 
    p->rb[bsize * p->id + i] = p->sb[bsize * p->id + i];

  MPI_Waitall(p->npe - 1, p->rreq, p->st);
  MPI_Waitall(p->npe - 1, p->sreq, p->st);
}

#ifdef HAVE_MPI2

static void alltoallp0(pfftss_plan p)
{
  long i;
  int bsize;

  MPI_Win_fence(MPI_MODE_NOPRECEDE, p->rwin);

  bsize = p->plx * p->ly * 2;

  for (i = 1; i < p->npe; i++) {
    int d;
    d = (p->id + i) % p->npe;
    MPI_Put(p->sb + bsize * d, bsize, MPI_DOUBLE, d, 
	    bsize * p->id, bsize, MPI_DOUBLE, p->rwin);
  }

#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (i = 0; i < bsize; i++) 
    p->rb[bsize * p->id + i] = p->sb[bsize * p->id + i];

  MPI_Win_fence(MPI_MODE_NOSUCCEED, p->rwin);
}

#endif

static void alltoallv0(pfftss_plan p)
{
  MPI_Alltoallv(p->sb, p->ssize, p->soff, MPI_DOUBLE,
		p->rb, p->rsize, p->roff, MPI_DOUBLE,
		p->comm);
}

static void alltoallvs0(pfftss_plan p)
{
  long i;

  for (i = 1; i < p->npe; i++) {
    int d;
    d = (p->id + i) % p->npe;
    MPI_Irecv(p->rb + p->roff[d], p->rsize[d], MPI_DOUBLE, d, 
	      MPI_ANY_TAG, p->comm, &(p->rreq[i - 1]));
  }
  for (i = 1; i < p->npe; i++) {
    int d;
    d = (p->id + i) % p->npe;
    MPI_Isend(p->sb + p->soff[d], p->ssize[d], MPI_DOUBLE, d, 
	      0, p->comm, &(p->sreq[i - 1]));
  }

#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (i = 0; i < p->ssize[p->id]; i++) 
    p->rb[p->roff[p->id] + i] = p->sb[p->soff[p->id] + i];

  MPI_Waitall(p->npe - 1, p->rreq, p->st);
  MPI_Waitall(p->npe - 1, p->sreq, p->st);
}

#ifdef HAVE_MPI2

static void alltoallvp0(pfftss_plan p)
{
  long i;

  MPI_Win_fence(MPI_MODE_NOPRECEDE, p->rwin);
  for (i = 1; i < p->npe; i++) {
    int d;
    d = (p->id + i) % p->npe;
    MPI_Put(p->sb + p->soff[d], p->ssize[d], MPI_DOUBLE, d,
	    p->rroff[d], p->ssize[d], MPI_DOUBLE, p->rwin);
  }

#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (i = 0; i < p->ssize[p->id]; i++) 
    p->rb[p->roff[p->id] + i] = p->sb[p->soff[p->id] + i];

  MPI_Win_fence(MPI_MODE_NOSUCCEED, p->rwin);
}

#endif

static void alltoall1(pfftss_plan p)
{
  MPI_Alltoall(p->rb, p->plx * p->ly * 2, MPI_DOUBLE,
	       p->sb, p->plx * p->ly * 2, MPI_DOUBLE,
	       p->comm);
}

static void alltoalls1(pfftss_plan p)
{
  long i;
  int bsize;

  bsize = p->plx * p->ly * 2;

  for (i = 1; i < p->npe; i++) {
    int d;
    d = (p->id + i) % p->npe;
    MPI_Irecv(p->sb + bsize * d, bsize, MPI_DOUBLE, d, 
	      MPI_ANY_TAG, p->comm, &(p->rreq[i - 1]));
  }
  for (i = 1; i < p->npe; i++) {
    int d;
    d = (p->id + i) % p->npe;
    MPI_Isend(p->rb + bsize * d, bsize, MPI_DOUBLE, d, 
	      0, p->comm, &(p->sreq[i - 1]));
  }

#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (i = 0; i < bsize; i++) 
    p->sb[bsize * p->id + i] = p->rb[bsize * p->id + i];

  MPI_Waitall(p->npe - 1, p->rreq, p->st);
  MPI_Waitall(p->npe - 1, p->sreq, p->st);
}

#ifdef HAVE_MPI2

static void alltoallp1(pfftss_plan p)
{
  long i;
  int bsize;

  MPI_Win_fence(MPI_MODE_NOPRECEDE, p->swin);

  bsize = p->plx * p->ly * 2;

  for (i = 1; i < p->npe; i++) {
    int d;
    d = (p->id + i) % p->npe;
    MPI_Put(p->rb + bsize * d, bsize, MPI_DOUBLE, d, 
	    bsize * p->id, bsize, MPI_DOUBLE, p->swin);
  }

#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (i = 0; i < bsize; i++) 
    p->sb[bsize * p->id + i] = p->rb[bsize * p->id + i];

  MPI_Win_fence(MPI_MODE_NOSUCCEED, p->swin);
}

#endif

static void alltoallv1(pfftss_plan p)
{
  MPI_Alltoallv(p->rb, p->rsize, p->roff, MPI_DOUBLE,
		p->sb, p->ssize, p->soff, MPI_DOUBLE,
		p->comm);
}

static void alltoallvs1(pfftss_plan p)
{
  long i;

  for (i = 1; i < p->npe; i++) {
    int d;
    d = (p->id + i) % p->npe;
    MPI_Irecv(p->sb + p->soff[d], p->ssize[d], MPI_DOUBLE, d, 
	      MPI_ANY_TAG, p->comm, &(p->rreq[i - 1]));
  }
  for (i = 1; i < p->npe; i++) {
    int d;
    d = (p->id + i) % p->npe;
    MPI_Isend(p->rb + p->roff[d], p->rsize[d], MPI_DOUBLE, d, 
	      0, p->comm, &(p->sreq[i - 1]));
  }

#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (i = 0; i < p->rsize[p->id]; i++) 
    p->sb[p->soff[p->id] + i] = p->rb[p->roff[p->id] + i];

  MPI_Waitall(p->npe - 1, p->rreq, p->st);
  MPI_Waitall(p->npe - 1, p->sreq, p->st);
}

#ifdef HAVE_MPI2

static void alltoallvp1(pfftss_plan p)
{
  long i;

  MPI_Win_fence(MPI_MODE_NOPRECEDE, p->swin);
  for (i = 1; i < p->npe; i++) {
    int d;
    d = (p->id + i) % p->npe;
    MPI_Put(p->rb + p->roff[d], p->rsize[d], MPI_DOUBLE, d, 
	    p->rsoff[d], p->rsize[d], MPI_DOUBLE, p->swin);
  }

#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (i = 0; i < p->rsize[p->id]; i++) 
    p->sb[p->soff[p->id] + i] = p->rb[p->roff[p->id] + i];

  MPI_Win_fence(MPI_MODE_NOSUCCEED, p->swin);
}

#endif

static void pack0(pfftss_plan p, double *inout)
{
  long i;

#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (i = 0; i < p->npe; i++) {
    long j, k;
    long lxp;
    double *src, *dst;

    lxp = p->plx;
    for (j = 0; j < p->ly; j++) {
      src = inout + p->lx * i * 2 + p->py * j * 2;
      dst = p->sb + p->ly * lxp * i * 2 + lxp * j * 2;
      for (k = 0; k < p->lx * 2; k++)
	dst[k] = src[k];
    }
  }
}  

static void packv0(pfftss_plan p, double *inout)
{
  long i;

#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (i = 0; i < p->npe; i++) {
    long j, k;
    long lx, lxp;
    double *src, *dst;

    lx = p->oxp[i + 1] - p->oxp[i];
    lxp = p->plx;
    for (j = 0; j < p->ly; j++) {
      src = inout + p->oxp[i] * 2 + p->py * j * 2;
      dst = p->sb + p->soff[i] + lxp * j * 2;
      for (k = 0; k < lx * 2; k++)
	dst[k] = src[k];
    }
  }
}  

static void unpack1(pfftss_plan p, double *inout)
{
  long i;

#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (i = 0; i < p->npe; i++) {
    long j, k;
    long lxp;
    double *src, *dst;

    lxp = p->plx;
    for (j = 0; j < p->ly; j++) {
      dst = inout + p->lx * i * 2 + p->py * j * 2;
      src = p->sb + lxp * p->ly * i * 2 + lxp * j * 2;
      for (k = 0; k < p->lx * 2; k++)
	dst[k] = src[k];
    }
  }
}

static void unpackv1(pfftss_plan p, double *inout)
{
  long i;

#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (i = 0; i < p->npe; i++) {
    long j, k;
    long lx, lxp;
    double *src, *dst;

    lx = p->oxp[i + 1] - p->oxp[i];
    lxp = p->plx;
    for (j = 0; j < p->ly; j++) {
      dst = inout + p->oxp[i] * 2 + p->py * j * 2;
      src = p->sb + p->soff[i] + lxp * j * 2;
      for (k = 0; k < lx * 2; k++)
	dst[k] = src[k];
    }
  }
}

void pfftss_execute_pdft_2d_a(pfftss_plan p, double *inout)
{
  trans_x(p, inout);
  pack0(p, inout);
  alltoall0(p);
  trans_y(p);
  alltoall1(p);
  unpack1(p, inout);

  return;
}

void pfftss_execute_pdft_2d_s(pfftss_plan p, double *inout)
{
  trans_x(p, inout);
  pack0(p, inout);
  alltoalls0(p);
  trans_y(p);
  alltoalls1(p);
  unpack1(p, inout);

  return;
}

#ifdef HAVE_MPI2

void pfftss_execute_pdft_2d_p(pfftss_plan p, double *inout)
{
  trans_x(p, inout);
  pack0(p, inout);
  alltoallp0(p);
  trans_y(p);
  alltoallp1(p);
  unpack1(p, inout);

  return;
}

#endif

void pfftss_execute_pdft_2d_va(pfftss_plan p, double *inout)
{
  trans_x(p, inout);
  packv0(p, inout);
  alltoallv0(p);
  trans_y(p);
  alltoallv1(p);
  unpackv1(p, inout);

  return;
}

void pfftss_execute_pdft_2d_vs(pfftss_plan p, double *inout)
{
  trans_x(p, inout);
  packv0(p, inout);
  alltoallvs0(p);
  trans_y(p);
  alltoallvs1(p);
  unpackv1(p, inout);

  return;
}

#ifdef HAVE_MPI2

void pfftss_execute_pdft_2d_vp(pfftss_plan p, double *inout)
{
  trans_x(p, inout);
  packv0(p, inout);
  alltoallvp0(p);
  trans_y(p);
  alltoallvp1(p);
  unpackv1(p, inout);

  return;
}

#endif

void pfftss_execute(pfftss_plan p)
{
  (*(p->fp))(p, p->inout);
  return;
}

void pfftss_execute_dft(pfftss_plan p, double *inout)
{
  (*(p->fp))(p, inout);
  return;
}

void pfftss_destroy_plan(pfftss_plan p)
{
  long i;

#ifdef HAVE_MPI_ALLOC_MEM
  MPI_Free_mem(p->sb);
  MPI_Free_mem(p->rb);
#else
  fftss_free(p->sb);
  fftss_free(p->rb);
#endif
  free(p->oxp);
  free(p->oyp);

  if (p->soff) {
    free(p->soff);
    free(p->roff);
    free(p->rsoff);
    free(p->rroff);
    free(p->ssize);
    free(p->rsize);
  }
  free(p->sreq);
  free(p->rreq);
  free(p->st);

  for (i = 0; i < omp_get_max_threads() * 2; i++)
    fftss_free(p->b[i]);
  free(p->b);
  if (p->p1 != p->p2) fftss_destroy_plan(p->p2);
  fftss_destroy_plan(p->p1);

#ifdef HAVE_MPI2
  MPI_Win_free(&p->rwin);
  MPI_Win_free(&p->swin);
#endif

  free(p);
}

static double test_run(pfftss_plan p, void (*fp)(pfftss_plan, double *))
{
  double t;

  MPI_Barrier(p->comm);
  t = fftss_get_wtime();
  (*fp)(p, p->inout);
  MPI_Barrier(p->comm);
  
  t = fftss_get_wtime() - t;
  if ((p->flags & FFTSS_VERBOSE) && p->id == 0) printf("%lf sec.\n", t);
  return t;
}

pfftss_plan 
pfftss_plan_dft_2d(long nx, long ny, long py, long oy, long ly,
		   double *inout, long sign, long flags, MPI_Comm comm)
{
  pfftss_plan p;
  long l;
  long i;
  double t0, t1;

  p = (pfftss_plan)malloc(sizeof(pfftss_plan_s));
  p->nx = nx; p->ny = ny; p->py = py; 
  p->oy = oy; p->ly = ly;
  p->comm = comm;
  p->flags = flags;
  MPI_Comm_size(comm, &(p->npe));
  MPI_Comm_rank(comm, &(p->id));

  p->ox = nx * p->id / p->npe;
  p->lx = nx * (p->id + 1) / p->npe - p->ox;
  p->plx = (nx + p->npe - 1) / p->npe;
  if ((p->plx & 1) == 0) p->plx++;

  p->inout = inout;

  l = (nx > ny) ? nx : ny;

  p->b = (double **)malloc(sizeof(double *) * 2 * omp_get_max_threads());

  for (i = 0; i < omp_get_max_threads(); i++) {
    p->b[i * 2] = (double *)fftss_malloc(sizeof(double) * 2 * l);
    p->b[i * 2 + 1] = (double *)fftss_malloc(sizeof(double) * 2 * l);
  }

  p->sreq = (MPI_Request *)malloc(sizeof(MPI_Request) * p->npe);
  p->rreq = (MPI_Request *)malloc(sizeof(MPI_Request) * p->npe);
  p->st = (MPI_Status *)malloc(sizeof(MPI_Status) * p->npe);

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
  
  p->p1 = fftss_plan_dft_1d(nx, p->b[0], p->b[1], sign, FFTSS_INOUT);
  if (nx != ny)
    p->p2 = fftss_plan_dft_1d(ny, p->b[0], p->b[1], sign, FFTSS_INOUT);
  else p->p2 = p->p1;

  p->oxp = (long *)malloc(sizeof(long) * (p->npe + 1));
  p->oyp = (long *)malloc(sizeof(long) * (p->npe + 1));

  MPI_Allgather(&(p->ox), 1, MPI_LONG, p->oxp, 1, MPI_LONG, comm);
  MPI_Allgather(&(p->oy), 1, MPI_LONG, p->oyp, 1, MPI_LONG, comm);

  p->oxp[p->npe] = nx;
  p->oyp[p->npe] = ny;

  l = p->ly;
  for (i = 0; i < p->npe; i++)
    if (p->oyp[i + 1] - p->oyp[i] != l) break;
  if (nx % p->npe == 0 && ny % p->npe == 0 && i == p->npe) {

    if ((p->flags & FFTSS_VERBOSE) && p->id == 0) printf("uniform mode\n");

    p->soff = NULL;
    p->roff = NULL;
    p->rsoff = NULL;
    p->rroff = NULL;
    p->ssize = NULL;
    p->rsize = NULL;

#ifdef HAVE_MPI_ALLOC_MEM
    MPI_Alloc_mem(p->plx * p->ny * sizeof(double) * 2,
		  MPI_INFO_NULL, &(p->sb));
    MPI_Alloc_mem(p->plx * p->ny * sizeof(double) * 2,
		  MPI_INFO_NULL, &(p->rb));
#else
    p->sb = 
      (double *)fftss_malloc(p->plx * p->ny * sizeof(double) * 2);
    p->rb =
      (double *)fftss_malloc(p->plx * p->ny * sizeof(double) * 2);
#endif

#ifdef HAVE_MPI2
    MPI_Win_create(p->sb, 
		   p->plx * p->ny * sizeof(double) * 2,
		   sizeof(double), MPI_INFO_NULL, comm, &(p->swin));
    MPI_Win_create(p->rb, 
		   p->plx * p->ny * sizeof(double) * 2,
		   sizeof(double), MPI_INFO_NULL, comm, &(p->rwin));
#endif

    p->fp = pfftss_execute_pdft_2d_a;
    t0 = test_run(p, pfftss_execute_pdft_2d_a);
    t1 = test_run(p, pfftss_execute_pdft_2d_a);
    if (t1 < t0) { t0 = t1; }
    t1 = test_run(p, pfftss_execute_pdft_2d_s);
    if (t1 < t0) { p->fp = pfftss_execute_pdft_2d_s; t0 = t1; }
    t1 = test_run(p, pfftss_execute_pdft_2d_s);
    if (t1 < t0) { p->fp = pfftss_execute_pdft_2d_s; t0 = t1; }

#ifdef HAVE_MPI2
    t1 = test_run(p, pfftss_execute_pdft_2d_p);
    if (t1 < t0) { p->fp = pfftss_execute_pdft_2d_p; t0 = t1; }
    t1 = test_run(p, pfftss_execute_pdft_2d_p);
    if (t1 < t0) { p->fp = pfftss_execute_pdft_2d_p; t0 = t1; }
#endif

  } else {

    p->ssize = (int *)malloc(sizeof(int) * p->npe);
    p->rsize = (int *)malloc(sizeof(int) * p->npe);
    p->soff = (int *)malloc(sizeof(int) * (p->npe + 1));
    p->roff = (int *)malloc(sizeof(int) * (p->npe + 1));
    
    for (i = 0; i < p->npe; i++) {
      p->ssize[i] = p->plx * ly * 2;
      p->rsize[i] = (p->oyp[i + 1] - p->oyp[i]) * p->plx * 2;
    }
    
    p->soff[0] = 0; p->roff[0] = 0;
    for (i = 0; i < p->npe; i++) {
      p->soff[i + 1] = p->soff[i] + p->ssize[i];
      p->roff[i + 1] = p->roff[i] + p->rsize[i];
    }

#ifdef HAVE_MPI_ALLOC_MEM    
    MPI_Alloc_mem(p->soff[p->npe] * sizeof(double), MPI_INFO_NULL, &(p->sb));
    MPI_Alloc_mem(p->roff[p->npe] * sizeof(double), MPI_INFO_NULL, &(p->rb));
#else
    p->sb = (double *)fftss_malloc(p->soff[p->npe] * sizeof(double));
    p->rb = (double *)fftss_malloc(p->roff[p->npe] * sizeof(double));
#endif

    p->rroff = (int *)malloc(sizeof(int) * p->npe);
    p->rsoff = (int *)malloc(sizeof(int) * p->npe);
    MPI_Alltoall(p->roff, 1, MPI_INT, p->rroff, 1, MPI_INT, comm);
    MPI_Alltoall(p->soff, 1, MPI_INT, p->rsoff, 1, MPI_INT, comm);

#ifdef HAVE_MPI2
    MPI_Win_create(p->sb, p->soff[p->npe] * sizeof(double), sizeof(double),
		   MPI_INFO_NULL, comm, &(p->swin));
    MPI_Win_create(p->rb, p->roff[p->npe] * sizeof(double), sizeof(double),
		   MPI_INFO_NULL, comm, &(p->rwin));
#endif
    
    p->fp = pfftss_execute_pdft_2d_va;
    t0 = test_run(p, pfftss_execute_pdft_2d_va);
    t1 = test_run(p, pfftss_execute_pdft_2d_va);
    if (t1 < t0) { t0 = t1; }
    t1 = test_run(p, pfftss_execute_pdft_2d_vs);
    if (t1 < t0) { p->fp = pfftss_execute_pdft_2d_vs; t0 = t1; }
    t1 = test_run(p, pfftss_execute_pdft_2d_vs);
    if (t1 < t0) { p->fp = pfftss_execute_pdft_2d_vs; t0 = t1; }

#ifdef HAVE_MPI2
    t1 = test_run(p, pfftss_execute_pdft_2d_vp);
    if (t1 < t0) { p->fp = pfftss_execute_pdft_2d_vp; t0 = t1; }
    t1 = test_run(p, pfftss_execute_pdft_2d_vp);
    if (t1 < t0) { p->fp = pfftss_execute_pdft_2d_vp; t0 = t1; }
#endif
  }

  return p;
}
