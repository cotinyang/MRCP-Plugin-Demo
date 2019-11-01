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
#include "libfftss.h"

static double d[4];

static void fftss_try_normal(void)
{
  long i;

  double d0, d1, d2, d3;

  d0 = d[0];
  d1 = d[1];
  d2 = d[2];
  d3 = d[3];
  
  for (i = 0; i < (1<<14); i++)
    d0 = d2 * (d1 * (d0 + d1 - d3));

  if (d0 > 1.0) printf("error in fftss_try_normal\n");
}

static void fftss_try_fma(void)
{
  long i;

  double d0, d1, d2;

  d0 = d[0];
  d1 = d[1];
  d2 = d[3];
  
  for (i = 0; i < (1<<14); i++)
    d0 = (d0 * d1 + d2) * d1 + d2;

  if (d0 > 1.0) printf("error in fftss_try_fma\n");
}

int fftss_test_fma(void)
{
  double t0, t1;
  int available;

  d[0] = 1.0; d[1] = 0.5; d[2] = 2.0; d[3] = 0.5;

  t0 = fftss_get_wtime();
  fftss_try_normal();
  t0 = fftss_get_wtime() - t0;

  t1 = fftss_get_wtime();
  fftss_try_fma();
  t1 = fftss_get_wtime() - t1;

  printf("normal: %lf sec.\tfma: %lf sec.\n", t0, t1);
  available = (t0 > t1 * 1.5);
  printf("FMA instructions are %s.\n", 
	 available ? "available" : "not available");

  return available;
}
