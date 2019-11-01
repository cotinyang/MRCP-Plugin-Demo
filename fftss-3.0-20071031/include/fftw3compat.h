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
#ifndef _FFTW3COMPAT_H
#define _FFTW3COMPAT_H      
#include "fftss.h"

#define fftw_plan            fftss_plan
#ifdef _Complex_I
typedef double _Complex fftw_complex;
#else
typedef double fftw_complex[2];
#endif

#define FFTW_FORWARD           FFTSS_FORWARD
#define FFTW_BACKWARD          FFTSS_BACKWARD

#define FFTW_MEASURE           FFTSS_MEASURE
#define FFTW_DESTROY_INPUT     FFTSS_DESTROY_INPUT
#define FFTW_UNALIGNED         FFTSS_UNALIGNED
#define FFTW_CONSERVE_MEMORY   FFTSS_CONSERVE_MEMORY
#define FFTW_EXHAUSTIVE        FFTSS_EXHAUSTIVE
#define FFTW_PATIENT           FFTSS_PATIENT
#define FFTW_ESTIMATE          FFTSS_ESTIMATE
#define FFTW_PRESERVE_INPUT    FFTSS_PRESERVE_INPUT

#define FFTW_NO_SIMD           FFTSS_NO_SIMD

#define fftw_malloc          fftss_malloc
#define fftw_free            fftss_free

#define fftw_plan_dft_1d(n,i,o,s,f)  \
        fftss_plan_dft_1d(n,(double*)(i),(double *)(o),s,f)
#define fftw_plan_dft_2d(nx,ny,i,o,s,f)  \
        fftss_plan_dft_2d(nx,ny,nx,(double*)(i),(double *)(o),s,f)
#define fftw_plan_dft_3d(nx,ny,nz,i,o,s,f)  \
        fftss_plan_dft_3d(nx,ny,nz,nx,(nx)*(ny),(double*)(i),(double *)(o),s,f)
#define fftw_destroy_plan    fftss_destroy_plan

#define fftw_execute         fftss_execute
#define fftw_execute_dft(p,i,o) \
        fftss_execute_dft(p,(double *)(i),(double *)(o))

#define fftw_init_threads	fftss_init_threads
#define fftw_cleanup_threads	fftss_cleanup_threads
#define fftw_plan_with_nthreads	fftss_plan_with_nthreads

#endif

