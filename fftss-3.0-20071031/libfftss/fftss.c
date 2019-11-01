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
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "libfftss.h"

extern void fftss_counter_clear(void);
extern void fftss_counter_start(void);
extern long fftss_counter_end(void);

int fftss_verbose = 0;

int fftss_map448(fftss_plan_1d p, long logn2)
{
  long i;
  long logn4, logn8;

  logn8 = logn2 & 1;
  logn4 = (logn2 - logn8 * 3) >> 1;
  for (i = 0; i < logn4; i++) p->k[i].radix = 4;
  for (i = 0; i < logn8; i++) p->k[i + logn4].radix = 8;
  p->stages = logn4 + logn8;

  return 0;
}

int fftss_map844(fftss_plan_1d p, long logn2)
{
  long i;
  long logn4, logn8;

  logn8 = logn2 & 1;
  logn4 = (logn2 - logn8 * 3) >> 1;
  for (i = 0; i < logn8; i++) p->k[i].radix = 8;
  for (i = 0; i < logn4; i++) p->k[i + logn8].radix = 4;
  p->stages = logn4 + logn8;

  if (logn8 == 0 || logn4 == 0) return -1;
  return 0;
}

int fftss_map488(fftss_plan_1d p, long logn2)
{
  long i;
  long logn4, logn8;

  logn4 = (3 - (logn2 % 3)) % 3;
  logn8 = (logn2 - logn4 * 2) / 3;
  for (i = 0; i < logn4; i++) p->k[i].radix = 4;
  for (i = 0; i < logn8; i++) p->k[i + logn4].radix = 8;
  p->stages = logn4 + logn8;
  
  return 0;
}

int fftss_map884(fftss_plan_1d p, long logn2)
{
  long i;
  long logn4, logn8;

  logn4 = (3 - (logn2 % 3)) % 3;
  logn8 = (logn2 - logn4 * 2) / 3;
  for (i = 0; i < logn8; i++) p->k[i].radix = 8;
  for (i = 0; i < logn4; i++) p->k[i + logn8].radix = 4;
  p->stages = logn4 + logn8;

  if (logn8 == 0 || logn4 == 0) return -1;
  return 0;
}

void fftss_use_kset(fftss_plan_1d p, fftss_kernel *kset4, fftss_kernel *kset8)
{
  fftss_kernel *kset;
  long i, j, k;

  j = p->n; k = 1;
  for (i = 0; i < p->stages; i++) {
    switch (p->k[i].radix) {
    case 4:
      kset = kset4;
      break;
    case 8:
      kset = kset8;
      break;
    }
      
    j /= p->k[i].radix;
    p->k[i].blocks = k;
    p->k[i].bsize = j;
    p->k[i].kern = j > 16 ? kset[KSET_ONE] :
      (j == 1 ? kset[KSET_UNROLL1] : 
       (j == 4 ? kset[KSET_UNROLL4] :
	(j == 8 ? kset[KSET_UNROLL8] : kset[KSET_NORMAL])));
    k *= p->k[i].radix;
  }
}

fftss_map fftss_map_list[] = 
  {fftss_map448, fftss_map844, fftss_map488, fftss_map884};

static void 
fftss_try_kset(fftss_plan_1d p, 
	       fftss_kernel *kset4, fftss_kernel *kset8,
	       long table_type, long kset_id, char *name)
{
  long i;

  if (fftss_verbose)
    printf("%16s [table=%ld,code=%ld]\t", name, table_type, kset_id);
  for (i = 0; i < FFTSS_MAP_MAX; i++) {
    if (fftss_map_list[i](p, p->logn2) < 0) continue;
    fftss_use_kset(p, kset4, kset8);
    (*(p->fp))(p, p->in, p->out);
    fftss_counter_start();
    (*(p->fp))(p, p->in, p->out);

    if (fftss_counter_end()) {
      p->map_id = i;
      p->table_type = table_type;
      p->kset_id = kset_id;
    }

  }
  if (fftss_verbose) printf("\n");
}


void fftss_plan_dft_1d_estimate(fftss_plan_1d p)
{
  char *kset_name = 
#if defined(USE_SSE3)
    "SSE3"
#elif defined(USE_SSE2)
    "SSE2 (1)"
#elif defined(__ia64__) && !defined(NO_ASM)
    "IA-64 asm"
#elif defined(BlueGene) && !defined(NO_ASM) && !defined(USE_WRAP440D)
    "Blue Gene asm"
#elif defined(BlueGene)
    "Blue Gene (PL)"
#else
    "FMA"
#endif
    ;
  
#ifdef HAVE_X86_CPUID
  if (fftss_cpu < 0) fftss_check_cpu();
  if (fftss_cpu & FFTSS_X86_AMD) kset_name = "FMA";
#endif

  for (p->kset_id = 0; fftss_kset_list[p->kset_id].name; p->kset_id++)
    if (strcmp(fftss_kset_list[p->kset_id].name, kset_name) == 0) break;

  if (fftss_kset_list[p->kset_id].name == NULL) {
    kset_name = "FMA";
    for (p->kset_id = 0; fftss_kset_list[p->kset_id].name; p->kset_id++)
      if (strcmp(fftss_kset_list[p->kset_id].name, kset_name) == 0) break;

    if (fftss_kset_list[p->kset_id].name == NULL) {
      printf("Internal Error in fftss_plan_dft_1d_estimate()\n");
      return;
    }
  }
  p->table_type = fftss_kset_list[p->kset_id].table_type;

  p->map_id = 0;
  
}

