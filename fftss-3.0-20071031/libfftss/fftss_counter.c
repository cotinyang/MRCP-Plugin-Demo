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

#ifdef _WIN32
#include <windows.h>

static LARGE_INTEGER fftss_last;
static LARGE_INTEGER fftss_counter = {0, 0};

void fftss_counter_clear(void)
{
  fftss_counter.QuadPart = -1;
}

void fftss_counter_start(void)
{
  QueryPerformanceCounter(&fftss_last);
}

long fftss_counter_end(void)
{
  LARGE_INTEGER current;

  QueryPerformanceCounter(&current);
  
  current.QuadPart -= fftss_last.QuadPart;
  if ((fftss_counter.QuadPart < 0) || 
      (current.QuadPart < fftss_counter.QuadPart)) {
    fftss_counter.QuadPart = current.QuadPart;
    return 1;
  }
  return 0;
}
#elif defined(__ia64__) && defined(__INTEL_COMPILER) && defined(HAVE_IA64INTRIN_H)
#include <ia64intrin.h>

static unsigned __int64 fftss_counter, fftss_last;

void fftss_counter_clear(void)
{
  fftss_counter = (unsigned __int64)1 << 62;
}

void fftss_counter_start(void)
{
  fftss_last = __getReg(_IA64_REG_AR_ITC);
}

long fftss_counter_end(void)
{
  fftss_last = __getReg(_IA64_REG_AR_ITC) - fftss_last;
  if (fftss_last < fftss_counter) {
    fftss_counter = fftss_last;
    return 1;
  }
  return 0;
}

#elif (defined(__i386__) || defined(__x86_64__)) && defined(HAVE_IA32INTRIN_H) && defined(__INTEL_COMPILER)
#include <ia32intrin.h>
static __int64 fftss_last, fftss_counter;

void fftss_counter_clear(void)
{
  fftss_counter = (__int64)1 << 62;
}

void fftss_counter_start(void)
{
  fftss_last = _rdtsc();
}

long fftss_counter_end(void)
{
  fftss_last = _rdtsc() - fftss_last;
  if (fftss_last < fftss_counter) {
    fftss_counter = fftss_last;
    return 1;
  }
  return 0;
}

#elif defined(__i386__) && !defined(NO_ASM)
static unsigned long long fftss_last, fftss_counter;

void fftss_counter_clear(void)
{
  fftss_counter = (unsigned long long)1 << 62;
}

unsigned long long rdtsc(void)
{
  __asm__ ("rdtsc");
  return;
}

void fftss_counter_start(void)
{
  fftss_last = rdtsc();
}

long fftss_counter_end(void)
{
  fftss_last = rdtsc() - fftss_last;
  if (fftss_last < fftss_counter) {
    fftss_counter = fftss_last;
    return 1;
  }
  return 0;
}

#elif defined(__x86_64__) && !defined(NO_ASM)
static long fftss_last, fftss_counter;

void fftss_counter_clear(void)
{
  fftss_counter = (long)1 << 62;
}

long rdtsc(void)
{
  long n;
  asm volatile ("rdtsc\n"
		"shlq $32,%%rdx\n"
		"orq %%rdx,%%rax"
		: "=a" (n) :: "%rdx");
  return n;
}

void fftss_counter_start(void)
{
  fftss_last = rdtsc();
}

long fftss_counter_end(void)
{
  fftss_last = rdtsc() - fftss_last;
  if (fftss_last < fftss_counter) {
    fftss_counter = fftss_last;
    return 1;
  }
  return 0;
}

#elif defined(__powerpc__) && !defined(NO_ASM)

static unsigned long long fftss_last, fftss_counter;

void fftss_counter_clear(void)
{
  fftss_counter = ((unsigned long long)1) << 62;
}

#if defined(__xlc__) && (defined(__IBMC__) && (__IBMC__>=800))

unsigned long long ppc_time(void)
{
  int h, l, t;

  do {
    h = __mftbu();
    l = __mftb();
    t = __mftbu();
  } while (h != t);

  return ((unsigned long long)h << 32 ) | l;
}

#else

#pragma noinline
unsigned long long ppc_time(void)
{
  __asm__ __volatile__ ("L0:\n"
      "mftbu 3\n"
      "mftb 4\n"
      "mftbu 5\n"
      "cmpw 5,3\n"
      "bne L0\n");
}

#endif

void fftss_counter_start(void)
{
  fftss_last = ppc_time();
}

long fftss_counter_end(void)
{
  fftss_last = ppc_time() - fftss_last;
  if (fftss_last < fftss_counter) {
    fftss_counter = fftss_last;
    return 1;
  }
  return 0;
}

#else
#include <sys/time.h>

static struct timeval fftss_last;
long fftss_counter;

void fftss_counter_clear(void)
{
  fftss_counter = -1;
}

void fftss_counter_start(void)
{
  gettimeofday(&fftss_last, NULL);
}

long fftss_counter_end(void)
{
  struct timeval current;
  long t;

  gettimeofday(&current, NULL);
  t = (current.tv_sec - fftss_last.tv_sec) * 1000000 
    + (current.tv_usec - fftss_last.tv_usec);
  if (fftss_counter < 0 || 
      t < fftss_counter) {
    fftss_counter = t;
    return 1;
  }

  return 0;
}

#endif
  
  
