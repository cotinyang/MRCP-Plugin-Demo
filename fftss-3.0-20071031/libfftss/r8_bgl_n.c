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

#ifdef USE_WRAP440D
#include "wrap440d.h"
#endif

static inline void 
kern_r8_bgl_n_f(double *restrict o0,
		 double *restrict o1,
		 double *restrict o2,
		 double *restrict o3,
		 double *restrict o4,
		 double *restrict o5,
		 double *restrict o6,
		 double *restrict o7,
		 double *i0, 
		 double *i1, 
		 double *i2, 
		 double *i3, 
		 double *i4, 
		 double *i5, 
		 double *i6, 
		 double *i7, 
		 double *wn, double *wn2,
		 long bsize, long blocks)
{
  long i, j;
  
  for (i = 0; i < blocks; i++) {

    double _Complex w1, w2, w3, w4, w5, w6, w7;

    w1 = __lfpd(wn + i * bsize * 2);
    w2 = __lfpd(wn + i * bsize * 4);
    w4 = __lfpd(wn + i * bsize * 8);
    w5 = __lfpd(wn + i * bsize * 2 + 2 * blocks * bsize);
    w3 = __lfpd(wn2 + i * bsize * 2);
    w6 = __lfpd(wn2 + i * bsize * 4);
    w7 = __lfpd(wn2 + i * bsize * 2 + blocks * bsize * 2);

    for (j = 0; j < bsize; j++) {

      double _Complex p0, p1, p2, p3, p4, p5, p6, p7;
      double _Complex q0, q1, q2, q3, q4, q5, q6, q7;
      double _Complex s1, s3, s5, s7;
      
      p5 = __lfpd(i5 + 2 * j);
      p7 = __lfpd(i7 + 2 * j);
      p4 = __lfpd(i4 + 2 * j);
      p2 = __lfpd(i2 + 2 * j);
      p6 = __lfpd(i6 + 2 * j);

      p1 = __lfpd(i1 + 2 * j);
      p5 = __fxcxnpma(p5, p5, __creal(w4));   /* [-pi5,pr5] */
      p3 = __lfpd(i3 + 2 * j);
      p7 = __fxcxnpma(p7, p7, __creal(w4));   /* [-pi7,pr7] */
      p0 = __lfpd(i0 + 2 * j);
      p4 = __fxcxnpma(p4, p4, __creal(w4));   /* [-pi4,pr4] */
      p2 = __fxcxnpma(p2, p2, __creal(w2));   /* [-pi2,pr2] */
      p6 = __fxcxnpma(p6, p6, __creal(w6));           /* [-pi6,pr6] */
      
      s1 = __fxcxnsma(p1, p5, __cimag(w4));   /* [sr1,si1] */
      s5 = __fxcxnpma(p1, p5, __cimag(w4));   /* [sr5,si5] */
      s3 = __fxcxnsma(p3, p7, __cimag(w4));   /* [sr3,si3] */
      s7 = __fxcxnpma(p3, p7, __cimag(w4));   /* [sr7,si7] */
      q0 = __fxcxnsma(p0, p4, __cimag(w4));   /* [qr0,qi0] */
      q4 = __fxcxnpma(p0, p4, __cimag(w4));   /* [qr4,qi4] */

      q2 = __fxcpmadd(p2, p6, __cimag(w6));           /* [-qi2,qr2] */
      q6 = __fxcpnmsub(p2, p6, __cimag(w6));          /* [-qi6,qr6] */
      
      q1 = __fxcxnpma(s1, s1, __creal(w1));     /* [-qi1,qr1] */
      q3 = __fxcxnpma(s3, s3, __creal(w3));             /* [-qi3,qr3] */
      q5 = __fxcxnpma(s5, s5, __creal(w5));     /* [-qi5,qr5] */
      q7 = __fxcxnpma(s7, s7, __creal(w7));             /* [-qi7,qr7] */
      
      p0 = __fxcxnsma(q0, q2, __cimag(w2));    /* [tr0,ti0] */
      p2 = __fxcxnpma(q0, q2, __cimag(w2));    /* [tr2,ti2] */
      p1 = __fxcpmadd(q1, q3, __cimag(w3));            /* [-ti1,tr1] */
      p3 = __fxcpnmsub(q1, q3, __cimag(w3));           /* [-ti3,tr3] */

      p4 = __fxcsnmsub(q4, q6, __cimag(w2));   /* [tr4,ti4] */
      p6 = __fxcsmadd(q4, q6, __cimag(w2));    /* [tr6,ti6] */      
      p5 = __fxcpmadd(q5, q7, __cimag(w7));            /* [-ti5,tr5] */
      p7 = __fxcpnmsub(q5, q7, __cimag(w7));           /* [-ti7,tr7] */

      q0 = __fxcxnsma(p0, p1, __cimag(w1));
      q1 = __fxcxnpma(p0, p1, __cimag(w1));
      q4 = __fxcxnsma(p4, p5, __cimag(w5));
      q5 = __fxcxnpma(p4, p5, __cimag(w5));
      __stfpd(o0 + 2 * j, q0);
      q2 = __fxcsnmsub(p2, p3, __cimag(w1));
      __stfpd(o4 + 2 * j, q1);
      q3 = __fxcsmadd(p2, p3, __cimag(w1));
      __stfpd(o1 + 2 * j, q4);
      q6 = __fxcsnmsub(p6, p7, __cimag(w5));
      __stfpd(o5 + 2 * j, q5);
      q7 = __fxcsmadd(p6, p7, __cimag(w5));
      __stfpd(o2 + 2 * j, q2);
      __stfpd(o6 + 2 * j, q3);
      __stfpd(o3 + 2 * j, q6);
      __stfpd(o7 + 2 * j, q7);
    }
    i0 += 16 * bsize; i1 += 16 * bsize;
    i2 += 16 * bsize; i3 += 16 * bsize;
    i4 += 16 * bsize; i5 += 16 * bsize;
    i6 += 16 * bsize; i7 += 16 * bsize;
    o0 += 2 * bsize; o1 += 2 * bsize;
    o2 += 2 * bsize; o3 += 2 * bsize;
    o4 += 2 * bsize; o5 += 2 * bsize;
    o6 += 2 * bsize; o7 += 2 * bsize;
  }
}

