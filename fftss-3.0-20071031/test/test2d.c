#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <fftss.h>

int main(int argc, char **argv)
{
  long i, j;
  double *vin;
  double should_be;
  fftss_plan pf, pb;
  long nx, ny, py;
  long lognx, logny;
  double t;
  double s, ss;
  long logn;

  if (argc > 1)
    fftss_plan_with_nthreads(atoi(argv[1]));

  for (logn = 7; logn <= 12; logn++) {
  
    lognx = logn;
    logny = logn;

    nx = 1 << lognx;
    ny = 1 << logny;
    py = nx + 1;
//    py = nx;
  
    vin = (double *)fftss_malloc(py * ny * sizeof(double) * 2);

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (i = 0; i < ny * py * 2; i++) vin[i] = 0.0;

    pf = fftss_plan_dft_2d(nx, ny, py, vin, vin,
                           FFTSS_FORWARD, FFTSS_INOUT|FFTSS_VERBOSE);
    pb = fftss_plan_dft_2d(nx, ny, py, vin, vin,
                           FFTSS_BACKWARD, FFTSS_INOUT);

#ifdef _OPENMP
#pragma omp parallel for private(j)
#endif
    for (i = 0; i < ny; i++) 
      for (j = 0; j < nx; j++) {
	vin[i * py * 2 + 2 * j + 0] = (double)(i * nx + j + 1);
	vin[i * py * 2 + 2 * j + 1] = 0.0;
      }

    should_be = (double)(nx * ny + 1) * 0.5 
      * (double)(nx * ny);

    t = fftss_get_wtime();
    fftss_execute(pf);
    fftss_execute(pb);
    t = fftss_get_wtime() - t;
    
    fftss_destroy_plan(pf);
    fftss_destroy_plan(pb);

    s = 1.0 / (double)nx / (double)ny;

    for (i = 0; i < ny * py * 2; i++) vin[i] *= s;

    s = 0.0;
    for (i = 0; i < ny; i++)
      for (j = 0; j < nx; j++) {
	ss = fabs(vin[i * py * 2 + 2 * j] - (double)(i * nx + j + 1));
	if (ss > s) s = ss;
	ss = fabs(vin[i * py * 2 + 2 * j + 1]);
	if (ss > s) s = ss;
      }

    s = s / (double)nx / (double)ny;

    fftss_free(vin);

    printf("%4ld x %4ld: %8d MFLOPS, %le sed, ERR=%le.\n", nx, ny,
	   (int)((double)nx * (double)ny 
		 * (double)(lognx + logny) * 10.0 / t * 0.000001), t,
	   s);

  }
}
