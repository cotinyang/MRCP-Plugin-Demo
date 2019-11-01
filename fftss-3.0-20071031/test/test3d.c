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
  long i, j, k;
  double *v;
  double should_be;
  fftss_plan pf, pb;
  long nx, ny, nz;
  long py, pz;
  long lognx, logny, lognz;
  double t;
  double s, ss;
  long logn;

  if (argc > 1)
    fftss_plan_with_nthreads(atoi(argv[1]));

  for (logn = 5; logn <= 10; logn++) {

    lognx = logn;
    logny = logn;
    lognz = logn;

    nx = 1 << lognx;
    ny = 1 << logny;
    nz = 1 << lognz;
    py = nx + 1;
    pz = (nx + 1) * ny + 1;
    
    v = (double *)fftss_malloc(pz * nz * sizeof(double) * 2);

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (i = 0; i < pz * nz * 2; i++) v[i] = 0.0;
	     
    pf = fftss_plan_dft_3d(nx, ny, nz, py, pz, v, v, FFTSS_FORWARD, 
			   FFTSS_INOUT);
    pb = fftss_plan_dft_3d(nx, ny, nz, py, pz, v, v, FFTSS_BACKWARD, 
			   FFTSS_INOUT);

#ifdef _OPENMP
#pragma omp parallel for private(i,j)
#endif
    for (k = 0; k < nz; k++)
      for (i = 0; i < ny; i++) 
	for (j = 0; j < nx; j++) {
	  v[k * pz * 2 + i * py * 2 + 2 * j + 0] = 
	    (double)(k * nx * ny + i * nx + j + 1);
	  v[k * pz * 2 + i * py * 2 + 2 * j + 1] = 0.0;
	}
    
    t = fftss_get_wtime();
    fftss_execute(pf);
    fftss_execute(pb);
    t = fftss_get_wtime() - t;
    
    fftss_destroy_plan(pf);
    fftss_destroy_plan(pb);

    s = 1.0 / (double)nx / (double)ny / (double)nz;

    for (i = 0; i < nz * pz * 2; i++) v[i] *= s;

    s = 0.0;
    for (k = 0; k < nz; k++) 
      for (i = 0; i < ny; i++)
        for (j = 0; j < nx; j++) {
          ss = fabs(v[k * pz * 2 + i * py * 2 + 2 * j] 
             - (double)(k * ny * nx + i * nx + j + 1));
          if (ss > s) s = ss;
          ss = fabs(v[k * pz * 2 + i * py * 2 + 2 * j + 1]);
          if (ss > s) s = ss;
        }

    s = s / (double)nx / (double)ny / (double)nz;

    fftss_free(v);
 
    printf("%3ld x %3ld x %3ld: % 8d MFLOPS, ERR=%le.\n", nx, ny, nz, 
	   (int)((double)nx * (double)ny * (double)nz
		 * (double)(lognx + logny + lognz) * 10.0 / t * 0.000001),
           s);
  }

}
