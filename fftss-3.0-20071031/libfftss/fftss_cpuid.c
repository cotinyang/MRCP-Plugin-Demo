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

#ifdef HAVE_X86_CPUID

int fftss_cpu = -1;

#ifdef HAVE_INTRIN_H
#define fftss_x86_cpuid(o, e) __cpuid(e, o)

#elif defined(_WIN64) && !defined(__INTEL_COMPILER)
/*
 *  Microsoft Visual Studio .NET 2003 does not support __asm keyword for EM64T/AMD64. 
 */
void fftss_x86_cpuid(int op, int *e_x) {
	fftss_cpu = FFTSS_X86_AMD|FFTSS_X86_MMX|FFTSS_X86_SSE|FFTSS_X86_SSE2;
};

#elif defined(_WIN32) && defined(_MSC_VER)
void fftss_x86_cpuid(int op, int *e_x)
{
    int ex[4];

	__asm {
		mov eax,op
		cpuid	
		mov dword ptr [ex+0],eax
		mov dword ptr [ex+4],ebx
		mov dword ptr [ex+8],ecx
		mov dword ptr [ex+12],edx
	}
	e_x[0] = ex[0];
	e_x[1] = ex[1];
	e_x[2] = ex[2];
	e_x[3] = ex[3];
}
#else
void fftss_x86_cpuid(int op, int *e_x)
{
  __asm__ ("cpuid"
	   : "=a" (e_x[0]),
	   "=b" (e_x[1]),
	   "=c" (e_x[2]),
	   "=d" (e_x[3])
	   : "a" (op));
}
#endif

void fftss_check_cpu(void)
{
  int e_x[4];

  fftss_cpu = 0;

  fftss_x86_cpuid(0, e_x);
/*  printf("%c\n", *(char *)e_x); */
  if (*(char *)e_x == 'G') fftss_cpu |= FFTSS_X86_INTEL;
  else fftss_cpu |= FFTSS_X86_AMD;

  fftss_x86_cpuid(1, e_x);
  if ((e_x[3] >> 23) & 1) fftss_cpu |= FFTSS_X86_MMX;
  if ((e_x[3] >> 25) & 1) fftss_cpu |= FFTSS_X86_SSE;
  if ((e_x[3] >> 26) & 1) fftss_cpu |= FFTSS_X86_SSE2;
  if ((e_x[2] >> 0) & 1) fftss_cpu |= FFTSS_X86_SSE3;
}

#else

int fftss_cpu = 0;
void fftss_check_cpu(void){};

#endif

