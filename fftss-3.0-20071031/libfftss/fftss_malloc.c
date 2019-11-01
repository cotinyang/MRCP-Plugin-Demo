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

#if defined(HAVE_MEMALIGN) || defined(HAVE_POSIX_MEMALIGN)

void *fftss_malloc(long size)
{
#if defined(HAVE_MEMALIGN)
  return memalign(16, size);
#elif defined(HAVE_POSIX_MEMALIGN)
  void *ptr;
  posix_memalign(&ptr, 16, size);
  return ptr;
#endif
}

void fftss_free(void *p)
{
  free(p);
  return;
}

#else

typedef struct _malloc_address {
  struct _malloc_address *next, *prev;
  void *address, *real_address;
} malloc_address;

malloc_address fftss_malloc_address_top = 
  {&fftss_malloc_address_top, &fftss_malloc_address_top, NULL, NULL};

void *fftss_malloc(long size)
{
  union {
    void *ptr;
    size_t l;
  } addr;
  malloc_address *ma;

  addr.ptr = malloc(size);
  if (addr.l & 0xf) {
    free(addr.ptr);
	addr.ptr = malloc(size + 16);
    ma = malloc(sizeof(malloc_address));
    ma->next = &fftss_malloc_address_top;
	ma->prev = fftss_malloc_address_top.prev;
	ma->prev->next = ma;
	ma->next->prev = ma;
	ma->real_address = addr.ptr;
	addr.l += 0xf;
	addr.l &= ~0xf;
	ma->address = addr.ptr;
  }

/*    printf("warning: allocated memory is not aligned.\n");  */
  
  return addr.ptr;
}

void fftss_free(void *p)
{
  malloc_address *ma;

  ma = fftss_malloc_address_top.next;
  while (ma->address) {
    if (ma->address == p) break;
	ma = ma->next;
  }
  if (ma->address) {
    ma->next->prev = ma->prev;
	ma->prev->next = ma->next;
    free(ma->real_address);
	free(ma);
	return;
  }
  free(p);
  return;
}

#endif

