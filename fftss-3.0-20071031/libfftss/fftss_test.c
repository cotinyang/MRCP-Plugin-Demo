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
#include <malloc.h>
#include <math.h>
#include "libfftss.h"

void fftss_set_testdata(fftss_plan_1d p)
{
  long i;
  for (i = 0; i < p->n; i++) {
    p->in[2 * i] = (double)(i + 1);
    p->in[2 * i + 1] = (double)(p->n - i);
  }
}

static int
fftss_test_kset(fftss_plan_1d p, fftss_kernel *kset4, fftss_kernel *kset8,
		long table_type, long kset_id, char *name, double *ans)
{
  long i, j;
  double t;
  int status = 0;

  for (i = 0; i < FFTSS_MAP_MAX; i++) {
    double err = 0.0;

    if (fftss_map_list[i](p, p->logn2) < 0) continue;
    fftss_use_kset(p, kset4, kset8);
    fftss_execute_dft_1d(p, p->in, p->out);

    fftss_set_testdata(p);
    t = fftss_get_wtime();
    fftss_execute_dft_1d(p, p->in, p->out);
    t = fftss_get_wtime() - t;

    err = 0.0;
    if (p->flags & FFTSS_INOUT) 
      for (j = 0; j < p->n * 2; j++)
	err += fabs(ans[j] - p->in[j]);
    else
      for (j = 0; j < p->n * 2; j++)
	err += fabs(ans[j] - p->out[j]);
   
    printf("%16s [table=%ld,code=%ld,map=%ld]\tt=%lf,err=%le\n", 
	   name, table_type, kset_id, i, t, err);
    if (fabs(err) > (double)(p->n) * 0.00000001) status++;
    
    if (p->fastest < 0 || t < p->fastest) {
      p->map_id = i;
      p->table_type = table_type;
      p->kset_id = kset_id;
      p->fastest = t;
    }
  }
  return status;
}

int fftss_test_dft_1d(long n, double *in, double *out, 
		     long sign, long flags)
{
  fftss_plan pp;
  fftss_plan_1d p;
  long i, k;
  long logn2;
  long tbl_type;
  double *ans;
  int available;
  int status = 0;

  printf("Testing %ld-point %s transform....\n",
	 n, sign == FFTSS_FORWARD ? "forward" : "backward");

  pp = malloc(sizeof(fftss_plan_s));
  pp->fp = NULL;
  p = malloc(sizeof(fftss_plan_1d_s));
  pp->p1 = p;
  pp->p2 = NULL;
  pp->p3 = NULL;
  p->n = n;
  p->in = in;
  p->out = out;
  p->flags = flags;
  p->sign = sign;
  if ((p->flags & FFTSS_INOUT) || (in == out))
    p->fp = fftss_execute_inplace_dft_1d;
  else
    p->fp = fftss_execute_dft_1d;
  if (((size_t)in & 0xf) || ((size_t)out & 0xf))
    p->flags |= FFTSS_UNALIGNED;

  logn2 = 0;
  for (k = n; k > 1; k >>= 1, logn2 ++);
  if (n != 1 << logn2) {
    printf("n=%ld: not supported.\n", n);
    free(p);
    return -1;
  }
  p->logn2 = logn2;
  if (logn2 < 2) { free(p); return 0; };
  p->stages = 0;
  p->k = malloc(sizeof(fftss_kern) * p->logn2);
  p->w = fftss_malloc(sizeof(double) * n * 2);

  p->fastest = -1.0;

  ans = fftss_malloc(sizeof(double) * n * 2);

  fftss_map_list[0](p, p->logn2);
  if (sign == FFTSS_FORWARD)
    fftss_use_kset(p, fftss_kset_list[0].r4f, fftss_kset_list[0].r8f);
  else
    fftss_use_kset(p, fftss_kset_list[0].r4b, fftss_kset_list[0].r8b);
  fftss_table_types[FFTSS_TABLE_NORMAL].gen(p);

  fftss_set_testdata(p);

  fftss_execute(pp);

  if (p->flags & FFTSS_INOUT) 
    for (i = 0; i < p->n * 2; i++)
      ans[i] = p->in[i];
  else
    for (i = 0; i < p->n * 2; i++)
      ans[i] = p->out[i];

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
      status += 
	fftss_test_kset(p, fftss_kset_list[i].r4f, fftss_kset_list[i].r8f, 
			tbl_type, i, fftss_kset_list[i].name, ans);
    else
      status += 
	fftss_test_kset(p, fftss_kset_list[i].r4b, fftss_kset_list[i].r8b,
			tbl_type, i, fftss_kset_list[i].name, ans);
    i++;
  }
  
  printf("%s (map %ld) is selected\n", 
	 fftss_kset_list[p->kset_id].name, p->map_id);
  fftss_free(ans);
  fftss_free(p->w);
  free(p->k);
  free(p);
  free(pp);

  if (status)
    printf("%d tests failed.\n", status);

  return status;
}

