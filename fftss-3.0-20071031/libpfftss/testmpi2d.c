#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <mpi.h>
#include <pfftss.h>

int main(int argc, char **argv)
{
  int id, npe;
  long i, j;
  double *v;
  double should_be;
  pfftss_plan p;
  long nx, ny, py;
  long ly, oy;
  long lognx, logny;
  double t;


  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &npe);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);

  lognx = 10;
  logny = 10;

  if (argc >= 3) {
    lognx = atol(argv[1]);
    logny = atol(argv[2]);
  }

#ifdef _OPENMP
  if (argc == 4)
    omp_set_num_threads(atoi(argv[3]));
#endif

  nx = 1 << lognx;
  ny = 1 << logny;
//  py = nx + 1;
  py = nx;
  
  oy = ny * id / npe;
  ly = ny * (id + 1) / npe - oy;

  v = fftss_malloc(py * ly * sizeof(double) * 2);

#ifdef _OPENMP
#pragma omp parallel for private(j)
#endif
  for (i = 0; i < ly; i++) 
    for (j = 0; j < nx * 2; j++)
      v[i * py * 2 + j] = 0.0;

  should_be = (double)(nx * ny + 1) * 0.5 
    * (double)(nx * ny);

  p = pfftss_plan_dft_2d(nx, ny, py, oy, ly, v, FFTSS_FORWARD, 0, 
			  MPI_COMM_WORLD);

#ifdef _OPENMP
#pragma omp parallel for private(j)
#endif
  for (i = 0; i < ly; i++) 
    for (j = 0; j < nx; j++) {
      v[i * py * 2 + 2 * j + 0] = (double)(i * nx + j + 1 + oy * nx);
      v[i * py * 2 + 2 * j + 1] = 0.0;
    }

  t = fftss_get_wtime();
  pfftss_execute(p);
  t = fftss_get_wtime() - t;

  pfftss_destroy_plan(p);

  if (id == 0) {
    printf("%le: sould be %le.\n", v[0], should_be);
    printf("%lf sec, %lf Mflops.\n", t, (double)nx * (double)ny 
	   * (double)(lognx + logny) * 5.0 / t * 0.000001);
  }

  MPI_Finalize();

  return 0;
}
