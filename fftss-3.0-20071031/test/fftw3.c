#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "fftw3compat.h"  
#include <math.h>

#ifndef FFTW_VERBOSE
#define FFTW_VERBOSE 0
#endif

double my_get_wtime(void);

int test(long logn)
{
  long i;
  fftw_complex *b0, *b1;
  long n;
  fftw_plan p0, p1;
  double t0, t1;
  long tries = 30;

  n = 1 << logn;

  b0 = fftw_malloc(sizeof(fftw_complex) * n);
  b1 = fftw_malloc(sizeof(fftw_complex) * n);
  p0 = fftw_plan_dft_1d(n, b0, b1, FFTW_BACKWARD, FFTW_MEASURE);
  p1 = fftw_plan_dft_1d(n, b1, b0, FFTW_FORWARD, FFTW_MEASURE);

#ifdef _Complex_I
  for (i = 0; i < n; i++)
    b0[i] = (double)(i + 1) + (double)(n - i) * _Complex_I;
  for (i = 0; i < n; i++) b1[i] = 0.0;
#else
  for (i = 0; i < n; i++) {
    b0[i][0] = (double)i + 1.0;
    b0[i][1] = (double)(n - i);
  }
  for (i = 0; i < n; i++) b1[i][0] = b1[i][1] = 0.0;
#endif


  t0 = fftss_get_wtime();
  for (i = 0; i < tries; i++) {
    fftw_execute(p0);
    fftw_execute(p1);
  }
  t1 = fftss_get_wtime() - t0;

  t0 = 5.0 * (double)n * log((double)n) / log(2.0) * 2.0 * (double)tries;
  
  printf("[%5ld]%lf sec. %lf Mflops.", n,
    t1 / (double)tries, t0 / t1 * 0.000001);
  
  t0 = (double)1.0 / (double)n;
  t1 = 1.0;
  for (i = 0; i < tries; i++) t1 *= t0;

#ifdef _Complex_I
  for (i = 0; i < n; i++)
    b0[i] *= t1;
  
  for (i = 0; i < n; i++)
    b0[i] -= (double)(i + 1) + (double)(n - i) * _Complex_I;

  for (i = 0; i < n; i++)
    b0[i] = fabs(creal(b0[i])) + fabs(cimag(b0[i])) * _Complex_I;

  {
    double max_err = 0.0;

    for (i = 0; i < n; i++) {
      if (creal(b0[i]) > max_err) max_err = creal(b0[i]);
      if (cimag(b0[i]) > max_err) max_err = cimag(b0[i]);
    }
    printf("Max error=%le\n", max_err);
  }
#else
  for (i = 0; i < n; i++) {
    b0[i][0] *= t1;
    b0[i][1] *= t1;
  }
  
  for (i = 0; i < n; i++) {
    b0[i][0] -= (double)i + 1.0;
    b0[i][1] -= (double)(n - i);
  }
  for (i = 0; i < n; i++) {
    b0[i][0] = fabs(b0[i][0]);
    b0[i][1] = fabs(b0[i][1]);
  }

  {
    double max_err = 0.0;

    for (i = 0; i < n; i++) {
      if (b0[i][0] > max_err) max_err = b0[i][0];
      if (b0[i][1] > max_err) max_err = b0[i][1];
    }
    printf("Max error=%le\n", max_err);
  }
#endif

  fftw_destroy_plan(p0);
  fftw_destroy_plan(p1);
  fftw_free(b0);
  fftw_free(b1);

  return 0;
}

int main(int argc, char **argv)
{
  long i;

  for (i = 1; i <= 16; i++)
    test(i);

  return 0;
}

#ifdef WIN32
#include <windows.h>

static LARGE_INTEGER t = {0, 0};

double my_get_wtime(void)
{
  LARGE_INTEGER l;
  if (t.QuadPart == 0)
    QueryPerformanceFrequency(&t);
  QueryPerformanceCounter(&l);
  return (double)l.QuadPart / (double)t.QuadPart;
}

#else
#include <sys/time.h>

double my_get_wtime(void)
{
  struct timeval tv;

  gettimeofday(&tv, NULL);
  return (double)tv.tv_sec + (double)tv.tv_usec * 0.000001;
}

#endif