void fftss_plan_dft_1d_measure(fftss_plan_1d p)
{
  long i;
  long tbl_type;
  int available;

  /*  p->fastest = -1.0;  */
  fftss_counter_clear();
  if (fftss_cpu < 0) fftss_check_cpu();

  available = fftss_cpu;
  if (!(p->flags & FFTSS_UNALIGNED))
    available |= FFTSS_ALIGN;
  
  tbl_type = -1;
  i = 0;
  while (fftss_kset_list[i].name) {
    if (fftss_kset_list[i].enabled == 0) { i++; continue; }
    if ((p->flags & FFTSS_NO_SIMD) && 
      (fftss_kset_list[i].required & FFTSS_SIMD)) { i++; continue; }
    if ((fftss_kset_list[i].required & available) 
	!= fftss_kset_list[i].required) 
      { i++; continue; }
    if (fftss_kset_list[i].table_type != tbl_type) {
      tbl_type = fftss_kset_list[i].table_type;
      fftss_table_types[tbl_type].gen(p);
    }
    if (p->sign == FFTSS_FORWARD)
      fftss_try_kset(p, fftss_kset_list[i].r4f, fftss_kset_list[i].r8f,
		     tbl_type, i, fftss_kset_list[i].name);
    else
      fftss_try_kset(p, fftss_kset_list[i].r4b, fftss_kset_list[i].r8b,
		     tbl_type, i, fftss_kset_list[i].name);
    i++;
  }
  
}


fftss_plan fftss_plan_dft_1d(long n, double *in, double *out, 
			     long sign, long flags)
{
  fftss_plan pp;
  fftss_plan_1d p;
  long k;
  long logn2;
  double *tbl;

  fftss_verbose = flags & FFTSS_VERBOSE;

  if (fftss_verbose) 
    printf("Planning %ld-point %s transform....\n",
	   n, sign == FFTSS_FORWARD ? "forward" : "backward");

  pp = malloc(sizeof(fftss_plan_s));
  pp->fp = NULL;
  pp->p2 = NULL;
  pp->p3 = NULL;
  pp->nx = n;
  pp->ny = 0;
  pp->nz = 0;

  p = malloc(sizeof(fftss_plan_1d_s));
  pp->p1 = p;
  p->n = n;
  p->in = in;
  p->out = out;
  p->flags = flags;
  p->sign = sign;
  if ((p->flags & FFTSS_INOUT) || (in == out))
    p->fp = fftss_execute_inplace_dft_1d;
  else
    p->fp = fftss_execute_dft_1d;
  if (((size_t)in & 0xf) || ((size_t)out & 0xf)) {
    p->flags |= FFTSS_UNALIGNED;
    if (((size_t)in & 0x7) || ((size_t)out & 0x7)) 
      printf("Warning: input/output array should be aligned \n"
	     "to at least 8byte boundary.\n");
  }

  logn2 = 0;
  for (k = n; k > 1; k >>= 1, logn2 ++);
  if (n != 1 << logn2) {
    printf("n=%ld: not supported.\n", n);
    return NULL;
  }
  p->logn2 = logn2;
  p->k = NULL;
  p->work = NULL;
  if (logn2 < 2) return pp;
  p->stages = 0;
  p->k = malloc(sizeof(fftss_kern) * p->logn2);
  p->w = fftss_malloc(sizeof(double) * n * 2);
  if ((flags & FFTSS_PRESERVE_INPUT) || (in == out)) {
#if 0
    if ((tbl = fftss_table_alloc(p->n, -1)) != NULL) {
      p->work = tbl;
      if (p->flags & FFTSS_VERBOSE)
	printf("Using shared work space.\n");
    } else {
#endif
      p->work = fftss_malloc(sizeof(double) * n * 2);
#if 0
      fftss_table_add(p->n, -1, p->work);
    }
#endif
  } else p->work = NULL;

  if (flags & FFTSS_ESTIMATE)
    fftss_plan_dft_1d_estimate(p);
  else
    fftss_plan_dft_1d_measure(p);

  if (fftss_verbose) printf("%s (map %ld) is selected\n", 
			   fftss_kset_list[p->kset_id].name, p->map_id);

  if ((tbl = fftss_table_alloc(p->n, p->table_type)) != NULL) {
	  if (fftss_verbose)
       printf("Using shared table.\n");
    fftss_free(p->w);
    p->w = tbl;
  } else {
    /*    if (p->table_type != tbl_type) */
      fftss_table_types[p->table_type].gen(p);
    fftss_table_add(p->n, p->table_type, p->w);
  }
  
  fftss_map_list[p->map_id](p, p->logn2);
  if (sign == FFTSS_FORWARD) {
    fftss_use_kset(p, fftss_kset_list[p->kset_id].r4f, 
		  fftss_kset_list[p->kset_id].r8f);
  } else {
    fftss_use_kset(p, fftss_kset_list[p->kset_id].r4b, 
		  fftss_kset_list[p->kset_id].r8b);
  }

  
  return pp;
}

