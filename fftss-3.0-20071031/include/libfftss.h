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
#ifndef _LIBFFTSS_H
#define _LIBFFTSS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#elif defined(_WIN32)
#include "win32config.h"
#endif

#include <math.h>

typedef struct {
  void (*kern)(double *, double *, double *, long, long);
  long bsize, blocks;
  long radix;
} fftss_kern;

typedef struct _fftss_plan_1d_s {
  void (*fp)(struct _fftss_plan_1d_s *, double *, double *);
  double *in, *out;
  double *w;
  double *work;
  long n, logn2;
  long stages;
  long flags;
  long sign;
  long map_id, table_type, kset_id;
  double fastest;
  fftss_kern *k;
} fftss_plan_1d_s;

typedef fftss_plan_1d_s *fftss_plan_1d;

typedef struct _fftss_plan_s {
  void (*fp)(struct _fftss_plan_s *, double *, double *);
  double *in, *out;
  long nx, ny, nz;
  long py, pz;
  long flags;
  double **b;
  double *work;
  fftss_plan_1d p1, p2, p3;
} fftss_plan_s;

typedef fftss_plan_s *fftss_plan;

extern void fftss_copyin(double *, double *, long, long);
extern void fftss_copyout(double *, double *, long, long);

#ifndef _OPENMP
#define omp_get_max_threads()   1
#define omp_get_thread_num()    0
#endif


#define FFTSS_FORWARD   -1
#define FFTSS_BACKWARD  1

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

#ifndef M_PI
#define M_PI 3.14159265358979323846 
#endif

extern int fftss_version(char *);
extern void fftss_execute(fftss_plan);
extern void fftss_execute_dft_1d(fftss_plan_1d, double *, double *);
extern void fftss_execute_inplace_dft_1d(fftss_plan_1d, double *, double *);
extern void *fftss_malloc(long);
extern void fftss_free(void *);
extern fftss_plan fftss_plan_dft_1d(long, double *, double *, long, long);
extern fftss_plan fftss_plan_dft_2d(long, long, long, double *, double *, long, long);
extern fftss_plan fftss_plan_dft_3d(long, long, long, long, long, double *, double *, long, long);
extern void fftss_set(fftss_plan, double *, double *);
extern void fftss_destroy_plan(fftss_plan);
extern double fftss_get_wtime(void);
extern int fftss_test_fma(void);


extern int fftss_verbose;

typedef void (*fftss_kernel)(double*, double*, double*, long, long);
enum{ KSET_NORMAL = 0, KSET_ONE, KSET_UNROLL1, KSET_UNROLL4, KSET_UNROLL8 };

typedef struct {
  long table_type;
  char *name;
  int required, enabled;
  fftss_kernel *r4f, *r8f;
  fftss_kernel *r4b, *r8b;
} fftss_kset;

typedef struct {
  char *name;
  void (*gen)(fftss_plan_1d);
} fftss_table_type;
enum { FFTSS_TABLE_NORMAL = 0, FFTSS_TABLE_FMA };

extern fftss_table_type fftss_table_types[];
extern fftss_kset fftss_kset_list[];

extern double *fftss_table_alloc(long, long);
extern void fftss_table_add(long, long, double *);

#define FFTSS_MAP_MAX 4
typedef int (*fftss_map)(fftss_plan_1d, long);
extern fftss_map fftss_map_list[];

extern void fftss_use_kset(fftss_plan_1d, fftss_kernel *, fftss_kernel *);

extern int fftss_cpu;
extern void fftss_check_cpu(void);

#define FFTSS_ANY        (0<<0)
#define FFTSS_X86_INTEL  (1<<0)
#define FFTSS_X86_AMD    (1<<1)

#define FFTSS_X86_MMX    (1<<4)
#define FFTSS_X86_SSE    (1<<5)
#define FFTSS_X86_SSE2   (1<<6)
#define FFTSS_X86_SSE3   (1<<7)
#define FFTSS_FMA        (1<<8)

#define FFTSS_SIMD       (FFTSS_X86_SSE2|FFTSS_X86_SSE3)
#define FFTSS_ALIGN      (1<<20)

#endif