static inline void 
kern_r8_bgl_n_b(double *restrict o0,
		 double *restrict o1,
		 double *restrict o2,
		 double *restrict o3,
		 double *restrict o4,
		 double *restrict o5,
		 double *restrict o6,
		 double *restrict o7,
		 double *i0, 
		 double *i1, 
		 double *i2, 
		 double *i3, 
		 double *i4, 
		 double *i5, 
		 double *i6, 
		 double *i7, 
		 double *wn, double *wn2,
		 long bsize, long blocks)
{
  long i, j;

  for (i = 0; i < blocks; i++) {

    double _Complex w1, w2, w3, w4, w5, w6, w7;

    w1 = __lfpd(wn + i * bsize * 2);
    w2 = __lfpd(wn + i * bsize * 4);
    w4 = __lfpd(wn + i * bsize * 8);
    w5 = __lfpd(wn + i * bsize * 2 + 2 * blocks * bsize);
    w3 = __lfpd(wn2 + i * bsize * 2);
    w6 = __lfpd(wn2 + i * bsize * 4);
    w7 = __lfpd(wn2 + i * bsize * 2 + blocks * bsize * 2);


    for (j = 0; j < bsize; j++) {

      double _Complex p0, p1, p2, p3, p4, p5, p6, p7;
      double _Complex q0, q1, q2, q3, q4, q5, q6, q7;
      double _Complex s1, s3, s5, s7;
      
      p5 = __lfpd(i5 + 2 * j);
      p7 = __lfpd(i7 + 2 * j);
      p4 = __lfpd(i4 + 2 * j);
      p2 = __lfpd(i2 + 2 * j);
      p6 = __lfpd(i6 + 2 * j);

      p1 = __lfpd(i1 + 2 * j);
      p5 = __fxcxnsma(p5, p5, __creal(w4));   /* [-pi5,pr5] */
      p3 = __lfpd(i3 + 2 * j);
      p7 = __fxcxnsma(p7, p7, __creal(w4));   /* [-pi7,pr7] */
      p0 = __lfpd(i0 + 2 * j);
      p4 = __fxcxnsma(p4, p4, __creal(w4));   /* [-pi4,pr4] */
      p2 = __fxcxnsma(p2, p2, __creal(w2));   /* [-pi2,pr2] */
      p6 = __fxcxnsma(p6, p6, __creal(w6));           /* [-pi6,pr6] */
      
      s1 = __fxcxnpma(p1, p5, __cimag(w4));   /* [sr1,si1] */
      s5 = __fxcxnsma(p1, p5, __cimag(w4));   /* [sr5,si5] */
      s3 = __fxcxnpma(p3, p7, __cimag(w4));   /* [sr3,si3] */
      s7 = __fxcxnsma(p3, p7, __cimag(w4));   /* [sr7,si7] */
      q0 = __fxcxnpma(p0, p4, __cimag(w4));   /* [qr0,qi0] */
      q4 = __fxcxnsma(p0, p4, __cimag(w4));   /* [qr4,qi4] */

      q2 = __fxcpmadd(p2, p6, __cimag(w6));           /* [-qi2,qr2] */
      q6 = __fxcpnmsub(p2, p6, __cimag(w6));          /* [-qi6,qr6] */
      
      q1 = __fxcxnsma(s1, s1, __creal(w1));     /* [-qi1,qr1] */
      q3 = __fxcxnsma(s3, s3, __creal(w3));             /* [-qi3,qr3] */
      q5 = __fxcxnsma(s5, s5, __creal(w5));     /* [-qi5,qr5] */
      q7 = __fxcxnsma(s7, s7, __creal(w7));             /* [-qi7,qr7] */
      
      p0 = __fxcxnpma(q0, q2, __cimag(w2));    /* [tr0,ti0] */
      p2 = __fxcxnsma(q0, q2, __cimag(w2));    /* [tr2,ti2] */
      p1 = __fxcpmadd(q1, q3, __cimag(w3));            /* [-ti1,tr1] */
      p3 = __fxcpnmsub(q1, q3, __cimag(w3));           /* [-ti3,tr3] */

      p4 = __fxcsnmsub(q4, q6, __cimag(w2));   /* [tr4,ti4] */
      p6 = __fxcsmadd(q4, q6, __cimag(w2));    /* [tr6,ti6] */      
      p5 = __fxcpmadd(q5, q7, __cimag(w7));            /* [-ti5,tr5] */
      p7 = __fxcpnmsub(q5, q7, __cimag(w7));           /* [-ti7,tr7] */

      q0 = __fxcxnpma(p0, p1, __cimag(w1));
      q1 = __fxcxnsma(p0, p1, __cimag(w1));
      q4 = __fxcxnpma(p4, p5, __cimag(w5));
      q5 = __fxcxnsma(p4, p5, __cimag(w5));
      __stfpd(o0 + 2 * j, q0);
      q2 = __fxcsnmsub(p2, p3, __cimag(w1));
      __stfpd(o4 + 2 * j, q1);
      q3 = __fxcsmadd(p2, p3, __cimag(w1));
      __stfpd(o1 + 2 * j, q4);
      q6 = __fxcsnmsub(p6, p7, __cimag(w5));
      __stfpd(o5 + 2 * j, q5);
      q7 = __fxcsmadd(p6, p7, __cimag(w5));
      __stfpd(o2 + 2 * j, q2);
      __stfpd(o6 + 2 * j, q3);
      __stfpd(o3 + 2 * j, q6);
      __stfpd(o7 + 2 * j, q7);

    }
    i0 += 16 * bsize; i1 += 16 * bsize;
    i2 += 16 * bsize; i3 += 16 * bsize;
    i4 += 16 * bsize; i5 += 16 * bsize;
    i6 += 16 * bsize; i7 += 16 * bsize;
    o0 += 2 * bsize; o1 += 2 * bsize;
    o2 += 2 * bsize; o3 += 2 * bsize;
    o4 += 2 * bsize; o5 += 2 * bsize;
    o6 += 2 * bsize; o7 += 2 * bsize;
  }
}


