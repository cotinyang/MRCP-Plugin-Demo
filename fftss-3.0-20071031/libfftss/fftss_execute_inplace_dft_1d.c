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

void fftss_execute_inplace_dft_1d(fftss_plan_1d p, double *in, double *out)
{
  double *b0, *b1, *b2, *b3;
  long i;

  if (in == out) out = p->work;
  b0 = in;
  b1 = out;

  if (p->logn2 < 2) {
    if (p->logn2 == 1) {
      double xr0, xi0, xr1, xi1;
      xr0 = b0[0] + b0[2];	xi0 = b0[1] + b0[3];
      xr1 = b0[0] - b0[2];	xi1 = b0[1] - b0[3];
      b0[0] = xr0; b0[1] = xi0; b0[2] = xr1; b0[3] = xi1;
    }
    return;
  }

  if (p->stages & 1) {
    b1 = in;
    b2 = out;
  } else {
    b1 = out;
    b2 = in;
  }
       
  p->k[0].kern(b0, b1, p->w, p->k[0].bsize, p->k[0].blocks);

  for (i = 1; i < p->stages; i++) {
    p->k[i].kern(b1, b2, p->w, p->k[i].bsize, p->k[i].blocks);
    b3 = b2; b2 = b1; b1 = b3;
  }

}
