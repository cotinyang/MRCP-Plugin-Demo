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
#include "libfftss.h"

typedef struct _fftss_table {
  struct _fftss_table *prev, *next;
  long n;
  long type;
  double *w;
  long count;
} fftss_table;

fftss_table tables = {&tables, &tables, -1, -1, NULL, -1};

double *fftss_table_alloc(long n, long type)
{
  fftss_table *tbl;

  tbl = tables.next;
  while (tbl->count > 0) {
    if (tbl->n == n && tbl->type == type) {
      tbl->count++;
      return tbl->w;
    }
    tbl = tbl->next;
  }
  return NULL;
}

void fftss_table_add(long n, long type, double *w)
{
  fftss_table *tbl0, *tbl1;

  tbl0 = malloc(sizeof(fftss_table));
  tbl1 = tables.prev;
  tables.prev = tbl0;
  tbl1->next = tbl0;
  tbl0->next = &tables;
  tbl0->prev = tbl1;
  tbl0->n = n;
  tbl0->type = type;
  tbl0->w = w;
  tbl0->count = 1;
}

void fftss_table_free(long n, long type)
{
  fftss_table *tbl;
    
  tbl = tables.next;
  while (tbl->count > 0) {
    if (tbl->n == n && tbl->type == type) {
      tbl->count--;
      if (tbl->count == 0) {
	fftss_free(tbl->w);
	tbl->prev->next = tbl->next;
	tbl->next->prev = tbl->prev;
	free(tbl);
      }
      return;
    }
    tbl = tbl->next;
  }
  return;
}

void fftss_fma_table(fftss_plan_1d p)
{
  long i, n;

  n = p->n;

  for (i = 1; i < n * 3 / 4; i++) {
    double sin_i;
    sin_i = sin(2.0 * M_PI * (double)i / (double)n);
    p->w[2 * i] = cos(2.0 * M_PI * (double)i / (double)n) / sin_i;
    p->w[2 * i + 1] = sin_i;
  }
  for (i = 1; i < n / 4; i++) { 
    p->w[2 * i + n * 3 / 2] = p->w[i * 6];
    p->w[2 * i + n * 3 / 2 + 1] = p->w[i * 6 + 1] / p->w[i * 2 + 1];
  }

  {
    double d0, d1;
    
    d0 = 1.0 / 4.0;       /* 1 << -2 */
    d1 = (double)(1<<16); /* 1 << 16 */
    d1 = d1 * d1;         /* 1 << 32 */
    d1 = d1 * d1;         /* 1 << 64 */
    d1 = d1 * d1;         /* 1 << 128 */
    d1 = d1 * d1;         /* 1 << 256 */
#if 0 && defined(__ia64__)
    d1 = d1 * d1;         /* 1 << 512 */
    d0 = d0 * d1;         /* 1 << 510 */
#endif
#if 0
    d1 = d1 * d0;         /* 1 << 1022 */
#endif
    d0 = 1.0 / d1;        /* 1 << -1022 */
    
    p->w[0] = d1; p->w[n] = -d1;
    p->w[1] = p->w[n + 1] = d0;
    p->w[n * 3 / 2] = d1;
    p->w[n * 3 / 2 + 1] = 1.0;
  }
}

void fftss_normal_table(fftss_plan_1d p)
{
  long i, n;

  n = p->n;

  for (i = 0; i < n; i++) {
    p->w[2 * i] = cos(2.0 * M_PI * (double)i / (double)n);
    p->w[2 * i + 1] = sin(2.0 * M_PI * (double)i / (double)n);
  }
}
