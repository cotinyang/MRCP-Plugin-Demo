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


#define DECL_KERNEL(k) \
extern void fftss_ ## k ## _f(double*,double*,double*,long,long); \
extern void fftss_ ## k ## _b(double*,double*,double*,long,long);

#define DECL_KERNEL_SET(k,n,o,u1,u4,u8) \
DECL_KERNEL(k ## _n)\
DECL_KERNEL(k ## _o)\
DECL_KERNEL(k ## _u1)\
DECL_KERNEL(k ## _u4)\
DECL_KERNEL(k ## _u8)\
static fftss_kernel fftss_ ## k ## _f[] = {\
  fftss_ ## k ## _ ## n ## _f,\
  fftss_ ## k ## _ ## o ## _f,\
  fftss_ ## k ## _ ## u1 ## _f,\
  fftss_ ## k ## _ ## u4 ## _f,\
  fftss_ ## k ## _ ## u8 ## _f};\
static fftss_kernel fftss_ ## k ## _b[] = {\
  fftss_ ## k ## _ ## n ## _b,\
  fftss_ ## k ## _ ## o ## _b,\
  fftss_ ## k ## _ ## u1 ## _b,\
  fftss_ ## k ## _ ## u4 ## _b,\
  fftss_ ## k ## _ ## u8 ## _b};\


DECL_KERNEL_SET(r4,n,o,u1,u4,o)
DECL_KERNEL_SET(r8,n,o,u1,o,o)

DECL_KERNEL_SET(r4_fma,n,o,u1,u4,o)
DECL_KERNEL_SET(r8_fma,n,o,u1,o,o)

#if defined(__ia64__) && !defined(NO_ASM)

DECL_KERNEL_SET(r4_ia64,o,o,o,o,o)
DECL_KERNEL_SET(r8_ia64,n,n,n,n,n)

#endif

#ifdef BlueGene

DECL_KERNEL_SET(r4_bgl,n,o,u1,n,n)
DECL_KERNEL_SET(r8_bgl,n,o,u1,n,n)
DECL_KERNEL_SET(r4_bgl_pl,n,n,u1,u4,n)

#if !defined(NO_ASM) && !defined(USE_WRAP440D)

DECL_KERNEL_SET(r4_bgl_asm,n,n,u1,u4,n)

#endif
#endif

#ifdef USE_SSE2

DECL_KERNEL_SET(r4_sse2,o,o,u1,u4,o);
DECL_KERNEL_SET(r4_sse2_1,n,n,n,n,n);
DECL_KERNEL_SET(r8_sse2,n,o,u1,n,n);

DECL_KERNEL(radix4_sse2_unroll1);
DECL_KERNEL(radix4_sse2_1);
DECL_KERNEL(radix8_sse2_one);
DECL_KERNEL(radix8_sse2_unroll1);

#endif

#ifdef USE_SSE3

DECL_KERNEL_SET(r4_sse3,n,o,u1,u4,o);
DECL_KERNEL_SET(r4_sse3_h,n,o,u1,n,n);
#if 0
#if defined(__x86_64__) && !defined(NO_ASM)
DECL_KERNEL_SET(r4_conroe,n,o,u1,u4,o);
DECL_KERNEL_SET(r8_conroe,n,n,n,n,n);
#endif
#endif
#endif

#if defined(USE_COMPLEX)
DECL_KERNEL_SET(r4_cmplx,n,n,n,n,n);
#endif

fftss_kset fftss_kset_list[] = {
  {FFTSS_TABLE_NORMAL, "normal", FFTSS_ANY, 1,
   fftss_r4_f, fftss_r8_f, fftss_r4_b, fftss_r8_b},
#if defined(USE_COMPLEX)
  {FFTSS_TABLE_NORMAL, "C99 complex", FFTSS_ALIGN, 1,
   fftss_r4_cmplx_f, fftss_r8_f, fftss_r4_cmplx_b, fftss_r8_b},
#endif
#ifdef USE_SSE2
  { FFTSS_TABLE_NORMAL, "SSE2 (1)", FFTSS_X86_SSE2|FFTSS_ALIGN, 1,
    fftss_r4_sse2_f, fftss_r8_sse2_f, fftss_r4_sse2_b, fftss_r8_sse2_b},
  { FFTSS_TABLE_NORMAL, "SSE2 (2)", FFTSS_X86_SSE2|FFTSS_ALIGN, 1,
    fftss_r4_sse2_1_f, fftss_r8_sse2_f, fftss_r4_sse2_1_b, fftss_r8_sse2_b},
#endif
#ifdef USE_SSE3
  { FFTSS_TABLE_NORMAL, "SSE3", 
    FFTSS_X86_SSE2|FFTSS_X86_SSE3|FFTSS_ALIGN, 1,
    fftss_r4_sse3_f, fftss_r8_sse2_f, fftss_r4_sse3_b, fftss_r8_sse2_b},
  { FFTSS_TABLE_NORMAL, "SSE3 (H)", 
    FFTSS_X86_SSE2|FFTSS_X86_SSE3|FFTSS_ALIGN, 1,
    fftss_r4_sse3_h_f, fftss_r8_sse2_f, fftss_r4_sse3_h_b, fftss_r8_sse2_b}, 
#if 0
#if defined(__x86_64__) && !defined(NO_ASM)
  { FFTSS_TABLE_NORMAL, "Conroe",
    FFTSS_X86_SSE2|FFTSS_X86_SSE3|FFTSS_ALIGN, 1,
    fftss_r4_conroe_f, fftss_r8_conroe_f, fftss_r4_conroe_b, fftss_r8_conroe_b},
#endif
#endif
#endif
  { FFTSS_TABLE_FMA, "FMA", FFTSS_ANY, 1,
    fftss_r4_fma_f, fftss_r8_fma_f, fftss_r4_fma_b, fftss_r8_fma_b},
#if defined(__ia64__) && !defined(NO_ASM)
  { FFTSS_TABLE_FMA, "IA-64 asm", FFTSS_ALIGN, 1,
    fftss_r4_ia64_f, fftss_r8_ia64_f, fftss_r4_ia64_b, fftss_r8_ia64_b},
#endif
#if defined(BlueGene)
  { FFTSS_TABLE_FMA, "Blue Gene", FFTSS_ANY, 1,
    fftss_r4_bgl_f, fftss_r8_bgl_f, fftss_r4_bgl_b, fftss_r8_bgl_b},
  { FFTSS_TABLE_FMA, "Blue Gene (PL)", FFTSS_ANY, 1,
    fftss_r4_bgl_pl_f, fftss_r8_bgl_f, fftss_r4_bgl_pl_b, fftss_r8_bgl_b},
#if !defined(NO_ASM) && !defined(USE_WRAP440D)
  { FFTSS_TABLE_FMA, "Blue Gene asm", FFTSS_ANY, 1,
    fftss_r4_bgl_asm_f, fftss_r8_bgl_f, fftss_r4_bgl_asm_b, fftss_r8_bgl_b},
#endif
#endif
  { -1, NULL, 0, 0, NULL, NULL, NULL, NULL },
};

extern void fftss_normal_table(fftss_plan_1d);
extern void fftss_fma_table(fftss_plan_1d);

fftss_table_type fftss_table_types[] = {
  { "Normal Table", fftss_normal_table },
  { "FMA Table", fftss_fma_table },
  { NULL, NULL }
};