void fftss_r8_bgl_n_f(double *in, double *out, double *w,
			 long bsize, long blocks)
{
  kern_r8_bgl_n_f(out, 
		   out + bsize * blocks * 2, 
		   out + bsize * blocks * 4, 
		   out + bsize * blocks * 6,
		   out + bsize * blocks * 8, 
		   out + bsize * blocks * 10,
		   out + bsize * blocks * 12, 
		   out + bsize * blocks * 14,
		   in, 
		   in + bsize * 2,
		   in + bsize * 4,
		   in + bsize * 6,
		   in + bsize * 8,
		   in + bsize * 10,
		   in + bsize * 12,
		   in + bsize * 14,
		   w, w + bsize * blocks * 12, bsize, blocks);
}

void fftss_r8_bgl_n_b(double *in, double *out, double *w,
		     long bsize, long blocks)
{
  kern_r8_bgl_n_b(out, 
		   out + bsize * blocks * 2, 
		   out + bsize * blocks * 4,
		   out + bsize * blocks * 6,
		   out + bsize * blocks * 8,
		   out + bsize * blocks * 10,
		   out + bsize * blocks * 12,
		   out + bsize * blocks * 14,
		   in,
		   in + bsize * 2,
		   in + bsize * 4,
		   in + bsize * 6,
		   in + bsize * 8,
		   in + bsize * 10,
		   in + bsize * 12,
		   in + bsize * 14,
		   w, w + bsize * blocks * 12, bsize, blocks);
}
		       
