#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "fftss.h"  
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif
extern int fftss_version(char *);
#ifdef __cplusplus
}
#endif

int testall()
{
  int status = 0;
  double *b0, *b1;
  long logn;
  long n;
  int ver;
  char name[16];

  ver = fftss_version(name);

  printf("FFTSS library version 2.0, update-%d(%s).\n\n", ver, name);

  for (logn = 1; logn <= 16; logn++) {
    n = 1 << logn;

    b0 = (double *)fftss_malloc(sizeof(double) * 2 * n);
    b1 = (double *)fftss_malloc(sizeof(double) * 2 * n);

    status += fftss_test_dft_1d(n, b0, b1, FFTSS_BACKWARD, 0);
    status += fftss_test_dft_1d(n, b1, b0, FFTSS_FORWARD, 0);

    fftss_free(b0);
    fftss_free(b1);
  }

  if (status) 
    printf("\nTotal %d test(s) failed.\n", status);
  else
    printf("\nAll tests passed in this library.\n");

  return status;
}

int test(long logn)
{
  long i, j;
  double *b0, *b1;
  long n = 1 << 10;
  fftss_plan p0, p1;
  double t0, t1, t2 = 0.0;
  long tries = 30;
  long global_tries = 10;
  double max_t, max_mflops = 0.0;
  double mflops;

  tries = 300 / logn ;
  n = 1 << logn;

  b0 = (double *)fftss_malloc(sizeof(double) * 2 * n);
  b1 = (double *)fftss_malloc(sizeof(double) * 2 * n);

#if 1
  p0 = fftss_plan_dft_1d(n, b0, b1, FFTSS_BACKWARD, FFTSS_MEASURE);
  p1 = fftss_plan_dft_1d(n, b1, b0, FFTSS_FORWARD, FFTSS_MEASURE);
#else
  p0 = fftss_plan_dft_1d(n, b0, b1, FFTSS_BACKWARD, FFTSS_VERBOSE);
  p1 = fftss_plan_dft_1d(n, b1, b0, FFTSS_FORWARD, FFTSS_VERBOSE);
#endif

  for (j = 0; j < global_tries; j++) {

    for (i = 0; i < n; i++) {
      b0[2 * i] = (double)i + 1.0;
      b0[2 * i + 1] = (double)(n - i);
    }
    for (i = 0; i < 2 * n; i++) b1[i] = 0.0;

    t0 = fftss_get_wtime();
    for (i = 0; i < tries; i++) {
      fftss_execute(p0);
      fftss_execute(p1);
    }
    t1 = fftss_get_wtime() - t0;


    t0 = 5.0 * (double)n * log((double)n) / log(2.0) * 2.0 * (double)tries;
  
    mflops = t0 / t1 * 0.000001;
    t1 /= (double)tries;

    if (mflops > max_mflops) {
      max_mflops = mflops;
      max_t = t1;
    }

  }

#if 0
  printf("[%5ld]%lf sec. %lf Mflops.\n", n,
	 max_t, max_mflops);
#else
  printf("[%5ld]%le sec. %lf Mflops.\n", n,
	 max_t, max_mflops);
#endif 
  t0 = (double)1.0 / (double)n;
  t1 = 1.0;
  for (i = 0; i < tries; i++) t1 *= t0;

  for (i = 0; i < 2 * n; i++) b0[i] *= t1;
  
  for (i = 0; i < n; i++) {
    b0[2 * i] -= (double)i + 1.0;
    b0[2 * i + 1] -= (double)(n - i);
  }
  for (i = 0; i < 2 * n; i++) b0[i] = fabs(b0[i]);

  {
    double max_err = 0.0;

    for (i = 0; i < n; i++) 
      if (b0[2 * i] > max_err) max_err = b0[2 * i];
    if (max_err > 1.0) printf("ERROR!\n");
  }

  fftss_destroy_plan(p0);
  fftss_destroy_plan(p1);
  fftss_free(b0);
  fftss_free(b1);

  return 0;
}

int main(int argc, char **argv)
{
  long i;

  if (argc == 2 && strcmp("-t", argv[1]) == 0) {
    testall();
    return 0;
  } 
  
  if (argc == 2) {
    i = atol(argv[1]);
    if (i) test(i);
    else printf("usage:\n\ttest -t    \t\trun all tests.\n"
                "\ttest [log2(N)]\t\tbenchmark [for length N]\n");
  } else {
    int ver;

    ver = fftss_version(NULL);
    printf("FFTSS library version 2.0, update %d.\n", ver);
    for (i = 1; i <= 16; i++) test(i);
  }
  
  return 0;
}


